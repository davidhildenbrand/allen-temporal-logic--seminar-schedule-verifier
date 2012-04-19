/*
 * Name        : logger.h
 * Project     : Allen temporal logic: seminar schedule verifier
 * Author      : David Hildenbrand, Tobias Schoknecht
 * Copyright   : David Hildenbrand, Tobias Schoknecht 2012
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * david.hildenbrand@gmail.com and tobias.schoknecht@gmail.com wrote this file.
 * As long as you retain this notice you can do whatever you want with this
 * stuff. If we meet some day, and you think this stuff is worth it, you can
 * buy us a beer in return David Hildenbrand, Tobias Schoknecht
 * ----------------------------------------------------------------------------
 */

#ifndef LOGGER_H_
#define LOGGER_H_

enum log_level {
	INFO, WARN, ERROR
};

void log(enum log_level level, char* format, ...);
void log_filled_line(char del);

#endif /* LOGGER_H_ */
