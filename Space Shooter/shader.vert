#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform float animscX;
uniform float animscY;
uniform float curranim;

void main()
{
	texCoord = vec2(vertex.z, 1.0 - vertex.w);
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);

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