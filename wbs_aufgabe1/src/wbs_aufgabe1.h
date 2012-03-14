/*
 Name        : WBS Aufgabe 1
 Author      : David Hildenbrand, Tobias Schoknecht
 */

#ifndef WBS_AUFGABE1_H_
#define WBS_AUFGABE1_H_

#include "allen.h"
#include "allen_web.h"

#define LECTURER_ENTRY_COUNT_MAX 20
#define FILE1_ENTRY_COUNT_MAX 20
#define FILE2_ENTRY_COUNT_MAX 20
#define FILE3_ENTRY_COUNT_MAX 20

struct file1_entry {
	unsigned short nr;
	char* name;
	short lecturer;
	unsigned short group;
	unsigned short length;
	unsigned short room;
};

struct file2_entry {
	unsigned short pre;
	unsigned short post;
};

struct file3_entry {
	unsigned short group;
	unsigned short event;
	unsigned short time_in_minutes;
};

struct file1 {
	struct file1_entry* entries[FILE1_ENTRY_COUNT_MAX];
	unsigned short count;
	char* path;
};

struct file2 {
	struct file2_entry* entries[FILE2_ENTRY_COUNT_MAX];
	unsigned short count;
	char* path;
};

struct file3 {
	struct file3_entry* entries[FILE3_ENTRY_COUNT_MAX];
	unsigned short count;
	char* path;
};

struct lecturer_control {
	char* elements[20];
	unsigned short count;
};

//find the index of lecturer in the global lecturer structure
short find_lecturer(char* name);

//add a lecture to the global lecturer structure
short add_lecturer(char* name);

//clear the global sql structure
void clear_lecturer();

//parse a csv line, copying from (source+*offset) till the next ';' and store the new position in offset
short read_next(char* source, char*destination, unsigned short* offset);

//read in file 1
int read_file1();

//read in file 2
int read_file2();

//read in file 3
int read_file3();

//process the dependencies between events of the same group
void process_group_dependence(struct allen_web* web);

//process the dependencies between events with the same lecturer
void process_lecturer_dependence(struct allen_web* web);

//process the dependencies between events in the same room
void process_room_dependence(struct allen_web* web);

//process the dependencies defined in file 2
void process_dependencies(struct allen_web* web);

//write the web to a file
short write_web_to_files(struct allen_web* web, char* path);

//process basic checks with the schedule defined in file 3
short process_basic_checks(struct allen_web* web);

//process advanced 90min without a break checks with the schedule defined in file 3
short process_90min_break_check(struct allen_web* web);

//clear and free the file 1 structure
void clear_file1();

//clear and free the file 2 structure
void clear_file2();

//clear and free the file 3 structure
void clear_file3();

//find the index in file 1 for the event with the given nr
int find_index_by_nr_in_file1(int nr);

//count the number of occurences of the given char in the string
int count_char(char* string, char element);

//convert a string with the format HH:MM to minutes
unsigned short time_in_min_from_string(char* string);

#endif /* WBS_AUFGABE1_H_ */
