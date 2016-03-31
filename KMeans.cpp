//
//  KMeans.cpp
//  pa3
//
//  Copyright © 2016 Melanie Woe. All rights reserved.
//


#include "KMeans.h"
#include "Exceptions.h"



namespace Clustering
{
    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter)
    {
        
        std::ifstream infile;
        infile.open(filename);
        if (!infile.good())   //if the file cannot be open
            throw DataFileOpenEx(filename);
        
        if (k == 0)
            throw ZeroClustersEx();
        
        __clusters = new Cluster*[__k];        //cluster allocation
        for (int i = 0; i < __k; ++i)
        {
            __clusters[i] = new Cluster(dim);
        }
        infile >> *__clusters[0];
        
        infile.close(); //close file
        
        
        __dimensionality = dim;
        __k = k;
        __iFileName = filename;
        __maxIter = maxIter;  //the maximum number of iterations the algorithm can run
        __numNonempty = 1;
        
        //create centroids
        __initCentroids = new Point *[k];
        
        for (int i = 0; i < k; ++i)
        {
            __initCentroids[i] = new Point(dim);
        }
        
        __clusters[0]->pickCentroids(k, __initCentroids);
    }
    
    KMeans :: ~KMeans()
    {
        // delete clusters
        for (unsigned int i = 0; i < __k; i++)
        {
            delete __clusters[i];
        }
        delete [] __clusters;
        
        // delete centroids
        for (unsigned int i = 0; i < __k; i++)
        {
            delete __initCentroids[i];
        }
        delete [] __initCentroids;
        
    }
    
    // accessors
    unsigned int KMeans::getMaxIter()
    {
        
        return __maxIter;
    }
    unsigned int KMeans::getNumIters()
    {
        return __numIter;
    }
    unsigned int KMeans::getNumNonemptyClusters()
    {
        return __numNonempty;
    }
    unsigned int KMeans::getNumMovesLastIter()
    {
        return __numMovesLastIter;
    }

    // element access (for testing, no bounds checking)
    Cluster &KMeans::operator[](unsigned int u)
    {
        return *__clusters[u];
    }
    const Cluster &KMeans::operator[](unsigned int u) const
    {
        return *__clusters[u];
    }
    
    // write out the results to a file
    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans)
    {
        for (int i = 0; i < kmeans.__k;  i++)
        {
            os << kmeans[i];
        }
        
        return os;
        
    }

    // clustering functions
    void KMeans::run()
    {
        //will be used for the termination condition
        int moves = 100;
        int iter = 0;
        
        while (moves > 0 and iter < __maxIter)
        {
            moves = 0; // restart the count for the new iteration
            
            for (int i = 0; i < __k; ++i)
            {
                for (int h = 0; h < __clusters[i]->getSize(); ++h)
                {
                    int closest = 0;
                    double distance = (*(__clusters[i]))[h].distanceTo(__clusters[closest]->centroid.get());
                    
                    for (int j = 0; j < __k; ++j)
                    {
                        
                        if (distance > (*(__clusters[i]))[h].distanceTo(__clusters[j]->centroid.get()))
                        {
                            distance = (*(__clusters[i]))[h].distanceTo(__clusters[j]->centroid.get());
                            closest = j;
                        }
                    }
                    
                    if (i != closest)
                    {
                        Cluster::Move move((*(__clusters[i]))[h], *(__clusters[i]), *(__clusters[closest]));
                        move.perform();
                        --h;
                        
                        moves++;
                    }
                }
            }
            
            
            for (int b = 0; b < __k; ++b)
            {
                if(!__clusters[b]->centroid.isValid())
                    __clusters[b]->centroid.compute();
            }
            
            iter++;
        }
        
        //set the met data
        __numIter = iter;
        __numMovesLastIter = moves;
        
        for (int b = 0; b < __k; ++b)
        {
            if (__clusters[b]->getSize() > 0)
                __numNonempty++;
        }
        
    }

}
 

