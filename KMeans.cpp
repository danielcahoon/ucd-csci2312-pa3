#include <iostream>
#include <fstream>

#include "Cluster.h"
#include "Exceptions.h"
#include "KMeans.h"

namespace Clustering
{
	// KMeans Constructor
	KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) :
		  __dimensionality(dim),
		  __k(k),
		  __iFileName(filename),
		  __maxIter(maxIter),
		  __numIter(0),
		  __numNonempty(0)
	{ 
		if (k == 0)
		{
			throw ZeroClustersEx();
		}

		__initCentroids = new Point*[k];
		
		std::ifstream dataIn(__iFileName);
		if (__iFileName != "")
		{
			__clusters = new Cluster*[__k];
			for (int i = 0; i < __k; ++i)
			{
				__clusters[i] = new Cluster(__dimensionality);
			}
			if (!dataIn.is_open())
			{
				for (int i = 0; i < k; ++i)
				{
					delete __clusters[i];
				}
				delete [] __clusters;
				throw DataFileOpenEx(__iFileName);
			}
			else
			{
				dataIn >> *__clusters[0];
				__numNonempty = 1;
				dataIn.close();
			}
		}
		else
		{
			throw DataFileOpenEx(__iFileName);
		}

		for (int i = 0; i < __k; ++i)
		{
			__initCentroids[i] = new Point(__dimensionality);
		}
		
		__clusters[0]->pickCentroids(k, __initCentroids);
		
		for (int i = 0; i < __k; ++i)
		{
			__clusters[i]->centroid.set(*__initCentroids[i]);
		}
	}

	// KMeans Destructor
	KMeans::~KMeans()
	{
		for (int i = 0; i < __k; ++i)
		{
			delete __initCentroids[i];
		}
		delete [] __initCentroids;

		for (int i = 0; i < __k; ++i)
		{
			delete __clusters[i];
		}
		delete [] __clusters;
	}

	// Accessors

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

	Cluster& KMeans::operator[](unsigned int u)
	{
		if (__k == 0)
		{
			throw ZeroClustersEx();
		}
		if (u >= __k)
		{
			throw DimensionalityMismatchEx(__k, u);
		}
		return *__clusters[u];
	}
	const Cluster& KMeans::operator[](unsigned int u) const
	{
		if (__k == 0)
		{
			throw ZeroClustersEx();
		}
		if (u >= __k)
		{
			throw DimensionalityMismatchEx(__k, u);
		}
		return *__clusters[u];
	}

	std::ostream& operator<<(std::ostream &os, const KMeans &kmeans)
	{
		Point inf(kmeans.__dimensionality);
		for (int i = 0; i < kmeans.__dimensionality; ++i)
		{
			inf[i] = std::numeric_limits<double>::max();
		}
		for (int i = 0; i < kmeans.__k; ++i)
		{
			if (!kmeans.__clusters[i]->centroid.equal(inf))
			{
				os << *kmeans.__clusters[i] << std::endl;	
			}
		}
		return os;
	}

	void KMeans::run()
	{
		// std::cout << "Entered run()" << std::endl;
		int moves = 100;
		int iter = 0;
		int nonempty = 0;

		while (moves > 0 && iter < __maxIter)
		{
			moves = 0;

			// Going thru all the clusters
			for (int i = 0; i < __k; ++i)
			{
				// std::cout << "i loop" << std::endl;
				// std::cout << "i = " << i << std::endl;
				// Going thru all the points in the current cluster
				for (int j = 0; j < __clusters[i]->getSize(); ++j)
				{
					// std::cout << "j loop" << std::endl;
					Cluster &c = *(__clusters[i]);

					Point current_point(__dimensionality);
					current_point = c[j];
					int smallest_dist_index = 0;
					double smallest_dist = current_point.distanceTo(*__initCentroids[0]);
					// Find closest centroid
					for (int e = 0; e < __k; e++)
					{
						// std::cout << "e loop" << std::endl;
						if (current_point.distanceTo(*__initCentroids[e]) < smallest_dist)
						{
							smallest_dist = current_point.distanceTo(*__initCentroids[e]);
							smallest_dist_index = e;
						}
						// std::cout << "end of e loop" << std::endl;
					}
					Cluster::Move change_clusters(current_point, *__clusters[i], *__clusters[smallest_dist_index]);
					change_clusters.perform();
					// Computes new centroids for the clusters
					for (int c = 0; c < __k; ++c)
					{
						// std::cout << "computed centroid" << std::endl;
						__clusters[c]->centroid.compute();
					}
					if (*__clusters[i] != *__clusters[smallest_dist_index])
					{
						moves++;
					}
					// std::cout << "end of j loop" << std::endl;
				}
			}
			iter++;
		}
		Point inf(__dimensionality);
		for (int i = 0; i < __dimensionality; ++i)
		{
			inf[i] = std::numeric_limits<double>::max();
		}

		for (int i = 0; i < __k; ++i)
		{
			if (__clusters[i]->centroid.get() != inf)
			{
				++nonempty;
			}
		}
		__numIter = iter;
		__numMovesLastIter = moves;
		__numNonempty = nonempty;
	}
}
