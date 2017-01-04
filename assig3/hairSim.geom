#version 150 compatibility

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

uniform float segLen;
uniform uint segCount;
uniform float gravityY;
flat out uint segNo;

in Vertices {
	vec3 normal;
} vertices[];

void main() {
	vec3 gravity = vec3(0.0f, gravityY, 0.0f);

	for (int i = 0; i < gl_in.length(); ++i) {
		vec4 startPosition = gl_in[i].gl_Position;
		vec3 startNormal = vertices[i].normal;

		for (uint j = uint(0); j < segCount; ++j) {
			gl_Position = gl_ProjectionMatrix * startPosition;
			EmitVertex();

			startNormal = normalize(startNormal + gravity);
			startPosition += vec4(startNormal, 0.0f) * segLen;

			gl_Position = gl_ProjectionMatrix * startPosition;
			EmitVertex();

			segNo = j;
			EndPrimitive();
		}
	}
}
