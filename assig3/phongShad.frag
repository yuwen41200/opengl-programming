#version 130

uniform int lightCount;
uniform sampler2D texColor;
in vec3 eyeSpace;
in vec3 normal;

void main() {
	vec4 ia, id, is;
	vec4 it = vec4(0.0f);
	vec3 n = normalize(normal);
	vec3 v = normalize(-eyeSpace);

	for (int i = 0; i < lightCount; ++i) {
		vec3 l = normalize(gl_LightSource[i].position.xyz - eyeSpace);
		vec3 r = normalize(reflect(-l, n));

		ia = gl_LightSource[i].ambient * gl_FrontMaterial.ambient;
		id = gl_LightSource[i].diffuse * gl_FrontMaterial.diffuse * max(dot(n, l), 0.0f);
		is = gl_LightSource[i].specular * gl_FrontMaterial.specular * \
		     pow(max(dot(r, v), 0.0f), gl_FrontMaterial.shininess);

		it += ia + id + is;
	}

	gl_FragColor = vec4(it) * texture(texColor, gl_TexCoord[0].xy);
}
