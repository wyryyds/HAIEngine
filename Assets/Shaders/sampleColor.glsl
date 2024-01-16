#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 Normal;	
out vec2 v_TexCoord;

void main()
{
    Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	v_TexCoord = a_TexCoords;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0) + vec4(0.1*Normal, 0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
           
in vec2 v_TexCoord;

void main()
{
	color = vec4(0.04, 0.28, 0.26, 1.0);
}