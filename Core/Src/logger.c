#include "logger.h"
#include <stdio.h>


void logger_log(const char *msg) {
	printf("LOG: %s\r\n", msg);
}


