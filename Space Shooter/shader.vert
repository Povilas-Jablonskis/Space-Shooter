#version 330 core
#extension GL_ARB_separate_shader_objects : enable
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
out vec2 TexCoord;
out vec3 fragColor;
uniform vec2 size;
uniform float angle;
uniform vec2 givenposition;
uniform vec2 animsc;
uniform vec2 curranim;
uniform vec3 color;
void main()
{
	mat4 scale = 
	mat4
	(
		vec4( size.x, 0.0, 0.0, 0.0 ),
		vec4( 0.0, size.y, 0.0, 0.0 ),
		vec4( 0.0, 0.0,    1.0, 0.0 ),
		vec4( 0.0, 0.0,    0.0, 1.0 )
	);
	mat4 trans = 
	mat4
	(
		vec4( 1.0, 0.0, 0.0, -1.0 + givenposition.x * 2 ),
		vec4( 0.0, 1.0, 0.0, -1.0 + givenposition.y * 2 ),
		vec4( 0.0, 0.0, 1.0, 1.0                        ),
		vec4( 0.0, 0.0, 0.0, 1.0                        )
	);
	mat4 rotateX = 
	mat4
	(
		vec4( 1.0, 0.0,       0.0,        0.0 ),
		vec4( 0.0, cos(45.0), -sin(45.0), 0.0 ),
		vec4( 0.0, sin(45.0), cos(45.0),  0.0 ),
		vec4( 0.0, 0.0,       0.0,        1.0 )
	);
	mat4 rotateY = 
	mat4
	(
		vec4( cos(angle), 0.0, sin(angle),  0.0 ),
		vec4( 0.0,        1.0, 0.0,         0.0 ),
		vec4( -sin(angle),0.0, cos(angle),  0.0 ),
		vec4( 0.0,        0.0, 0.0,         1.0 )
	);
	mat4 rotateZ = 
	mat4
	(
		vec4( cos(45.0), -sin(45.0), 0.0,  0.0 ),
		vec4( sin(45.0), cos(45.0),  0.0,  0.0 ),
		vec4( 0.0,       0.0,        1.0,  0.0 ),
		vec4( 0.0,       0.0,        0.0,  1.0 )
	);
	fragColor = color;
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
	TexCoord.x /= animsc.x;
	TexCoord.x -= (curranim.x/animsc.x);
	TexCoord.y /= animsc.y;
	TexCoord.y += (curranim.y/animsc.y);
	gl_Position = vec4(position, 1.0f);
	gl_Position *= scale;
	gl_Position *= trans;
	gl_Position.xy += size.xy;
}