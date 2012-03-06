/*
 * alan.c
 *
 *  Created on: 05.03.2012
 *      Author: davidhildenbrand
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alan.h"

const alan_relation alan_table[REL_COUNT][REL_COUNT] = {
		{Aeq,Asm,Abi,Ad,Adi,Ao,Aoi,Am,Ami,As,Asi,Af,Afi},
		{Asm,Asm,All,Asm|Ao|Am|Ad|As,As,As,Asm|Ao|Am|Ad|As,As,Asm|Ao|Am|Ad|As,As,As,Asm|Ao|Am|Ad|As,As},
		{Abi,All,Abi,Abi|Aoi|Ami|Ad|Af,Abi,Abi|Aoi|Ami|Ad|Af,Abi,Abi|Aoi|Ami|Ad|Af,Abi,Abi|Aoi|Ami|Ad|Af,Abi,Abi,Abi},
		{Ad,Asm,Abi,Ad,All,Asm|Ao|Am|Ad|As,Abi|Aoi|Ami|Ad|Af,Asm,Abi,Ad,Abi|Aoi|Ami|Ad|Af,Ad,Asm|Ao|Am|Ad|As},
		{Adi,Asm|Ao|Am|Adi|Afi,Asm|Aoi|Ami|Adi|Asi,Aeq|Ad|Adi|Ao|Aoi|As|Asi|Af|Afi,Adi,Ao|Adi|Afi,Aoi|Adi|Asi,Ao|Adi|Afi,Aoi|Adi|Asi,Ao|Adi|Afi,Adi,Aoi|Adi|Asi,Adi},
		{Ao,Asm,Abi|Aoi|Ami|Adi|Asi,Ao|Ad|As,Asm|Ao|Am|Adi|Afi,Asm|Ao|Am,Aeq|Ad|Adi|Ao|Aoi|As|Asi|Af|Afi,Asm|Aoi|Adi|Asi,Ao|Adi|Afi|Ao,Ad|As|Ao,Asm|Ao|Am},
		{Aoi,Asm|Ao|Am|Adi|Afi,Abi,Aoi|Ad|Af,Asm|Aoi|Ami|Adi|Asi,Aeq|Ad|Adi|Ao|Aoi|As|Asi|Af|Afi,Abi|Aoi|Ami,Ao|Adi|Afi,Abi,Aoi|Ad|Af,Abi|Aoi|Ami,Aoi,Aoi|Asi|Adi},
		{Am,Asm,Abi|Aoi|Ami|Adi|Asi,Ao|Ad|As,Asm,Asm,Ao|Ad|As,Asm,Aeq|Af|Afi,Am,Am,Ad|As|Ao,Asm},
		{Ami,Asm|Ao|Am|Adi|Afi,Abi,Aoi|Ad|Af,Abi,Aoi|Adi|Af,Abi,Aeq|As|Asi,Abi,Ad|Af|Aoi,Abi,Ami,Ami},
		{As,Asm,Abi,Ad,Asm|Ao|Am|Adi|Afi,Asm|Ao|Am,Aoi|Ad|Af,Asm,Ami,As,Aeq|As|Asi,Ad,Asm|Ao|Am},
		{Asi,Asm|Ao|Am|Adi|Afi,Abi,Aoi|Ad|Af,Adi,Ao|Adi|Afi,Aoi,Ao|Adi|Afi,Ami,Aeq|As|Asi,Asi,Aoi,Adi},
		{Af,Asm,Abi,Ad,Abi|Aoi|Ami|Adi|Asi,Ao|Ad|As,Abi|Aoi|Ami,Am,Abi,Ad,Abi|Aoi|Ami,Af,Aeq|Af|Afi},
		{Afi,Asm,Abi|Aoi|Ami|Adi|Asi,Ao|Ad|As,Adi,Ao,Aoi|Adi|Asi,Am,Aoi|Adi|Asi,Ao,Adi,Aeq|Af|Afi,Afi}
};

alan_relation reverse_relation(alan_relation source){
	alan_relation destination = 0x0;

	if(source & Aeq)
		destination |= Aeq;
	if(source & Asm)
			destination |= Abi;
	if(source & Abi)
			destination |= Asm;
	if(source & Ad)
			destination |= Adi;
	if(source & Adi)
			destination |= Ad;
	if(source & Ao)
			destination |= Aoi;
	if(source & Aoi)
			destination |= Ao;
	if(source & Am)
			destination |= Ami;
	if(source & Ami)
			destination |= Am;
	if(source & As)
			destination |= Asi;
	if(source & Asi)
			destination |= As;
	if(source & Af)
			destination |= Afi;
	if(source & Afi)
			destination |= Af;

	return destination;
}

char* relation_to_ascii(alan_relation source){
	char buffer[128];
	unsigned short pos = 0;

	if(source & Aeq){
		buffer[pos++] = '=';
		buffer[pos++] = ' ';
	}
	if(source & Asm){
		buffer[pos++] = '<';
		buffer[pos++] = ' ';
	}
	if(source & Abi){
		buffer[pos++] = '>';
		buffer[pos++] = ' ';
	}
	if(source & Ad){
		buffer[pos++] = 'd';
		buffer[pos++] = ' ';
	}
	if(source & Adi){
		buffer[pos++] = 'd';
		buffer[pos++] = 'i';
		buffer[pos++] = ' ';
	}
	if(source & Ao){
		buffer[pos++] = 'o';
		buffer[pos++] = ' ';
	}
	if(source & Aoi){
		buffer[pos++] = 'o';
		buffer[pos++] = 'i';
		buffer[pos++] = ' ';
	}
	if(source & Am){
		buffer[pos++] = 'm';
		buffer[pos++] = ' ';
	}
	if(source & Ami){
		buffer[pos++] = 'm';
		buffer[pos++] = 'i';
		buffer[pos++] = ' ';
	}
	if(source & As){
		buffer[pos++] = 's';
		buffer[pos++] = ' ';
	}
	if(source & Asi){
		buffer[pos++] = 's';
		buffer[pos++] = 'i';
		buffer[pos++] = ' ';
	}
	if(source & Af){
		buffer[pos++] = 'f';
		buffer[pos++] = ' ';
	}
	if(source & Afi){
		buffer[pos++] = 'f';
		buffer[pos++] = 'i';
		buffer[pos++] = ' ';
	}

	char* copy = NULL;

	if(pos == 0){
		copy = (char*)malloc(4*sizeof(char));
		copy = strcpy(copy,"NIL");
	}
	else{
		copy = (char*)malloc((pos+1)*sizeof(char));
		buffer[--pos] = '\0';
		copy = strcpy(copy,buffer);
	}

	return copy;
}

alan_relation relation_from_ascii(char *source){
	int i;
	alan_relation destination = 0;

	if (source == NULL)
		return 0;

	for(i=0;source[i] != '\0';++i){
		switch (source[i]){
		case '=':
			destination |= Aeq;
			break;
		case '<':
			destination |= Asm;
			break;
		case '>':
			destination |= Abi;
			break;
		case 'd':
			if(source[i+1] == 'i'){
				destination |= Adi;
			}
			else
				destination |= Ad;
			break;
		case 'm':
			if(source[i+1] == 'i'){
				destination |= Ami;
			}
			else
				destination |= Am;
			break;
		case 'o':
			if(source[i+1] == 'i'){
				destination |= Aoi;
			}
			else
				destination |= Ao;
			break;
		case 's':
			if(source[i+1] == 'i'){
				destination |= Asi;
			}
			else
				destination |= As;
			break;
		case 'f':
			if(source[i+1] == 'i'){
				destination |= Afi;
			}
			else
				destination |= Af;
			break;
		default:
			break;
		};
	}
	return destination;
}

alan_relation conjunct_relations(alan_relation a,alan_relation b){
	return a|b;
}

alan_relation substract_relation(alan_relation a,alan_relation b){
	return a&(~b);
}

alan_relation intersect_relations(alan_relation a,alan_relation b){
	return a&b;
}

alan_relation P(alan_relation a,alan_relation b){
	alan_relation destination = 0x0, temp;
	short i,j;

	for(i=0;i<REL_COUNT;a>>=1,++i){
		if(a & 0x1){
			temp = b;
			for(j=0;j<REL_COUNT;temp>>=1,++j){
				if(temp & 0x1){
					destination = conjunct_relations(destination,alan_table[i][j]);
				}
			}
		}
	}

	return destination;
}
