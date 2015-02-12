attribute vec3 aPosition;
attribute vec4 aColor;

uniform mat4 uMVP; // Model view projection matrix

varying vec4 vColor;

void main()
{
	vec4 posVec = vec4(aPosition, 1.0); // 1.0 for position vector
	gl_Position = uMVP * posVec;
	vColor = aColor;
}