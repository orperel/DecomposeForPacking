#pragma once

#include <memory>
#include <vector>
#include <xstddef>
#include <iostream>

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
		std::size_t res = std::hash<int>()(p.getX()) + 17*std::hash<int>()(p.getY());
		//std::cout << p.getX() << " - " << p.getY() << " - " << res << std::endl;
		return res;
	}
};

typedef std::shared_ptr<Point> PointPtr;
typedef std::vector<Point> PointList;