#pragma once

#include "AbstractGLDisplayHelper.h"

/** Logic visualization for 3d with OpenGL */
class GL3DDisplayHelper : public AbstractGLDisplayHelper
{
	class World3DDisplayVisitor : public IWorldVisitor
	{
	public:
		World3DDisplayVisitor(shared_ptr<OpenGLRenderContext> renderContext);
		void visit(World& world, Point point) override;
	private:
		shared_ptr<OpenGLRenderContext> _renderContext;
	};

public:
	GL3DDisplayHelper();
	virtual ~GL3DDisplayHelper();
	
protected:
	virtual void paintWorld(WorldPtr world) override;
	virtual void paintSingleSolution(WorldPtr world, PartLocationListPtr parts) override;

private:
	static void paintCube(shared_ptr<OpenGLRenderContext> renderContext, World& world,
						  float x, float y, float z, float r, float g, float b, float a);
};

