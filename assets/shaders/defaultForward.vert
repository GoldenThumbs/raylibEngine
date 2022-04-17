#version 330

#define MAX_LIGHTS 64

struct SKF_Light
{
	vec3 position;
	vec3 direction;
	vec4 color;
	float radius;
	int type;
};

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;

uniform mat4 mvp;
uniform mat4 matModel;
uniform mat4 matNormal;

uniform SKF_Light lights[MAX_LIGHTS];

out vec2 fragTexCoord;
out vec3 fragLight;

void main()
{
	fragTexCoord = vertexTexCoord;

	vec3 wPos = vec3(matModel * vec4(vertexPosition, 1.0));
	vec3 wNrm = normalize(vec3(matNormal * vec4(vertexNormal, 0.0)));

	vec3 lightCol = vec3(0.0);
	for (int i=0; i<MAX_LIGHTS; i++)
	{
		SKF_Light light = lights[i];
		if (light.type != 0)
		{
			float atten = 1.0;
			vec3 wLPos;
			if (light.type > 1)
			{
				vec3 wLPosRaw = light.position - wPos;
				float lDist = dot(wLPosRaw, wLPosRaw);
				float tmp = clamp(1.0 - lDist/(light.radius*light.radius), 0.0, 1.0);
				atten = tmp*tmp;

				float rRad = 1.0 / light.radius;
				wLPos = normalize(wLPosRaw) * rRad;
			} else
				wLPos = light.direction;

			float NdL = clamp(dot(wNrm, wLPos), 0.0, 1.0) * atten;

			
			lightCol += light.color.rgb * NdL;
		}
	}
	fragLight = lightCol;

	gl_Position = mvp * vec4(vertexPosition, 1.0);
}