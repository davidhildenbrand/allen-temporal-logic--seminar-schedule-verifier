/*
 * wbs_aufgabe1.h
 *
 *  Created on: 06.03.2012
 *      Author: davidhildenbrand
 */

#ifndef WBS_AUFGABE1_H_
#define WBS_AUFGABE1_H_

#define LECTURER_ENTRY_COUNT_MAX 20
#define FILE1_ENTRY_COUNT_MAX 20
#define FILE2_ENTRY_COUNT_MAX 20
#define FILE3_ENTRY_COUNT_MAX 20

struct file1_entry{
	unsigned short nr;
	char* name;
	short lecturer;
	unsigned short group;
	unsigned short length;
	unsigned short room;
};

struct file2_entry{
	unsigned short pre;
	unsigned short post;
};

struct file3_entry{
	unsigned short group;
	unsigned short event;
	unsigned short time_in_minutes;
};

struct file1{
	struct file1_entry* entries[FILE1_ENTRY_COUNT_MAX];
	unsigned short count;
	char* path;
};

struct file2{
	struct file2_entry* entries[FILE2_ENTRY_COUNT_MAX];
	unsigned short count;
	char* path;
};

struct file3{
	struct file3_entry* entries[FILE3_ENTRY_COUNT_MAX];
	unsigned short count;
	char* path;
};

struct lecturer_control{
	char* elements[20];
	unsigned short count;
};

short find_lecturer(char* name);
short add_lecturer(char* name);
void clear_lecturer();
short read_next(char* source,char*destination,unsigned short* offset);
int read_file1();
int read_file2();
int read_file3();
void process_group_dependence(struct alan_web* web);
void process_lecturer_dependence(struct alan_web* web);
void process_room_dependence(struct alan_web* web);
unsigned short find_index_in_file1(unsigned short nr);
void process_dependencies(struct alan_web* web);
alan_relation intervals_to_alan(unsigned int starta, unsigned int stopa, unsigned int startb, unsigned int stopb);
short process_check(struct alan_web* web);
void clear_file1();
void clear_file2();
void clear_file3();


#endif /* WBS_AUFGABE1_H_ */
