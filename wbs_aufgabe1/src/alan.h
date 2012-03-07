/*
 Name        : WBS Aufgabe 1
 Author      : David hildenbrand, Tobias Schoknecht
 */

#ifndef _ALAN_H_
#define _ALAN_H_

#include <string.h>

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

typedef unsigned short alan_relation;

extern const alan_relation alan_table[REL_COUNT][REL_COUNT];

char* relation_to_ascii(alan_relation source);
alan_relation relation_from_ascii(char *source);
alan_relation conjunct_relations(alan_relation a, alan_relation b);
alan_relation substract_relation(alan_relation a, alan_relation b);
alan_relation intersect_relations(alan_relation a, alan_relation b);
alan_relation P(alan_relation a, alan_relation b);

#endif /*_ALAN_H_*/
