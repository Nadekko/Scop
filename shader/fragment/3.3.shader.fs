#version 330 core

out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float mixValue;

void main()
{
    vec4 baseColor = vec4(ourColor, 1.0);
    vec4 texColor = texture(ourTexture, TexCoord);

    FragColor = mix(baseColor, texColor, mixValue);
}