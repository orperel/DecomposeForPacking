#include <iostream>

using std::string;

// Core configuration parameters (external paths, etc)

/** The root path for all shader files used by OpenGL & loaded dynamically */
static const string SHADERS_PATH = "..\\DecomposeForPacking\\";

/** The relative paths (filename included) for the default 2d shaders */
static const string DEFAULT_2D_VERTSHADER_PATH = SHADERS_PATH + "Default2DVertexShader.vert";
static const string DEFAULT_2D_FRAGSHADER_PATH = SHADERS_PATH + "Default2DFragmentShader.frag";


// Logic configuration parameters (controls how the algorithm behaves)

/** Maximum number of solutions returned by the Algorithm X.
 *  This cap is used to control the amount of processing Algorithm X does, to avoid
 *  very long running times at the cost of missing some solutions.
 */
static const int DLX_SOLVER_SOLUTIONS_LIMIT = 128;

/** When Algorithm X is configured to return the best partial cover solutions available if no exact
 *  cover solution is available, this parameter comes into use.
 *  This parameter determines whether the algorithm should stop when N
 *  partial cover solutions equal in cover amount are found, (where N = DLX_SOLVER_SOLUTIONS_LIMIT).
 *  True - an exact cover solution may exist and not found yet, but the algorithm is guaranteed to stop.
 *  False - the algorithm will not stop until N exact cover solutions are found. It may return any number
 *          of partial solutions.
 */
static const bool IS_LIMIT_NUMBER_OF_PARTIAL_SOLUTIONS = true;

/** The grade percentage of the bounding box and the number of parts. */
static const int BOUNDING_BOX_WEIGHT = 60;
static const int NUM_OF_PARTS_WEIGHT = 40;