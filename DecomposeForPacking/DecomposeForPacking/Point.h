#pragma once
#include <list>
#include <memory>

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

typedef std::shared_ptr<Point> PointPtr;
typedef std::list<PointPtr> PointList;