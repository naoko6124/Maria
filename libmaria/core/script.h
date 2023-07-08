#pragma once

#include <lua.hpp>
#include <entneko.h>

#include <core/window.h>
#include <renderer/transform.h>
#include <util/assets.h>

namespace maria
{
    class script
    {
    public:
        script()
        {
            L = luaL_newstate();
            luaL_openlibs(L);
        }
        ~script()
        {
            lua_close(L);
        }
        void set_entity(entneko::entity ent, entneko::world *wrl)
        {
            e = ent;
            w = wrl;
        }
        void set_window(window* m_window)
        {
            wnd = m_window;
        }
        void load(std::string path)
        {
            luaL_dofile(L, path.c_str());

            lua_pushlightuserdata(L, &e);
            lua_setglobal(L, "libmaria_entity");

            lua_pushlightuserdata(L, w);
            lua_setglobal(L, "libmaria_world");
            
            lua_pushlightuserdata(L, wnd);
            lua_setglobal(L, "libmaria_window");

            lua_newtable(L);
            lua_pushcfunction(L, l_get_component);
            lua_setfield(L, -2, "get_component");
            lua_setglobal(L, "entity");

            lua_newtable(L);
            lua_pushcfunction(L, l_get_key_down);
            lua_setfield(L, -2, "get_key_down");
            lua_setglobal(L, "window");
        }
        void start()
        {
            lua_getglobal(L, "start");

            lua_call(L, 0, 0);
        }
        void update(float delta)
        {
            lua_getglobal(L, "update");
            lua_pushnumber(L, delta);
            lua_call(L, 1, 0);
        }

    public:
        entneko::entity e;
        entneko::world *w;
        window* wnd;
    private:
        lua_State *L;

    private:
        // window functions
        static int l_get_key_down(lua_State *L)
        {
            const char* keycode = luaL_checkstring(L, 1);

            lua_getglobal(L, "libmaria_window");
            window* wnd = (window *)lua_touserdata(L, -1);
            lua_pop(L, 1);

            bool key_down = wnd->get_key_down(keycode[0]);

            lua_pushboolean(L, key_down);
            return 1;
        }

        static int l_get_component(lua_State *L)
        {
            std::string name = luaL_checkstring(L, 1);

            lua_getglobal(L, "libmaria_entity");
            entneko::entity *e = (entneko::entity *)lua_touserdata(L, -1);
            lua_pop(L, 1);

            lua_getglobal(L, "libmaria_world");
            entneko::world *w = (entneko::world *)lua_touserdata(L, -1);
            lua_pop(L, 1);

            if (name == "transform")
            {
                lua_newtable(L);

                transform &t = w->get_component<transform>(*e);

                lua_newtable(L);
                lua_pushnumber(L, t.position.x);
                lua_setfield(L, -2, "x");
                lua_pushnumber(L, t.position.y);
                lua_setfield(L, -2, "y");
                lua_pushnumber(L, t.position.z);
                lua_setfield(L, -2, "z");

                lua_setfield(L, -2, "position");

                lua_pushcfunction(L, l_transform_translate);
                lua_setfield(L, -2, "translate");

                lua_pushcfunction(L, l_transform_set_rotation);
                lua_setfield(L, -2, "set_rotation");
            }
            
            if (name == "animator")
            {
                animator &a = w->get_component<animator>(*e);
                
                lua_newtable(L);
                lua_pushcfunction(L, l_animator_set_animation);
                lua_setfield(L, -2, "set_animation");
            }

            return 1;
        }

        // transform functions
        static int l_transform_translate(lua_State *L)
        {
            double x = luaL_checknumber(L, 1);
            double y = luaL_checknumber(L, 2);
            double z = luaL_checknumber(L, 3);

            lua_getglobal(L, "libmaria_entity");
            entneko::entity *e = (entneko::entity *)lua_touserdata(L, -1);
            lua_pop(L, 1);

            lua_getglobal(L, "libmaria_world");
            entneko::world *w = (entneko::world *)lua_touserdata(L, -1);
            lua_pop(L, 1);

            transform &t = w->get_component<transform>(*e);

            t.position.x += x;
            t.position.y += y;
            t.position.z += z;

            return 0;
        }
        static int l_transform_set_rotation(lua_State *L)
        {
            double x = luaL_checknumber(L, 1);
            double y = luaL_checknumber(L, 2);
            double z = luaL_checknumber(L, 3);

            lua_getglobal(L, "libmaria_entity");
            entneko::entity *e = (entneko::entity *)lua_touserdata(L, -1);
            lua_pop(L, 1);

            lua_getglobal(L, "libmaria_world");
            entneko::world *w = (entneko::world *)lua_touserdata(L, -1);
            lua_pop(L, 1);

            transform &t = w->get_component<transform>(*e);

            t.rotation = glm::quat(glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z)));

            return 0;
        }

        // animation functions
        static int l_animator_set_animation(lua_State *L)
        {
            std::string anim_name = luaL_checkstring(L, 1);
            
            lua_getglobal(L, "libmaria_entity");
            entneko::entity *e = (entneko::entity *)lua_touserdata(L, -1);
            lua_pop(L, 1);

            lua_getglobal(L, "libmaria_world");
            entneko::world *w = (entneko::world *)lua_touserdata(L, -1);
            lua_pop(L, 1);

            animator &a = w->get_component<animator>(*e);

            a.current = anim_name;

            return 0;
        }
    };
}