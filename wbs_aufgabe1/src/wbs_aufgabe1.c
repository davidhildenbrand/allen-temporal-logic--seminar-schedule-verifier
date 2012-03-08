/*
 Name        : WBS Aufgabe 1
 Author      : David hildenbrand, Tobias Schoknecht
 */

#include <stdio.h>
#include <stdlib.h>
#include "allan.h"
#include "allan_web.h"
#include "wbs_aufgabe1.h"
#include "logger.h"

/*	global variables	*/
struct lecturer_control lecturer;
struct file1 file1;
struct file2 file2;
struct file3 file3;

int main(int argc, char** argv) {
	log(INFO, "Allan schedule verifier.");
	log_filled_line('-');
	log(INFO, "Copyright David Hildenbrand, Tobias Schoknecht - 2012.");
	log_filled_line('-');

	file1.path =
			"/Users/davidhildenbrand/Dropbox/DHBW/6. Semester/WBS/Aufgabe 1/A_017_1_Bsp.csv";
	file2.path =
			"/Users/davidhildenbrand/Dropbox/DHBW/6. Semester/WBS/Aufgabe 1/A_017_2_Bsp.csv";
	file3.path =
			"/Users/davidhildenbrand/Dropbox/DHBW/6. Semester/WBS/Aufgabe 1/A_017_3p_Bsp.csv";

	if (argc == 3){
		file1.path = argv[2];
		file2.path = argv[3];
		file3.path = argv[4];
	}
	else{
		log(WARN,"Loading files from default path!");
		log(WARN,"Usage: <executable> <file1> <file2> <file3>");
		log_filled_line('-');
	}

	log(INFO, "file1: %s",file1.path);
	log(INFO, "file2: %s",file2.path);
	log(INFO, "file3: %s",file3.path);

	log_filled_line('-');

	log(INFO, "Loading data sets from file1 and file2 ...");

	if (read_file1() != NULL) {
		log(ERROR, "Error loading file: %s", file1.path);
		goto error;
	} else
		log(INFO, "... %d data sets loaded from file1.", file1.count);

	if (read_file2() != NULL) {
		log(ERROR, "Error loading file: %s", file2.path);
		goto error;
	} else
		log(INFO, "... %2d data sets loaded from file2.", file2.count);

	log_filled_line('-');


	log(INFO,"The following distinct lecturers have been identified:");

	int i;
	for (i = 0; i < lecturer.count; ++i) {
		log(INFO,"\tLecturer %d: %s", i+1, lecturer.elements[i]);
	}

	log_filled_line('-');

	struct allan_web* web = new_web(file1.count);

	log(INFO,"Allan web of size %d created.", web->size);
	init_web(web, All);

	log_filled_line('-');

	//map the group nr to the nodes
	for (i = 0; i < file1.count; ++i) {
		map_nr_to_index(web, file1.entries[i]->nr, i);
	}

	log(INFO,"Adding dependencies to the web...");

	log(INFO,"... between events of the same group.");
	process_group_dependence(web);

	log(INFO,"... between events of the lecturer.");
	process_lecturer_dependence(web);

	log(INFO,"... between events in the same room.");
	process_room_dependence(web);

	log(INFO,"... between events defined in file2.");
	process_dependencies(web);

	log(INFO,"All dependencies processed.");

	log_filled_line('-');

	log(INFO,"Checking web consistency (path consistency method) ...");

	if (path_consistency_method(web) != 0) {
		log(ERROR,"Check was not successful!");
		goto error;
	} else {
		log(INFO,"Check was successful!");
	}

	log_filled_line('-');

	log(INFO,"Verifying schedule from file3 ...");

	if (read_file3() != NULL) {
		log(ERROR,"Error loading file: %s", file3.path);
		goto error;
	} else {
		log(INFO,"... %2d data sets loaded from file3.", file3.count);
	}

	log(INFO,"... running checks ...");

	if (process_check(web) != 0) {
		log(ERROR,"Check was not successful!");
		goto error;
	} else {
		log(INFO,"Check was successful!");
	}

	error:

	log_filled_line('-');
	log(INFO,"Quitting!");
	clear_file1();
	clear_file2();
	clear_file3();
	clear_lecturer();
	free_web(web);
	return EXIT_SUCCESS;
}

short find_lecturer(char* name) {
	if (name == NULL)
		return -1;

	int i;

	for (i = 0; i < lecturer.count; ++i) {
		if (strcmp(name, lecturer.elements[i]) == 0) {
			return i;
		}
	}

	//not in the array
	return -1;
}

short add_lecturer(char* name) {
	if (name == NULL)
		return -1;
	if (lecturer.count + 1 == LECTURER_ENTRY_COUNT_MAX) {
		log(ERROR,"Too many lecturer used. Max is set to %d.",
				LECTURER_ENTRY_COUNT_MAX);
		exit(-1);
	}

	lecturer.elements[lecturer.count] = (char*) malloc(
			strlen(name) * sizeof(char));
	strcpy(lecturer.elements[lecturer.count], name);

	return lecturer.count++;
}

void clear_lecturer() {
	int i;

	for (i = 0; i < lecturer.count; ++i) {
		if (lecturer.elements[i] != NULL) {
			free(lecturer.elements[i]);
		}
	}
}

short read_next(char* source, char*destination, unsigned short* offset) {
	char* curPos = source + *offset;

	if (source == NULL || destination == NULL || *curPos == '\n'
			|| *curPos == EOF || *curPos == '\0') {
		return -1;
	}

	while (*curPos != ';' && *curPos != '\n' && *curPos != EOF
			&& *curPos != '\0') {
		*destination = *curPos;
		destination++;
		curPos++;
		*offset += 1;
	}
	*offset += 1;
	*destination = '\0';

	return 0;
}

int read_file1() {
	FILE* file = NULL;

	if (file1.path == NULL) {
		return -1;
	}

	file = fopen(file1.path, "r");

	if (file == NULL) {
		return -2;
	}

	char buffer[128];

	//table description
	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		fclose(file);
		return -3;
	}

	//process the records
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (file1.count + 1 == FILE1_ENTRY_COUNT_MAX) {
			fclose(file);
			return -3;
		}
		struct file1_entry* new_entry = (struct file1_entry*) malloc(
				sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the nr
		read_next(buffer, temp, &offset);
		new_entry->nr = atoi(temp);

		//read the name
		read_next(buffer, temp, &offset);
		new_entry->name = (char*) malloc(strlen(temp) * sizeof(char));
		strcpy(&new_entry->name, temp);

		//read the dozent
		read_next(buffer, temp, &offset);
		int index = find_lecturer(temp);
		if (index >= 0)
			new_entry->lecturer = index;
		else
			new_entry->lecturer = add_lecturer(temp);

		//read the gruppe
		read_next(buffer, temp, &offset);
		new_entry->group = atoi(temp);

		//read the länge
		read_next(buffer, temp, &offset);
		new_entry->length = atoi(temp);

		//read the raum
		read_next(buffer, temp, &offset);
		new_entry->room = atoi(temp);

		file1.entries[file1.count++] = new_entry;
	}

	fclose(file);

	return 0;
}

int read_file2() {
	FILE* file = NULL;

	file = fopen(file2.path, "r");

	if (file == NULL) {
		return -1;
	}

	char buffer[128];

	//table description
	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		fclose(file);
		return -2;
	}

	//process the records
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (file2.count + 1 == FILE2_ENTRY_COUNT_MAX) {
			fclose(file);
			return -3;
		}
		struct file2_entry* new_entry = (struct file2_entry*) malloc(
				sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the vorher
		read_next(buffer, temp, &offset);
		new_entry->pre = atoi(temp);

		//read the aufbauend
		read_next(buffer, temp, &offset);
		new_entry->post = atoi(temp);

		file2.entries[file2.count++] = new_entry;
	}

	fclose(file);

	return 0;
}

int read_file3() {
	FILE* file = NULL;

	file = fopen(file3.path, "r");

	if (file == NULL) {
		return -1;
	}

	char buffer[128];

	//table description
	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		fclose(file);
		return -2;
	}

	//process the records
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (file3.count + 1 == FILE3_ENTRY_COUNT_MAX) {
			fclose(file);
			return -3;
		}
		struct file3_entry* new_entry = (struct file3_entry*) malloc(
				sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the gruppe
		read_next(buffer, temp, &offset);
		new_entry->group = atoi(temp);

		//read the veranstaltung
		read_next(buffer, temp, &offset);
		new_entry->event = atoi(temp);

		//read the zeit
		read_next(buffer, temp, &offset);

		char time[3];
		time[0] = temp[0];
		time[1] = temp[1];
		time[2] = '\0';
		new_entry->time_in_minutes = atoi(time) * 60;
		time[0] = temp[3];
		time[1] = temp[4];
		new_entry->time_in_minutes += atoi(time);

		file3.entries[file3.count++] = new_entry;
	}
	fclose(file);
	return 0;
}
void process_group_dependence(struct allan_web* web) {
	//a group can only join one lecture at a time
	int i, j;
	//default is that they have a break in between
	allan_relation relation = allan_rel_from_ascii("< >");
	//iterate entries in file 1
	for (i = 0; i < file1.count; ++i) {
		//iterate starting at the next position
		for (j = i + 1; j < file1.count; ++j) {
			if (file1.entries[i]->group == file1.entries[j]->group) {
				//if first one is smaller that 90min, they can meet
				if (file1.entries[i]->length < 2) {
					relation |= Am;
				}
				//if second one is smaller that 90min, they can meet
				if (file1.entries[j]->length < 2) {
					relation |= Ami;
				}
				//intersect the relation
				intersect_relation(web, i, j, relation);
			}
		}
	}
}

void process_lecturer_dependence(struct allan_web* web) {
	//same lecturers can't take part in a lecture at the same time
	int i, j;
	const allan_relation relation = allan_rel_from_ascii("m mi < >");
	//iterate entries in file 1
	for (i = 0; i < file1.count; ++i) {
		//iterate starting at the next position
		for (j = i + 1; j < file1.count; ++j) {
			if (file1.entries[i]->lecturer == file1.entries[j]->lecturer) {
				//intersect the relation
				intersect_relation(web, i, j, relation);
			}
		}
	}
}

void process_room_dependence(struct allan_web* web) {
	//a room can only contain one group at the time
	int i, j;
	const allan_relation relation = allan_rel_from_ascii("m mi < >");
	//iterate entries in file 1
	for (i = 0; i < file1.count; ++i) {
		//iterate starting at the next position
		for (j = i + 1; j < file1.count; ++j) {
			if (file1.entries[i]->room == file1.entries[j]->room) {
				//intersect the relation
				intersect_relation(web, i, j, relation);
			}
		}
	}
}

void process_dependencies(struct allan_web* web) {
	//process the dependencies described in file 2
	const allan_relation relation = allan_rel_from_ascii("m <");
	struct file2_entry* cur = NULL;
	int i;

	for (i = 0; i < file2.count; ++i) {
		cur = file2.entries[i];

		unsigned short pre = get_mapped_index(web, cur->pre);
		unsigned short post = get_mapped_index(web, cur->post);

		intersect_relation(web, pre, post, relation);
	}
}

short process_check(struct allan_web* web) {
	if (web == NULL)
		return -3;

	int i, j, k;

	//to arrays for additional dependencies checking
	char found[file1.count];
	char required[file1.count];
	for (i = 0; i < file1.count; required[i] = 0, found[i++] = 0)
		;

	//variables used inside the loop
	struct file3_entry* entrya = NULL;
	struct file3_entry* entryb = NULL;
	unsigned short indexa = -1;
	unsigned short indexb = -1;
	unsigned short starta = 0;
	unsigned short startb = 0;
	unsigned short stopa = 0;
	unsigned short stopb = 0;
	allan_relation rel = 0;
	allan_relation rel2 = 0;
	allan_relation erg = 0;

	//compare each pair of entries in file3
	for (i = 0; i < file3.count; ++i) {
		entrya = file3.entries[i];
		indexa = get_mapped_index(web, entrya->event);

		//check if the event has already been placed
		if (found[indexa] != 0) {
			log(ERROR,"Duplicate event in file3: %d!", entrya->event);
			return -4;
		}
		found[indexa] = 1;
		//find all events that the current event depends on.
		for (k = 0; k < file2.count; ++k) {
			if (file2.entries[k]->post == entrya->event) {
				required[get_mapped_index(web, file2.entries[k]->pre)] = 1;
			}
		}

		if (indexa < 0) {
			log(ERROR,"Event %d was not defined in file1!",
					entrya->event);
			return -2;
		}
		starta = entrya->time_in_minutes;
		stopa = starta;

		if (file1.entries[indexa]->length == 2)
			stopa += 90;
		else
			stopa += 45;

		for (j = i + 1; j < file3.count; ++j) {
			entryb = file3.entries[j];
			indexb = get_mapped_index(web, entryb->event);

			if (indexb < 0) {
				log(ERROR,"Event %d was not defined in file1!",
						entryb->event);
				return -2;
			}

			startb = entryb->time_in_minutes;
			stopb = startb;

			if (file1.entries[indexb]->length == 2)
				stopb += 90;
			else
				stopb += 45;

			rel = allan_rel_from_intervals(starta, stopa, startb, stopb);
			rel2 = get_relation(web, indexa, indexb);
			erg = intersect_allan_rel(rel2, rel);

			if (erg == 0) {
				log(ERROR,"Detected incompatible events %d and %d.",
						entrya->event, entryb->event);
				log(ERROR,"Their relation is '%s', but only '%s' is allowed.",
						allan_rel_to_ascii(rel), allan_rel_to_ascii(rel2),
						allan_rel_to_ascii(erg));
				return 1;
			}

		}
	}

	//check if all required events have been found
	for (i = 0; i < file1.count; ++i) {
		if (required[i] == 1 && found[i] == 0) {
			log(ERROR,"Required event %d was not defined in the schedule!",
					get_mapped_nr(web, i));
			return -2;
		}
	}

	return 0;
}

void clear_file1() {
	int i;

	for (i = 0; i < file1.count; ++i) {
		free(file1.entries[i]);
		file1.entries[i] = NULL;
	}

	file1.count = 0;
}

void clear_file2() {
	int i;

	for (i = 0; i < file2.count; ++i) {
		free(file2.entries[i]);
		file2.entries[i] = NULL;
	}

	file2.count = 0;
}

void clear_file3() {
	int i;

	for (i = 0; i < file3.count; ++i) {
		free(file3.entries[i]);
		file3.entries[i] = NULL;
	}

	file3.count = 0;
}
