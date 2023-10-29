#type vertex
#version 440 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 aNormal;

out vec3 FragPos;  
out vec3 Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
    Normal = aNormal;

    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 440 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform int sepcness;
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), sepcness);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}