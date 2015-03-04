#include "Arcball.h"
#include <algorithm>

Arcball::Arcball() : _isArcballOn(false), _lastXPos(0), _lastYPos(0), _currXPos(0), _currYPos(0), _angle(0)
{
	_axis_in_camera_coord = VECTOR_3D(1.0f);
	_prevModelRotationMatrix =  _lastModelRotationMatrix = std::make_shared<MATRIX_4X4>();
}


Arcball::~Arcball()
{
}

void Arcball::handleMousePress(bool isLeftMousePressed, int x, int y)
{
	if (isLeftMousePressed)
	{
		if (!_isArcballOn)
		{
			_isArcballOn = true;
			_lastXPos = _currXPos = x;
			_lastYPos = _currYPos = y;
		}
	}
	else
	{
		_isArcballOn = false;
		_prevModelRotationMatrix = _lastModelRotationMatrix;

		// Make sure subsequent rotations while arcball is off cause no new rotation
		_angle = 0;
		_axis_in_camera_coord = VECTOR_3D(1.0f);
	}
}

void Arcball::handleMouseMove(int x, int y)
{
	if (_isArcballOn) {  // if left button is pressed
		_currXPos = x;
		_currYPos = y;

		// Calculate vectors from the ball's center to the press point (a) and current point (b)
		VECTOR_3D va = getArcballVector(_lastXPos, _lastYPos);
		VECTOR_3D vb = getArcballVector(_currXPos, _currYPos);

		// Angle between va and vb is calculated using the dot product (va and vb are assumed to be of length 1)
		// We use min to fix small precision errors and make sure we cap at 1.0
		_angle = acos(std::min(1.0f, glm::dot(va, vb)));
		_axis_in_camera_coord = VECTOR_3D(glm::cross(va, vb));
	}
}

void Arcball::updateScreenSize(int screenWidth, int screenHeight)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
}

VECTOR_3D Arcball::getArcballVector(int x, int y)
{
	// Screen coordinates to [-1,1] coordinates
	VECTOR_3D p = VECTOR_3D(1.0*x / _screenWidth * 2 - 1.0,
							1.0*y / _screenHeight * 2 - 1.0,
							0);
	p.y = -p.y; // Normalize y axis because it is inverted for OpenGL

	float vecOPLengthsquared = p.x * p.x + p.y * p.y;
	if (vecOPLengthsquared <= 1 * 1)
		p.z = sqrt(1 * 1 - vecOPLengthsquared);  // Pythagore
	else
		p = glm::normalize(p);  // Nearest point
	return p;
}

shared_ptr<MATRIX_4X4> Arcball::modelRotationMatrix(shared_ptr<MATRIX_4X4> model, shared_ptr<MATRIX_4X4> view)
{
	VECTOR_3D rotationAxis = glm::inverse(glm::mat3(*view)) * _axis_in_camera_coord;
	MATRIX_4X4 rotationMatrix = glm::rotate(glm::degrees(_angle) / 10.0f, rotationAxis);

	_lastModelRotationMatrix = std::make_shared<MATRIX_4X4>(rotationMatrix * (*_prevModelRotationMatrix));
	return _lastModelRotationMatrix;
}