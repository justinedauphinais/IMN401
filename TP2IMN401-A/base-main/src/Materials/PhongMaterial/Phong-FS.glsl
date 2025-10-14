#version 460

in vec3 vertexColor;
layout(location = 0) out vec4 Color;

in vec3 L;
in vec3 N;
in vec3 R;
in vec3 V;

uniform float kd;
uniform float ka;
uniform float ks;
uniform float s;

vec3 lightColor = vec3(0.5, 0.5, 0.5);

void main() {
    // Intensité diffuse 
    float Id = kd * max(dot(N, L), 0.0f);

    // Intensité spéculaire
    float Is = ks * pow(max(dot(R, V), 0.0f), s); 

    vec3 color = ka * lightColor * vertexColor + (Id * vertexColor) * lightColor;

    Color = vec4(color, 1.0);
}