//
// Created by simone_mancini on 07/10/16.
//

#include <unistd.h>
#include "connection.h"

#define MAX_SLEEP 128

int connect_retry_exponensial_backof(int sockfd, const struct sockaddr *address, socklen_t alen){

    int nsec;

    for(nsec = 1; nsec <= MAX_SLEEP; nsec <<= 1){  /*<<= left shift AND assignment operator. nsec = nsec << 1 (nsec*2)*/
        if(connect(sockfd, address, alen) == 0)
            return 0;

        if(nsec <= MAX_SLEEP/2)
            sleep(nsec);
    }

    return -1;
}