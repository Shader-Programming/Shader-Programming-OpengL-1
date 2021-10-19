
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 posWS;


void main()
{  
    gl_Position = projection * view * model*vec4(aPos,1.0);
    posWS = (model * vec4(aPos,1.0)).xyz;
    normal = aNormals ;
    normal = (model * vec4(aNormals,0.0)).xyz;

}