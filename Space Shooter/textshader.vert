#version 330 core
layout (location = 0) in vec4 coord;
out vec2 texCoord;

uniform mat4 projection;
uniform float animscX;
uniform float animscY;
uniform float curranim;

void main(void) 
{
	gl_Position = projection * vec4(coord.xy, 0, 1);
	texCoord = coord.zw;

	if(curranim > 0.0f)
	{
		int modulus = int(curranim - 1) % int(animscX);
		int fullinteger = int(curranim - 1) / int(animscX);

		texCoord.x /= animscX;
		texCoord.x += (float(modulus) / animscX);
		texCoord.y /= animscY;
		texCoord.y += (float(fullinteger) / animscY);
	}
}