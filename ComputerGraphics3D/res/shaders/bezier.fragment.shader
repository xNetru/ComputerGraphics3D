#version 330

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;


uniform sampler2D texture_diffuse1;

void main()
{    
    // FragColor = texture(texture_diffuse1, TexCoords);
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    FragColor = vec4(Normal + vec3(1.0), 1.0);
}