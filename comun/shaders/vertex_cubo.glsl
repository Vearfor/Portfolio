//-----------------------------------------------------------------------------
#version 330 core

// El tipo de malla que hemos definido: eVer_PosTex: 
layout (location = 0) in vec3 pos;			// Atributo 0: 3 de posiciones
layout (location = 1) in vec2 texCoord;		// Atributo 1: 2 de texturas

out vec2 TexCoord;

uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	TexCoord = texCoord;
}
//-----------------------------------------------------------------------------
