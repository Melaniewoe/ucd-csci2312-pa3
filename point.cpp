//
//  point.cpp
//  pa3
//
//  Copyright © 2016 Melanie Woe. All rights reserved.
//



#include "Point.h"
#include "Exceptions.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>

namespace Clustering
{
    //id generator
     unsigned int Point::__idGen = 0;
    
    //Define and initialize the static const char POINT_VALUE_DELIM to ','
     const char Point::POINT_VALUE_DELIM = ',';
    
    
    void Point::rewindIdGen()
    {
        __idGen--;
    }
    
    
    Point::Point(unsigned int dimensions)
    {
        if (dimensions==0)
        {
            throw ZeroDimensionsEx();
        }
        __dim = dimensions;  //assign dim
        
        __id = __idGen; //assign id
        __idGen++; //increment id
        
        __values = new double[__dim];
        
        //array
        for (int i = 0; i < __dim; i++)
        {
            __values[i] = 0;
        }
        
        
        
    }
 
    Point::Point(unsigned int dimensions, double *array)
    {
        __dim = dimensions;  //assign dim
        
        __id = __idGen; //assign id
        
        __idGen++; //increment id
        
        __values = new double[dimensions];
        
        //array
        for (int i = 0; i < dimensions; i++)
        {
            __values[i] = array[i];
        }
        
    }
 
    // Big three: cpy ctor, overloaded operator=, dtor
    Point::Point(const Point &p)   //copy constructor
    {
        
        __dim = p.__dim;
        
        __values = new double[p.__dim];
        
        for (int i = 0; i < __dim ; ++i)
        {
            __values[i] = p.__values[i];
        }
        
        __id = p.__id;
         
    }
    
    //overloaded operator=
    Point &Point::operator=(const Point &p)
    {
        if (__dim != p.__dim)
        {
            throw DimensionalityMismatchEx(__dim, p.__dim);
        }
       // else if (this != &p)
       // {
            __id = p.__id;
            __dim = p.__dim;
            
            if(__values != nullptr)
                delete [] __values;
            
            __values = new double[__dim];
            
            for (int i = 0; i < __dim; i++)
            {
                __values[i] = p.__values[i];
            }
            
            
            
     //   }
        
        return *this;
    }
    
    //destructor
    Point::~Point()
    {
        
            delete [] __values;
        
    }
    
    // Accessors & mutators
    int Point::getId() const
    {
        return __id;
    }
    unsigned int Point::getDims() const
    {
        return __dim ;
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
        
        double dist = 0;
        
        for(int i = 0; i < __dim; i++)
        {
            //distance = (x2-x1)^2
            dist += pow((p.getValue(i)) - __values[i] ,2);
        }
        
        return sqrt(dist);
    }
    
    //overload operator
    Point &Point::operator*=(double rhs)
    {
        for (int i = 0; i < __dim; ++i)
        {
            __values[i] *= rhs;
        }
        
        return *this;
        
    }
    
    Point &Point::operator/=(double rhs)
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
            p *= rhs;
        }
        return p;
    }
    
    const Point Point::operator/(double rhs) const
    {
        Point p(__dim);
        for (int i = 0; i < __dim; ++i)
        {
            p /= rhs;
            
        }
        
        return p;
    }
    
    double &Point::operator[](unsigned int index)
    {
        if (index >= __dim)
        {
            throw OutOfBoundsEx(__dim, index);
        }
        return __values[index];
    }
    
    const double &Point::operator[](unsigned int index)const
    {
        if (index >= __dim)
        {
            throw OutOfBoundsEx(__dim, index);
        }
        
        return __values[index];
    }
    
    // Friends
    Point &operator+=(Point &p, const Point &q)
    {
        if (p.__dim != q.__dim)
        {
            throw DimensionalityMismatchEx(p.__dim, q.__dim);
        }
        
        for (int i = 0; i < p.__dim; i++)
        {
            p.__values[i] += q.__values[i];
        }
        
        return p;
        
    }
    Point &operator-=(Point &p, const Point &q)
    {
        if (p.__dim != q.__dim)
        {
            throw DimensionalityMismatchEx(p.__dim, q.__dim);
        }
        
        for(int i = 0; i < p.__dim; i++)
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
        
        if (lhs.__id != rhs.__id)
            return false;
        //bool isEqual = true;
        for (int i = 0; i < lhs.__dim; i++)
        {
            if (lhs.__values[i] != rhs.__values[i])
            {
                return false;
            }
        }
        return true;
    }
    
    bool operator!=(const Point &lhs, const Point &rhs)
    {
        if (lhs.__dim != rhs.__dim)
        {
            throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
        }
        return !(lhs ==rhs);
    }
    
    bool operator<(const Point &lhs, const Point &rhs)
    {
      
        if (lhs.__dim != rhs.__dim)
        {
            throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
        }

        
        int usedDims = std::max(lhs.__dim, rhs.__dim);
        
        for (int i = 0; i < usedDims; ++i)
        {
            if (lhs.getValue(i) != rhs.getValue(i))
            {
                return (lhs.getValue(i) < rhs.getValue(i));
            }
        }
        
        
        return false;
        
    }
    
    bool operator>(const Point &lhs, const Point &rhs)
    {
        if (lhs.__dim != rhs.__dim)
        {
            throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
        }
        return (rhs < lhs);
    }

    bool operator<=(const Point &lhs, const Point &rhs)
    {
        if (lhs.__dim != rhs.__dim)
        {
            throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
        }
        return !(lhs > rhs);
    }
    
    bool operator>=(const Point &lhs, const Point &rhs)
    {
        if (lhs.__dim != rhs.__dim)
        {
            throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
        }
        return !(lhs<rhs);
    }
    
     std::ostream& operator<<(std::ostream &os, const Point &rhs)
    {
        int i = 0;
        for ( ; i < rhs.__dim - 1; ++i)
        {
            os << rhs.__values[i] << ", ";
        }
        
        os << rhs.__values[i];
        
        return os;
    }
    
    std::istream& operator>>(std::istream &is, Point &rhs)
    {
        int i = 0;
        double d;
        std::string line;
        
        
        while (getline(is, line, Point::POINT_VALUE_DELIM))
        {
            d = stod(line);
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

 