attribute vec3 aPosition; // Position is in model space
attribute vec3 aNormal; // Normal is in model space
attribute vec4 aColor;

uniform mat4 uMVP; // Model view projection matrix
uniform mat4 uM; // Model matrix
uniform mat4 uV; // View matrix
uniform vec3 uLightPosition; // Light position in world space (model is assumed to be identity matrix)

varying vec4 vColor;
varying vec3 vPosition_worldspace;
varying vec3 vLightDirection_cameraspace;
varying vec3 vNormalDirection_cameraspace;
varying vec3 vEyeDirection_cameraspace;

void main()
{
	vec4 posVec = vec4(aPosition, 1.0); // 1.0 for position vector
	gl_Position = uMVP * posVec;
	vec3 position_worldspace = (uM * posVec).xyz; // Position of the vertex, in worldspace : M * position
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 position_cameraspace = (uV * uM * posVec).xyz;
	vec3 eyeDirection_cameraspace = vec3(0, 0, 0) - position_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightPosition_cameraspace = ( uV * vec4(uLightPosition, 1)).xyz;
	vec3 lightDirection_cameraspace = lightPosition_cameraspace + eyeDirection_cameraspace;

	// Normal of the the vertex, in camera space
	// Only correct if ModelMatrix does not scale the model, should use its inverse transpose if not.
	vec3 normal_cameraspace = ( uV * uM * vec4(aNormal, 0)).xyz; // This is a direction vector

	vColor = aColor;
	vPosition_worldspace = position_worldspace;
	vLightDirection_cameraspace = lightDirection_cameraspace;
	vNormalDirection_cameraspace = normal_cameraspace;
	vEyeDirection_cameraspace = eyeDirection_cameraspace;
}