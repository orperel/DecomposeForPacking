#pragma once

#include "OpenGLLibs.h" // Must be first to enable OpenGL import
#include <memory>

using std::shared_ptr;

/** A camera controller using the arcball method */
class Arcball
{
public:
	Arcball();
	~Arcball();

	/** Update the state of the arcball according to mouse press */
	void handleMousePress(bool isLeftMousePressed, int x, int y);

	/** Updates the angle of the arcball */
	void handleMouseMove(int x, int y);

	void updateScreenSize(int screenWidth, int screenHeight);

	/** Rotate the given object matrix according to the arcball parameters.
	 *  The View matrix is needed to accumulate rotations.
	 *  The matrix returned is the accumulated rotation matrix of all rotation made on the arcball
	 *  (to be used to build the model matrix)
	 */
	shared_ptr<MATRIX_4X4> modelRotationMatrix(shared_ptr<MATRIX_4X4> model, shared_ptr<MATRIX_4X4> view);

private:
	bool _isArcballOn;
	int _lastXPos, _lastYPos;
	int _currXPos, _currYPos;
	int _screenWidth, _screenHeight;
	VECTOR_3D _axis_in_camera_coord;
	float _angle;
	shared_ptr<MATRIX_4X4> _lastModelRotationMatrix; // The last model-rotation matrix returned
	shared_ptr<MATRIX_4X4> _prevModelRotationMatrix; // The last model-rotation matrix returned when arcball was on

	/**
	* Get a normalized vector from the center of the virtual ball O to a
	* point P on the virtual ball surface, such that P is aligned on
	* screen's (X,Y) coordinates.  If (X,Y) is too far away from the
	* sphere, return the nearest point on the virtual ball surface.
	*/
	VECTOR_3D getArcballVector(int x, int y);
};

