#version 100

attribute vec4 position;
varying vec2 Texcoord;
uniform vec2 screen;
uniform vec2 textureSize;

void main()
{
   Texcoord = vec2(position.z / textureSize.x, position.w / textureSize.y);
   gl_Position = vec4(position.x / screen.x - 1.0, 1.0 - position.y / screen.y, 0.0, 1.0);
}
