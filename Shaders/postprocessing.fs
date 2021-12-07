#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 brightColor;

in vec2 uv;
uniform sampler2D image;



void main()
{    
    FragColor = texture(image, uv);
}

