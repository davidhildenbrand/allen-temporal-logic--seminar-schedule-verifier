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
#include "wbs_aufgabe1.h"


/*	global variables	*/
struct lecturer_control lecturer;
struct file1 file1;
struct file2 file2;
struct file3 file3;


int main(void) {
	file1.path = "/Users/davidhildenbrand/Dropbox/DHBW/6. Semester/WBS/Aufgabe 1/A_017_1_Bsp.csv";
	file2.path = "/Users/davidhildenbrand/Dropbox/DHBW/6. Semester/WBS/Aufgabe 1/A_017_2_Bsp.csv";
	file3.path = "/Users/davidhildenbrand/Dropbox/DHBW/6. Semester/WBS/Aufgabe 1/A_017_3p_Bsp.csv";

	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	printf("Loading data sets from files:\n");

	if(read_file1() != NULL){
		fprintf(stderr,"Error loading file: %s\n",file1.path);
		exit(1);
	}
	else{
		printf("\tfile1: %d data sets loaded.\n", file1.count);
	}

	if(read_file2() != NULL){
		fprintf(stderr,"Error loading file: %s\n",file2.path);
		exit(2);
	}
	else{
		printf("\tfile2: %d data sets loaded.\n", file2.count);
	}

	if(read_file3() != NULL){
		fprintf(stderr,"Error loading file: %s\n",file3.path);
		exit(3);
	}
	else{
		printf("\tfile3: %d data sets loaded.\n", file3.count);
	}

	printf("The following lecturers have been identified:\n");

	int i;
	for(i=0;i<lecturer.count;++i){
		printf("\tLecturer %d: %s\n",i,lecturer.elements[i]);
	}

	struct alan_web* web = new_web(file1.count);

	printf("Alan web of size %d created.\n",web->size);
	init_web(web, All);

	process_group_dependence(web);
	process_lecturer_dependence(web);
	process_room_dependence(web);
	process_dependencies(web);

	if(process_check(web) != 0){
		printf("Check was not successful!\n");
	}
	else{
		printf("Check was successful!\n");
	}

	clear_file1();
	clear_file2();
	clear_file3();
	clear_lecturer();
	free_web(web);
	return EXIT_SUCCESS;
}

short find_lecturer(char* name){
	if(name == NULL)
		return -1;

	int i;

	for(i=0;i<lecturer.count;++i){
		if(strcmp(name,lecturer.elements[i]) == 0){
			return i;
		}
	}

	//not in the array
	return -1;
}

short add_lecturer(char* name){
	if(name == NULL)
		return -1;
	if(lecturer.count+1 == LECTURER_ENTRY_COUNT_MAX){
		fprintf(stderr,"ERROR: Too many lecturer used. Max is set to %d.\n",LECTURER_ENTRY_COUNT_MAX);
		exit(-1);
	}

	lecturer.elements[lecturer.count] = (char* )malloc(strlen(name)*sizeof(char));
	strcpy(lecturer.elements[lecturer.count],name);

	return lecturer.count++;
}

void clear_lecturer(){
	int i;

	for(i=0;i<lecturer.count;++i){
		if(lecturer.elements[i] != NULL){
			free(lecturer.elements[i]);
		}
	}
}

short read_next(char* source,char*destination,unsigned short* offset){
	char* curPos = source + *offset;

	if(source == NULL || destination == NULL || *curPos == '\n' || *curPos == EOF || *curPos == '\0'){
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

	if(file1.path == NULL){
		return -1;
	}

	file = fopen(file1.path,"r");

	if(file == NULL){
		return -2;
	}

	char buffer[128];

	//table description
	if(fgets(buffer,sizeof(buffer),file) == NULL){
		fclose(file);
		return -3;
	}

	//process the records
	while (fgets(buffer,sizeof(buffer),file) != NULL){
		if(file1.count+1 == FILE1_ENTRY_COUNT_MAX){
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
		int index = find_lecturer(temp);
		if(index >= 0)
			new_entry->lecturer = index;
		else
			new_entry->lecturer = add_lecturer(temp);

		//read the gruppe
		read_next(buffer,temp,&offset);
		new_entry->group = atoi(temp);

		//read the länge
		read_next(buffer,temp,&offset);
		new_entry->length = atoi(temp);

		//read the raum
		read_next(buffer,temp,&offset);
		new_entry->room = atoi(temp);

		file1.entries[file1.count++] = new_entry;
	}

	fclose(file);

	return 0;
}

int read_file2(){
	FILE* file = NULL;

	file = fopen(file2.path,"r");

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
		if(file2.count+1 == FILE2_ENTRY_COUNT_MAX){
			fclose(file);
			return -3;
		}
		struct file2_entry* new_entry = (struct file2_entry*)malloc(sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the vorher
		read_next(buffer,temp,&offset);
		new_entry->pre = atoi(temp);

		//read the aufbauend
		read_next(buffer,temp,&offset);
		new_entry->post = atoi(temp);

		file2.entries[file2.count++] = new_entry;
	}

	fclose(file);

	return 0;
}

int read_file3(){
	FILE* file = NULL;

	file = fopen(file3.path,"r");

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
		if(file3.count+1 == FILE3_ENTRY_COUNT_MAX){
			fclose(file);
			return -3;
		}
		struct file3_entry* new_entry = (struct file3_entry*)malloc(sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the gruppe
		read_next(buffer,temp,&offset);
		new_entry->group = atoi(temp);

		//read the veranstaltung
		read_next(buffer,temp,&offset);
		new_entry->event = atoi(temp);

		//read the zeit
		read_next(buffer,temp,&offset);

		char time[3];
		time[0] = temp[0];
		time[1] = temp[1];
		time[2] = '\0';
		new_entry->time_in_minutes = atoi(time)*60;
		time[0] = temp[3];
		time[1] = temp[4];
		new_entry->time_in_minutes += atoi(time);

		file3.entries[file3.count++] = new_entry;
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
	for(i=0;i<file1.count;++i){
		//iterate starting at the next position
		for(j=i+1;j<file1.count;++j){
			if(file1.entries[i]->group == file1.entries[j]->group){
				//if first one is smaller that 90min, they can meet
				if(file1.entries[i]->length < 2){
					relation |= Am;
				}
				//if second one is smaller that 90min, they can meet
				if(file1.entries[j]->length < 2){
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
	for(i=0;i<file1.count;++i){
		//iterate starting at the next position
		for(j=i+1;j<file1.count;++j){
			if(file1.entries[i]->lecturer == file1.entries[j]->lecturer){
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
	for(i=0;i<file1.count;++i){
		//iterate starting at the next position
		for(j=i+1;j<file1.count;++j){
			if(file1.entries[i]->room == file1.entries[j]->room){
				//intersect the relation
				intersect_relation(web,i,j,relation);
			}
		}
	}
}

unsigned short find_index_in_file1(unsigned short nr){
	int i;
	unsigned short fnr = -1;

	for(i=0;i<file1.count;++i){
		if(file1.entries[i] != NULL){
			if(file1.entries[i]->nr == nr){
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

	for(i=0;i<file2.count;++i){
		cur = file2.entries[i];

		unsigned short pre = find_index_in_file1(cur->pre);
		unsigned short post = find_index_in_file1(cur->post);

		intersect_relation(web,pre,post,relation);
	}
}

alan_relation intervals_to_alan(unsigned int starta, unsigned int stopa, unsigned int startb, unsigned int stopb){
	alan_relation destination = 0;

	if(starta > stopa){
		unsigned int temp = starta;
		starta = stopa;
		stopa = temp;
	}
	if(startb > stopb){
		unsigned int temp = startb;
		startb = stopb;
		stopb = temp;
	}

	if(starta == startb){
		if(stopa == stopb)
			destination |= Aeq;
		else if(stopa < stopb)
			destination |= As;
		else
			destination |= Asi;
	}
	else if(stopa == stopb){
		if(starta > startb)
			destination |= Af;
		else
			destination |= Afi;

	}
	else if(starta < startb){
		if(stopa < startb)
			destination |= Asm;
		else if(stopa == startb)
			destination |= Am;
		else if(stopa > stopb)
			destination |= Adi;
		else
			destination |= Ao;
	}
	else if(startb < starta){
		if(stopb < starta)
			destination |= Abi;
		else if(stopb == starta)
			destination |= Ami;
		else if(stopb > stopa)
			destination |= Ad;
		else
			destination |= Aoi;
	}

	return destination;
}

short process_check(struct alan_web* web){
	int i,j;

	for(i=0;i<file3.count;++i){
		for(j=i+1;j<file3.count;++j){
			struct file3_entry* entrya = file3.entries[i];
			struct file3_entry* entryb = file3.entries[j];

			unsigned short indexa = find_index_in_file1(entrya->event);
			unsigned short indexb = find_index_in_file1(entryb->event);
			unsigned short starta = entrya->time_in_minutes;
			unsigned short startb = entryb->time_in_minutes;
			unsigned short stopa =  starta;
			unsigned short stopb = startb;

			if(file1.entries[indexa]->length == 2)
				stopa += 90;
			else
				stopa += 45;

			if(file1.entries[indexb]->length == 2)
							stopb += 90;
						else
							stopb += 45;

			alan_relation rel = intervals_to_alan(starta,stopa,startb,stopb);
			alan_relation rel2 = get_relation(web,indexa,indexb);
			alan_relation erg = intersect_relations(rel2,rel);

			if(erg == 0){
				printf("Checking %d and %d.\n",entrya->event,entryb->event);
				printf("Index %d and %d.\n",indexa,indexb);
				printf("'%s' A '%s' = '%s'\n",relation_to_ascii(rel),relation_to_ascii(rel2),relation_to_ascii(erg));
				return 1;
			}

		}
	}

	return 0;
}

void clear_file1(){
	int i;

	for(i=0;i<file1.count;++i){
		free(file1.entries[i]);
		file1.entries[i] = NULL;
	}

	file1.count = 0;
}

void clear_file2(){
	int i;

	for(i=0;i<file2.count;++i){
		free(file2.entries[i]);
		file2.entries[i] = NULL;
	}

	file2.count = 0;
}

void clear_file3(){
	int i;

	for(i=0;i<file3.count;++i){
		free(file3.entries[i]);
		file3.entries[i] = NULL;
	}

	file3.count = 0;
}
