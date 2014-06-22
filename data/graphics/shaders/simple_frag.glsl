#version 130
in vec2 Texcoord;
out vec4 outColor;
uniform sampler2D tex;
uniform vec4 color;
uniform float drawType;
uniform float alpha;

void main()
{
    if (drawType >= 0.1)
    {
        outColor = texelFetch(tex, ivec2(Texcoord.x, Texcoord.y), 0) * vec4(1.0, 1.0, 1.0, alpha);
    }
    else
    {
        outColor = color;
    }
}
