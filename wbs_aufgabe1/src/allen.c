/*
 Name        : WBS Aufgabe 1
 Author      : David Hildenbrand, Tobias Schoknecht
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allen.h"

const allen_relation allen_table[REL_COUNT][REL_COUNT] = { { Aeq, Asm, Abi, Ad,
		Adi, Ao, Aoi, Am, Ami, As, Asi, Af, Afi }, { Asm, Asm, All, Asm | Ao
		| Am | Ad | As, Asm, Asm, Asm | Ao | Am | Ad | As, Asm, Asm | Ao | Am | Ad
		| As, Asm, Asm, Asm | Ao | Am | Ad | As, Asm }, { Abi, All, Abi, Abi | Aoi
		| Ami | Ad | Af, Abi, Abi | Aoi | Ami | Ad | Af, Abi, Abi | Aoi | Ami
		| Ad | Af, Abi, Abi | Aoi | Ami | Ad | Af, Abi, Abi, Abi }, { Ad, Asm,
		Abi, Ad, All, Asm | Ao | Am | Ad | As, Abi | Aoi | Ami | Ad | Af, Asm,
		Abi, Ad, Abi | Aoi | Ami | Ad | Af, Ad, Asm | Ao | Am | Ad | As }, {
		Adi, Asm | Ao | Am | Adi | Afi, Asm | Aoi | Ami | Adi | Asi, Aeq | Ad
				| Adi | Ao | Aoi | As | Asi | Af | Afi, Adi, Ao | Adi | Afi, Aoi
				| Adi | Asi, Ao | Adi | Afi, Aoi | Adi | Asi, Ao | Adi | Afi,
		Adi, Aoi | Adi | Asi, Adi }, { Ao, Asm, Abi | Aoi | Ami | Adi | Asi, Ao
		| Ad | As, Asm | Ao | Am | Adi | Afi, Asm | Ao | Am, Aeq | Ad | Adi | Ao
		| Aoi | As | Asi | Af | Afi, Asm, Aoi | Adi | Asi, Ao, Adi | Afi | Ao,
		Ad | As | Ao, Asm | Ao | Am }, { Aoi, Asm | Ao | Am | Adi | Afi, Abi,
		Aoi | Ad | Af, Asm | Aoi | Ami | Adi | Asi, Aeq | Ad | Adi | Ao | Aoi
				| As | Asi | Af | Afi, Abi | Aoi | Ami, Ao | Adi | Afi, Abi, Aoi
				| Ad | Af, Abi | Aoi | Ami, Aoi, Aoi | Asi | Adi }, { Am, Asm,
		Abi | Aoi | Ami | Adi | Asi, Ao | Ad | As, Asm, Asm, Ao | Ad | As, Asm,
		Aeq | Af | Afi, Am, Am, Ad | As | Ao, Asm }, { Ami, Asm | Ao | Am | Adi
		| Afi, Abi, Aoi | Ad | Af, Abi, Aoi | Ad | Af, Abi, Aeq | As | Asi,
		Abi, Ad | Af | Aoi, Abi, Ami, Ami }, { As, Asm, Abi, Ad, Asm | Ao | Am
		| Adi | Afi, Asm | Ao | Am, Aoi | Ad | Af, Asm, Ami, As, Aeq | As | Asi,
		Ad, Asm | Ao | Am }, { Asi, Asm | Ao | Am | Adi | Afi, Abi, Aoi | Ad
		| Af, Adi, Ao | Adi | Afi, Aoi, Ao | Adi | Afi, Ami, Aeq | As | Asi,
		Asi, Aoi, Adi }, { Af, Asm, Abi, Ad, Abi | Aoi | Ami | Adi | Asi, Ao
		| Ad | As, Abi | Aoi | Ami, Am, Abi, Ad, Abi | Aoi | Ami, Af, Aeq | Af
		| Afi }, { Afi, Asm, Abi | Aoi | Ami | Adi | Asi, Ao | Ad | As, Adi, Ao,
		Aoi | Adi | Asi, Am, Aoi | Adi | Asi, Ao, Adi, Aeq | Af | Afi, Afi } };

allen_relation reverse_allen_rel(allen_relation source) {
	allen_relation destination = 0x0;

	if (source & Aeq)
		destination |= Aeq;
	if (source & Asm)
		destination |= Abi;
	if (source & Abi)
		destination |= Asm;
	if (source & Ad)
		destination |= Adi;
	if (source & Adi)
		destination |= Ad;
	if (source & Ao)
		destination |= Aoi;
	if (source & Aoi)
		destination |= Ao;
	if (source & Am)
		destination |= Ami;
	if (source & Ami)
		destination |= Am;
	if (source & As)
		destination |= Asi;
	if (source & Asi)
		destination |= As;
	if (source & Af)
		destination |= Afi;
	if (source & Afi)
		destination |= Af;

	return destination;
}

char* allen_rel_to_ascii(allen_relation source) {
	char buffer[128];
	unsigned short pos = 0;

	if (source & Aeq) {
		buffer[pos++] = '=';
		buffer[pos++] = ' ';
	}
	if (source & Asm) {
		buffer[pos++] = '<';
		buffer[pos++] = ' ';
	}
	if (source & Abi) {
		buffer[pos++] = '>';
		buffer[pos++] = ' ';
	}
	if (source & Ad) {
		buffer[pos++] = 'd';
		buffer[pos++] = ' ';
	}
	if (source & Adi) {
		buffer[pos++] = 'd';
		buffer[pos++] = 'i';
		buffer[pos++] = ' ';
	}
	if (source & Ao) {
		buffer[pos++] = 'o';
		buffer[pos++] = ' ';
	}
	if (source & Aoi) {
		buffer[pos++] = 'o';
		buffer[pos++] = 'i';
		buffer[pos++] = ' ';
	}
	if (source & Am) {
		buffer[pos++] = 'm';
		buffer[pos++] = ' ';
	}
	if (source & Ami) {
		buffer[pos++] = 'm';
		buffer[pos++] = 'i';
		buffer[pos++] = ' ';
	}
	if (source & As) {
		buffer[pos++] = 's';
		buffer[pos++] = ' ';
	}
	if (source & Asi) {
		buffer[pos++] = 's';
		buffer[pos++] = 'i';
		buffer[pos++] = ' ';
	}
	if (source & Af) {
		buffer[pos++] = 'f';
		buffer[pos++] = ' ';
	}
	if (source & Afi) {
		buffer[pos++] = 'f';
		buffer[pos++] = 'i';
		buffer[pos++] = ' ';
	}

	char* copy = NULL;

	if (pos == 0) {
		copy = (char*) malloc(4 * sizeof(char));
		copy = strcpy(copy,"NIL");
	} else {
		copy = (char*) malloc((pos + 1) * sizeof(char));
		buffer[--pos] = '\0';
		copy = strcpy(copy,buffer);
	}

	return copy;
}

allen_relation allen_rel_from_ascii(char *source) {
	int i;
	allen_relation destination = 0;

	if (source == NULL)
		return 0;

	for (i = 0; source[i] != '\0'; ++i) {
		switch (source[i]) {
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
			if (source[i + 1] == 'i') {
				destination |= Adi;
			} else
				destination |= Ad;
			break;
		case 'm':
			if (source[i + 1] == 'i') {
				destination |= Ami;
			} else
				destination |= Am;
			break;
		case 'o':
			if (source[i + 1] == 'i') {
				destination |= Aoi;
			} else
				destination |= Ao;
			break;
		case 's':
			if (source[i + 1] == 'i') {
				destination |= Asi;
			} else
				destination |= As;
			break;
		case 'f':
			if (source[i + 1] == 'i') {
				destination |= Afi;
			} else
				destination |= Af;
			break;
		default:
			break;
		};
	}
	return destination;
}

allen_relation conjunct_allen_rel(allen_relation a, allen_relation b) {
	return a | b;
}

allen_relation substract_allen_rel(allen_relation a, allen_relation b) {
	return a & (~b);
}

allen_relation intersect_allen_rel(allen_relation a, allen_relation b) {
	return a & b;
}

allen_relation allen_p_function(allen_relation a, allen_relation b) {
	allen_relation destination = 0x0, temp;
	short i, j;

	for (i = 0; i < REL_COUNT; a >>= 1, ++i) {
		if (a & 0x1) {
			temp = b;
			for (j = 0; j < REL_COUNT; temp >>= 1, ++j) {
				if (temp & 0x1) {
					destination = conjunct_allen_rel(destination,
							allen_table[i][j]);
				}
			}
		}
	}

	return destination;
}

short check_allen_rel_consistency(allen_relation ab, allen_relation bc, allen_relation ac){
	allen_relation calculated_ac = allen_p_function(ab,bc);

	if(intersect_allen_rel(ac,calculated_ac) == 0)
		return 1;
	else
		return 0;
}

allen_relation allen_rel_from_intervals(unsigned int starta, unsigned int stopa,
		unsigned int startb, unsigned int stopb) {
	allen_relation destination = 0;

	if (starta > stopa) {
		unsigned int temp = starta;
		starta = stopa;
		stopa = temp;
	}
	if (startb > stopb) {
		unsigned int temp = startb;
		startb = stopb;
		stopb = temp;
	}

	if (starta == startb) {
		if (stopa == stopb)
			destination |= Aeq;
		else if (stopa < stopb)
			destination |= As;
		else
			destination |= Asi;
	} else if (stopa == stopb) {
		if (starta > startb)
			destination |= Af;
		else
			destination |= Afi;

	} else if (starta < startb) {
		if (stopa < startb)
			destination |= Asm;
		else if (stopa == startb)
			destination |= Am;
		else if (stopa > stopb)
			destination |= Adi;
		else
			destination |= Ao;
	} else if (startb < starta) {
		if (stopb < starta)
			destination |= Abi;
		else if (stopb == starta)
			destination |= Ami;
		else if (stopb > stopa)
			destination |= Ad;
		else
			destination |= Aoi;
	}

	return destination;
}

void print_allen_table(FILE* file, char delimiter) {
	int i, j;
	if (file == NULL)
		return;

	fprintf(file, "\n");
	for(i=0;i<REL_COUNT;++i){
		for(j=0;j<REL_COUNT;++j){
			char* rel = allen_rel_to_ascii(allen_table[i][j]);
			fprintf(file, "%s%c", rel,delimiter);
			free(rel);
		}
		fprintf(file, "\n");
	}
	fflush(file);
}
