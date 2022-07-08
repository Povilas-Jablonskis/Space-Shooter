#version 330 core
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform vec4 color;
uniform vec4 spriteCoordinates;

void main()
{
	vec2 spriteUV = spriteCoordinates.xy + (texCoord * spriteCoordinates.zw);
	gl_FragColor = texture(ourTexture, spriteUV) * color;
}