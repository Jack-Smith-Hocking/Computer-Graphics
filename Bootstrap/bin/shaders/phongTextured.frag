// classic Phong fragment shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 LightColour[8]; 
uniform vec3 LightPosition[8];
uniform float LightIntensity[8];

uniform int NumLights;

uniform vec3 Ka; // Ambient material colour
uniform vec3 Kd; // Diffuse material colour
uniform vec3 Ks; // Specular material colour

uniform vec3 CameraPosition; // Position of the camera

uniform float specularPower; // Material specular power

uniform sampler2D diffuseTexture;

out vec4 FragColour;

vec3 DiffuseLight(vec3 N, vec3 lightDir, vec3 intensity)
{
	vec3 L = normalize(lightDir);

	return  max( 0, min( 1, dot( N, -L ) ) ) * intensity;
}

vec3 SpecularLight(vec3 N, vec3 lightDir, vec3 intensity)
{
	// Calculate view vector and reflection vector
	vec3 L = normalize(lightDir);
	vec3 V = normalize( CameraPosition - vPosition.xyz );
	vec3 R = reflect( L, N );

	return pow( max( 0, dot( R, V ) ), specularPower) * intensity;
}

void main() 
{
	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);

	vec3 lightDirection= vec3(0, 0, 0);
	vec3 toLight = vec3(0, 0, 0);
	vec3 intensity = vec3(0, 0, 0);

	// calculate lambert term (negate light direction)
	vec3 lambertTerm = vec3(0, 0, 0);
	// Calculate specular term
	vec3 specularTerm = vec3(0, 0, 0);

	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	float distanceSqrd = 0;
	float attenuation = 0;

	vec3 ambient = vec3(0, 0, 0);

	for (int i = 0; i < NumLights; i++)
	{
		toLight = vec3(vPosition) - LightPosition[i];
		lightDirection =  normalize(toLight);

		distanceSqrd = dot(toLight, toLight);
		attenuation = 1.0 / distanceSqrd;

		intensity = (LightColour[i] * LightIntensity[i]) * attenuation;

		lambertTerm = DiffuseLight(N, lightDirection, intensity);
		specularTerm = SpecularLight(N, lightDirection, intensity); 

		diffuse += LightColour[i] * Kd * lambertTerm;
		specular += LightColour[i] * Ks * specularTerm;

		// Calculate each colour property
		ambient += LightColour[i] * Ka;
	}

	// output lambert as grayscale
	FragColour = vec4( ambient + diffuse + specular, 1 ) + texture( diffuseTexture, vTexCoord );
}