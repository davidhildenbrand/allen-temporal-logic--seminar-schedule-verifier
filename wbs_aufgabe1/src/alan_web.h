/*
 Name        : WBS Aufgabe 1
 Author      : David hildenbrand, Tobias Schoknecht
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
alan_relation intersect_relation(struct alan_web* web, int a, int b,
		alan_relation rel);
alan_relation conjunct_relation(struct alan_web* web, int a, int b,
		alan_relation rel);
alan_relation get_relation_by_mapping(struct alan_web* web, void* x, void* y);
void set_relation_by_mapping(struct alan_web* web, void* x, void* y,
		alan_relation rel);
void print_web(struct alan_web* web, FILE* file, char delimiter);

#endif /* ALAN_WEB_H_ */
