/*
 Name        : WBS Aufgabe 1
 Author      : David hildenbrand, Tobias Schoknecht
 */

#include "alan_web.h"
#include <stdio.h>
#include <stdlib.h>

struct alan_web* new_web(unsigned int size) {
	struct alan_web* web = (struct alan_web*) malloc(sizeof(struct alan_web));
	int i;

	web->relations = malloc(size * sizeof(alan_relation *));
	for (i = 0; i < size; ++i) {
		web->relations[i] = malloc(i * sizeof(alan_relation));
	}

	web->nodes_mapped = 0;
	web->node_mapping = malloc(size * sizeof(void*));
	for (i = 0; i < size; ++i) {
		web->node_mapping[i] = NULL;
	}

	web->size = size;

	return web;
}

struct alan_web* init_web(struct alan_web* web, alan_relation relation) {
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

struct alan_web* free_web(struct alan_web* web) {
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

int add_mapping(struct alan_web* web, void* element) {
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

void remove_mapping(struct alan_web* web, void* element) {
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

int get_mapped_index(struct alan_web* web, void* element) {
	if (element == NULL || web == NULL)
		return -1;

	int i;

	for (i = 0; i < web->nodes_mapped; ++i) {
		if (web->node_mapping[i] == element)
			return i;
	}

	return -1;
}

alan_relation get_relation(struct alan_web* web, int a, int b) {
	if (web == NULL)
		return 0;
	if (a == b || a < 0 || a >= web->size || b < 0 || b >= web->size) {
		return 0;
	}

	if (a < b)
		return reverse_relation(web->relations[b][a]);
	else
		return web->relations[a][b];
}

alan_relation intersect_relation(struct alan_web* web, int a, int b,
		alan_relation rel) {
	if (web == NULL)
		return 0;
	if (a == b || a < 0 || a >= web->size || b < 0 || b >= web->size) {
		return 0;
	}

	////
	if (a == 1 && b == 6) {
		printf("Intersecting with: %s\n", relation_to_ascii(rel));
	}
	////
	if (a < b) {
		web->relations[b][a] = intersect_relations(reverse_relation(rel),
				web->relations[b][a]);
		return reverse_relation(web->relations[b][a]);
	} else {
		web->relations[a][b] = intersect_relations(rel, web->relations[a][b]);
		return web->relations[a][b];
	}
}

alan_relation conjunct_relation(struct alan_web* web, int a, int b,
		alan_relation rel) {
	if (web == NULL)
		return 0;
	if (a == b || a < 0 || a >= web->size || b < 0 || b >= web->size) {
		return 0;
	}

	if (a < b) {
		web->relations[b][a] = conjunct_relations(reverse_relation(rel),
				web->relations[b][a]);
		return reverse_relation(web->relations[b][a]);
	} else {
		web->relations[a][b] = conjunct_relations(rel, web->relations[a][b]);
		return web->relations[a][b];
	}
}

void set_relation(struct alan_web* web, int a, int b, alan_relation rel) {
	if (web == NULL)
		return;
	if (a == b || a < 0 || a >= web->size || b < 0 || b >= web->size) {
		return;
	}

	if (a < b)
		web->relations[b][a] = reverse_relation(rel);
	else
		web->relations[a][b] = rel;
}

alan_relation get_relation_by_mapping(struct alan_web* web, void* x, void* y) {
	if (web == NULL)
		return 0;
	int a = get_mapped_index(web, x);
	int b = get_mapped_index(web, y);

	return (get_relation(web, a, b));
}

void set_relation_by_mapping(struct alan_web* web, void* x, void* y,
		alan_relation rel) {
	if (web == NULL)
		return;

	int a = get_mapped_index(web, x);
	int b = get_mapped_index(web, y);

	set_relation(web, a, b, rel);
}

void print_web(struct alan_web* web, FILE* file, char delimiter) {
	int i, j;
	if (file == NULL)
		return;

	for (i = 0; i < web->size; ++i) {
		for (j = 0; j < i; ++j) {
			char* rel = relation_to_ascii(web->relations[i][j]);
			fprintf(file, "%s", rel);
			fflush(stdout);
			free(rel);

			if (j + 1 < i)
				fprintf(file, "%c", delimiter);
		}
		fprintf(file, "\n");
	}
}
