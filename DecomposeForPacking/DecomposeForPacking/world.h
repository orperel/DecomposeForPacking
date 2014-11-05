#pragma once

/* Represents the world to be decompose or packed into, i.e. the spatial in which we locate the parts that assemble
   the object. The world is composed of transparent and non-transparent pixels; the non-transparents are the pixels
   of the object/pack itself. */
class World
{

public:
	World();
	virtual ~World();

	/* Returns the world's height. */
	int getHeight();

	/* Returns the world's width. */
	int getWidth();

	/* Returns true if the pixel is part of the object, else false.
	   Params x and y are the coordinates of the pixel, related to the top-left corner as the origin. */
	bool isPixelPainted(int x, int y);

};