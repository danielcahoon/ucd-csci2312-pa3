//Cluster.cpp
#include <iostream>
#include <sstream>
#include <cassert>

#include "Cluster.h"
#include "Exceptions.h"

using std::cout;
using std::endl;

namespace Clustering
{
	// ---- LNode ----

	// Two argument constructor
	LNode::LNode(const Point &p, LNodePtr n) : point(p), next(n)
	{
	}

	// ---- Centroid ---- 
	Cluster::Centroid::Centroid(unsigned int d, const Cluster &c): __dimensions(d), __c(c), __p(d) 
	{
		__valid = false;
	}

	// getters/setters
	const Point Cluster::Centroid::get() const
	{
		return __p;
	}
	void Cluster::Centroid::set(const Point &p)
	{
		__p = p;
		__valid = true;
	}
	bool Cluster::Centroid::isValid() const
	{
		return __valid;
	}
	void Cluster::Centroid::setValid(bool valid)
	{
		__valid = valid;
	}

	// functions
	void Cluster::Centroid::compute()
	{
		if(__c.__size > 0)
		{
			Point p(__dimensions);
			LNodePtr temp = __c.__head;
			int i = 0;
			while (temp != nullptr)
			{
				p += temp->point / __c.getSize();
				++i;
				temp = temp->next;
			}
			set(p);
		}
		else if (__c.__size == 0)
		{
			assert(__c.__head == nullptr);
			__valid = true;
			toInfinity();
		}
	}
	bool Cluster::Centroid::equal(const Point &p) const
	{
		if (p == __p)
			return true;
		return false;
	}
	void Cluster::Centroid::toInfinity()
	{
		for (int i = 0; i < __p.getDims(); ++i)
		{
			__p[i] = std::numeric_limits<double>::max();
		}
	}

	// ---- Cluster ----

	// ID Generator
	unsigned int Cluster::__idGenerator = 0;
	// Deleting function
	void Cluster::__del()
	{
		while (__head != nullptr && __head->next != nullptr)
		{
			int i = 1;
			// cout << "Entered while loop time " << i << endl;
			LNodePtr temp;
			temp = __head->next;
			delete __head;
			__head = temp;
			++i;
		}
		// cout << "Exited while loop" << endl;
		if (__head != nullptr && __head->next == nullptr)
		{
			delete __head;
		}
	}

	// Copying Function
	void Cluster::__cpy(LNodePtr pts)
	{
		if (pts == nullptr)
			return;
		this->__head = new LNode(pts->point, nullptr);
		this->__size++;
			
		pts = pts->next;
		while (pts != nullptr)
		{
			this->add(pts->point);
			pts = pts->next;
		}
	}

	// Point id comparison function
	bool Cluster::__in(const Point &p) const
	{
		if (__dimensionality != p.getDims())
		{
			throw DimensionalityMismatchEx(__dimensionality, p.getDims());
		}
		if (__size == 0)
		{
			return false;
		}
		LNodePtr curr = __head;
		while (curr->next != nullptr)
		{
			if (curr->point.getId() == p.getId())
			{
				return true;
			}
			else
			{
				curr = curr->next;
			}
		}
		if (curr->point.getId() == p.getId())
		{
			return true;
		}
		return false;
	}
	
	const char Cluster::POINT_CLUSTER_ID_DELIM = ':';
	
	// One Argument Constructor
	Cluster::Cluster(unsigned int d): __size(0), __head(nullptr), __dimensionality(d), centroid(d, *this)
	{
		__id = __idGenerator;
		__idGenerator++;
	}

	// Copy Constructor
	Cluster::Cluster(const Cluster &c) :__size(0), __dimensionality(c.__dimensionality), centroid(c.__dimensionality, c)
	{
		if (c.__size == 0)
		{
			__head = nullptr;
			__id = c.__id;
		}
		else if (c.__size == 1)
		{
			__head = new LNode(c.__head->point, nullptr);
			__size++;
			__id = c.__id;
		}
		else if (c.__size > 1)
		{
			LNodePtr temp = c.__head;
			__cpy(temp);
			__id = c.__id;
		}
		centroid.compute();
		assert(__size == c.__size);
	}

	// Overloaded Assignment Operator
	Cluster& Cluster::operator=(const Cluster &c) //Fix this for centroid
	{

		if (this == &c)
		{
			return *this;
		}
		else
		{
			__del();
			LNodePtr temp = c.__head;
			__size = 0;
			__cpy(temp);
			centroid.compute();
			assert(this->__size == c.__size);
			__id = c.__id;
			return *this;
		}
	}

	// Destructor
	Cluster::~Cluster()
	{
		__del();
	}

	// getters/setters
	unsigned int Cluster::getSize() const
	{
		return __size;
	}
	unsigned int Cluster::getDimensionality() const
	{
		return __dimensionality;
	}
	unsigned int Cluster::getId() const
	{
		return __id;
	}

	// Set functions
	void Cluster::add(const Point &p)
	{
		if (p.getDims() != __dimensionality)
		{
			throw DimensionalityMismatchEx(__dimensionality, p.getDims());
		}
		LNodePtr temp = new LNode(p, nullptr);
		LNodePtr prev = __head;
		LNodePtr curr = __head;
		
		if (__size == 0)
		{
			__head = temp;
			__size++;
		}
		else if (__size == 1)
		{
			if (temp->point < curr->point)
			{
				LNodePtr foo = __head;
				temp->next = foo;
				__head = temp;
				__size++;
			}
			else 
			{
				__head->next = temp;
				__size++;
			}
		}
		else if (__size > 1)
		{
			int i = 0;

			while (curr->next != nullptr)
			{
				if (temp->point < curr->point)
				{
					if (curr == __head)
					{
						LNodePtr foo = __head;
						temp->next = foo;
						__head = temp;
						break;
					}
					else
					{
						temp->next = curr;
						prev->next = temp;
						break;
					}
				}

				curr = curr->next;
				
				if (i > 0)
				{
					prev = prev->next;
				}
				i++;
			}
			if (curr->next == nullptr)
			{
				curr->next = temp;
			}
			__size++;
			centroid.setValid(false);
		}
	}

	const Point& Cluster::remove(const Point &p)
	{
		if (p.getDims() != __dimensionality)
		{
			throw DimensionalityMismatchEx(__dimensionality, p.getDims());
		}
		int i = 0;
		LNodePtr temp;
		LNodePtr prev = __head;
		LNodePtr curr = __head;

		while (curr != nullptr)
		{
			if (curr->point == p)
			{
				if (curr == __head)
				{
					__head = __head->next;
					delete curr;
					--__size;
					break;
				}
				temp = curr;
				curr = curr->next;
				prev->next = curr;
				delete temp; 
				--__size;
				break;
			}

			curr = curr->next;
			if (i > 0)
			{
				prev = prev->next;
			}
			++i;
		}
		if (__size == 0)
		{
			centroid.setValid(false);
		}
		return p;
	}

	bool Cluster::contains(const Point &p) const
	{
		if (__dimensionality != p.getDims())
		{
			throw DimensionalityMismatchEx(__dimensionality, p.getDims());
		}
		if (__in(p))
		{
			return true;
		}
		return false;
	}
	void Cluster::pickCentroids(unsigned int k, Point **pointArray)
	{
		LNodePtr curr = __head;
		Point inf(__dimensionality); 
		for (int i = 0; i < __dimensionality; ++i)
		{
			inf[i] = std::numeric_limits<double>::max();
		}
		if (k >= __size)
		{
			for (int i = 0; i < __size; ++i)
			{
				*pointArray[i] = curr->point;
				curr = curr->next;
			}
			for (int i = __size; i < k; ++i)
			{
				*pointArray[i] = inf;
			}
		}
		else if (k < __size)
		{
			for (int i = 0; i < k; ++i)
			{
				*pointArray[i] = curr->point;
				curr = curr->next;
			}
		}
	}

	// Members: Subscript
	const Point& Cluster::operator[](unsigned int index) const 
	{
		if (__size <= 0)
		{
			throw EmptyClusterEx();
		}
		if (index >= __size)
		{
			throw OutOfBoundsEx(__size, index);
		}
		
		LNodePtr curr = __head;
		
		int i = 1;
		if (index == 0)
		{
			return __head->point;
		}
		curr = curr->next;

		while (curr->next != nullptr)
		{
			if (i == index)
			{
				return curr->point;
			}
			curr = curr->next;
			++i;
		}
		return curr->point;
	}

	//Members: Compound assignment (Point argument)
	Cluster& Cluster::operator+=(const Point &p)
	{
		if (p.getDims() != __dimensionality)
		{
			throw DimensionalityMismatchEx(__dimensionality, p.getDims());
		}
		this->add(p);
		centroid.setValid(false);
		return *this;
	}

	Cluster& Cluster::operator-=(const Point &p)
	{
		if (p.getDims() != __dimensionality)
		{
			throw DimensionalityMismatchEx(__dimensionality, p.getDims());
		}
		if (this->contains(p))
		{
			this->remove(p);
			centroid.setValid(false);
		}
		
		return *this;
	}

	// Members: Compound assignment (Cluster argument)
	Cluster& Cluster::operator+=(const Cluster &c) //Union
	{
		LNodePtr temp = c.__head;

		while (temp != nullptr)
		{
			if (!this->__in(temp->point))
			{
				this->add(temp->point);
			}
			temp = temp->next;
		}
		return *this;
	}

	Cluster& Cluster::operator-=(const Cluster &c) // (asymmetric) difference
	{
		LNodePtr temp = c.__head;
		while (temp != nullptr)
		{
			if (__in(temp->point))
			{
				remove(temp->point);
				centroid.setValid(false);
			}
			temp = temp->next;
		}
		
		if (__size == 0)
		{
			centroid.setValid(false);
		}
		return *this;
	}

	// Friends: IO
	std::ostream& operator<<(std::ostream &os, const Cluster &c)
	{
		LNodePtr curr = c.__head;
		if (c.__size == 0)
		{
			return os;
		}
		else 
		{
			while (curr->next != nullptr)
			{
				os << curr->point << ' ' << Cluster::POINT_CLUSTER_ID_DELIM << ' ' << c.__id << endl;
				curr = curr->next;
			}
			os << curr->point;
			os << ' ' << Cluster::POINT_CLUSTER_ID_DELIM << ' ' << c.__id;
		return os;
		}
	}

	std::istream& operator>>(std::istream &is, Cluster &c)
	{
		
		std::string wholeLine;

		while (getline(is,wholeLine))
		{
			Point p(c.__dimensionality);
        	std::stringstream lineStream(wholeLine);
        	std::string value;
        	int i = 0;
        	double d = 0.0;
        	while (getline(lineStream, value, Point::POINT_VALUE_DELIM))
        	{
        		try
        		{
        			if (i >= c.__dimensionality)
        			{
        				p.rewindIdGen();
        				throw OutOfBoundsEx(c.__dimensionality, i);
	       			}
        			d = stod(value);
        			p.setValue(i, d);
        		}
        		catch (OutOfBoundsEx &ex)
        		{
        			std::cerr << "Caught Exception: " << ex << endl;
        			p.rewindIdGen();
        		} 
        		++i;
        	}
        	try
        	{
        		if (i != c.__dimensionality)
        		{
        			throw DimensionalityMismatchEx(c.__dimensionality, i);
        		}
        		c.add(p);	
        	}
        	catch (DimensionalityMismatchEx &ex)
        	{
        		std::cerr << "Caught Exception: " << ex << endl;

        	}
        }

		return is;
	}

	// Friends: Comparison
	bool operator==(const Cluster &lhs, const Cluster &rhs)
	{
		if (lhs.__dimensionality != rhs.__dimensionality)
		{
			throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
		}

		if (lhs.__size != rhs.__size)
		{
			return false;
		}

		LNodePtr lCurr = lhs.__head;
		LNodePtr rCurr = rhs.__head;

		while (lCurr != nullptr)
		{
			if (lCurr->point != rCurr->point )
			{
				return false;
			}

			lCurr = lCurr->next;
			rCurr = rCurr->next;
		}

		return true;
	}

	bool operator!=(const Cluster &lhs, const Cluster &rhs)
	{
		if (lhs.__dimensionality != rhs.__dimensionality)
		{
			throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
		}

		if (lhs == rhs)
		{
			return false;
		}
		return true;
	}

	// Friends: Arithmetic (Cluster and Point)
	const Cluster operator+(const Cluster &lhs, const Point &rhs)
	{
		if (lhs.__dimensionality != rhs.getDims())
		{
			throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.getDims());
		}

		Cluster c(lhs);
		c.add(rhs);
		return c;
	}

	const Cluster operator-(const Cluster &lhs, const Point &rhs)
	{
		if (lhs.__dimensionality != rhs.getDims())
		{
			throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.getDims());
		}

		Cluster c(lhs);
		if (c.contains(rhs))
		{
			c.remove(rhs);
		}
		return c;
	}

	// Friends: Arithmetic (two Clusters)

	const Cluster operator+(const Cluster &lhs, const Cluster &rhs) //Union
	{
		if (lhs.__dimensionality != rhs.__dimensionality)
		{
			throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
		}

		Cluster c(lhs);
		c += rhs;
		return c;
	}

	const Cluster operator-(const Cluster &lhs, const Cluster &rhs) // (asymmetric) difference
	{
		if (lhs.__dimensionality != rhs.__dimensionality)
		{
			throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
		}

		Cluster c(lhs);
		c -= rhs;
		return c;
	}

	// ---- Move ----
	
	// Constructor
	Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __p(p), __from(from), __to(to)
	{
	}
	// Methods
	void Cluster::Move::perform()
	{
		__to.add(__from.remove(__p));
	}
}