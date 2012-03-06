/*
 Name        : wbs_aufgabe1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 */

#include <stdio.h>
#include <stdlib.h>
#include "alan.h"
#include "alan_web.h"

struct file1_entry{
	unsigned short nr;
	char* name;
	short dozent;
	unsigned short gruppe;
	unsigned short laenge;
	unsigned short raum;
};

struct file2_entry{
	unsigned short vorher;
	unsigned short aufbauend;
};

struct file3_entry{
	unsigned short gruppe;
	unsigned short veranstaltung;
	unsigned short zeit_stunde;
	unsigned short zeit_minute;
};


//20 entries for now
char* dozenten[20];
#define DOZENTEN_ENTRY_COUNT_MAX 20
unsigned short dozenten_count = 0;

//20 entries for now
char file1_path[] = "/Users/davidhildenbrand/Dropbox/DHBW/6. Semester/WBS/Aufgabe 1/A_017_1_Bsp.csv";
#define FILE1_ENTRY_COUNT_MAX 20
struct file1_entry* file1_entries[FILE1_ENTRY_COUNT_MAX];
unsigned short file1_entry_count = 0;

//20 entries for now
char file2_path[] = "/Users/davidhildenbrand/Dropbox/DHBW/6. Semester/WBS/Aufgabe 1/A_017_2_Bsp.csv";
#define FILE2_ENTRY_COUNT_MAX 20
struct file2_entry* file2_entries[FILE2_ENTRY_COUNT_MAX];
unsigned short file2_entry_count = 0;

//20 entries for now
char file3_path[] = "/Users/davidhildenbrand/Dropbox/DHBW/6. Semester/WBS/Aufgabe 1/A_017_3p_Bsp.csv";
#define FILE3_ENTRY_COUNT_MAX 20
struct file3_entry* file3_entries[FILE3_ENTRY_COUNT_MAX];
unsigned short file3_entry_count = 0;

short find_dozent(char* name){
	if(name == NULL)
		return -1;

	int i;

	for(i=0;i<dozenten_count;++i){
		if(strcmp(name,dozenten[i]) == 0){
			return i;
		}
	}

	//not in the array
	return -1;
}

short add_dozent(char* name){
	if(name == NULL)
		return -1;
	if(dozenten_count+1 == DOZENTEN_ENTRY_COUNT_MAX)
		return -2;

	dozenten[dozenten_count] = (char* )malloc(strlen(name)*sizeof(char));
	strcpy(dozenten[dozenten_count],name);
	dozenten_count++;

	return 0;
}

short read_next(char* source,char*destination,unsigned short* offset){
	char* curPos = source + *offset;

	if(*curPos == '\n' || *curPos == EOF || *curPos == '\0'){
		return -1;
	}

	while(*curPos != ';' && *curPos != '\n' && *curPos != EOF && *curPos != '\0'){
		*destination = *curPos;
		destination++;
		curPos++;
		*offset += 1;
	}
	*offset += 1;
	*destination = '\0';

	return 0;
}


int read_file1(){
	FILE* file = NULL;

	file = fopen(file1_path,"r");

	if(file == NULL){
		return -1;
	}

	char buffer[128];

	//table description
	if(fgets(buffer,sizeof(buffer),file) == NULL){
		fclose(file);
		return -2;
	}

	//process the records
	while (fgets(buffer,sizeof(buffer),file) != NULL){
		if(file1_entry_count+1 == FILE1_ENTRY_COUNT_MAX){
			fclose(file);
			return -3;
		}
		struct file1_entry* new_entry = (struct file1_entry*)malloc(sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the nr
		read_next(buffer,temp,&offset);
		new_entry->nr = atoi(temp);

		//read the name
		read_next(buffer,temp,&offset);
		new_entry->name = (char*)malloc(strlen(temp)*sizeof(char));
		strcpy(&new_entry->name,temp);

		//read the dozent
		read_next(buffer,temp,&offset);
		int index = find_dozent(temp);
		if(index >= 0)
			new_entry->dozent = index;
		else
			new_entry->dozent = add_dozent(temp);

		//read the gruppe
		read_next(buffer,temp,&offset);
		new_entry->gruppe = atoi(temp);

		//read the länge
		read_next(buffer,temp,&offset);
		new_entry->laenge = atoi(temp);

		//read the raum
		read_next(buffer,temp,&offset);
		new_entry->raum = atoi(temp);

		file1_entries[file1_entry_count++] = new_entry;
	}

	fclose(file);

	return 0;
}

int read_file2(){
	FILE* file = NULL;

	file = fopen(file2_path,"r");

	if(file == NULL){
		return -1;
	}

	char buffer[128];

	//table description
	if(fgets(buffer,sizeof(buffer),file) == NULL){
		fclose(file);
		return -2;
	}

	//process the records
	while (fgets(buffer,sizeof(buffer),file) != NULL){
		if(file2_entry_count+1 == FILE2_ENTRY_COUNT_MAX){
			fclose(file);
			return -3;
		}
		struct file2_entry* new_entry = (struct file2_entry*)malloc(sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the vorher
		read_next(buffer,temp,&offset);
		new_entry->vorher = atoi(temp);

		//read the aufbauend
		read_next(buffer,temp,&offset);
		new_entry->aufbauend = atoi(temp);

		file2_entries[file2_entry_count++] = new_entry;
	}

	fclose(file);

	return 0;
}

int read_file3(){
	FILE* file = NULL;

	file = fopen(file3_path,"r");

	if(file == NULL){
		return -1;
	}

	char buffer[128];

	//table description
	if(fgets(buffer,sizeof(buffer),file) == NULL){
		fclose(file);
		return -2;
	}

	//process the records
	while (fgets(buffer,sizeof(buffer),file) != NULL){
		if(file3_entry_count+1 == FILE3_ENTRY_COUNT_MAX){
			fclose(file);
			return -3;
		}
		struct file3_entry* new_entry = (struct file3_entry*)malloc(sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the gruppe
		read_next(buffer,temp,&offset);
		new_entry->gruppe = atoi(temp);

		//read the veranstaltung
		read_next(buffer,temp,&offset);
		new_entry->veranstaltung = atoi(temp);

		//read the zeit
		read_next(buffer,temp,&offset);

		char time[3];
		time[0] = temp[0];
		time[1] = temp[1];
		time[2] = '\0';
		new_entry->zeit_stunde = atoi(time);
		time[0] = temp[3];
		time[1] = temp[4];
		new_entry->zeit_minute = atoi(time);

		file3_entries[file3_entry_count++] = new_entry;
	}
	fclose(file);
	return 0;
}

void process_group_dependence(struct alan_web* web){
	//a group can only join one lecture at a time
	int i,j;
	//default is that they have a break in between
	alan_relation relation = relation_from_ascii("< >");
	//iterate entries in file 1
	for(i=0;i<file1_entry_count;++i){
		//iterate starting at the next position
		for(j=i+1;j<file1_entry_count;++j){
			if(file1_entries[i]->gruppe == file1_entries[j]->gruppe){
				//if first one is smaller that 90min, they can meet
				if(file1_entries[i]->laenge < 2){
					relation |= Am;
				}
				//if second one is smaller that 90min, they can meet
				if(file1_entries[j]->laenge < 2){
					relation |= Ami;
				}
				//intersect the relation
				intersect_relation(web,i,j,relation);
			}
		}
	}
}

void process_lecturer_dependence(struct alan_web* web){
	//same lecturers can't take part in a lecture at the same time
	int i,j;
	const alan_relation relation = relation_from_ascii("m mi < >");
	//iterate entries in file 1
	for(i=0;i<file1_entry_count;++i){
		//iterate starting at the next position
		for(j=i;j<file1_entry_count;++j){
			if(file1_entries[i]->dozent == file1_entries[j]->dozent){
				//intersect the relation
				intersect_relation(web,i,j,relation);
			}
		}
	}
}

void process_room_dependence(struct alan_web* web){
	//a room can only contain one group at the time
	int i,j;
	const alan_relation relation = relation_from_ascii("m mi < >");
	//iterate entries in file 1
	for(i=0;i<file1_entry_count;++i){
		//iterate starting at the next position
		for(j=i;j<file1_entry_count;++j){
			if(file1_entries[i]->raum == file1_entries[j]->raum){
				//intersect the relation
				intersect_relation(web,i,j,relation);
			}
		}
	}
}

unsigned short find_index_in_file1(unsigned short nr){
	int i;
	unsigned short fnr = -1;

	for(i=0;i<file1_entry_count;++i){
		if(file1_entries[i] != NULL){
			if(file1_entries[i]->nr == nr){
				fnr = i;
				break;
			}
		}
	}

	return fnr;
}

void process_dependencies(struct alan_web* web){
	//process the dependencies described in file 2
	const alan_relation relation = relation_from_ascii("m <");
	struct file2_entry* cur = NULL;
	int i;

	for(i=0;i<file2_entry_count;++i){
		cur = file2_entries[i];

		unsigned short pre = find_index_in_file1(cur->vorher);
		unsigned short post = find_index_in_file1(cur->aufbauend);

		intersect_relation(web,pre,post,relation);
	}
}


int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	printf("Loading data sets from files:\n");

	if(read_file1() != NULL){
		fprintf(stderr,"Error: File 1 not found: %s\n",file1_path);
		exit(1);
	}
	else{
		printf("\tfile1: %d data sets loaded.\n", file1_entry_count);
	}

	if(read_file2() != NULL){
		fprintf(stderr,"Error: File 2 not found: %s\n",file2_path);
		exit(2);
	}
	else{
		printf("\tfile2: %d data sets loaded.\n", file2_entry_count);
	}

	if(read_file3() != NULL){
		fprintf(stderr,"Error: File 2 not found: %s\n",file3_path);
		exit(3);
	}
	else{
		printf("\tfile3: %d data sets loaded.\n", file3_entry_count);
	}

	printf("The following lecturers have been identified:\n");

	int i;
	for(i=0;i<dozenten_count;++i){
		printf("\tLecturer %d: %s\n",i,dozenten[i]);
	}

	struct alan_web* web = new_web(file1_entry_count);
	printf("Alan web of size %d created.\n",web->size);
	init_web(web, All);
	print_web(web,stdout,';');

	process_group_dependence(web);
	process_lecturer_dependence(web);
	process_room_dependence(web);
	process_dependencies(web);

	print_web(web,stdout,';');

	char* rel = relation_to_ascii(get_relation(web,0,1));
	printf("'%s'\n",rel);
	free(rel);

	free_web(web);
	return EXIT_SUCCESS;
}
