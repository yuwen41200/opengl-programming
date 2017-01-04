#version 150 compatibility

uniform uint segCount;
flat in uint segNo;

void main() {
	vec4 firstSegColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 finalSegColor = vec4(0.3f, 0.3f, 0.7f, 0.0f);
	float weight = segNo / (segCount - 1.0f);
	gl_FragColor = mix(firstSegColor, finalSegColor, weight);
}
