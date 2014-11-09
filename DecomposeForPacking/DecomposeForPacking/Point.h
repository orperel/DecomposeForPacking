#pragma once

#include <memory>
#include <vector>
#include <xstddef>

class Point
{
public:
	Point();
	Point(int x, int y);
	virtual ~Point();

	Point operator+(Point p);

	int getX() const;

	int getY() const;

	bool operator==(const Point& point) const;


private:
	int m_x;
	int m_y;
};

template <class T>
class PointHash;

template<>
class PointHash < Point >
{
public:
	std::size_t operator()(Point const& p) const
	{
		return p.getX() * 1000 + p.getY();
	}
};

typedef std::shared_ptr<Point> PointPtr;
typedef std::vector<Point> PointList;