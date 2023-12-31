#version 300 es
precision mediump float;

out vec4 FragColor;
  
in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * ourColor;  
}