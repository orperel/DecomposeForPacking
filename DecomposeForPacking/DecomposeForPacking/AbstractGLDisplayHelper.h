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

/** Abstract class for displaying the logic results - 
 *  contains common code for 2d & 3d: dealing with the render loop, geenrating color tables,
 *  input callbacks, etc.
 */
class AbstractGLDisplayHelper: public IInputListener
{
public:
	AbstractGLDisplayHelper();
	virtual ~AbstractGLDisplayHelper();
	void displayResults(WorldPtr world, DecomposeAndPackResult dapResults);
	void onKeyPressed(KEYBOARD_KEY key) override;
protected:
	void displayDecomposePackResults(WorldPtr world, FinalDecomposeResults decomposeResult, FinalPackResults packResult);
	void initRenderingLoop();

	// Visualization methods are implmented differently for 2d & 3d
	virtual void paintWorld(WorldPtr world) = 0;
	virtual void paintSingleSolution(WorldPtr world, PartLocationListPtr parts) = 0;

	unique_ptr<OpenGLRenderer> _renderer;
	shared_ptr<OpenGLRenderContext> _renderContext;
	bool _isCallbackReceived;
	KEYBOARD_KEY _lastCallbackKey;
	unordered_map< int, RGB_COLOR > _partOrientationToColor;
};

