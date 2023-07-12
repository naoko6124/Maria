#version 330
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

out vec2 uv;
out vec3 normal;
out vec3 position;

uniform mat4 model;
uniform mat4 camera;

void main()
{
    gl_Position = camera * model * vec4(in_position, 1.0);
    uv = in_uv;
    normal = in_normal;
    position = vec3(model * vec4(in_position, 1.0));
}