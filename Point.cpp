//Point.cpp
#include <cmath>
#include <iostream>
#include <cassert>

#include "Point.h"
#include "Exceptions.h"


namespace Clustering
{

	unsigned int Point::__idGen = 0;
	const char Point::POINT_VALUE_DELIM = ',';

	void Point::rewindIdGen()
	{
		__idGen--;
	}
	//One argument constructor
	Point::Point(unsigned int dim)
	{
		if (dim == 0)
		{
			throw ZeroDimensionsEx();
		}
		__dim = dim;
		__values = new double[__dim];
		__id = __idGen;
		__idGen++;

		for (int i = 0; i < __dim; ++i)
		{
			__values[i] = 0;
		}
	}

	// Two argument constructor

	// Copy constructor
	Point::Point(const Point &p)
	{
		__dim = p.__dim;
		__values = new double[p.__dim];
		for (int i = 0; i < __dim; ++i)
		{
			__values[i] = p.__values[i]; 
		}
		__id = p.__id;
	}

	// Overloaded assignment operator
	Point& Point::operator=(const Point &p)
	{
		std::size_t i;
		double *newPoint;

		if (this == &p)
		{
			return *this;
		}
		if (__dim != p.__dim)
		{
			throw DimensionalityMismatchEx(__dim, p.__dim);
		}
		for (i = 0; i < __dim; ++i)
		{
			__values[i] = p.__values[i];
		}
		__id = p.__id;
		return *this;
	}

	Point::~Point()
	{
		delete [] __values;
	}

	// Accesors and Mutators
	int Point::getId() const
	{
		return __id;
	}

	unsigned int Point::getDims() const
	{
		return __dim;
	}

	void Point::setValue(unsigned int i, double p)
	{
		if (i >= __dim)
		{
			throw OutOfBoundsEx(__dim, i);
		}
		__values[i] = p;
	}

	double Point::getValue(unsigned int i) const
	{
		if (i >= __dim)
		{
			throw OutOfBoundsEx(__dim, i);
		}
		return __values[i];
	}

	// Functions
	double Point::distanceTo(const Point &p) const
	{
		if (__dim != p.getDims())
		{
			throw DimensionalityMismatchEx(__dim, p.getDims());
		}
		double distance = 0;
		double deltaX = 0;

		for (int i = 0; i < __dim; ++i)
		{
			deltaX += pow((__values[i] - p[i]), 2);
		}
		distance = sqrt(deltaX);
		return distance;
	}

	//Members
	Point& Point::operator*=(double rhs)
	{
		for (int i = 0; i < __dim; ++i)
		{
			__values[i] = __values[i] * rhs;
		}

		return *this;
	}

	Point& Point::operator/=(double rhs)
	{
		for (int i = 0; i < __dim; ++i)
		{
			__values[i] /= rhs;
		}
		return *this;
	}

	const Point Point::operator*(double rhs) const
	{
		Point p(__dim);
		for (int i = 0; i < __dim; ++i)
		{
			p.__values[i] = __values[i] * rhs;
		}
		return p;
	}

	const Point Point::operator/(double rhs) const
	{
		Point p(__dim);
		for (int i = 0; i < __dim; ++i)
		{
			p.__values[i] = __values[i] / rhs;
		}
		return p;
	}

	double& Point::operator[](unsigned int index)
	{
		if (index >= __dim)
		{
			throw OutOfBoundsEx(__dim, index);
		}
		return __values[index];
	}
	const double& Point::operator[](unsigned int index) const
	{
		if (index >= __dim)
		{
			throw OutOfBoundsEx(__dim, index);
		}
		return __values[index];
	}
	//Friends
	Point& operator+=(Point &p, const Point &q)
	{
		if (p.__dim != q.__dim)
		{
			throw DimensionalityMismatchEx(p.__dim, q.__dim);
		}
		for (int i = 0; i < p.__dim; ++i)
		{
			p.__values[i] += q.__values[i];
		}
		return p;
	}

	Point& operator-=(Point &p, const Point &q)
	{
		if (p.__dim != q.__dim)
		{
			throw DimensionalityMismatchEx(p.__dim, q.__dim);
		}
		for (int i = 0; i < p.__dim; ++i)
		{
			p.__values[i] -= q.__values[i];
		}
		return p;
	}

	const Point operator+(const Point &lhs, const Point &rhs)
	{
		if (lhs.__dim != rhs.__dim)
		{
			throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
		}
		Point p(lhs);
		p += rhs;
		return p;
	}

	const Point operator-(const Point &lhs, const Point &rhs)
	{
		if (lhs.__dim != rhs.__dim)
		{
			throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
		}
		Point p(lhs);
		p -= rhs;
		return p;
	}

	bool operator==(const Point &lhs, const Point &rhs)
	{
		if (lhs.__dim != rhs.__dim)
		{
			throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
		}

		for (int i = 0; i < lhs.__dim; ++i)
		{
			if (lhs.__values[i] != rhs.__values[i])
				return false;
		}
		return true;
	}

	bool operator!=(const Point &lhs, const Point &rhs)
	{
		if (lhs.__dim != rhs.__dim)
		{
			throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
		}
		
		if (lhs == rhs)
		{
			return false;
		}
		return true;
	}

	bool operator<(const Point &lhs, const Point &rhs)
	{
		if (lhs.__dim != rhs.__dim)
		{
			throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
		}
		for (int i = 0; i < lhs.__dim; ++i)
		{
			if (lhs.__values[i] < rhs.__values[i])
				return true;
			else if (lhs.__values[i] > rhs.__values[i])
				return false;
		}
		return false;
	}

	bool operator>(const Point &lhs, const Point &rhs)
	{
		if (lhs.__dim != rhs.__dim)
		{
			throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
		}
		for (int i = 0; i < lhs.__dim; ++i)
		{
			if (lhs.__values[i] > rhs.__values[i])
				return true;
		}
		return false;
	}

	bool operator<=(const Point &lhs, const Point &rhs)
	{
		if (lhs.__dim != rhs.__dim)
		{
			throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
		}

		if (lhs > rhs)
			return false;
		else	
			return true;
	}

	bool operator>=(const Point &lhs, const Point &rhs)
	{
		if (lhs.__dim != rhs.__dim)
		{
			throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
		}

		if (lhs < rhs)
			return false;
		else	
			return true;
	}

	std::ostream& operator<<(std::ostream &os, const Point &rhs)
	{
		int i = 0;
		for (; i < rhs.__dim - 1; ++i)
		{
			os << rhs.__values[i] << ", ";
		}
		os << rhs.__values[i];
		return os;
	}

	std::istream& operator>>(std::istream &is, Point &rhs)
	{
		std::string value;
		double d;
		int i = 0;

		while (getline(is, value, Point::POINT_VALUE_DELIM))
		{
			d = stod(value);
			rhs.__values[i++] = d;
		}
		if (i != rhs.__dim)
		{
			Point::rewindIdGen();
			throw DimensionalityMismatchEx(rhs.__dim, i);
		}
		return is;
	}
}