#pragma once

#include <map>
#include <string>
#include <filesystem>
#include <entneko.h>
#include <yaml-cpp/yaml.h>

#include <core/collider.h>
#include <core/components.h>
#include <core/script.h>
#include <renderer/camera.h>
#include <renderer/shader.h>
#include <util/assets.h>

namespace fs = std::filesystem;

namespace maria
{
    class scene
    {
    public:
        scene()
        {
            struct position
            {
                int x;
                int y;
            };

            entneko::entity e = world.create_entity();
            position& p1 = world.add_component<position>(e);
            position& p2 = world.get_component<position>(e);
            p1.x = 4;
        }
        ~scene() {}
        void load(fs::path app_path, std::string path, window* m_window)
        {
            application_path = app_path;
            YAML::Node data = YAML::LoadFile(path);
            for (int i = 0; i < data["shaders"].size(); i++)
            {
                std::string shader_name = data["shaders"][i]["name"].as<std::string>();
                shaders[shader_name] = shader();
                shaders[shader_name].init();

                fs::path vertex_path = application_path;
                vertex_path.append(data["shaders"][i]["vertex"].as<std::string>());
                shaders[shader_name].load_vs(assets::load_file(vertex_path.string()));

                fs::path fragment_path = application_path;
                fragment_path.append(data["shaders"][i]["fragment"].as<std::string>());
                shaders[shader_name].load_fs(assets::load_file(fragment_path.string()));
            }
            for (int i = 0; i < data["entities"].size(); i++)
            {
                YAML::Node ent_node = data["entities"][i];
                std::string ent_name = data["entities"][i]["name"].as<std::string>();

                entneko::entity e = world.create_entity();
                if (ent_node["transform"])
                {
                    transform& t = world.add_component<transform>(e);
                    t.position.x = ent_node["transform"]["position"]["x"].as<float>();
                    t.position.y = ent_node["transform"]["position"]["y"].as<float>();
                    t.position.z = ent_node["transform"]["position"]["z"].as<float>();
                    t.rotation.w = ent_node["transform"]["rotation"]["w"].as<float>();
                    t.rotation.x = ent_node["transform"]["rotation"]["x"].as<float>();
                    t.rotation.y = ent_node["transform"]["rotation"]["y"].as<float>();
                    t.rotation.z = ent_node["transform"]["rotation"]["z"].as<float>();
                    t.scale.x = ent_node["transform"]["scale"]["x"].as<float>();
                    t.scale.y = ent_node["transform"]["scale"]["y"].as<float>();
                    t.scale.z = ent_node["transform"]["scale"]["z"].as<float>();
                }
                if (ent_node["shading"])
                {
                    shading& s = world.add_component<shading>(e);
                    s.name = ent_node["shading"].as<std::string>();
                }
                if (ent_node["mesh"] && !ent_node["skeleton"])
                {
                    fs::path mesh_path = application_path;
                    mesh_path.append(ent_node["mesh"]["file"].as<std::string>());
                    mesh& m = world.add_component<mesh>(e);
                    assets::load_mesh(m, mesh_path.string());
                }
                if (ent_node["mesh"] && ent_node["skeleton"])
                {
                    fs::path mesh_path = application_path;
                    mesh_path.append(ent_node["mesh"]["file"].as<std::string>());
                    skinned_mesh& m = world.add_component<skinned_mesh>(e);
                    assets::load_skinned_mesh(m, mesh_path.string());
                    
                    fs::path skeleton_path = application_path;
                    skeleton_path.append(ent_node["skeleton"]["file"].as<std::string>());
                    skeleton& s = world.add_component<skeleton>(e);
                    assets::load_skeleton(s, skeleton_path.string());

                    if (ent_node["animations"].size() > 0)
                    {
                        animator& a = world.add_component<animator>(e);
                        for (int j = 0; j < ent_node["animations"].size(); j++)
                        {
                            fs::path animation_path = application_path;
                            animation_path.append(ent_node["animations"][j]["file"].as<std::string>());
                            a.anims[ent_node["animations"][j]["name"].as<std::string>()] = assets::load_animation(animation_path.string());
                        }
                        if (ent_node["animation"])
                        {
                            a.current = ent_node["animation"].as<std::string>();
                        }
                    }
                }
                if (ent_node["texture"])
                {
                    fs::path texture_path = application_path;
                    texture_path.append(ent_node["texture"]["file"].as<std::string>());
                    texture& tex = world.add_component<texture>(e);
                    assets::load_texture(tex, texture_path.string());
                }
                if (ent_node["script"])
                {
                    fs::path script_path = application_path;
                    script_path.append(ent_node["script"]["file"].as<std::string>());
                    script& scp = world.add_component<script>(e);
                    scp.set_entity(e, &world);
                    scp.set_window(m_window);
                    scp.load(script_path.string());
                }
                if (ent_node["collider"])
                {
                    collider& col = world.add_component<collider>(e);
                    transform& t = world.get_component<transform>(e);
                    col.tf = &t;
                    glm::vec3 size;
                    size.x = ent_node["collider"]["x"].as<float>();
                    size.y = ent_node["collider"]["y"].as<float>();
                    size.z = ent_node["collider"]["z"].as<float>();
                    col.size = size;
                }
            }
        }
        void start()
        {
            entneko::world& w = world;
            world.query<script>([&w](entneko::entity e) {
                script& s = w.get_component<script>(e);
                s.start();
            });
        }
        void update(float delta)
        {
            main_cam.position.z = 5.0f;
            main_cam.position.y = 3.0f;
            main_cam.rotation.x = -30.0f;

            entneko::world& w = world;
            std::map<std::string, shader>& sh = shaders;
            camera& c = main_cam;

            world.query<script>([&w, &delta](entneko::entity e) {
                script& s = w.get_component<script>(e);
                s.update(delta);
            });

            world.query<collider, script>([&w](entneko::entity e) {
                script& s = w.get_component<script>(e);
                collider& c1 = w.get_component<collider>(e);

                w.query<collider>([&w, &s, &c1](entneko::entity e) {
                    collider& c2 = w.get_component<collider>(e);
                    if (c2 == c1)
                        return;

                    auto idt = c1.intersect(c2);
                    if (idt.intersect)
                        s.on_collision(idt.distance);
                });
            });

            world.query<shading, transform, mesh, texture>([&w, &sh, &c](entneko::entity e) {
                shading& s = w.get_component<shading>(e);
                transform& t = w.get_component<transform>(e);
                mesh& msh = w.get_component<mesh>(e);
                texture& tex = w.get_component<texture>(e);
                sh[s.name].use();
                sh[s.name].upload_mat4("camera", c.get_matrix());
                sh[s.name].upload_mat4("model", t.get_matrix());

                tex.apply();
                msh.render();
            });

            world.query<shading, transform, skinned_mesh, skeleton, animator, texture>([&w, &sh, &c](entneko::entity e) {
                shading& s = w.get_component<shading>(e);
                transform& t = w.get_component<transform>(e);
                skinned_mesh& msh = w.get_component<skinned_mesh>(e);
                texture& tex = w.get_component<texture>(e);
                skeleton& skl = w.get_component<skeleton>(e);
                animator& ani = w.get_component<animator>(e);
                sh[s.name].use();
                sh[s.name].upload_mat4("camera", c.get_matrix());
                sh[s.name].upload_mat4("model", t.get_matrix());

                skl.set_animation(ani.anims[ani.current], glfwGetTime() * 20);

                skl.apply(sh[s.name]);

                tex.apply();
                msh.render();
            });
        }
    private:
        fs::path application_path;
        entneko::world world;
        std::map<std::string, shader> shaders;
        camera main_cam;
    };
}