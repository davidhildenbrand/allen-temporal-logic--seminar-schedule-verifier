/*
 * alan_web.h
 *
 *  Created on: 06.03.2012
 *      Author: davidhildenbrand
 */

#ifndef ALAN_WEB_H_
#define ALAN_WEB_H_

#include "alan.h"
#include <stdio.h>
#include <stdlib.h>


struct alan_web {
	int size;
	alan_relation** relations;
	void** node_mapping;
	unsigned int nodes_mapped;
};

struct alan_web* new_web(unsigned int size);
struct alan_web* init_web(struct alan_web* web, alan_relation relation);
struct alan_web* free_web(struct alan_web* web);
int add_mapping(struct alan_web* web, void* element);
int get_mapped_index(struct alan_web* web, void* element);
alan_relation get_relation(struct alan_web* web, int a, int b);
void set_relation(struct alan_web* web, int a, int b, alan_relation rel);
alan_relation intersect_relation(struct alan_web* web, int a, int b, alan_relation rel);
alan_relation conjunct_relation(struct alan_web* web, int a, int b, alan_relation rel);
alan_relation get_relation_by_mapping(struct alan_web* web, void* x, void* y);
void set_relation_by_mapping(struct alan_web* web, void* x, void* y, alan_relation rel);
void print_web(struct alan_web* web, FILE* file, char delimiter);

//////////////////////////////////////////////////
//
//
//struct alan_web_node {
//	int node_nr;
//	int alt_node_nr;
//	void* data;
//};
//
//struct alan_web_edge {
//	struct alan_web_node* left;
//	struct alan_web_node* right;
//	alan_relation relation;
//};
//
//struct alan_web_edge_list_item {
//	struct alan_web_edge* edge;
//	struct alan_web_edge_list_item* next;
//};

#endif /* ALAN_WEB_H_ */
