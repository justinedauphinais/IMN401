#version 460

uniform float time;

out gl_PerVertex
{
	vec4 gl_Position;
};
out vec2 vertex_pos;

layout (location = 0) in vec3 position;

void main()
{
	vec3 pos = position * (cos(time)*0.5f+0.6f);
	vertex_pos = position.xy;	//pour que le cercle devienne pas juste blanc quand il est petit
	//vertex_pos = pos.xy;		//comme l'exemple
	pos.x += sin(2*time)*0.9f;
	pos.y += cos(time)*0.9f;

	gl_Position = vec4(pos, 1.0);
}