//============================================================================
// Name        : CPP.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <tr1/functional>
#include "sorting.h"
#include "hash_table.h"
#include "binarytree.h"
#include "graph.h"
std::size_t hashFunction(const std::string& val)
{
	static std::tr1::hash<std::string> fn;
	return fn(val);
}
int main() {

	matrix_graph<std::string> graph;
	graph.addVertice("A");
	graph.addVertice("B");
	graph.addEdge("A","B",5);

//	const int n5 = 5;
//int arr[n5] = {1};
//cout << arr[4];
//	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
//	return 0;
	std::vector<int> array;
	array.push_back(5);
	array.push_back(6);
	array.push_back(3);
	array.push_back(2);
	array.push_back(14);

	std::vector<int> array2 = array;
	insert_sort<int>(array2);
	std::vector<int> array3 = merge_sort<int>(array2);

	std::vector<int> array4 = array;
	quick_sort(array4);

	hash_table<std::string,int> mytable(100,hashFunction);
	mytable.add("one",1);
	mytable.add("two",2);
	mytable.add("three",3);

	int ret;
	mytable.get("one",ret);
	mytable.get("two",ret);
	mytable.get("three",ret);
//	return 0;

	binary_tree<int> myTree;

	myTree.add(18);
	myTree.add(11);
	myTree.add(2);
	myTree.add(21);
	myTree.add(7);
	myTree.add(6);
	myTree.add(11);
	myTree.add(3);
	myTree.add(19);
	myTree.add(27);
	myTree.add(54);
	myTree.add(34);
	myTree.add(21);
	myTree.add(16);
	myTree.add(17);
	myTree.add(13);
	myTree.add(18);
	myTree.add(25);
	myTree.add(24);
	myTree.add(37);
	myTree.add(56);
	myTree.add(22);
	myTree.add(33);
	myTree.add(44);
	myTree.add(8);

	std::cout << "PreOrder" << std::endl;
	myTree.preOrder(std::cout);

	std::cout << "inOrder" << std::endl;
	myTree.inOrder(std::cout);

	std::cout << "iterative" << std::endl;
	myTree.iterativeTraversal(std::cout);

	std::cout << "PostOrder" << std::endl;
	myTree.postOrder(std::cout);


	return 0;

}
