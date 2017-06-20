#version 330 core
in vec2 TexCoord;
in vec3 fragColor;
uniform sampler2D ourTexture;
void main()
{
	if( fragColor.x == 0 && fragColor.y == 0 && fragColor.z == 0 )
	{
		gl_FragColor = texture(ourTexture, TexCoord);
	}
	else
	{
		gl_FragColor = vec4(fragColor.xyz, 1.0);
	}
}