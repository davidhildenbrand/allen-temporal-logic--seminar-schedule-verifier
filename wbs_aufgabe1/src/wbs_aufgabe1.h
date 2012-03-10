/*
 Name        : WBS Aufgabe 1
 Author      : David hildenbrand, Tobias Schoknecht
 */

#ifndef WBS_AUFGABE1_H_
#define WBS_AUFGABE1_H_

#include "allan.h"
#include "allan_web.h"

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

short find_lecturer(char* name);
short add_lecturer(char* name);
void clear_lecturer();
short read_next(char* source, char*destination, unsigned short* offset);
int read_file1();
int read_file2();
int read_file3();
void process_group_dependence(struct allan_web* web);
void process_lecturer_dependence(struct allan_web* web);
void process_room_dependence(struct allan_web* web);
void process_dependencies(struct allan_web* web);
short write_web_to_files(struct allan_web* web, char* path);
short process_basic_checks(struct allan_web* web);
short process_90min_break_check(struct allan_web* web);
void clear_file1();
void clear_file2();
void clear_file3();
int find_index_by_nr_in_file1(int nr);
int count_char(char* string, char element);
unsigned short time_in_min_from_string(char* string);

#endif /* WBS_AUFGABE1_H_ */
