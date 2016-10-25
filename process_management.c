//
// Created by simone_mancini on 23/10/16.
//

#include "process_management.h"
#include "typedef.h"
#include "files.h"



pid_t spown_preforked_process(preforked_process *pf_d, int sock[2]){

    pid_t pid;

    pid = fork();

    if(pid != 0){
        return pid;
    }

    pf_d->sock_pipe = sock;

    close_fd(pf_d->sock_pipe[0]);



}