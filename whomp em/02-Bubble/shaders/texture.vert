#version 330

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 view;
uniform vec2 texCoordDispl;

in vec2 position;
in vec2 texCoord;
out vec2 texCoordFrag;

void main()
{
	// Transforma la posición del vértice usando las matrices de modelview, view y projection
	gl_Position = projection * view * modelview * vec4(position, 0.0, 1.0);
	
	// Pasa las coordenadas de textura al fragment shader
	texCoordFrag = texCoord + texCoordDispl;
}

