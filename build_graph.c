/***
Author: Quan Dong
Simple main class that allows for the creation of an adjacency list representation of graphs.

*/


#include <stdlib.h>
#include <stdio.h>
#include "simple_graph.h"

int main (void){
	printf("Reading graph data...\n");

	///Create the initial graph to be populated/printed/destroyed
	ObjectList* graph = ol_create();
	
	///Fill the graph with nodes 
	grh_load_file(graph, stdin);
	

	printf("Finished reading graph data.\n");
	
	///Print out the graph's data
	grh_print_graph(graph);
	
	///Delete the graph
	grh_delete_graph(graph);

	return 0;
}