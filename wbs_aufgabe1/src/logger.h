/*
 Name        : WBS Aufgabe 1
 Author      : David Hildenbrand, Tobias Schoknecht
 */

#ifndef LOGGER_H_
#define LOGGER_H_

enum log_level {
	INFO,
	WARN,
	ERROR
};

void log(enum log_level level, char* format, ...);
void log_filled_line(char del);


#endif /* LOGGER_H_ */
