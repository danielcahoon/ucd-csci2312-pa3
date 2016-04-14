CXX  	 := c++
CXXFLAGS := -g -std=gnu++11

ucd-csci2312-pa3: ErrorContext.o ClusteringTests.o Exceptions.o KMeans.o Cluster.o Point.o main.o
	$(CXX) $(CXXFLAGS) -o ucd-csci2312-pa3 Exceptions.o KMeans.o Cluster.o ClusteringTests.o ErrorContext.o Point.o main.o

main.o: main.cpp ClusteringTests.h ErrorContext.h
	$(CXX) $(CXXFLAGS) -c -o main.o main.cpp

Point.o: Point.h Point.cpp
	$(CXX) $(CXXFLAGS) -c -o Point.o Point.cpp

Cluster.o: Cluster.h Cluster.cpp
	$(CXX) $(CXXFLAGS) -c -o Cluster.o Cluster.cpp

KMeans.o: KMeans.h KMeans.cpp
	$(CXX) $(CXXFLAGS) -c -o KMeans.o KMeans.cpp

Exceptions.o: Exceptions.h Exceptions.cpp
	$(CXX) $(CXXFLAGS) -c -o Exceptions.o Exceptions.cpp

ClusteringTests.o: ClusteringTests.h ClusteringTests.cpp
	$(CXX) $(CXXFLAGS) -c -o ClusteringTests.o ClusteringTests.cpp

ErrorContext.o: ErrorContext.h ErrorContext.cpp
	$(CXX) $(CXXFLAGS) -c -o ErrorContext.o ErrorContext.cpp

.PHONY: clean test

clean:
	rm -f ErrorContext.o ClusteringTests.o Exceptions.o KMeans.o Cluster.o Point.o main.o ucd-csci2312-pa3 

test: ucd-csci2312-pa3
	./ucd-csci2312-pa3