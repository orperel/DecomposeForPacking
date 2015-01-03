#include <iostream>

using std::string;

static const string SHADERS_PATH = "..\\DecomposeForPacking\\";
static const string DEFAULT_2D_VERTSHADER_PATH = SHADERS_PATH + "Default2DVertexShader.vert";
static const string DEFAULT_2D_FRAGSHADER_PATH = SHADERS_PATH + "Default2DFragmentShader.frag";

static const int DLX_SOLVER_SOLUTIONS_LIMIT = 128;