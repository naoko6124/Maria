#version 330
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 position;

uniform sampler2D tex;

void main()
{
    vec4 object = texture(tex, uv);
    
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * vec4(1);

    vec3 light_pos = vec3(0, 2, 0);

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - position);
    
    float diff = max(dot(norm, light_dir), 0.0);
    vec4 diffuse = diff * vec4(1);

    vec4 light = ambient + diffuse;

    FragColor = light * object;
}