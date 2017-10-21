#include <stdlib.h>
#include <stdio.h>
#include "object_list.h"
#include "simple_graph.h"
#include <string.h>





GraphNode * grh_create_node (char * name){

	GraphNode *ptr = malloc(sizeof(GraphNode));

	strcpy(ptr->name, name);

	ptr->neighbors = ol_create();

	return ptr;

}


void grh_delete_graph(ObjectList * graph){
	Iter *mIterator = ol_iterator(graph);
	GraphNode *ptr;
	while (ol_has_next(mIterator)){
		ptr = ol_next(mIterator);
		grh_delete_node(ptr);
	}
	free(mIterator);
	ol_destroy(graph);
}

void grh_delete_node(GraphNode * node){
	GraphNode *ptr;
	ol_destroy(node->neighbors);
	free(node);
}


GraphNode * grh_find_node_by_name( ObjectList * graph, char * name ){
	Iter* iterator = ol_iterator(graph);
	while (ol_has_next(iterator)){
		GraphNode *ptr = ol_next(iterator);
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
	while (ol_has_next(iterator)){
		GraphNode *ptr = ol_next(iterator);
		printf("%s:", ptr->name);
		
		
	
		Iter * nIterator = ol_iterator(ptr->neighbors);
		
		while (ol_has_next(nIterator)){
			GraphNode *neighborPtr = ol_next(nIterator);
			printf(" %s", neighborPtr->name);
			if (ol_has_next(nIterator)){
				printf(",");
			}
		}
		printf("\n");
		free(nIterator);
	}
	free(iterator);
}


void grh_load_file( ObjectList * graph, FILE * input ){
	char line[MAX_FILE_LINE_LENGTH];
	GraphNode *homeNode;
	GraphNode *neighborNode;
	char *homeName;
	char *neighborName;
	while (fgets (line, sizeof(line), input) != NULL){
		if (line[0] != '\n'){
			if (line[strlen(line)-1] == '\n'){
				line[strlen(line)-1] = '\0';
			}
			
			homeName = strtok(line, ",");

			if (!grh_find_node_by_name(graph, homeName)){
				//printf("Adding %s\n", homeName);
				homeNode = grh_create_node(homeName);
				ol_insert(graph, homeNode);
			}
			homeNode = grh_find_node_by_name(graph, homeName);
			neighborName = strtok(NULL, ",");
			while (neighborName != NULL){
				
				//printf("Processing %s\n", neighborName);
				if(strcmp (neighborName, homeName) != 0 ){
					if (!grh_find_node_by_name(graph, neighborName)){
						//printf("Adding %s\n", neighborName);
						ol_insert(graph, grh_create_node(neighborName));
					}
					neighborNode = grh_find_node_by_name(graph, neighborName);
					if (!grh_find_node_by_name(homeNode->neighbors, neighborName)){
						ol_insert(homeNode->neighbors, neighborNode);
						if (!grh_find_node_by_name(neighborNode->neighbors, homeName)){
							ol_insert(neighborNode->neighbors, homeNode);
						}
					}
				}
				memset(neighborName, 0, strlen(neighborName));
				neighborName = strtok(NULL, ",");
			}
		}
	}
}