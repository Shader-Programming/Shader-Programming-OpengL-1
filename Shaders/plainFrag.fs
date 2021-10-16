#version 410 core

out vec4 FragColor;

in vec3 normal;
in vec3 posWS;

uniform vec3 lightCol;
uniform vec3 lightDir;
uniform vec3 objectCol;
uniform vec3 viewPos;

float ambientFactor = 0.5;
float shine = 64;
float specularStrength = 0.2;

void main()
{    	
    vec3 norm = normalize(normal);
    //Ambient
    vec3 ambientColor = lightCol * objectCol * ambientFactor;

    //diffuse
    float diffuseFactor = dot(norm, -lightDir);
    diffuseFactor = max(diffuseFactor, 0.0);
    vec3 diffuseColor = lightCol*objectCol*diffuseFactor;

    //specular
    vec3 viewDir = normalize(viewPos - posWS);
    vec3 reflectDir = -lightDir;
    float specularFactor = dot(viewDir, reflectDir);
    specularFactor = max(specularFactor, 0.0);
    specularFactor = pow(specularFactor, shine);
    vec3 specluarColor = lightCol * specularFactor * specularStrength;

    vec3 result = ambientColor + diffuseColor + specluarColor;
    FragColor = vec4(result, 1.0f);
}

