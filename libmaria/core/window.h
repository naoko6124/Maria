#pragma once

#include <glad/gl.h>
#include <glfw/glfw3.h>
#include <string>

#include <util/assets.h>

namespace maria
{
    class window
    {
    public:
        void init(std::string title, int width, int height, std::string icon_path)
        {
            glfwInit();
            instance = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

            assets::image icon(icon_path);
            GLFWimage images[1];
            images[0].width = icon.width;
            images[0].height = icon.height;
            images[0].pixels = icon.data;
            glfwSetWindowIcon(instance, 1, images);

            glfwMakeContextCurrent(instance);
            gladLoaderLoadGL();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }
        void terminate()
        {
            glfwDestroyWindow(instance);
            glfwTerminate();
        }
        void get_events()
        {
            glfwPollEvents();
        }
        void clear()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        void present()
        {
            glfwSwapBuffers(instance);
        }
        bool is_running()
        {
            return !glfwWindowShouldClose(instance);
        }
        bool get_key_down(int keycode)
        {
            return glfwGetKey(instance, keycode) == GLFW_PRESS;
        }
        bool get_key_up(int keycode)
        {
            return glfwGetKey(instance, keycode) == GLFW_RELEASE;
        }
        bool get_key_down(char keycode)
        {
            return glfwGetKey(instance, keycode) == GLFW_PRESS;
        }
        bool get_key_up(char keycode)
        {
            return glfwGetKey(instance, keycode) == GLFW_RELEASE;
        }
    private:
        GLFWwindow* instance;
    };
}