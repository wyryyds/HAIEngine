#type vertex
#version 440 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;

out vec2 TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    TexCoords = a_TexCoords;

    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texturel;

void main()
{
    FragColor = texture(texturel, TexCoords);
} 