/*
 Name        : WBS Aufgabe 1
 Author      : David hildenbrand, Tobias Schoknecht
 */

#include "allan_web.h"
#include "allan.h"
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

struct allan_web* new_web(unsigned int size) {
	struct allan_web* web = (struct allan_web*) malloc(
			sizeof(struct allan_web));
	int i;

	web->relations = malloc(size * sizeof(allan_relation *));
	//create only a half table. first row has 0, last row has i-1 entries!
	//-> other relations are either "=" or can be computed using the reverse!
	for (i = 0; i < size; ++i) {
		if(i == 0)
			//could be removed but it is easier to access the data!
			web->relations[i] = NULL;
		else
			web->relations[i] = malloc(i * sizeof(allan_relation));
	}

	web->node_mapping = malloc(size * sizeof(int));
	//init the mapping of numbers to nodes to the default
	for (i = 0; i < size; ++i) {
		web->node_mapping[i] = -1;
	}

	web->size = size;

	return web;
}

struct allan_web* copy_web(struct allan_web* web) {
	if(web == NULL)
		return NULL;

	struct allan_web* copy = new_web(web->size);
	int i,j;

	copy->size = web->size;

	//copy the mapping and the data
	for(i=0;i<copy->size;++i){
		copy->node_mapping[i] = web->node_mapping[i];

		for(j=0;j<i;++j)
			copy->relations[i][j] = web->relations[i][j];
	}
}

void init_web(struct allan_web* web, allan_relation relation) {
	int i, j;
	if (web == NULL)
		return;

	for (i = 0; i < web->size; ++i) {
		for (j = 0; j < i; ++j) {
			web->relations[i][j] = relation;
		}
	}

	return;
}

int get_mapped_nr(struct allan_web* web, unsigned short index) {
	if (web == NULL)
		return -3;

	if (index >= web->size)
		return -1;

	int nr = web->node_mapping[index];
	//as a default the mapping will be the index
	if (nr == -1)
		nr = index;
	return nr;
}

int get_mapped_index(struct allan_web* web, unsigned short nr) {
	if (web == NULL)
		return -3;

	unsigned short i;
	//index does not exist
	if (index < web->size)
		return -1;

	//find the mapped value
	for (i = 0; i < web->size; ++i) {
		if (web->node_mapping[i] == nr)
			return i;
	}

	//value not mapped?
	return -2;
}

short map_nr_to_index(struct allan_web* web, unsigned short nr,
		unsigned short index) {
	if (web == NULL)
		return -3;

	int old_index = -1;
	//index does not exist
	if (index >= web->size)
		return -1;

	if (web->node_mapping[index] != -1)
		log(WARN,"Overwriting existing mapping of index %d.", index);

	if (old_index = get_mapped_index(web, nr) >= 0) {
		log(ERROR,"Nr %d already mapped to index %d.", nr, old_index);
		return -2;
	}

	web->node_mapping[index] = nr;

	return 0;
}

void remove_mapping(struct allan_web* web, unsigned short index) {
	if (web == NULL)
		return;

	if (index >= web->size)
		return;

	web->node_mapping[index] = -1;
}

void clear_mapping(struct allan_web* web) {
	int i;

	if (web == NULL)
		return;

	for (i = 0; i < web->size; ++i) {
		web->node_mapping[i] = -1;
	}
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

allan_relation get_relation(struct allan_web* web, int a, int b) {
	if (web == NULL)
		return 0;
	if (a < 0 || a >= web->size || b < 0 || b >= web->size) {
		return 0;
	}
	if(a == b)
		return Aeq;
	else if (a < b)
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

void print_web(struct allan_web* web, FILE* file, char delimiter) {
	int i, j;
	if (file == NULL)
		return;

	//table header with node identifiers
	fprintf(file, "%c", delimiter);
	for (i = 0; i < web->size; i++) {
		fprintf(file, "%d", get_mapped_nr(web, i));
		fprintf(file, "%c", delimiter);
	}
	fprintf(file, "\n");

	//table content
	for (i = 0; i < web->size; ++i) {
		//node identifier
		fprintf(file, "%d%c", get_mapped_nr(web, i), delimiter);
		for (j = 0; j < web->size; ++j) {

			char* rel = allan_rel_to_ascii(get_relation(web,i,j));
			fprintf(file, "%s", rel);
			free(rel);

			fprintf(file, "%c", delimiter);
		}
		fprintf(file, "\n");
	}
	fflush(file);
}

short check_tripple_consistency(struct allan_web* web, unsigned int a,
		unsigned int b, unsigned int c) {
	allan_relation ab = get_relation(web, a, b);
	allan_relation bc = get_relation(web, b, c);
	allan_relation ac = get_relation(web, a, c);

	if (check_allan_rel_consistency(ab, bc, ac) != 0) {
		log(ERROR,"Consistency check of %d,%d and %d failed!", a, b, c);
		return 1;
	} else
		return 0;
}

short path_consistency_method_tripple(struct allan_web* web, unsigned int a,
		unsigned int b, unsigned int c) {
	allan_relation ab = get_relation(web, a, b);
	allan_relation bc = get_relation(web, b, c);
	allan_relation ac = get_relation(web, a, c);

	//calculate the ac
	allan_relation cal_ac = allan_p_function(ab, bc);

	//intersect the calculated one with the existing one
	allan_relation new_ac = intersect_relation(web, a, c, cal_ac);

	//error in the web
	if (new_ac == 0) {
		log(ERROR,"Consistency check of %d,%d and %d failed!",
				get_mapped_nr(web, a), get_mapped_nr(web, b),
				get_mapped_nr(web, c));
		return -1;
	}
	//nothing changed
	else if (new_ac == ac)
		return 0;
	//value changed
	else {
		return 1;
	}
}

short path_consistency_method(struct allan_web* web) {
	if (web == NULL)
		return -1;

	if (web->size < 3) {
		log(ERROR,"Size of web is smaller than 3. Cannot check consistency!");
		return -2;
	} else if (web->size > 3) {
		log(WARN,"Size of web is bigger than 3. Web might be path-consistent but incomplete and unsatisfiable!");
	}

	int i, j, k;
	short change = 1;

	//perform the path method while we have a change
	while (change == 1) {
		change = 0;
		//iterate all edges
		for (i = 0; i < web->size; ++i) {
			for (j = i + 1; j < web->size; ++j) {
				for (k = j + 1; k < web->size; ++k) {
					short ret = path_consistency_method_tripple(web, i, j, k);
					if (ret == 1)
						change = 1;
					if (ret == -1)
						return 1;
				}
			}
		}
	}
	return 0;
}