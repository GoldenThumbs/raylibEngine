#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;

uniform mat4 mvp;
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matNormal;

out vec3 fragNormal;

void main()
{
	fragNormal = normalize(vec3((matView * matNormal) * vec4(vertexNormal, 0.0)));
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}
