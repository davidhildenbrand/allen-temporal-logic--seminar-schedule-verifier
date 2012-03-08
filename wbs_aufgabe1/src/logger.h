/*
 * logger.h
 *
 *  Created on: 08.03.2012
 *      Author: davidhildenbrand
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
