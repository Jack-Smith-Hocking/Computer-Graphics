// classic Phong fragment shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 Ia; // Ambient light colour
uniform vec3 colour;

uniform vec3 Ka; // Ambient material colour
uniform vec3 Kd; // Diffuse material colour
uniform vec3 Ks; // Specular material colour

uniform vec3 CameraPosition; // Position of the camera

uniform float specularPower; // Material specular power

uniform vec3 LightPosition;

uniform sampler2D diffuseTexture;

out vec4 FragColour;

void main() 
{
	vec3 lightDirection = vec3(vPosition) - LightPosition;

	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(lightDirection);

	// calculate lambert term (negate light direction)
	float lambertTerm = max( 0, min( 1, dot( N, -L ) ) );

	// Calculate view vector and reflection vector
	vec3 V = normalize( CameraPosition - vPosition.xyz );
	vec3 R = reflect( L, N );

	// Calculate specular term
	float specularTerm = pow( max( 0, dot( R, V ) ), specularPower);

	// Calculate each colour property
	vec3 ambient = Ia * Ka;
	vec3 diffuse = colour * Kd * lambertTerm;
	vec3 specular = colour * Ks * specularTerm;

	// output lambert as grayscale
	FragColour = vec4( ambient + diffuse + specular, 1 ) + texture( diffuseTexture, vTexCoord );
}