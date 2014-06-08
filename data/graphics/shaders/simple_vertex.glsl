#version 150 core
in vec2 simplePos;
uniform vec2 screen;
void main()
{
   gl_Position = vec4(simplePos.x / screen.x - 1, 1 - simplePos.y / screen.y, 0.0, 1.0);
}
