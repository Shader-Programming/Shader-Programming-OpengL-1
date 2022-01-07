#version 420 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 brightColor;
layout (location = 2) out vec4 doFColor;

uniform bool horizontal;

in vec2 uv;
uniform sampler2D image;
uniform sampler2D image2;
uniform sampler2D depthMap;
uniform bool blurToggle;
uniform bool depthOfFieldToggle;

float weights[] = {
0.045588748213977244, 0.04469312448750337, 0.04323941779418976, 0.041283252881657614, 0.03889759757448154,
0.036168179845020715, 0.03318836107227639, 0.030053872747518973, 0.026857814141164747, 0.023686258756849855,
0.020614736353370078, 0.01770575674976994, 0.015007434722657838, 0.012553174649950505, 0.01036228951631275,
0.00844136848839372, 0.006786173670846112, 0.05383839343129145, 0.004215162373482302, 0.003256804954062264,
0.002483273675394342};

int r = 21;
float bias = 1f;



void main()
{
    vec3 color;
    vec3 color2;
    vec3 color3;

if(blurToggle)
{
    vec2 textureOffset = 1.0/textureSize(image,0);
    color = texture(image,uv).rgb * (weights[0]*bias);
    color2 = texture(image2,uv).rgb * (weights[0]*bias);
    if(horizontal)
    {
        for(int i = 1; i<r;i++)
        {
            color = color + texture(image, uv + vec2(textureOffset.x *i,0.0)).rgb * (weights[i]*bias);
            color = color + texture(image, uv - vec2(textureOffset.x *i,0.0)).rgb * (weights[i]*bias);
            color2 = color2 + texture(image2, uv + vec2(textureOffset.x *i,0.0)).rgb * (weights[i]*bias);
            color2 = color2 + texture(image2, uv - vec2(textureOffset.x *i,0.0)).rgb * (weights[i]*bias);
        }
    }
    else
    {
        for(int i = 1; i<r;i++)
        {
            color = color + texture(image, uv + vec2(0.0,textureOffset.y *i)).rgb * (weights[i]*bias);
            color = color + texture(image, uv - vec2(0.0,textureOffset.y *i)).rgb * (weights[i]*bias);
            color2 = color2 + texture(image2, uv + vec2(0.0,textureOffset.y *i)).rgb * (weights[i]*bias);
            color2 = color2 + texture(image2, uv - vec2(0.0,textureOffset.y *i)).rgb * (weights[i]*bias);
        }
    }

    
}
else
{
    color = texture(image,uv).rgb;
    color2 = texture(image2,uv).rgb;
}
if(depthOfFieldToggle)
{
    vec2 coord = vec2(0.5,0.5);

    vec3 depthCoor = texture(depthMap,uv).rgb;
    vec3 depthCurrentFragment = texture(depthMap,coord).rgb;

    

    vec3 inFocusFragment = texture(image,uv).rgb;
    vec3 outFocusFragment = color;

    float depthDistance = distance(depthCoor,depthCurrentFragment);
    //vec3 depth = mix(depthCoor,color4,depthDistance); 

    inFocusFragment = inFocusFragment * vec3(1,1,1);
    //color3 = mix(inFocusFragment,outFocusFragment,depthDistance);
    color3 = depthCurrentFragment;
}
FragColor = vec4(color, 1.0);
brightColor = vec4(color2,1.0);
doFColor = vec4(color3,1.0);
}

