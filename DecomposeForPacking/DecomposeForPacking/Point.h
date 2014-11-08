#pragma once
#include <list>

class Point
{
public:
	Point(int x, int y);
	virtual ~Point();

	Point operator+(Point p);

private:
	int m_x;
	int m_y;
};

typedef std::list<Point> PointList;