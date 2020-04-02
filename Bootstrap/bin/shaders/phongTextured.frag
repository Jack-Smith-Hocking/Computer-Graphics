// classic Phong fragment shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 Ia; // Ambient light colour

uniform vec3 LightColour[8]; 
uniform vec3 LightPosition[8];
uniform float LightIntensity[8];

uniform vec3 Ka; // Ambient material colour
uniform vec3 Kd; // Diffuse material colour
uniform vec3 Ks; // Specular material colour

uniform vec3 CameraPosition; // Position of the camera

uniform float specularPower; // Material specular power

uniform sampler2D diffuseTexture;

out vec4 FragColour;

vec3 DiffuseLight(vec3 N, vec3 lightDir, int index)
{
	vec3 L = normalize(lightDir);

	return  max( 0, min( 1, dot( N, -L ) ) ) * LightColour[index] * LightIntensity[index];
}

vec3 SpecularLight(vec3 N, vec3 lightDir, int index)
{
	// Calculate view vector and reflection vector
	vec3 L = normalize(lightDir);
	vec3 V = normalize( CameraPosition - vPosition.xyz );
	vec3 R = reflect( L, N );

	return pow( max( 0, dot( R, V ) ), specularPower) * vec3(1, 1, 1) * LightIntensity[index];
}

void main() 
{
	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);

	vec3 lightDirection= vec3(0, 0, 0);

	// calculate lambert term (negate light direction)
	vec3 lambertTerm = vec3(0, 0, 0);
	// Calculate specular term
	vec3 specularTerm = vec3(0, 0, 0);

	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	for (int i = 0; i < 8; i++)
	{
		lightDirection =  normalize(vPosition.xyz - LightPosition[i]);

		lambertTerm += DiffuseLight(N, lightDirection, i);
		specularTerm += SpecularLight(N, lightDirection, i); 

		diffuse += LightColour[i] * Kd * lambertTerm;
		specular += LightColour[i] * Ks * specularTerm;
	}

	// Calculate each colour property
	vec3 ambient = Ia * Ka;


	// output lambert as grayscale
	FragColour = vec4( ambient + diffuse + specular, 1 ) + texture( diffuseTexture, vTexCoord );
}