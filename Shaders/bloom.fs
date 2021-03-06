#version 420 core
layout (location = 0) out vec4 FragColor;

in vec2 uv;

uniform sampler2D image;
uniform sampler2D bloomBlur;

void main()
{
    vec3 hdrColor = texture(image,uv).rgb;
    vec3 bloomColor = texture(bloomBlur,uv).rgb;
    hdrColor += bloomColor; //additive blending
    //Tone mapping
    vec3 reinhard = hdrColor/(hdrColor + vec3(1.0));
    FragColor = vec4(reinhard,1.0);
}

