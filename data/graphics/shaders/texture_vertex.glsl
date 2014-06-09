#version 150 core
in vec2 texturePos;
in vec2 texcoord;
out vec2 Texcoord;
uniform vec2 screen;
void main()
{
    Texcoord = texcoord;
    gl_Position = vec4(texturePos.x / screen.x - 1, 1 - texturePos.y / screen.y, 0.0, 1.0);
}
