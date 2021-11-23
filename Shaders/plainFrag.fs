#version 410 core

out vec4 FragColor;

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


uniform vec3 lightCol;
uniform vec3 lightDir;
uniform vec3 objectCol;
uniform vec3 viewPos;
uniform pointLight pLight;
uniform spotLight sLight;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalMap;
uniform sampler2D dispMap;

uniform bool toggleNormalMap;
uniform bool toggleDispMap;

float ambientFactor = 0.5;
float shine = 64;
float specularStrength = 0.8;



vec3 SpotLight(vec3 norm, vec3 viewDir)
{
    vec3 diffMapColor = texture(diffuseTexture, uv).xyz;
    float dist = length(sLight.position - posWS);
    float attn = 1.0/(sLight.Kc + sLight.Kl * dist + (sLight.Ke * (dist*dist)));
    vec3 sLightDir = normalize(sLight.position - posWS);


    //diffuse
    float diffuseFactor = dot(norm, sLightDir);
    diffuseFactor = max(diffuseFactor, 0.0);
    vec3 diffuseColor = sLight.color*diffMapColor*diffuseFactor;
    diffuseColor = diffuseColor*attn;

    //specular
    vec3 reflectDir = -sLightDir;
    float specularFactor = dot(viewDir, reflectDir);
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, shine);
    vec3 specluarColor = sLight.color * specularFactor * specularStrength;
    specluarColor = specluarColor * attn;

    float theta = dot(-sLightDir, normalize(sLight.direction));
    float denom = (sLight.innerRad - sLight.outerRad);
    float illum = (theta - sLight.outerRad ) / denom;
    illum = clamp(illum, 0.0,1.0);
    
    diffuseColor =  diffuseColor * illum;
    specluarColor = specluarColor * illum;


    vec3 result =  diffuseColor + specluarColor;
    return result;
}

vec3 PointLight(vec3 norm, vec3 viewDir)
{
    vec3 diffMapColor = texture(diffuseTexture, uv).xyz;

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

    //specular
    vec3 reflectDir = -pLightDir;
    float specularFactor = dot(viewDir, reflectDir);
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, shine);
    vec3 specluarColor = pLight.color * specularFactor * specularStrength;
    specluarColor = specluarColor * attn;

    vec3 result = ambientColor + diffuseColor + specluarColor;
    return result;
}

vec3 DirectionalLight(vec3 norm, vec3 viewDir,vec2 texCoords)
{
    vec3 diffMapColor = texture(diffuseTexture, texCoords).xyz;

//Ambient
vec3 ambientColor = lightCol * diffMapColor * ambientFactor;

//diffuse
float diffuseFactor = dot(norm, -lightDir);
diffuseFactor = max(diffuseFactor, 0.0);
vec3 diffuseColor = lightCol*diffMapColor*diffuseFactor;

//specular
vec3 reflectDir = reflect(lightDir,norm);
float specularFactor = dot(viewDir, reflectDir);
specularFactor = max(specularFactor, 0.0);
specularFactor = pow(specularFactor, shine);
vec3 specluarColor = lightCol * specularFactor * specularStrength *texture(specularTexture,texCoords).x;

vec3 result = ambientColor + diffuseColor + specluarColor;
return result;
}

vec3 RimLight(vec3 norm, vec3 viewDir)
{

float rim = 1.0 - max(dot(viewDir,norm),0.0);
vec3 color = vec3(1.0,0.5,0.0) * rim;
return color;


}


vec2 ParallaxMapping(vec2 texCoords,vec3 viewDir)
{
float height = texture(dispMap,texCoords).r;
return texCoords - (viewDir.xy) * (height * 0.0175);
}


void main()
{    	
    
    vec3 norm;

    vec2 texCoords = uv;
    vec3 viewDir = normalize(viewPos - posWS);

    if(toggleDispMap)
    {
    texCoords = ParallaxMapping(uv,viewDir);

    }

    if(toggleNormalMap)
    {
    norm = texture(normalMap,texCoords).xyz;
    norm = norm*2.0-1.0;
    norm = normalize(TBN * norm);
    }
    else
    {
    norm = normalize(normal);
    }





    //vec3 result =  SpotLight(norm,viewDir) + PointLight(norm,viewDir) ;
    vec3 result =  DirectionalLight(norm,viewDir,texCoords) ;


    FragColor = vec4(result, 1.0f);
}

