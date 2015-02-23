#include <tuple>
#include <cmath>

using std::tuple;

//
// A General utility for linear algebra operations on vectors and matrices
//

typedef tuple<float, float, float> VECTOR3D;

/** Returns the cross product of 2 vectors */
VECTOR3D crossProduct(float v1X, float v1Y, float v1Z,
					  float v2X, float v2Y, float v2Z)
{
	return std::make_tuple<float, float, float>(v1Y * v2Z - v1Z * v2Y,
												v1Z * v2X - v1X * v2Z,
												v1X * v2Y - v1Y * v2X);
}

/** Returns the vector with a normalized length of 1 */
VECTOR3D normalizeVector(float vX, float vY, float vZ)
{
	float vectorLength = sqrt(vX*vX + vY*vY + vZ*vZ);

	return std::make_tuple<float, float, float>(vX / vectorLength,
												vY / vectorLength,
												vZ / vectorLength);
}

/** Returns the vector with a normalized length of 1 */
VECTOR3D normalizeVector(tuple<float, float, float> v)
{
	return normalizeVector(std::get<0>(v), std::get<1>(v), std::get<2>(v));
}

/** Calculates the normal vector perpendicular to the triangle formed by the 3 given points. */
VECTOR3D calculateNormal(float v1X, float v1Y, float v1Z,
						 float v2X, float v2Y, float v2Z,
						 float v3X, float v3Y, float v3Z)
{
	float vecAx = v2X - v1X;
	float vecAy = v2Y - v1Y;
	float vecAz = v2Z - v1Z;

	float vecBx = v3X - v1X;
	float vecBy = v3Y - v1Y;
	float vecBz = v3Z - v1Z;

	auto perpendicularVector = crossProduct(vecAx, vecAy, vecAz, vecBx, vecBy, vecBz);

	return normalizeVector(perpendicularVector);
}

/** Calculates the normal vector for the vertex by combining the normal vectors of all adjacent surfaces. */
VECTOR3D calculateVertexNormal(std::initializer_list<VECTOR3D> adjacentSurfaceVectors)
{
	VECTOR3D vertexNormal = std::make_tuple<float, float, float>(0.0f, 0.0f, 0.0f);

	for (auto surfaceVector : adjacentSurfaceVectors)
	{
		std::get<0>(vertexNormal) = std::get<0>(vertexNormal) + std::get<0>(surfaceVector);
		std::get<1>(vertexNormal) = std::get<1>(vertexNormal) + std::get<1>(surfaceVector);
		std::get<2>(vertexNormal) = std::get<2>(vertexNormal) + std::get<2>(surfaceVector);
	}

	return normalizeVector(vertexNormal);
}