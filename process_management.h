//
// Created by simone_mancini on 23/10/16.
//

#ifndef WEBSERVER_PROCESS_MANAGEMENT_H
#define WEBSERVER_PROCESS_MANAGEMENT_H

#include <unistd.h>
#include "typedef.h"

pid_t spawn_preforked_process(int, int, preforked_process *);

void take_data(int);

#endif //WEBSERVER_PROCESS_MANAGEMENT_H
