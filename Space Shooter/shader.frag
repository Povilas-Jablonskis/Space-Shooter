#version 330 core
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform vec4 color;
uniform float renderMode;

void main()
{
	if( renderMode == 0.0 )
	{
		gl_FragColor = color;
	}
	else if( renderMode == 1.0 )
	{
		gl_FragColor = texture(ourTexture, texCoord);
	}
	else if( renderMode == 2.0 )
	{
		gl_FragColor = texture(ourTexture, texCoord) * color;
	}
}