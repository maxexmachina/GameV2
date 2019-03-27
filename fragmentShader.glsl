#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float mixer;

void main() {
    FragColor = mix(texture(Texture1, texCoord), texture(Texture2, vec2(texCoord.x, texCoord.y)), mixer);
}