#pragma once
#include <bitset>
#include <queue>
#include <array>
#include <vector>
#include <functional>

namespace entneko
{
    typedef const char* exception;

    const int max_entities = 32;
    const int max_components = 32;

    typedef uint32_t entity;
    typedef std::bitset<max_components> entity_data;

    class world
    {
    public:
        world() // fill the queue of unused entities
        {
            for(entity e = 0; e < max_entities; e++)
            {
                unused_entities.push(e);
            }
        }

        ~world() // cleaning the component pools
        {
        }

        entity create_entity()
        {
            if (living_entities >= max_entities) // verify the entity limit
                throw exception("too many entities"); // throw an error if reach the limit
            entity id = unused_entities.front(); // take the first entity id from the queue
            unused_entities.pop(); // remove the id from that queue
            ++living_entities; // increase the living entities counter
            return id; // return the entity id
        }
        
        void destroy_entity(entity e)
        {
            if (e >= max_entities) // verify the entity limit
                throw exception("entity out of range"); // throw an error if is out of range
            entities[e].reset();
            unused_entities.push(e);
            --living_entities;
        }

        template<typename type>
        type& add_component(entity e)
        {
            if (e >= max_entities) // verify the entity limit
                throw exception("entity out of range"); // throw an error if is out of range
            int component_id = get_id<type>();
            if (component_id >= max_components) // verify the component limit
                throw exception("too many components"); // throw an error if reach the limit
            type* component = new type();
            pools[component_id].set(e, static_cast<void*>(component));
            entities[e].set(component_id);
            return *component;
        }

        template<typename type>
        type& get_component(entity e)
        {
            if (e >= max_entities) // verify the entity limit
                throw exception("entity out of range"); // throw an error if is out of range
            int component_id = get_id<type>();
            if (component_id >= max_components) // verify the component limit
                throw exception("component out of range"); // throw an error if is out of range
            if (!entities[e].test(component_id)) // check if entity has component
                throw exception("entity has no component"); // throw an error if not
            type* component = static_cast<type*>(pools[component_id].get(e));
            return *component;
        }

        template<typename type>
        bool has_component(entity e)
        {
            if (e >= max_entities) // verify the entity limit
                throw exception("entity out of range"); // throw an error if is out of range
            int component_id = get_id<type>();
            if (component_id >= max_components) // verify the component limit
                throw exception("component out of range"); // throw an error if is out of range
            return entities[e].test(component_id); // check if entity has component
        }

        template<typename type>
        void sub_component(entity e)
        {
            if (e >= max_entities) // verify the entity limit
                throw exception("entity out of range"); // throw an error if is out of range
            int component_id = get_id<type>();
            if (component_id >= max_components) // verify the component limit
                throw exception("component out of range"); // throw an error if is out of range
            if (!entities[e].test(component_id)) // check if entity has component
                throw exception("entity has no component"); // throw an error if not
            entities[e].reset(component_id);
        }

        template<typename... types>
        void query(std::function<void(entity e)> fn)
        {
            if (sizeof...(types) == 0)
                throw exception("view needs at least one component");
            int components_id[] = { get_id<types>() ...};
            std::vector<entity> valid_entities{};
            for (int e = 0; e < max_entities; e++)
            {
                bool valid = true;
                for (int c = 0; c < sizeof...(types); c++)
                {
                    if (!entities[e].test(components_id[c]))
                    {
                        valid = false;
                        break;
                    }
                }
                if (valid)
                    valid_entities.push_back(e);
            }
            for (size_t i = 0; i < valid_entities.size(); i++)
            {
                fn(valid_entities[i]);
            }
        }
    private:
        static int components_counter;

        template<class type>
        int get_id()
        {
            static int component_id = components_counter++;
            return component_id;
        }

        std::queue<entity> unused_entities{};
        std::array<entity_data, max_entities> entities{};
        entity living_entities{};
 
        struct component_pool
        {
            component_pool() {}

            ~component_pool()
            {
                for(void* component : data)
                {
                    delete component;
                }
            }

            inline void* get(entity index)
            {
                return data[index];
            }

            inline void set(entity index, void* component)
            {
                delete data[index];
                data[index] = component;
            }

            std::array<void*, max_entities> data{};
        };

        std::array<component_pool, max_components> pools{};
    };
}
