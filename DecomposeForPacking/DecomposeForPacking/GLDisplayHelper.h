#pragma once

#include "World.h"
#include "DecomposeResult.h"
#include "OpenGLRenderer.h"
#include "OpenGLRenderContext.h"
#include "IInputListener.h"
#include "DecomposeAndPack.h"
#include <unordered_map>
#include <tuple>

using std::unordered_map;
using std::tuple;
typedef tuple<float, float, float> RGB_COLOR;

class GLDisplayHelper: public IInputListener
{
private:
	class WorldDisplayVisitor: public IWorldVisitor
	{
	public:
		WorldDisplayVisitor(shared_ptr<OpenGLRenderContext> renderContext);
		void visit(World& world, Point point) override;
	private:
		shared_ptr<OpenGLRenderContext> _renderContext;
	};

public:
	GLDisplayHelper();
	virtual ~GLDisplayHelper();
	void displayResults(WorldPtr world, DecomposeAndPackResult dapResults);
	void onKeyPressed(KEYBOARD_KEY key) override;
private:
	void displayDecomposePackResults(WorldPtr world, FinalDecomposeResults decomposeResult, FinalPackResults packResult);
	void initRenderingLoop();
	void paintWorld(WorldPtr world);
	void paintSingleSolution(WorldPtr world, PartLocationListPtr parts);
	void paintSquare(WorldPtr world, float x, float y, float r, float g, float b, float a);

	OpenGLRenderer _renderer;
	shared_ptr<OpenGLRenderContext> _renderContext;
	bool _isCallbackReceived;
	KEYBOARD_KEY _lastCallbackKey;
	unordered_map< int, RGB_COLOR > _partOrientationToColor;
};

