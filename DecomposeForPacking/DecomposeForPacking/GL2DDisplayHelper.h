#pragma once

#include "AbstractGLDisplayHelper.h"

/** Logic visualization for 2d with OpenGL */
class GL2DDisplayHelper : public AbstractGLDisplayHelper
{

private:
	class WorldDisplayVisitor : public IWorldVisitor
	{
	public:
		WorldDisplayVisitor(shared_ptr<OpenGLRenderContext> renderContext);
		void visit(World& world, Point point) override;
	private:
		shared_ptr<OpenGLRenderContext> _renderContext;
	};

public:
	GL2DDisplayHelper();
	~GL2DDisplayHelper();

protected:
	virtual void paintWorld(WorldPtr world) override;
	virtual void paintSingleSolution(WorldPtr world, PartLocationListPtr parts) override;

private:
	void paintSquare(WorldPtr world, float x, float y, float r, float g, float b, float a);
};

