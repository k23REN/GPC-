#version 330
#define saturate(x) clamp(x,0.0f,1.0f)

in vec2 fragTexCoord;

in vec3 fragNormal;

in vec3 fragWorldPos;

out vec4 outColor;

uniform sampler2D uTexture;

struct DirectionalLight
{
	vec3 mDirection;
	vec3 mDiffuseColor;
	vec3 mSpecColor;
};

struct PointLight
{
  vec3 mPosition;
  vec3 mDiffuseColor;
  vec3 mSpecColor;
  float mRadius;
};

uniform vec3 uCameraPos;
uniform float uSpecPower;
uniform vec3 uAmbientLight;

uniform DirectionalLight uDirLight;
uniform PointLight uPointLight[4];

void main()
{
	vec3 N = normalize(fragNormal);
	vec3 L = normalize(-uDirLight.mDirection);
	vec3 V = normalize(uCameraPos - fragWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);

	Phong = vec3(0.0f,0.0f,0.0f);

	for(int i = 0; i < 4; i++){
		vec3 dir = uPointLight[i].mPosition - fragWorldPos;
		float dist = length(dir);
		dir /= dist;

		if(dist < uPointLight[i].mRadius){
			//‹——£‚Ì”ä—¦‚ðŒvŽZ
			float atte = 1.0f - saturate(dist / uPointLight[i].mRadius);
			atte *= atte;

			//ŠgŽUŒõŒvŽZ
			{
				float lightDiffuse = dot(N, dir);
				lightDiffuse = saturate(lightDiffuse);

				lightDiffuse *= atte; 

				//³‹K‰»Lambert
				lightDiffuse /= 3.1415926535;

				Phong += (uPointLight[i].mDiffuseColor * lightDiffuse);
			}

			//”½ŽËŒõŒvŽZ
			float specPower = 10.0f;
			//vec3 H = normalize(reflect(-dir,N));
			//float NdotH = saturate(dot(H, V));
			//float spec = pow(NdotH,specPower) * atte;

			vec3 H = normalize(dir + V);
			float NdotH = saturate(dot(N, H));
			float spec = pow(NdotH,specPower);
			spec *= ((specPower + 2) / (2 * 3.1415926535)) * atte * 0.5f;

			Phong += uPointLight[i].mSpecColor * spec;
		}
	}

    outColor += texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
