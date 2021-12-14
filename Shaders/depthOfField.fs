#version 420 core

out vec4 FragColor;

uniform bool horizontal;

in vec2 uv;
uniform sampler2D inFocus;
uniform sampler2D outFocus;
uniform sampler2D depthMap;

uniform bool depthOfFieldToggle;



void main()
{
    vec3 color;

if(depthOfFieldToggle)
{
    vec2 centreUV = vec2(0.5,0.5);

    vec3 depthCoor = texture(depthMap,centreUV).rgb;
    vec3 depthCurrentFragment = texture(depthMap,uv).rgb;

    vec3 inFocusFragment = texture(inFocus,uv).rgb;
    vec3 outFocusFragment = texture(outFocus,uv).rgb;

    float depthDistance = distance(depthCoor,depthCurrentFragment);

    color = Mix(inFocusFragment,outFocusFragment,depthDistance);
    
}
else
{
    color = texture(image,uv).rgb;
}
FragColor = vec4(color, 1.0);
}

