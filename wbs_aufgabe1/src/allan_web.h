/*
 Name        : WBS Aufgabe 1
 Author      : David hildenbrand, Tobias Schoknecht
 */

#ifndef ALLAN_WEB_H_
#define ALLAN_WEB_H_

#include "allan.h"
#include <stdio.h>
#include <stdlib.h>

struct allan_web {
	unsigned short size;
	allan_relation** relations;
	int* node_mapping;
};

struct allan_web* new_web(unsigned int size);
void init_web(struct allan_web* web, allan_relation relation);
struct allan_web* free_web(struct allan_web* web);
short map_nr_to_index(struct allan_web* web, unsigned short nr,
		unsigned short index);
int get_mapped_index(struct allan_web* web, unsigned short nr);
int get_mapped_nr(struct allan_web* web, unsigned short index);
allan_relation get_relation(struct allan_web* web, int a, int b);
void set_relation(struct allan_web* web, int a, int b, allan_relation rel);
allan_relation intersect_relation(struct allan_web* web, int a, int b,
		allan_relation rel);
allan_relation conjunct_relation(struct allan_web* web, int a, int b,
		allan_relation rel);
void print_web(struct allan_web* web, FILE* file, char delimiter);
short check_tripple_consistency(struct allan_web* web, unsigned int a,
		unsigned int b, unsigned int c);
short path_consistency_method_tripple(struct allan_web* web, unsigned int a,
		unsigned int b, unsigned int c);
short path_consistency_method(struct allan_web* web);

#endif /* ALLAN_WEB_H_ */
