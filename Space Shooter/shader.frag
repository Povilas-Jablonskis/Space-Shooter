#version 330 core
in vec2 TexCoord;
in vec4 fragColor;
in float outMode;
uniform sampler2D ourTexture;
void main()
{
	if( outMode == 1.0f )
	{
		gl_FragColor = texture(ourTexture, TexCoord);
	}
	else
	{
		gl_FragColor = fragColor;
	}
}