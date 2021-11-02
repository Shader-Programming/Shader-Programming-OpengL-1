#version 410 core

out vec4 FragColor;

in vec3 normal;
in vec3 posWS;
in vec2 uv;

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

float ambientFactor = 0.5;
float shine = 64;
float specularStrength = 0.2;



vec3 SpotLight(vec3 norm, vec3 viewDir)
{
    float dist = length(sLight.position - posWS);
    float attn = 1.0/(sLight.Kc + sLight.Kl * dist + (sLight.Ke * (dist*dist)));
    vec3 sLightDir = normalize(sLight.position - posWS);


    //diffuse
    float diffuseFactor = dot(norm, sLightDir);
    diffuseFactor = max(diffuseFactor, 0.0);
    vec3 diffuseColor = sLight.color*objectCol*diffuseFactor;
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
    float dist = length(pLight.position - posWS);
    float attn = 1.0/(pLight.Kc + pLight.Kl * dist + (pLight.Ke * (dist*dist)));
    vec3 pLightDir = normalize(pLight.position - posWS);

    //Ambient
    vec3 ambientColor = pLight.color * objectCol * ambientFactor;
    ambientColor = ambientColor * attn;

    //diffuse
    float diffuseFactor = dot(norm, pLightDir);
    diffuseFactor = max(diffuseFactor, 0.0);
    vec3 diffuseColor = pLight.color*objectCol*diffuseFactor;
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

vec3 DirectionalLight(vec3 norm, vec3 viewDir)
{
//Ambient
vec3 ambientColor = lightCol * objectCol * ambientFactor;

//diffuse
float diffuseFactor = dot(norm, -lightDir);
diffuseFactor = max(diffuseFactor, 0.0);
vec3 diffuseColor = lightCol*objectCol*diffuseFactor;

//specular
vec3 reflectDir = -lightDir;
float specularFactor = dot(viewDir, reflectDir);
specularFactor = max(specularFactor, 0.0);
specularFactor = pow(specularFactor, shine);
vec3 specluarColor = lightCol * specularFactor * specularStrength;

vec3 result = ambientColor + diffuseColor + specluarColor;
return result;
}

vec3 RimLight(vec3 norm, vec3 viewDir)
{

float rim = 1.0 - max(dot(viewDir,norm),0.0);
vec3 color = vec3(1.0,0.5,0.0) * rim;
return color;


}



void main()
{    	
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - posWS);

    vec3 result =  SpotLight(norm,viewDir) + PointLight(norm,viewDir) ;

    FragColor = vec4(result, 1.0f);
}

