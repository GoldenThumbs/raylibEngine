#version 330

in vec3 fragNormal;

out vec4 colorBuf;
out vec4 specBuf;
out vec4 normBuf;

void main()
{
	colorBuf = vec4(0.75, 0.75, 0.25, 1.0);
	specBuf = vec4(0.5, 0.5, 0.5, 1.0);
	normBuf = vec4(fragNormal, 1.0);
}
