#include "GL3DDisplayHelper.h"
#include "OpenGL3DRenderer.h"

static const float CUBE_SIZE = 1;

GL3DDisplayHelper::GL3DDisplayHelper() : AbstractGLDisplayHelper()
{
	_renderer = unique_ptr<OpenGL3DRenderer>(new OpenGL3DRenderer());
}


GL3DDisplayHelper::~GL3DDisplayHelper()
{
}

void GL3DDisplayHelper::paintCube(WorldPtr world, float x, float y, float z, float r, float g, float b, float a)
{
	//_renderContext->writeTriangle(x, y, z)
}

void GL3DDisplayHelper::paintWorld(WorldPtr world)
{
	
}

void GL3DDisplayHelper::paintSingleSolution(WorldPtr world, PartLocationListPtr parts)
{

}