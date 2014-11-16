#pragma once

#include <memory>
#include <vector>
#include <xstddef>
#include <iostream>

class Point;

const int BLACK = 0;

/**
*	Point represents place in the space.
*   Each point contains X dimension, y  dimension and a color.
*/
class Point
{
public:
	// Default C'tor
	Point();

	/*
	 * C'tor
	 * @param	int - X Dimension
	 * @param	int - Y Dimension
	 * @param	int - Color - Default value: Black
	*/
	Point(int x, int y, int color = BLACK);

	// Default D'tor
	virtual ~Point();

	/* 
	 * Operator+ - adds two points.
	 * NOTICE:	The color taken is the color of the left point.
	*/
	Point operator+(Point p);

	/*
	 * X getter.
	*/
	int getX() const;

	/*
	* Y getter.
	*/
	int getY() const;

	/*
	* Color getter.
	*/
	int getColor() const;

	/*
	* Operator== - check if two points are equale.
	* NOTICE:	The color is NOT taken in consideration when checking equality.
	*/
	bool operator==(const Point& point) const;

private:
	int m_x; // X
	int m_y; // Y
	int m_color; // Color
};


/*
 * Hash Class for Point
*/
template <class T>
class PointHash;

template<>
class PointHash < Point >
{
public:
	/*
	* Hash Function for Point
	*/
	std::size_t operator()(Point const& p) const
	{
		std::size_t res = std::hash<int>()(p.getX()) + 17*std::hash<int>()(p.getY());
		return res;
	}
};

typedef std::shared_ptr<Point> PointPtr;
typedef std::vector<Point> PointList;
typedef std::shared_ptr<PointList> PointListPtr;