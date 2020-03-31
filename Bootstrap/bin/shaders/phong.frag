// classic Phong fragment shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform vec3 Ia; // Ambient light colour
uniform vec3 Id; // Diffuse light colour
uniform vec3 Is; // Specular light colour

uniform vec3 Ka; // Ambient material colour
uniform vec3 Kd; // Diffuse material colour
uniform vec3 Ks; // Specular material colour

uniform vec3 CameraPosition; // Position of the camera

uniform float specularPower; // Material specular power

uniform vec3 LightDirection;

uniform sampler2D diffuseTexture;

out vec4 FragColour;

void main() 
{
	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	// calculate lambert term (negate light direction)
	float lambertTerm = max( 0, min( 1, dot( N, -L ) ) );

	// Calculate view vector and reflection vector
	vec3 V = normalize( CameraPosition - vPosition.xyz );
	vec3 R = reflect( L, N );

	// Calculate specular term
	float specularTerm = pow( max( 0, dot( R, V ) ), specularPower);

	// Calculate each colour property
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * lambertTerm;
	vec3 specular = Is * Ks * specularTerm;

	// output lambert as grayscale
	FragColour = vec4( ambient + diffuse + specular, 1 ) + texture( diffuseTexture, vTexCoord );
}