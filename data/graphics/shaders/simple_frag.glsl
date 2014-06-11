#version 150 core
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D tex;
uniform vec4 color;
uniform float drawType;
uniform float alpha;

void main()
{
    outColor = texture(tex, Texcoord);
    if (drawType >= 0.1)
        outColor = texture(tex, Texcoord) * vec4(1.0, 1.0, 1.0, alpha);
    else
        outColor = color;
}
