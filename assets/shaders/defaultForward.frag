#version 330

in vec2 fragTexCoord;
in vec3 fragLight;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 color;

void main()
{
	const float gamma = 1.0 / 2.2;
	vec4 texelColor = texture(texture0, fragTexCoord) * colDiffuse;
	texelColor.rgb *= fragLight;

	color.rgb = pow(texelColor.rgb, vec3(gamma));
	color.a = texelColor.a;
}
