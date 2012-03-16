/*
 Name        : WBS Aufgabe 1
 Author      : David Hildenbrand, Tobias Schoknecht
 Copyright   : David Hildenbrand, Tobias Schoknecht 2012
 */

#ifndef _ALLEN_H_
#define _ALLEN_H_

#include <string.h>
#include <stdio.h>

#define ONE 0x0001

#define ANeq 0
#define ANsm 1
#define ANbi 2
#define ANd 3
#define ANdi 4
#define ANo 5
#define ANoi 6
#define ANm 7
#define ANmi 8
#define ANs 9
#define ANsi 10
#define ANf 11
#define ANfi 12

#define Aeq ONE<<ANeq
#define Asm ONE<<ANsm
#define Abi ONE<<ANbi
#define Ad ONE<<ANd
#define Adi ONE<<ANdi
#define Ao ONE<<ANo
#define Aoi ONE<<ANoi
#define Am ONE<<ANm
#define Ami ONE<<ANmi
#define As ONE<<ANs
#define Asi ONE<<ANsi
#define Af ONE<<ANf
#define Afi ONE<<ANfi

#define All Aeq|Asm|Abi|Ad|Adi|Ao|Aoi|Am|Ami|As|Asi|Af|Afi

#define REL_COUNT 13

typedef unsigned short allen_relation;

//allow access to the table from other programs
extern const allen_relation allen_table[REL_COUNT][REL_COUNT];

//returns the reverse of a relation
allen_relation reverse_allen_rel(allen_relation source);

//visualizes a relation
char* allen_rel_to_ascii(allen_relation source);

//creates a relation from ascii
allen_relation allen_rel_from_ascii(char *source);

//calculate the conjunction of two relations
allen_relation conjunct_allen_rel(allen_relation a, allen_relation b);

//substract relation b from a
allen_relation substract_allen_rel(allen_relation a, allen_relation b);

//calculate the intersection of two relations
allen_relation intersect_allen_rel(allen_relation a, allen_relation b);

//calculate the resulting relation (a->c) from two relations a->b and b->c -> Allens P()-function
allen_relation allen_p_function(allen_relation a, allen_relation b);

//check if three relations are consistent a->b, b->c, a->c
short check_allen_rel_consistency(allen_relation ab, allen_relation bc, allen_relation ac);

//determine the relation of two intervals having a start and a stop time
allen_relation allen_rel_from_intervals(unsigned int starta, unsigned int stopa,
		unsigned int startb, unsigned int stopb);

//print the allen table to the file/stdout
void print_allen_table(FILE* file, char delimiter);

#endif /*_ALLEN_H_*/
