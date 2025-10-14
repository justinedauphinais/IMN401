#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

uniform float u_time;
uniform float u_amp; 
uniform float u_freq;

out vec3 vertexColor;

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout(location = 0) in vec3 Position;
layout(location = 2) in vec3 Normal;

uniform vec3 posLum;
uniform vec3 posCam;

out vec3 L;
out vec3 N;
out vec3 V;

void main() {
    L = posLum - Position;
    N = Normal;
    V = posCam - Position;

    //float disp = u_amp * sin(6.28318530718 * u_freq * u_time);
    vec3 displacedPos = Position;// + normalize(Normal) * disp;

    gl_Position = Proj * View * Model * vec4(displacedPos, 1.0);
}