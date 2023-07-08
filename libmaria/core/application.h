#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <yaml-cpp/yaml.h>

#include <core/window.h>
#include <core/scene.h>

namespace fs = std::filesystem;

namespace maria
{
    class application
    {
    public:
        std::string name;
        fs::path path;
    public:
        application(std::string spec_path)
        {
            path = fs::path(spec_path).parent_path();
            YAML::Node spec = YAML::LoadFile(spec_path);
            name = spec["name"].as<std::string>();

            fs::path icon_path = path;
            icon_path.append(spec["icon"].as<std::string>());
            
            int width = spec["resolution"]["width"].as<int>();
            int height = spec["resolution"]["height"].as<int>();
            m_window.init(name, width, height, icon_path.string());

            fs::path scene_path = path;
            scene_path.append(spec["scenes"][0].as<std::string>());
            main_scene.load(path, scene_path.string(), &m_window);
        }
        ~application()
        {
            m_window.terminate();
        }
        void run()
        {
            double current_frame = glfwGetTime();
            double last_frame = current_frame;
            double delta_time;

            main_scene.start();

            while(m_window.is_running())
            {
                current_frame = glfwGetTime();
                delta_time = current_frame - last_frame;
                last_frame = current_frame;

                m_window.get_events();
                m_window.clear();

                main_scene.update(delta_time);

                m_window.present();
            }
        }
    private:
        window m_window;
        scene main_scene;
    };
}