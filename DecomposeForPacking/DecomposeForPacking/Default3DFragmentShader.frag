varying vec4 vColor; // Stands for "material color"
varying vec3 vPosition_worldspace;
varying vec3 vLightDirection_cameraspace;
varying vec3 vNormalDirection_cameraspace;
varying vec3 vEyeDirection_cameraspace;

uniform vec3 uLightPosition; // Light position in world space (model is assumed to be identity matrix)

void main()
{
	// Constants for controlling the light-source color and intensity
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	float lightPower = 100.0f;
	vec3 materialSpecularColor = vec3(1.0, 1.0, 1.0);
	float specularLobeThickness = 5.0f; // The higher, the thinner

	vec3 normal = normalize(vNormalDirection_cameraspace); // Normal of the computed fragment, in camera space
	vec3 lightDirection = normalize(vLightDirection_cameraspace);  // Direction of the light (from the fragment to the light)

	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot(normal, lightDirection), 0, 1 );

	// Distance of the fragment from the light-source position
	float distance = length(uLightPosition - vPosition_worldspace);

	// Simulates light that comes from the light-source and hits the surface at a certain angle
	vec3 diffuseColor = vColor.xyz * lightColor * lightPower * cosTheta / (distance * distance) ;

	// A "hack" - simluates light reflected back from the wall (here the object simply illuminates light)
	vec3 ambientColor = vec3(0.3, 0.3, 0.3) * vColor.xyz;

	// Eye vector (towards the camera)
	vec3 eyeVector = normalize(vEyeDirection_cameraspace);

	// Direction in which the triangle reflects the light
	vec3 lightReflectionDirection = reflect(-lightDirection, normal);

	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot(eyeVector, lightReflectionDirection), 0, 1);
 
	// Specular : reflective highlight, like a mirror
	vec3 specularColor = materialSpecularColor * lightColor * lightPower *
						 pow(cosAlpha, specularLobeThickness) / (distance * distance);

	gl_FragColor = vec4(diffuseColor + ambientColor + specularColor, vColor.a);
}