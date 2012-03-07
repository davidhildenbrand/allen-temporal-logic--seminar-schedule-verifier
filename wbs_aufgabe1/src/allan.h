/*
 Name        : WBS Aufgabe 1
 Author      : David hildenbrand, Tobias Schoknecht
 */

#ifndef _ALLAN_H_
#define _ALLAN_H_

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

typedef unsigned short allan_relation;

extern const allan_relation allan_table[REL_COUNT][REL_COUNT];

allan_relation reverse_allan_rel(allan_relation source);
char* allan_rel_to_ascii(allan_relation source);
allan_relation allan_rel_from_ascii(char *source);
allan_relation conjunct_allan_rel(allan_relation a, allan_relation b);
allan_relation substract_allan_rel(allan_relation a, allan_relation b);
allan_relation intersect_allan_rel(allan_relation a, allan_relation b);
allan_relation allan_p_function(allan_relation a, allan_relation b);
short check_allan_rel_consistency(allan_relation ab, allan_relation bc, allan_relation ac);
allan_relation allan_rel_from_intervals(unsigned int starta, unsigned int stopa,
		unsigned int startb, unsigned int stopb);

#endif /*_ALLAN_H_*/
