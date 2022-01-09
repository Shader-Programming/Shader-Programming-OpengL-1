#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 brightColor;

in vec3 normal;
in vec3 posWS;
in vec2 uv;
in mat3 TBN;

struct pointLight
{
    vec3 position;
    vec3 color;
    float Kc;
    float Kl;
    float Ke;
    float intensity;
};

struct spotLight
{
    vec3 position;
    vec3 direction;
    vec3 color;
    float Kc;
    float Kl;
    float Ke;
    float innerRad;
    float outerRad;
};

struct directLight
{
    vec3 lightCol;
    vec3 lightDir;
};

struct material
{
    vec3 objectCol;
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D normalMap;
    sampler2D dispMap;
};


uniform vec3 viewPos;
uniform pointLight pLight;
uniform spotLight sLight;
uniform directLight dLight;

uniform material mat;

uniform bool toggleNormalMap;
uniform bool toggleDispMap;
uniform int selectedLight;

float ambientFactor = 0.4;
float shine = 32;
float specularStrength = 0.7;

uniform sampler2D depthMap;
uniform mat4 lightSpaceMatrix;


float calcShadow(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depthMap,projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = 0;
    float bias = 0.015;

    for(int i = -1; i <2; i++)
    {
        for (int j = -1; j<2; j++)
        {
            float pcf = texture(depthMap, projCoords.xy + vec2(i,j)*1).r;
                if(currentDepth-bias > closestDepth)
                    shadow += 1;
        }
    }

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow/9;
}


vec3 SpotLight(vec3 norm, vec3 viewDir)
{
    vec3 diffMapColor = texture(mat.diffuseTexture, uv).xyz;
    float dist = length(sLight.position - posWS);
    float attn = 1.0/(sLight.Kc + sLight.Kl * dist + (sLight.Ke * (dist*dist)));
    vec3 sLightDir = normalize(sLight.position - posWS);


    //diffuse
    float diffuseFactor = dot(norm, sLightDir);
    diffuseFactor = max(diffuseFactor, 0.0);
    vec3 diffuseColor = sLight.color*diffMapColor*diffuseFactor;
    diffuseColor = diffuseColor*attn;


    //Specular
    vec3 halfWay = normalize(sLightDir + viewDir);
    float specFactor = pow(max(dot(norm,halfWay),0.0),shine);
    vec3 specluarColor = sLight.color * specFactor *  0.3 * attn;


    float theta = dot(-sLightDir, normalize(sLight.direction));
    float denom = (sLight.innerRad - sLight.outerRad);
    float illum = (theta - sLight.outerRad ) / denom;
    illum = clamp(illum, 0.0,1.0);
    
    diffuseColor =  diffuseColor * illum;
    specluarColor = specluarColor * illum;


    vec3 result = diffuseColor + specluarColor;
    return result;
}

vec3 PointLight(vec3 norm, vec3 viewDir,vec2 texCoords)
{
    vec3 diffMapColor = texture(mat.diffuseTexture, uv).xyz;

    float dist = length(pLight.position - posWS);
    float attn = 1.0/(pLight.Kc + pLight.Kl * dist + (pLight.Ke * (dist*dist)));
    vec3 pLightDir = normalize(pLight.position - posWS);

    //Ambient
    vec3 ambientColor = pLight.color * diffMapColor * ambientFactor;
    ambientColor = ambientColor * attn;

    //diffuse
    float diffuseFactor = dot(norm, pLightDir);
    diffuseFactor = max(diffuseFactor, 0.0);
    vec3 diffuseColor = pLight.color*diffMapColor*diffuseFactor;
    diffuseColor = diffuseColor*attn;

    //Specular
    vec3 halfWay = normalize(pLightDir + viewDir);
    float specFactor = pow(max(dot(normal,halfWay),0.0),shine);
    vec3 specluarColor = pLight.color * specFactor *  specularStrength  * attn * texture(mat.specularTexture,texCoords).x;

    vec3 result = (ambientColor + diffuseColor + specluarColor) * pLight.intensity;
    return result;
}

vec3 DirectionalLight(vec3 norm, vec3 viewDir,vec2 texCoords, float shadow)
{
    vec3 diffMapColor = texture(mat.diffuseTexture, texCoords).xyz;

//Ambient
vec3 ambientColor = dLight.lightCol * diffMapColor * ambientFactor;

//diffuse
float diffuseFactor = dot(-norm, dLight.lightDir);
diffuseFactor = max(diffuseFactor, 0.0);
vec3 diffuseColor = dLight.lightCol * diffMapColor  * diffuseFactor;

//Specular
vec3 halfWay = normalize(-dLight.lightDir + viewDir);
float specFactor = pow(max(dot(norm,halfWay),0.0),shine);
vec3 specluarColor = dLight.lightCol * specFactor  * specularStrength * texture(mat.specularTexture,texCoords).x;

vec3 result = ambientColor +(1.0 - shadow) * (diffuseColor + specluarColor);
return result;
}

vec3 RimLight(vec3 norm, vec3 viewDir)
{

float rim = 1.0 - max(dot(viewDir,norm),0.0);
rim = clamp(rim - 0.5,0.0,1.0);

vec3 color = vec3(1.0,0.0,0.0) * rim;

return color;


}


vec2 ParallaxMapping(vec2 texCoords,vec3 viewDir)
{
float height = texture(mat.dispMap,texCoords).r;
return texCoords - (viewDir.xy) * (height * 0.0175);
}


void main()
{    	
    vec4 posLS = lightSpaceMatrix * vec4(posWS,1.0);
    float shadow = calcShadow(posLS) * 0.4;

    vec3 norm;

    vec2 texCoords = uv;
    vec3 viewDir = normalize(viewPos - posWS);

    if(toggleDispMap)
    {
    texCoords = ParallaxMapping(uv,viewDir);

    }

    if(toggleNormalMap)
    {
    norm = texture(mat.normalMap,texCoords).xyz;
    norm = norm*2.0-1.0;
    norm = normalize(TBN * norm);
    }
    else
    {
    norm = normalize(normal);
    }

    vec3 result;
    if(selectedLight == 0)
    {
        result =  DirectionalLight(norm,viewDir,texCoords,shadow);
    }
    else if(selectedLight == 1)
    {
        result =  PointLight(norm,viewDir,texCoords);
    }
    else if(selectedLight == 2)
    {
        result =  SpotLight(norm,viewDir);
    }

    FragColor = vec4(result, 1.0f);
    //float brightness = (result.x + result.y + result.z) /3;
    float brightness = max(max(result.r,result.g),result.b);
    if(brightness > 0.7)
        brightColor = FragColor;
    else
        brightColor = vec4(vec3(0.0),1.0);
}

