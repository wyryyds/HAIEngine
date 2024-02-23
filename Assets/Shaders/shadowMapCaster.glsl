#type vertex
#version 440 core

layout(location = 0) in vec3 a_position;

uniform mat4 u_lightSpaceMatrix;
uniform mat4 u_transform;

void main()
{
    gl_Position = u_lightSpaceMatrix * u_transform * vec4(a_position, 1.0);
}

#type fragment
#version 440 core

void main()
{
    // gl_FragDepth = gl_FragCoord.z;
} 