#version 330
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;
layout (location = 3) in ivec4 in_bone_id;
layout (location = 4) in vec4 in_weight;

out vec2 uv;

uniform mat4 model;
uniform mat4 camera;

uniform mat4 bones[32];

void main()
{
    vec4 skin = vec4(0);
    for (int i = 0; i < 4; i++)
    {
        if (in_bone_id[i] == -1)
            continue;
        if (in_bone_id[i] >= 32)
        {
            skin = vec4(in_position, 1);
            break;
        }
        vec4 local_pos = bones[in_bone_id[i]] * vec4(in_position, 1);
        skin += local_pos * in_weight[i];
    }
    gl_Position = camera * model * skin;
    uv = in_uv;
}