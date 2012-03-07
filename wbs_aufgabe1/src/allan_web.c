/*
 Name        : WBS Aufgabe 1
 Author      : David hildenbrand, Tobias Schoknecht
 */

#include "allan_web.h"
#include "allan.h"
#include <stdio.h>
#include <stdlib.h>

struct allan_web* new_web(unsigned int size) {
	struct allan_web* web = (struct allan_web*) malloc(sizeof(struct allan_web));
	int i;

	web->relations = malloc(size * sizeof(allan_relation *));
	for (i = 0; i < size; ++i) {
		web->relations[i] = malloc(i * sizeof(allan_relation));
	}

	web->nodes_mapped = 0;
	web->node_mapping = malloc(size * sizeof(void*));
	for (i = 0; i < size; ++i) {
		web->node_mapping[i] = NULL;
	}

	web->size = size;

	return web;
}

struct allan_web* init_web(struct allan_web* web, allan_relation relation) {
	int i, j;
	if (web == NULL)
		return NULL;

	for (i = 0; i < web->size; ++i) {
		for (j = 0; j < i; ++j) {
			web->relations[i][j] = relation;
		}
	}

	return NULL;
}

struct allan_web* free_web(struct allan_web* web) {
	int i;
	if (web == NULL)
		return NULL;

	for (i = 0; i < web->size; ++i) {
		free(web->relations[i]);
	}
	free(web->relations);
	free(web->node_mapping);
	free(web);

	return NULL;
}

int add_mapping(struct allan_web* web, void* element) {
	int i;
	if (web->nodes_mapped == web->size || element == NULL || web == NULL)
		return -1;

	for (i = 0; i < web->size; ++i) {
		if (web->node_mapping[i] == NULL) {
			web->node_mapping[i] = element;
			web->nodes_mapped++;
			return i;
		}
	}

	return -2;
}

void remove_mapping(struct allan_web* web, void* element) {
	int i;
	if (element == NULL || web == NULL)
		return;

	for (i = 0; i < web->size; ++i) {
		if (web->node_mapping[i] == element) {
			web->node_mapping[i] = NULL;
			web->nodes_mapped--;
		}
	}
}

int get_mapped_index(struct allan_web* web, void* element) {
	if (element == NULL || web == NULL)
		return -1;

	int i;

	for (i = 0; i < web->nodes_mapped; ++i) {
		if (web->node_mapping[i] == element)
			return i;
	}

	return -1;
}

allan_relation get_relation(struct allan_web* web, int a, int b) {
	if (web == NULL)
		return 0;
	if (a == b || a < 0 || a >= web->size || b < 0 || b >= web->size) {
		return 0;
	}

	if (a < b)
		return reverse_allan_rel(web->relations[b][a]);
	else
		return web->relations[a][b];
}

allan_relation intersect_relation(struct allan_web* web, int a, int b,
		allan_relation rel) {
	if (web == NULL)
		return 0;
	if (a == b || a < 0 || a >= web->size || b < 0 || b >= web->size) {
		return 0;
	}

	////
	if (a == 1 && b == 6) {
		printf("Intersecting with: %s\n", allan_rel_to_ascii(rel));
	}
	////
	if (a < b) {
		web->relations[b][a] = intersect_allan_rel(reverse_allan_rel(rel),
				web->relations[b][a]);
		return reverse_allan_rel(web->relations[b][a]);
	} else {
		web->relations[a][b] = intersect_allan_rel(rel, web->relations[a][b]);
		return web->relations[a][b];
	}
}

allan_relation conjunct_relation(struct allan_web* web, int a, int b,
		allan_relation rel) {
	if (web == NULL)
		return 0;
	if (a == b || a < 0 || a >= web->size || b < 0 || b >= web->size) {
		return 0;
	}

	if (a < b) {
		web->relations[b][a] = conjunct_allan_rel(reverse_allan_rel(rel),
				web->relations[b][a]);
		return reverse_allan_rel(web->relations[b][a]);
	} else {
		web->relations[a][b] = conjunct_allan_rel(rel, web->relations[a][b]);
		return web->relations[a][b];
	}
}

void set_relation(struct allan_web* web, int a, int b, allan_relation rel) {
	if (web == NULL)
		return;
	if (a == b || a < 0 || a >= web->size || b < 0 || b >= web->size) {
		return;
	}

	if (a < b)
		web->relations[b][a] = reverse_allan_rel(rel);
	else
		web->relations[a][b] = rel;
}

allan_relation get_relation_by_mapping(struct allan_web* web, void* x, void* y) {
	if (web == NULL)
		return 0;
	int a = get_mapped_index(web, x);
	int b = get_mapped_index(web, y);

	return (get_relation(web, a, b));
}

void set_relation_by_mapping(struct allan_web* web, void* x, void* y,
		allan_relation rel) {
	if (web == NULL)
		return;

	int a = get_mapped_index(web, x);
	int b = get_mapped_index(web, y);

	set_relation(web, a, b, rel);
}

void print_web(struct allan_web* web, FILE* file, char delimiter) {
	int i, j;
	if (file == NULL)
		return;

	for (i = 0; i < web->size; ++i) {
		for (j = 0; j < i; ++j) {
			char* rel = allan_rel_to_ascii(web->relations[i][j]);
			fprintf(file, "%s", rel);
			fflush(stdout);
			free(rel);

			if (j + 1 < i)
				fprintf(file, "%c", delimiter);
		}
		fprintf(file, "\n");
	}
}

short check_tripple_consistency(struct allan_web* web, unsigned int a, unsigned int b, unsigned int c){
	allan_relation ab = get_relation(web,a,b);
	allan_relation bc = get_relation(web,b,c);
	allan_relation ac = get_relation(web,a,c);

	if(check_allan_rel_consistency(ab,bc,ac) != 0){
		printf("ERROR: Consistency check of %d,%d and %d failed!\n",a,b,c);
		return 1;
	}
	else
		return 0;
}

short check_consistency(struct allan_web* web){
	if(web == NULL)
		return -1;

	if(web->size < 3){
			printf("ERROR: Size of web is smaller than 3. Checking consistency can't be performed!\n");
			return -2;
	}
	else if(web->size > 3){
		printf("WARNING: Size of web is bigger than 3. Checking local consistencies only!\n");
	}

	int i,j,k;
	for(i=0;i<web->size;++i){
		for(j=i+1;j<web->size;++j){
			for(k=j+1;k<web->size;++k){
				if(check_tripple_consistency(web, i,j,k) != 0)
					return 1;
			}
		}
	}
	return 0;
}
