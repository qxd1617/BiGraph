/***
Author: Quan Dong
Class Description: Class that handles the creation, destruction, printing, and searching
of graph nodes as well as the parsing of CSV files to generate initial graph status.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "simple_graph.h"

GraphNode * grh_create_node (char * name){

	///Allocate space for GraphNode
	GraphNode *ptr = malloc(sizeof(GraphNode));

	///Assign the name with strcpy
	strcpy(ptr->name, name);

	///Create a blank list according to object_list.h
	ptr->neighbors = ol_create();

	///Return ptr for use.
	return ptr;

}


void grh_delete_graph(ObjectList * graph){
	/**
	Function to delete an entire graph, including all of the objects inside it by freeing the memory used.
	@args graph: the graph to be deleted
	*/
	///Create an iterator to go through the graph's list of nodes
	Iter *mIterator = ol_iterator(graph);
	GraphNode *ptr;
	
	
	while (ol_has_next(mIterator)){
		///While the list has more nodes, grab the next node in the list and delete it with grh_delete_node
		ptr = ol_next(mIterator);
		grh_delete_node(ptr);
	}
	///Free the iterator
	free(mIterator);
	///Destroy the graph as specified by object_list.h
	ol_destroy(graph);
}

void grh_delete_node(GraphNode * node){
	///Destroy the list of it's neighbors. Since all neighbors will be in the main node list, we can 
	///just remove the list itself and the nodes will be cleaned up in the outer loop.
	ol_destroy(node->neighbors);
	///Free the node itself.
	free(node);
}


GraphNode * grh_find_node_by_name( ObjectList * graph, char * name ){
	
	
	Iter* iterator = ol_iterator(graph);
	
	while (ol_has_next(iterator)){
		
		GraphNode *ptr = ol_next(iterator);
		
		///If the node's name matches the name given in @args, free the iterator and return a pointer to the node.
		if (strcmp(ptr->name, name) == 0){
			free(iterator);
			return ptr;
		}
	}
	free(iterator);
	return 0;
	
}


void grh_print_graph( ObjectList * graph ){
	Iter *iterator = ol_iterator(graph);
	if (ol_has_next(iterator)){
		///Only print if the graph's nodelist has nodes
		printf("Graph: {name: neighbor names list}\n");
		while (ol_has_next(iterator)){
			
			GraphNode *ptr = ol_next(iterator);
			///Print the main node's name
			printf("%s:", ptr->name);
			
			
			Iter * nIterator = ol_iterator(ptr->neighbors);
			while (ol_has_next(nIterator)){
				///Iterate through the main node's neighbors and print them out.
				GraphNode *neighborPtr = ol_next(nIterator);
				printf(" %s", neighborPtr->name);
				if (ol_has_next(nIterator)){
					printf(",");
				}
			}
			printf("\n");
			///Free the iterators after use.
			free(nIterator);
		}
	}
	free(iterator);
}


void grh_load_file( ObjectList * graph, FILE * input ){
	/***
	Function that takes  a ObjectList (named graph) and populates it according to well-formatted CSV files.
	*/
	///Define the line to be the max length given in this file's .h equivalent
	char line[MAX_FILE_LINE_LENGTH];
	
	///Declare strings/graphnode pointers
	GraphNode *homeNode;
	GraphNode *neighborNode;
	char *homeName;
	char *neighborName;
	
	
	while (fgets (line, sizeof(line), input) != NULL){
		///If the line is just a newline, then skip the line.
		if (line[0] != '\n'){
			if (line[strlen(line)-1] == '\n'){
				///Strip the newline from the end of the line for formatting purposes.
				line[strlen(line)-1] = '\0';
			}
			
			///Use strtok to grab the first comma separated term in the list.
			homeName = strtok(line, ",");
			
			///If the node isn't already in the graph's nodelist, then add it.
			if (!grh_find_node_by_name(graph, homeName)){
				homeNode = grh_create_node(homeName);
				ol_insert(graph, homeNode);
			}
			
			///Reassign the homeNode pointer to the location of the home node, either newly created or found by search.
			homeNode = grh_find_node_by_name(graph, homeName);

			///Grab the next term in the line
			neighborName = strtok(NULL, ",");
			
			
			while (neighborName != NULL){
				///While the line has more comma separated terms
				if(strcmp (neighborName, homeName) != 0 ){
					///If the current term is not equivalent to the home term 
					///(if they are, we ignore them as you cannot be a neighbor of yourself.)
					
					if (!grh_find_node_by_name(graph, neighborName)){
						///If the neighborNode is not in the main graph's nodelist, create the node and add it.
						ol_insert(graph, grh_create_node(neighborName));
					}
					///Set neighborNode pointer to the node that was just created (or found).
					neighborNode = grh_find_node_by_name(graph, neighborName);
					
					
					if (!grh_find_node_by_name(homeNode->neighbors, neighborName)){
						///If the neighborNode isn't in the homeNode's neighbor list, then add it.
						ol_insert(homeNode->neighbors, neighborNode);
						
						
						if (!grh_find_node_by_name(neighborNode->neighbors, homeName)){
							///If the homeNode isn't in the neighborNode's neighbor list, then add it as the graph is bidirectional.
							ol_insert(neighborNode->neighbors, homeNode);
						}
					}
				}
				neighborName = strtok(NULL, ",");
			}
		}
	}
}