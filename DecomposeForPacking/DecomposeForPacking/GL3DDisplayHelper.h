#pragma once

#include "AbstractGLDisplayHelper.h"

/** Logic visualization for 3d with OpenGL */
class GL3DDisplayHelper : public AbstractGLDisplayHelper
{

public:
	GL3DDisplayHelper();
	virtual ~GL3DDisplayHelper();

protected:
	virtual void paintWorld(WorldPtr world) override;
	virtual void paintSingleSolution(WorldPtr world, PartLocationListPtr parts) override;

private:
	void paintCube(WorldPtr world, float x, float y, float z, float r, float g, float b, float a);
};

