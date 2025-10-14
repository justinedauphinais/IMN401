#version 460

uniform vec3 color;

in vec2 vertex_pos;

layout(location = 0) out vec4 fragColor;

vec3 posToRGB( vec2 pos )
{
	float a = 0.5*atan(pos.y,pos.x)/3.14159;
	if (a < 0.0) a += 1.0 ; float l = length(pos);
	vec3 rgb = clamp( abs(mod(a*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
	return mix( vec3(1.0), rgb, l);
}

void main() {
	fragColor = vec4(posToRGB(vertex_pos), 1.0);
}