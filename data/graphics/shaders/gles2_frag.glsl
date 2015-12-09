#version 100
precision mediump float;

uniform sampler2D tex;
uniform vec4 color;
uniform float drawType;
uniform float alpha;
varying mediump vec2 Texcoord;

void main()
{
    if (drawType >= 0.1)
    {
        gl_FragColor = texture2D(tex, Texcoord) * vec4(1.0, 1.0, 1.0, alpha);
    }
    else
    {
        gl_FragColor = color;
    }
}
