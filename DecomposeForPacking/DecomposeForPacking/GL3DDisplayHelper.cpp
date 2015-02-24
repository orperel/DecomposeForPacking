#include "GL3DDisplayHelper.h"
#include "OpenGL3DRenderer.h"
#include "VectorMathUtil.h"

static const float CUBE_SIZE = 1;
static const float CUBE_ALPHA = 0.55f / 2;

GL3DDisplayHelper::GL3DDisplayHelper() : AbstractGLDisplayHelper()
{
	_renderer = unique_ptr<OpenGL3DRenderer>(new OpenGL3DRenderer());
}


GL3DDisplayHelper::~GL3DDisplayHelper()
{
}

void GL3DDisplayHelper::paintCube(shared_ptr<OpenGLRenderContext> renderContext, World& world,
								  float x, float y, float z, float r, float g, float b, float a)
{
	y = world.getHeight() - y; // OpenGL Y Axis is inverted

	//          4--------3
	//          /       / |
	//        1/-------/2 |
	//       8 |        | 7
	//         |        | |
	//        5---------6/
	//
	Colored3DVertexEntry v1 = { x, y, z, 0.0f, 0.0f, 0.0f, r, g, b, a };
	Colored3DVertexEntry v2 = { x + CUBE_SIZE, y, z, 0.0f, 0.0f, 0.0f, r, g, b, a };
	Colored3DVertexEntry v3 = { x + CUBE_SIZE, y, z + CUBE_SIZE, 0.0f, 0.0f, 0.0f, r, g, b, a };
	Colored3DVertexEntry v4 = { x, y, z + CUBE_SIZE, 0.0f, 0.0f, 0.0f, r, g, b, a };
	Colored3DVertexEntry v5 = { x, y + CUBE_SIZE, z, 0.0f, 0.0f, 0.0f, r, g, b, a };
	Colored3DVertexEntry v6 = { x + CUBE_SIZE, y + CUBE_SIZE, z, 0.0f, 0.0f, 0.0f, r, g, b, a };
	Colored3DVertexEntry v7 = { x + CUBE_SIZE, y + CUBE_SIZE, z + CUBE_SIZE, 0.0f, 0.0f, 0.0f, r, g, b, a };
	Colored3DVertexEntry v8 = { x, y + CUBE_SIZE, z + CUBE_SIZE, 0.0f, 0.0f, 0.0f, r, g, b, a };

	// Vertices order should be counter-clockwise
	VECTOR3D frontNormal = calculateNormal(v1.x, v1.y, v1.z,
										   v5.x, v5.y, v5.z,
										   v2.x, v2.y, v2.z);
	VECTOR3D backNormal = calculateNormal(v3.x, v3.y, v3.z,
										  v8.x, v8.y, v8.z,
										  v4.x, v4.y, v4.z);
	VECTOR3D topNormal = calculateNormal(v1.x, v1.y, v1.z,
										 v3.x, v3.y, v3.z,
										 v2.x, v2.y, v2.z);
	VECTOR3D bottomNormal = calculateNormal(v5.x, v5.y, v5.z,
											v6.x, v6.y, v6.z,
											v7.x, v7.y, v7.z);
	VECTOR3D leftNormal = calculateNormal(v1.x, v1.y, v1.z,
										  v4.x, v4.y, v4.z,
										  v8.x, v8.y, v8.z);
	VECTOR3D rightNormal = calculateNormal(v2.x, v2.y, v2.z,
										   v6.x, v6.y, v6.z,
										   v3.x, v3.y, v3.z);

	VECTOR3D v1Normal = calculateVertexNormal({ frontNormal, topNormal, leftNormal });
	VECTOR3D v2Normal = calculateVertexNormal({ frontNormal, topNormal, rightNormal });
	VECTOR3D v3Normal = calculateVertexNormal({ backNormal, topNormal, rightNormal });
	VECTOR3D v4Normal = calculateVertexNormal({ backNormal, topNormal, leftNormal });
	VECTOR3D v5Normal = calculateVertexNormal({ frontNormal, bottomNormal, leftNormal });
	VECTOR3D v6Normal = calculateVertexNormal({ frontNormal, bottomNormal, rightNormal });
	VECTOR3D v7Normal = calculateVertexNormal({ backNormal, bottomNormal, rightNormal });
	VECTOR3D v8Normal = calculateVertexNormal({ backNormal, bottomNormal, leftNormal });

	v1.nx = std::get<0>(v1Normal); v1.ny = std::get<1>(v1Normal); v1.nz = std::get<2>(v1Normal);
	v2.nx = std::get<0>(v2Normal); v2.ny = std::get<1>(v2Normal); v2.nz = std::get<2>(v2Normal);
	v3.nx = std::get<0>(v3Normal); v3.ny = std::get<1>(v3Normal); v3.nz = std::get<2>(v3Normal);
	v4.nx = std::get<0>(v4Normal); v4.ny = std::get<1>(v4Normal); v4.nz = std::get<2>(v4Normal);
	v5.nx = std::get<0>(v5Normal); v5.ny = std::get<1>(v5Normal); v5.nz = std::get<2>(v5Normal);
	v6.nx = std::get<0>(v6Normal); v6.ny = std::get<1>(v6Normal); v6.nz = std::get<2>(v6Normal);
	v7.nx = std::get<0>(v7Normal); v7.ny = std::get<1>(v7Normal); v7.nz = std::get<2>(v7Normal);
	v8.nx = std::get<0>(v8Normal); v8.ny = std::get<1>(v8Normal); v8.nz = std::get<2>(v8Normal);

	// Front square
	renderContext->write3DTriangle(v2, v1, v5);
	renderContext->write3DTriangle(v2, v5, v6);
	renderContext->write3DTriangle(v2, v5, v1);
	renderContext->write3DTriangle(v2, v6, v5);
	// Back square
	renderContext->write3DTriangle(v3, v4, v8);
	renderContext->write3DTriangle(v3, v7, v8);
	renderContext->write3DTriangle(v3, v8, v4);
	renderContext->write3DTriangle(v3, v8, v7);
	// Left square
	renderContext->write3DTriangle(v1, v4, v5);
	renderContext->write3DTriangle(v4, v5, v8);
	renderContext->write3DTriangle(v1, v5, v4);
	renderContext->write3DTriangle(v4, v8, v5);
	// Right square
	renderContext->write3DTriangle(v2, v3, v6);
	renderContext->write3DTriangle(v3, v6, v7);
	renderContext->write3DTriangle(v2, v6, v3);
	renderContext->write3DTriangle(v3, v7, v6);
	// Top square
	renderContext->write3DTriangle(v1, v2, v3);
	renderContext->write3DTriangle(v1, v3, v4);
	renderContext->write3DTriangle(v1, v3, v2);
	renderContext->write3DTriangle(v1, v4, v3);
	// Bottom square
	renderContext->write3DTriangle(v5, v6, v7);
	renderContext->write3DTriangle(v5, v7, v8);
	renderContext->write3DTriangle(v5, v7, v6);
	renderContext->write3DTriangle(v5, v8, v7);
}


GL3DDisplayHelper::World3DDisplayVisitor::World3DDisplayVisitor(shared_ptr<OpenGLRenderContext> renderContext) :
_renderContext(renderContext)
{
}

void GL3DDisplayHelper::World3DDisplayVisitor::visit(World& world, Point point)
{
	// Paint a cube for each unmasked voxel in the world
	float r, g, b, a;
	a = CUBE_ALPHA;
	r = 255;
	r /= 255.0;
	g = 255;
	g /= 255.0;
	b = 255;
	b /= 255.0;

	paintCube(_renderContext, world, point.getX(), point.getY(), point.getZ(), r, g, b, a);
}


void GL3DDisplayHelper::paintWorld(WorldPtr world)
{
	// Paint the world pixels to the context
	auto worldPainter = std::make_shared<World3DDisplayVisitor>(_renderContext);
	world->accept(worldPainter);
}

// TODO: Remove
void testPaint(WorldPtr world, PartLocationListPtr parts)
{
	//paintCube(_renderContext, *world, 3.0f, 2.0f, 2.0f, 1.0f, 0.0f, 1.0f, alpha);
	//paintCube(_renderContext, *world, 3.0f, 2.0f, 1.0f, 1.0f, 0.0f, 1.0f, alpha);
	//paintCube(_renderContext, *world, 3.0f, 2.0f, 0.0f, 1.0f, 0.0f, 1.0f, alpha);

	//paintCube(_renderContext, *world, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, alpha);
	//paintCube(_renderContext, *world, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, alpha);

	//paintCube(_renderContext, *world, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, alpha);
	//paintCube(_renderContext, *world, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, alpha);
	//paintCube(_renderContext, *world, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, alpha);
	//paintCube(_renderContext, *world, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, alpha);

	//paintCube(_renderContext, *world, 3.0f, 3.0f, 3.0f, 0.5f, 0.4f, 0.0f, alpha);
	//paintCube(_renderContext, *world, 3.0f, 2.0f, 3.0f, 0.5f, 0.4f, 0.0f, alpha);
	//paintCube(_renderContext, *world, 2.0f, 3.0f, 3.0f, 0.5f, 0.4f, 0.0f, alpha);
	//paintCube(_renderContext, *world, 3.0f, 3.0f, 2.0f, 0.5f, 0.4f, 0.0f, alpha);
}

void GL3DDisplayHelper::paintSingleSolution(WorldPtr world, PartLocationListPtr parts)
{
	float alpha = CUBE_ALPHA;

	for (tuple<PartOrientationPtr, Point>& partLocation : *parts)
	{
		PartOrientationPtr currentOrientation = std::get<0>(partLocation);
		Point anchorPartPoint = std::get<1>(partLocation);

		int orientationId = currentOrientation->getId();
		if (_partOrientationToColor.find(orientationId) == _partOrientationToColor.end())
		{
			float r, g, b;
			r = (rand() % 255);
			g = (rand() % 255);
			b = (rand() % 255);
			RGB_COLOR color = std::make_tuple(r, g, b);
			_partOrientationToColor[orientationId] = color;
		}

		RGB_COLOR orientationColor = _partOrientationToColor.at(currentOrientation->getId());
		float r = std::get<0>(orientationColor);
		float g = std::get<1>(orientationColor);
		float b = std::get<2>(orientationColor);
		float a;
		r /= 255.0;
		g /= 255.0;
		b /= 255.0;
		a = 0.55f;

		for (Point& relativePartCoordinate : *(currentOrientation->getPointList()))
		{
			Point point = anchorPartPoint + relativePartCoordinate;

			paintCube(_renderContext, *world, point.getX(), point.getY(), point.getZ(), r, g, b, a);
		}
	}
}