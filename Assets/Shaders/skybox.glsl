#type vertex
#version 440 core

layout(location = 0) in vec3 a_position;

uniform mat4 u_viewProjection;
	
out vec2 v_texCoords;

void main()
{
	v_texCoord = a_position;
    vec4 pos = u_viewProjection * vec4(a_position, 1.0);
	gl_Position = pos.xyww;
}

#type fragment
#version 440 core

layout(location = 0) out vec4 fragcolor;
           
in vec2 v_texCoords;

uniform samplerCube skybox;

void main()
{
	fragcolor = texture(skybox, v_texCoords);
}