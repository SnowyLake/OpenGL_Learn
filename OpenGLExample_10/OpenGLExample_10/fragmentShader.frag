#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

//texture sampler
uniform sampler2D texture1;

void main()
{
    FragColor = texture2D(texture1, TexCoord);
}