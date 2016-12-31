#version 130

out vec3 eyeSpace;
out vec3 normal;

void main() {
	eyeSpace = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
