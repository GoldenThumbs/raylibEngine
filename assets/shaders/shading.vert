#version 330

in vec3 vertexPosition;

uniform mat4 mvp;

out vec2 fragTexCoord;

void main()
{
	gl_Position = mvp * vec4(vertexPosition, 1.0);

	fragTexCoord = gl_Position.xy / gl_Position.w;
}
