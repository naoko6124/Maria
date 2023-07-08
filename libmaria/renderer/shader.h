#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace maria
{
    class shader
    {
    public:
        void init()
        {
            program = glCreateProgram();
        }
        void terminate()
        {
            glDeleteProgram(program);
        }
        void load_vs(std::string code)
        {
            unsigned int vertex_shader;
            vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            const char* vertex_code = code.c_str();
            glShaderSource(vertex_shader, 1, &vertex_code, 0);
            glCompileShader(vertex_shader);

            glAttachShader(program, vertex_shader);
            glLinkProgram(program);
            glDeleteShader(vertex_shader);
        }
        void load_fs(std::string code)
        {
            unsigned int fragment_shader;
            fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const char* fragment_code = code.c_str();
            glShaderSource(fragment_shader, 1, &fragment_code, 0);
            glCompileShader(fragment_shader);

            glAttachShader(program, fragment_shader);
            glLinkProgram(program);
            glDeleteShader(fragment_shader);
        }
        void use()
        {
            glUseProgram(program);
        }
        void upload_mat4(std::string location, glm::mat4 value)
        {
            int loc = glGetUniformLocation(program, location.c_str());
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
        }
    private:
        unsigned int program;
    };
};