#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <algorithm>
#include <map>
#include <iterator>


template<class T>
class matrix_graph
{

	struct Weight {

		Weight() = default;
		Weight(int x) : weight_(x)
		{

		}
		int weight_=-1;
	};

	std::vector<std::vector<Weight> > edges_;
	std::vector<T> vertices_;
	std::map<T,int> vertice_indexes_;

public:


	void addEdge(const T& start,const T& end, const int weight)
	{
		// from x to y
		int start_index = vertice_indexes_[start];
		int end_index = vertice_indexes_[end];
		edges_.at(start_index).at(end_index) = Weight(weight);

		edges_[start_index][end_index]=weight;
	}
	void addVertice(const T& newVertice)
	{
		// add new object to our list of vertices
		vertices_.push_back(newVertice);
		vertice_indexes_[newVertice] = vertices_.size()-1;

		// add new weight (an empty one all the other
		// edges

		// ha! use lambda... to add another weight
		std::for_each(edges_.begin(),edges_.end(),[&](std::vector<Weight>& we){
		     we.push_back(Weight(-1));
		});

		// now create the weights for the
		std::vector<Weight> newWeights;
		newWeights.resize(vertices_.size());
		edges_.push_back(newWeights);
	}



};


#endif
