//
//  Cluster.cpp
//  pa3
//
//  Copyright © 2016 Melanie Woe. All rights reserved.
//


#include "Cluster.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>

namespace Clustering
{
    
    
    LNode::LNode(const Point &p, LNodePtr n) : point (p), next(n)
    {
        point = p;
        next = n;
    }
    
    const char Cluster::POINT_CLUSTER_ID_DELIM = ':';

    
    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c):__dimensions(d), __c(c), __p(d) // needs ref to cluster
    {
       // __dimensions = d;
        
        __valid = false;
    }
    
    // getters/setters
    const Point Cluster::Centroid::get() const // doesn't check for validity
    {
        return __p;
    }
    void Cluster::Centroid::set(const Point &p) // sets to valid
    {
        __p = p;
        __valid =true;
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
    void Cluster::Centroid::compute()  //to compute the average of its points
    {
        
        if (__c.__points == nullptr)
        {
            assert(__c.__size == 0);
            __valid = true;
            toInfinity();
            //return;
        }
        
        if (__c.__size > 0 )
        {
            LNodePtr curr = __c.__points;
            Point p(__dimensions);
            int size = 0;
        
            while (curr !=nullptr)
            {
                p += curr->point / __c.getSize();
                curr = curr->next;
                size++;     //keep tracks of the number of points
            }
        
            assert(size == __c.getSize());   //to make sure the correct size
        
            set(p);
        }
        
    
        
    }
    
    bool Cluster::Centroid::equal(const Point &p) const // only compares the double values and ignores the id of the Point argument
    {
        //for (int i = 0; i< __dimensions; i++)
            if (__p == p)
                return true;
        return false;
    }

    void Cluster::Centroid::toInfinity()  //sets the centroid to an infinite point
    {
        for (int i = 0; i < __p.getDims(); ++i)
        {
            __p[i] = std::numeric_limits<double>::max();
        }
    }
    
    unsigned int Cluster::__idGenerator = 0;
    
    // Deleting function
    void Cluster::__del()
    {
        while (__points != nullptr && __points->next != nullptr)
        {
            int i = 1;
            
            LNodePtr temp;
            temp = __points->next;
            delete __points;
            __points = temp;
            ++i;
        }
        if (__points != nullptr && __points->next == nullptr)
        {
            delete __points;
        }
    }
    
    // Copying Function
    void Cluster::__cpy(LNodePtr pts)
    {
        if (pts == nullptr)
            return;
        this->__points = new LNode(pts->point, nullptr);
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
        LNodePtr curr = __points;
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
    
    Cluster::Cluster(unsigned int d) : __size(0), __points(nullptr), __dimensionality(d),centroid(d, *this)
    {
        __dimensionality = d;
        __size = 0;
        __points = nullptr;
        __id = __idGenerator;
        __idGenerator++;
    }
    
    //copy constructor
    Cluster::Cluster(const Cluster &c) : __size(0), __dimensionality(c.__dimensionality),centroid(c.__dimensionality, c)
    {
        __id = c.__id;
        
        if (c.__size == 0)
        {
           
            __points = nullptr;
            
        }
        else if (c.__size == 1)
        {
            __points = new LNode(c.__points->point, nullptr);
            __size++;
            
        }
        else if (c.__size > 1)
        {
            LNodePtr temp = c.__points;
            __cpy(temp);
            
        }
        
        assert(__size == c.__size);
        
        centroid.compute();

 
        for (int i = 0; i < c.__size; i++)
        {
            add(c[i]);
        }
        
 
    }
    
    //need
    Cluster& Cluster::operator=(const Cluster &c)
    {
        __points = nullptr;
        
        
        
        if (__size > 0)
        {
            __size = 0;
            LNodePtr curr= __points;
            LNodePtr prev;
            
            
            
            while (curr != nullptr)
            {
                prev = curr;
                curr = curr->next;
                
                delete prev;
            }
            
        }
        
        for (int i = 0; i < c.__size; ++i)
        {
            add(c[i]);
        }
        
        return *this;
    }
    
    
    Cluster::~Cluster()
    {
 
        LNodePtr currNode;
        while (__points != nullptr)
        {
            currNode = __points;
            __points = currNode -> next;
            
            delete currNode;
            
        }
 
    }
    
    
    // Getters
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
    
    void Cluster::add(const Point &p)
    {
        
        if (__size == 0)
        {
            __points = new LNode(p, nullptr);
            __size++;
            return;
            
        }
        else
        {
            if (contains(p))
                return;
            
            LNodePtr next;
            LNodePtr prev;
            
            next = __points;
            prev = nullptr;
            
            
            while (next != nullptr)
            {
                if (p < next->point)
                {
                    
                    if (prev == nullptr)
                    {
                        
                        __points = new LNode(p, next);
                        
                        __size++;
                        
                        return;
                    }
                    else
                    {
                        
                        prev->next = new LNode(p, next);
                        
                        __size++;
                        
                        return;
                    }
                }
                
                prev = next;
                next = next->next;
                
            }
            
            
            prev->next = new LNode(p, nullptr);
            
            __size++;
        }
    }
    
    
    const Point &Cluster::remove(const Point &p)
    {
        
        LNodePtr curr;
        LNodePtr before = nullptr;
        
        curr = __points;
        
        if (contains(p))
        {
            
            
            while (curr != nullptr)
            {
                if (curr->point == p)
                {
                    __points = curr->next;
                    
                    delete curr;
                    
                    __size--;
                    
                    break;
                    
                }
                else if (before != nullptr)
                {
                    
                    before->next = curr->next;
                    
                    delete curr;
                    
                    __size--;
                    
                    break;
                }
                
                
                before = curr;
                curr = curr->next;
            }
        }
        
        return p;
    }
    
    
    bool Cluster::contains(const Point &p) const
    {
        LNodePtr curr = this -> __points;
        for ( ; curr != nullptr; curr = curr -> next)
        {
            if (curr -> point == p)
            {
                return true;
            }
        }
        
        return false;
        
    }
    
    // Centroid functions
    void Cluster::pickCentroids(unsigned int k, Point **po) // pick k initial centroids
    {
        LNodePtr curr = __points;
        Point inf(__dimensionality);
        for (int i = 0; i < __dimensionality; ++i)
        {
            inf[i] = std::numeric_limits<double>::max();
        }
        if (k >= __size)
        {
            for (int i = 0; i < __size; ++i)
            {
                *po[i] = curr->point;
                curr = curr->next;
            }
            for (int i = __size; i < k; ++i)
            {
                *po[i] = inf;
            }
        }
        else if (k < __size)
        {
            for (int i = 0; i < k; ++i)
            {
                *po[i] = curr->point;
                curr = curr->next;
            }
        }
    }
    
    // Overloaded operators
    
    // Members: Subscript
    const Point &Cluster::operator[](unsigned int index) const
    {
        
        if (__size == 0)
            throw EmptyClusterEx();
        if (index >= __size)
            throw OutOfBoundsEx(__size, index);
        
        LNodePtr curr = __points;
        
        for (int i = 0; i < index; i++ )
        {
            
            curr = curr -> next;
        }
        
        return curr -> point;
         
        
    }
    
    // Friends: IO
    std::ostream &operator<<(std::ostream &out, const Cluster &c)
    {
        
        LNodePtr curr = c.__points;
        if (curr != NULL)
        {
            out << std::setprecision(10);
            
            for (int i = 0; i < c.__size; i++)
            {
                out << curr -> point << " " << Cluster::POINT_CLUSTER_ID_DELIM << " " << c.__id;
                curr = curr -> next;
            }
            
            
        }
         
       
        return out;
         
        
        
    }
    
    
    
    std::istream &operator>>(std::istream &i, Cluster &c)
    {
        std::string lines;
        
        while (getline(i,lines))
        {
            Point p(c.__dimensionality);
            std::stringstream stream(lines);
            std::string value;
            int i = 0;
            double d = 0.0;
            while (getline(stream, value, Point::POINT_VALUE_DELIM))
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
                    std::cerr << "Caught Exception: " << ex << std::endl;
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
                std::cerr << "Caught Exception: " << ex << std::endl;
                
            }
        }
        
        return i;
        
        
    }
    
    
    // Friends: Comparison
    bool operator==(const Cluster &c, const Cluster &c1)
    {
        bool equal = true;
        if (c.__dimensionality != c1.__dimensionality)
        {
            throw DimensionalityMismatchEx(c.__dimensionality, c1.__dimensionality);
        }
        else if (c.__size == c1.__size)
        {
            return true;
        }
        while(c.__points != nullptr && c1.__points != nullptr)
        {
            if ( c.__points -> point != c1.__points -> point)
                equal = false;
            c.__points -> next = c.__points;
            c1.__points -> next = c1.__points;
        }
        return true;
    }
    
    
    bool operator!=(const Cluster &c, const Cluster &c1)
    {
        if (c.__dimensionality != c1.__dimensionality)
        {
            throw DimensionalityMismatchEx(c.__dimensionality, c1.__dimensionality);
        }

        return !(c==c1);
    }
    

    
    // Members: Compound assignment (Point argument)
    Cluster &Cluster::operator+=(const Point &p)
    {
        
        if (__dimensionality != p.getDims())
        {
            throw DimensionalityMismatchEx(__dimensionality, p.getDims());
        }
        
        this -> add(p);
        centroid.setValid(false);
        
        return *this;
    }
    
    Cluster &Cluster::operator-=(const Point &p)
    {
        
         if (__dimensionality != p.getDims())
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
    Cluster &Cluster::operator+=(const Cluster &rhs) // union
    {
        
        LNodePtr temp = rhs.__points;
        
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
    
    Cluster &Cluster::operator-=(const Cluster &rhs) // (asymmetric) difference
    {
        
        LNodePtr temp = rhs.__points;
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
    
    
    
        // Friends: Arithmetic (Cluster and Point)
    const Cluster operator+(const Cluster &c, const Point &p)
    {
        if (c.__dimensionality != p.getDims())
        {
            throw DimensionalityMismatchEx(c.__dimensionality, p.getDims());
        }
        
        Cluster newCluster(c);
        newCluster += p;
        return newCluster;
        
    }
    
    const Cluster operator-(const Cluster &c, const Point &p)
    {
        if (c.__dimensionality != p.getDims())
        {
            throw DimensionalityMismatchEx(c.__dimensionality, p.getDims());
        }
        
        Cluster newCluster (c);
        newCluster -= p;
        return newCluster;
    }
    
    // Friends: Arithmetic (two Clusters)
    const Cluster operator+(const Cluster &c, const Cluster &p) // union
    {
        if (c.__dimensionality != p.__dimensionality)
        {
            throw DimensionalityMismatchEx(c.__dimensionality, p.__dimensionality);
        }
        
        Cluster newCluster(c);
        newCluster += p;
        return newCluster;
    }
    
    const Cluster operator-(const Cluster &c, const Cluster &p) // (asymmetric) difference
    {
        if (c.__dimensionality != p.__dimensionality)
        {
            throw DimensionalityMismatchEx(c.__dimensionality, p.__dimensionality);
        }
        
        Cluster newCluster (c);
        newCluster -= p;
        return newCluster;
    }

    // Move Inner class
    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to)
    : __p(p), __from(from), __to(to)
    {
        
    }
    
    void Cluster::Move::perform()
    {
        
        if (__from.contains(__p))
        {
            __to.add(__from.remove(__p));
        }
        
    }

}
