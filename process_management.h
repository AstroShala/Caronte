//
// Created by simone_mancini on 23/10/16.
//

#ifndef WEBSERVER_PROCESS_MANAGEMENT_H
#define WEBSERVER_PROCESS_MANAGEMENT_H

#include <unistd.h>
#include "typedef.h"

pid_t spown_preforked_process(preforked_process *pf_p, int sock[2]);

#endif //WEBSERVER_PROCESS_MANAGEMENT_H
