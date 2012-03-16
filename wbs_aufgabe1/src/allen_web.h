/*
 Name        : WBS Aufgabe 1
 Author      : David Hildenbrand, Tobias Schoknecht
 Copyright   : David Hildenbrand, Tobias Schoknecht 2012
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* david.hildenbrand@gmail.com and tobias.schoknecht@gmail.com wrote this file.
* As long as you retain this notice you can do whatever you want with this
* stuff. If we meet some day, and you think this stuff is worth it, you can
* buy us a beer in return David Hildenbrand, Tobias Schoknecht
* ----------------------------------------------------------------------------
 */

#ifndef ALLEN_WEB_H_
#define ALLEN_WEB_H_

#include "allen.h"
#include <stdio.h>
#include <stdlib.h>

struct allen_web {
	unsigned short size;
	allen_relation** relations;
	int* node_mapping;
};

//create an new allen web with %size% nodes
struct allen_web* new_web(unsigned int size);

//init an allen web filling it with a specified relation
void init_web(struct allen_web* web, allen_relation relation);

//free an allen web
struct allen_web* free_web(struct allen_web* web);

//copy an allen web to a new web
struct allen_web* copy_web(struct allen_web* web);

//map an own (node) nr to an index in the table to have a correct output for error messages and for printing purposes
short map_nr_to_index(struct allen_web* web, unsigned short nr,
		unsigned short index);

//get the index for a mapped nr
int get_mapped_index(struct allen_web* web, unsigned short nr);

//get the nr for a mapped index
int get_mapped_nr(struct allen_web* web, unsigned short index);

//get the relation between two nodes of the web (the edge)
allen_relation get_relation(struct allen_web* web, int a, int b);

//set the relation between two nodes
void set_relation(struct allen_web* web, int a, int b, allen_relation rel);

//intersect an edge in the web with a new relation, storing the calculated relation in the web
allen_relation intersect_relation(struct allen_web* web, int a, int b,
		allen_relation rel);

//conjunct an edge in the web with a new relation, storing the calculated relation in the web
allen_relation conjunct_relation(struct allen_web* web, int a, int b,
		allen_relation rel);

//print the web to a file/stdout
void print_web(struct allen_web* web, FILE* file, char delimiter);

//check the consistency between 3 nodes
short check_tripple_consistency(struct allen_web* web, unsigned int a,
		unsigned int b, unsigned int c);

//perform the path consistency method on 3 nodes in the web, storing the intersected values in the web
short path_consistency_method_tripple(struct allen_web* web, unsigned int a,
		unsigned int b, unsigned int c);

//perform the path consistency method on a web, storing the intersected values in the web
short path_consistency_method(struct allen_web* web);

#endif /* ALLEN_WEB_H_ */
