/*
 Name        : WBS Aufgabe 1
 Author      : David Hildenbrand, Tobias Schoknecht
 Copyright   : David Hildenbrand, Tobias Schoknecht 2012
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* david.hildenbrand@gmail.com and tobias.schoknecht@gmail.com wrote this file.
* As long as you retain this notice you can do whatever you want with this
* stuff. If we meet some day, and you think this stuff is worth it, you can
* buy us a beer in return David Hildenbrand, Tobias Schoknecht
* ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include "allen.h"
#include "allen_web.h"
#include "wbs_aufgabe1.h"
#include "logger.h"

/*	global variables	*/

//found lecturers
struct lecturer_control lecturer;
//file1-3
struct file1 file1;
struct file2 file2;
struct file3 file3;
//found events in the schedule
char found_events[FILE1_ENTRY_COUNT_MAX];
//required events in the schedule
char required_events[FILE1_ENTRY_COUNT_MAX];


int main(int argc, char** argv) {
	int error = EXIT_SUCCESS;
	struct allen_web* web = NULL;

	log(INFO, "Allen schedule verifier.");
	log_filled_line('-');
	log(INFO, "Copyright David Hildenbrand, Tobias Schoknecht - 2012.");
	log_filled_line('-');

	file1.path = "A_017_1_Bsp.csv";
	file2.path = "A_017_2_Bsp.csv";
	file3.path = "A_017_3_Bsp.csv";
	char* out_file = "out.csv";

	if (argc > 3) {
		file1.path = argv[1];
		file2.path = argv[2];
		file3.path = argv[3];
	} else {
		log(WARN, "Loading files from default path!");
		log(WARN, "Usage: <executable> <file1> <file2> <file3> [<outfile>]");
		log_filled_line('-');
	}

	if (argc > 4)
		out_file = argv[4];

	log(INFO, "file1: %s", file1.path);
	log(INFO, "file2: %s", file2.path);
	log(INFO, "file3: %s", file3.path);

	log_filled_line('-');

	log(INFO, "Loading data sets from file1 and file2 ...");

	if (read_file1() != 0) {
		log(ERROR, "Error loading file: %s", file1.path);
		error = 1;
		goto error;
	} else
		log(INFO, "... %d data sets loaded from file1.", file1.count);

	if (read_file2() != 0) {
		log(ERROR, "Error loading file: %s", file2.path);
		error = 1;
		goto error;
	} else
		log(INFO, "... %2d data sets loaded from file2.", file2.count);

	log_filled_line('-');

	log(INFO, "The following distinct lecturers have been identified:");

	int i;
	for (i = 0; i < lecturer.count; ++i) {
		log(INFO, "\tLecturer %d: %s", i + 1, lecturer.elements[i]);
	}

	log_filled_line('-');

	log(INFO, "The following lectures have been read:");

	for (i = 0; i < file1.count; ++i) {
		log(INFO, "\t%d: %s", file1.entries[i]->nr, file1.entries[i]->name);
	}

	log_filled_line('-');

	web = new_web(file1.count);

	log(INFO, "Allen web of size %d created.", web->size);
	init_web(web, All);

	log_filled_line('-');

	//map the group nr to the nodes
	for (i = 0; i < file1.count; ++i) {
		map_nr_to_index(web, file1.entries[i]->nr, i);
	}

	log(INFO, "Adding dependencies to the web...");

	log(INFO, "... between events of the same group.");
	process_group_dependence(web);

	log(INFO, "... between events of the lecturer.");
	process_lecturer_dependence(web);

	log(INFO, "... between events in the same room.");
	process_room_dependence(web);

	log(INFO, "... between events defined in file2.");
	process_dependencies(web);

	log(INFO, "All dependencies processed.");

	log_filled_line('-');

	log(INFO, "Checking web consistency (path consistency method) ...");

	if (path_consistency_method(web) != 0) {
		log(ERROR, "Check was not successful!");
		error = 2;
		goto error;
	} else {
		log(INFO, "Check was successful!");
	}

	log_filled_line('-');

	log(INFO, "Saving web to file ...");

	if (write_web_to_files(web, out_file) != 0)
		log(ERROR, "... web could not be written to file.");
	else
		log(INFO, " ... web written to file.");

	log_filled_line('-');

	log(INFO, "Verifying schedule from file3 ...");

	if (read_file3() != 0) {
		log(ERROR, "Error loading file: %s", file3.path);
		error = 1;
		goto error;
	} else {
		log(INFO, "... %2d data sets loaded from file3.", file3.count);
	}

	log(INFO, "... running basic consistency checks");

	if (process_basic_checks(web) != 0) {
		log(ERROR, "Check was not successful!");
		error = 3;
		goto error;
	}

	log(INFO, "... advanced '90min break' check");

	if (process_90min_break_check(web) != 0) {
		log(ERROR, "Check was not successful!");
		error = 4;
		goto error;
	}

	log(INFO, "... final web consistency check (path consistency method)");

	if (path_consistency_method(web) != 0) {
		log(ERROR, "Check was not successful!");
		error = 5;
		goto error;
	} else {
		log(INFO, "Check was successful!");
	}

	error:

	log_filled_line('-');
	log(INFO, "Quitting!");
	clear_file1();
	clear_file2();
	clear_file3();
	clear_lecturer();
	free_web(web);
	return error;
}

short write_web_to_files(struct allen_web* web, char* path) {
	if (path == NULL) {
		return -1;
	}

	FILE* file = fopen(path, "w");

	if (file == NULL) {
		log(ERROR, "File could not be opened.");
		return -2;
	}

	print_web(web, file, ';');

	fclose(file);

	return 0;
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
		log(ERROR, "Too many lecturer used. Max is set to %d.",
				LECTURER_ENTRY_COUNT_MAX);
		return (-1);
	}

	lecturer.elements[lecturer.count] = (char*) malloc(
			(strlen(name) + 1) * sizeof(char));
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
	int error = EXIT_SUCCESS;
	struct file1_entry* new_entry = NULL;
	char buffer[128];

	if (file1.path == NULL) {
		error = -1;
		goto error;
	}

	file = fopen(file1.path, "r");

	if (file == NULL) {
		error = -2;
		goto error;
	}

	//table description
	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		log(ERROR, "file1 has wrong/no content.");
		error = -3;
		goto error;
	}

	//process the records
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (file1.count + 1 == FILE1_ENTRY_COUNT_MAX) {
			log(ERROR, "Sorry, maximum of entries for file1 is %d!",
					FILE1_ENTRY_COUNT_MAX);
			error = -3;
			goto error;
		}

		//Min of delimiters needed
		if (count_char(buffer, ';') < 5) {
			log(ERROR, "6 columns are required in file1!",
					FILE1_ENTRY_COUNT_MAX);
			error = -6;
			goto error;
		}

		new_entry = (struct file1_entry*) malloc(sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the nr
		read_next(buffer, temp, &offset);
		new_entry->nr = atoi(temp);
		//existing entry?
		if (find_index_by_nr_in_file1(new_entry->nr) >= 0) {
			log(ERROR, "Duplicate use of event nr %d in file1.", new_entry->nr);
			error = -5;
			goto error;
		}

		//read the name
		read_next(buffer, temp, &offset);
		new_entry->name = (char*) malloc((strlen(temp) + 1) * sizeof(char));
		strcpy(new_entry->name, temp);

		//read the lecturer
		read_next(buffer, temp, &offset);
		int index = find_lecturer(temp);
		if (index >= 0)
			new_entry->lecturer = index;
		else
			new_entry->lecturer = add_lecturer(temp);

		//read the group
		read_next(buffer, temp, &offset);
		new_entry->group = atoi(temp);

		//read the length
		read_next(buffer, temp, &offset);
		new_entry->length = atoi(temp);
		//either 1 or 2, 45 or 90, other values are not allowed!
		switch (new_entry->length) {
		case 1:
		case 45:
			new_entry->length = 1;
			break;
		case 2:
		case 90:
			new_entry->length = 2;
			break;
		default:
			log(ERROR, "Length of event %d is not allowed in file1.",
					new_entry->nr);
			error = -4;
			goto error;
		}

		//read the room
		read_next(buffer, temp, &offset);
		new_entry->room = atoi(temp);

		//add it to our structure
		file1.entries[file1.count++] = new_entry;
		//important to set it to null!
		new_entry = NULL;
	}

	error: if (new_entry != NULL)
		free(new_entry);
	if (file != NULL)
		fclose(file);
	return error;
}

int read_file2() {
	FILE* file = NULL;
	int error = EXIT_SUCCESS;
	char buffer[128];
	struct file2_entry* new_entry = NULL;

	file = fopen(file2.path, "r");

	if (file == NULL) {
		error = -1;
		goto error;
	}

	//table description
	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		log(ERROR, "file2 has wrong/no content.");
		error = -2;
		goto error;
	}

	//process the records
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (file2.count + 1 == FILE2_ENTRY_COUNT_MAX) {
			log(ERROR, "Sorry, maximum of entries for file2 is %d!",
					FILE2_ENTRY_COUNT_MAX);
			error = -3;
			goto error;
		}

		//Min of delimiters needed
		if (count_char(buffer, ';') < 1) {
			log(ERROR, "2 columns are required in file2!",
					FILE1_ENTRY_COUNT_MAX);
			error = -4;
			goto error;
		}

		new_entry = (struct file2_entry*) malloc(sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the pre-event
		read_next(buffer, temp, &offset);
		new_entry->pre = atoi(temp);
		//existing entry?
		if (find_index_by_nr_in_file1(new_entry->pre) < 0) {
			log(ERROR, "Event nr %d not referenced in file1!", new_entry->pre);
			error = -5;
			goto error;
		}

		//read the post-event
		read_next(buffer, temp, &offset);
		new_entry->post = atoi(temp);
		//existing entry?
		if (find_index_by_nr_in_file1(new_entry->post) < 0) {
			log(ERROR, "Event nr %d not referenced in file1!", new_entry->post);
			error = -5;
			goto error;
		}

		file2.entries[file2.count++] = new_entry;
		new_entry = NULL;
	}

	error: if (new_entry != NULL)
		free(new_entry);
	if (file != NULL)
		fclose(file);
	return error;
}

int read_file3() {
	FILE* file = NULL;
	int error = 0;
	char buffer[128];
	struct file3_entry* new_entry = NULL;

	file = fopen(file3.path, "r");

	if (file == NULL) {
		error = -1;
		goto error;
	}

	//table description
	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		log(ERROR, "file3 has wrong/no content.");
		error = -2;
		goto error;
	}

	//process the records
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (file3.count + 1 == FILE3_ENTRY_COUNT_MAX) {
			log(ERROR, "Sorry, maximum of entries for file2 is %d!",
					FILE3_ENTRY_COUNT_MAX);
			error = -3;
			goto error;
		}

		//Min of delimiters needed
		if (count_char(buffer, ';') < 2) {
			log(ERROR, "3 columns are required in file3!",
					FILE1_ENTRY_COUNT_MAX);
			error = -4;
			goto error;
		}

		new_entry = (struct file3_entry*) malloc(sizeof(struct file1_entry));

		char temp[128];
		unsigned short offset = 0;

		//read the group
		read_next(buffer, temp, &offset);
		new_entry->group = atoi(temp);

		//read the event nr
		read_next(buffer, temp, &offset);
		new_entry->event = atoi(temp);
		//verify the referenced event nr
		int file1_index = find_index_by_nr_in_file1(new_entry->event);
		if (file1_index < 0) {
			log(ERROR, "Event nr %d not referenced in file1!",
					new_entry->event);
			error = -5;
			goto error;
		}
		//verify the group
		if (file1.entries[file1_index]->group != new_entry->group) {
			log(WARN, "Event nr %d has wrong group-entry in file3 (not used)!",
					new_entry->event);
		}

		//read the time
		read_next(buffer, temp, &offset);

		new_entry->time_in_minutes = time_in_min_from_string(temp);

		file3.entries[file3.count++] = new_entry;
		new_entry = NULL;
	}

	error: if (new_entry != NULL)
		free(new_entry);
	if (file != NULL)
		fclose(file);
	return error;
}
void process_group_dependence(struct allen_web* web) {
	//a group can only join one lecture at a time
	int i, j;
	allen_relation refrel = allen_rel_from_ascii("< >");

	//process all combinations of file1 entries
	//iterate entries in file 1
	for (i = 0; i < file1.count; ++i) {
		//iterate starting at the next position
		for (j = i + 1; j < file1.count; ++j) {

			//default is that they have a break in between
			allen_relation relation = refrel;


			if (file1.entries[i]->group == file1.entries[j]->group) {
				//if one lecture is longer than 90 min, they can't meet otherwise both can meet
				if (file1.entries[i]->length < 2 && file1.entries[j]->length < 2) {
					relation |= Am;
					relation |= Ami;
				}

				//intersect the relation
				intersect_relation(web, i, j, relation);
			}
		}
	}
}

void process_lecturer_dependence(struct allen_web* web) {
	//same lecturer can't take part in two lectures at the same time
	int i, j;
	const allen_relation relation = allen_rel_from_ascii("m mi < >");

	//process all combinations of file1
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

void process_room_dependence(struct allen_web* web) {
	//a room can only contain one group at the time
	int i, j;
	const allen_relation relation = allen_rel_from_ascii("m mi < >");

	//process all combinations of file1
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

void process_dependencies(struct allen_web* web) {
	//process the dependencies described in file 2
	const allen_relation relation = allen_rel_from_ascii("m <");
	struct file2_entry* cur = NULL;
	int i;

	//process each entry in file2
	for (i = 0; i < file2.count; ++i) {
		cur = file2.entries[i];

		unsigned short pre = get_mapped_index(web, cur->pre);
		unsigned short post = get_mapped_index(web, cur->post);

		//enforce the "m <" relation
		intersect_relation(web, pre, post, relation);
	}
}

short process_basic_checks(struct allen_web* web) {
	if (web == NULL)
		return -3;

	int i, j, k;

	//reset the arrays of found and required events
	for (i = 0; i < file1.count; required_events[i] = 0, found_events[i++] = 0)
		;

	//variables used inside the loop
	struct file3_entry* entrya = NULL;
	struct file3_entry* entryb = NULL;
	int indexa = -1;
	int indexb = -1;
	unsigned short starta = 0;
	unsigned short startb = 0;
	unsigned short stopa = 0;
	unsigned short stopb = 0;
	allen_relation rel = 0;
	allen_relation rel2 = 0;
	allen_relation erg = 0;

	//compare each pair of entries in file3
	for (i = 0; i < file3.count; ++i) {
		entrya = file3.entries[i];
		indexa = get_mapped_index(web, entrya->event);

		//check if the event has already been placed
		if (found_events[indexa] != 0) {
			log(ERROR, "Duplicate event in file3: %d!", entrya->event);
			return -4;
		}
		//mark it as found
		found_events[indexa] = 1;
		//find all events that the current event depends on.
		for (k = 0; k < file2.count; ++k) {
			if (file2.entries[k]->post == entrya->event) {
				//mark it as required
				required_events[get_mapped_index(web, file2.entries[k]->pre)] = 1;
			}
		}

		if (indexa < 0) {
			log(ERROR, "Event %d was not defined in file1!", entrya->event);
			return -2;
		}
		starta = entrya->time_in_minutes;
		stopa = starta;

		//determine the end time of the event
		if (file1.entries[indexa]->length == 2)
			stopa += 90;
		else
			stopa += 45;

		//iterate starting after the current entry in file3
		for (j = i + 1; j < file3.count; ++j) {
			entryb = file3.entries[j];
			indexb = get_mapped_index(web, entryb->event);

			startb = entryb->time_in_minutes;
			stopb = startb;

			//determine the end time of second event
			if (file1.entries[indexb]->length == 2)
				stopb += 90;
			else
				stopb += 45;

			//find out the relation of both events
			rel = allen_rel_from_intervals(starta, stopa, startb, stopb);
			//intersect the current relation with their real relation described in file3
			rel2 = get_relation(web, indexa, indexb);
			erg = intersect_relation(web, indexa, indexb, rel);

			//if the intersect is NIL we have a problem
			if (erg == 0) {
				log(ERROR, "Detected incompatible events %d and %d.",
						entrya->event, entryb->event);
				log(ERROR, "Their relation is '%s', but only '%s' is allowed.",
						allen_rel_to_ascii(rel), allen_rel_to_ascii(rel2),
						allen_rel_to_ascii(erg));
				return 1;
			}

		}
	}

	//check if all required events have been found
	for (i = 0; i < file1.count; ++i) {
		//event required but not found
		if (required_events[i] == 1 && found_events[i] == 0) {
			log(ERROR, "Required event %d was not defined in the schedule!",
					get_mapped_nr(web, i));
			return -2;
		} else if (found_events[i] == 0) {
			log(WARN,
					"Event %d was not defined in the schedule but is not needed!",
					get_mapped_nr(web, i));
		}
	}

	return 0;
}

short process_90min_break_check(struct allen_web* web) {
	if (web == NULL)
		return -3;
	int i, j, k;
	//if 2 events of a group meet(--> 90 min), a third one of that group must either be smaller or bigger than one of the nodes
	//otherwise a group could have lectures with more than 90 minutes in a row!
	allen_relation refrel = allen_rel_from_ascii("< >");

	//process all edges
	for (i = 0; i < web->size; ++i) {
		unsigned short group1 = file1.entries[i]->group;
		for (j = 0; j < web->size; ++j) {
			unsigned short group2 = file1.entries[j]->group;

			//find 2 events of the same group which meet and have been found in the schedule
			if (found_events[i] && found_events[j] && group1 == group2 && get_relation(web, i, j) == Am) {
				//j only allows connection to other nodes of type "<"
				//process all edges of j
				for (k = 0; k < web->size; ++k) {
					unsigned short group3 = file1.entries[k]->group;
					//not the edge which is checked at the moment and only events with the same group and which have been found in the schedule!
					if (found_events[k] && j != k && k != i && group2 == group3) {
						//this edge can only be smaller or greater!
						allen_relation erg = intersect_relation(web, j, k,
								refrel);
						if (erg == 0) {
							log(ERROR,
									"Detected incompatible events %d, %d and %d.",
									get_mapped_nr(web, i),
									get_mapped_nr(web, j),
									get_mapped_nr(web, k));
							log(ERROR,
									"The group %d has more than two lectures of 45min without a break!",
									group1);
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

void clear_file1() {
	int i;

	for (i = 0; i < file1.count; ++i) {
		if (file1.entries[i]->name != NULL)
			free(file1.entries[i]->name);
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

int find_index_by_nr_in_file1(int nr) {
	int i;

	for (i = 0; i < file1.count; ++i) {
		if (file1.entries[i]->nr == nr)
			return i;
	}

	return -1;
}

int count_char(char* string, char element) {
	if (string == NULL)
		return NULL;

	int count = 0;

	while (*string != '\0') {
		if (*string == element)
			count++;
		string++;
	}

	return count;
}

unsigned short time_in_min_from_string(char* string) {
	if (string == NULL)
		return 0;

	unsigned short time = 0;
	char temp[10];
	char* cur = temp;

	//read the hours
	while (*string != '\0' && *string != ':') {
		*cur = *string;
		string++;
		cur++;
	}
	*cur = '\0';
	time = atoi(temp) * 60;

	//read the minutes
	if (*string == ':') {
		cur = temp;
		string++;
		while (*string != '\0') {
			*cur = *string;
			string++;
			cur++;
		}
		*cur = '\0';
		time += atoi(temp);
	}

	return time;
}
