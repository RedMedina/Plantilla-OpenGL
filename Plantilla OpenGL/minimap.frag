#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

uniform sampler2D myTextureSampler;

void main(){
	// Output color = color of the texture at the specified UV

	vec4 colorFinal = texture( myTextureSampler, UV );

	color = colorFinal;
}
