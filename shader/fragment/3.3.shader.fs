#version 330 core

out vec4 FragColor;

in vec2 ourTexCoord;
in vec3 ourColor;

uniform sampler2D ourTexture;
uniform float mixValue;

void main()
{
    vec4 texColor = texture(ourTexture, ourTexCoord);
    vec4 baseColor = vec4(ourColor, 1.0);
    FragColor = mix(baseColor, texColor, mixValue);
};