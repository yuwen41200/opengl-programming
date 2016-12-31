#version 130

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

uniform float segLen;
uniform uint segCount;
uniform float gravityY;

in Vertices {
	vec3 normal;
} vertices[];

void main() {
	for (int i = 0; i < gl_in.length(); ++i) {
		gl_Position = gl_ProjectionMatrix * gl_in[i].gl_Position;
		EmitVertex();

		gl_Position = gl_ProjectionMatrix * (gl_in[i].gl_Position + vec4(vertices[i].normal, 0.0f) * segLen);
		EmitVertex();

		EndPrimitive();
	}
}
