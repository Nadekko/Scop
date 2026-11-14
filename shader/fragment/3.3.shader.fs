#version 330 core

out vec4 FragColor;
in vec3 Color;
in vec2 TexCoord;
uniform sampler2D ourTexture;
uniform float mixValue;

void main()
{
    vec4 materialColor = vec4(Color, 1.0);
    vec4 textureColor = texture(ourTexture, TexCoord);

    FragColor = mix(materialColor, textureColor, clamp(mixValue, 0.0, 1.0));
}