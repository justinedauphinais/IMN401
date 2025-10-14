#version 460

in vec3 vL;
in vec3 vV;
in vec3 vN;

uniform vec3 objectColor;
uniform float ka;
uniform float kd;
uniform float ks;
uniform float s;

out vec4 FragColor;

void main() {
    vec3 N = normalize(vN);
    vec3 L = normalize(vL); 
    vec3 V = normalize(vV);

    float NL = max(dot(N, L), 0.0);
    vec3 ambient = ka * objectColor;
    vec3 diffuse = kd * NL * objectColor;

    vec3 R = reflect(-L, N);
    float RV = max(dot(R, V), 0.0);
    float specPow = pow(RV, s);
    vec3 specular = ks * specPow * vec3(1.0);

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}