/*
 Name        : WBS Aufgabe 1
 Author      : David Hildenbrand, Tobias Schoknecht
 */

#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

void log(enum log_level level, char* format, ...) {
	va_list args;
	va_start(args, format);

	char* type = NULL;

	switch (level) {
	case INFO:
		type = "INFO";
		break;
	case WARN:
		type = "WARNING";
		break;
	case ERROR:
		type = "ERROR";
		break;
	default:
		type = "UNKNOWN";
		break;
	}

	FILE *out = stdout;

	if(level == ERROR){
		out = stderr;
	}

	fprintf(out,"%s\t: ",type);
	vfprintf(out,format, args);

	fprintf(out,"\n");

	fflush(out);

	va_end(args);
}

void log_filled_line(char del){
	char buffer[51];
	int i;

	for(i=0;i<50;++i){
		buffer[i] = del;
	}
	buffer[50] = '\0';

	log(INFO,"%s",buffer);
}
