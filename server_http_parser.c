//
// Created by simone_mancini on 19/10/16.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "server_http_parser.h"
#include "http-parser-master/http_parser.h"
#include "error_management.h"
#include "typedef.h"

#define MAX_HEADER_LINES 10
#define MAX_ELEMENT_SIZE 2048
#define MAX_CHUNKS 16
#define HOST "Host"

/*SETTINGS ONLY FOR DEBUG*/
/*http_parser_settings settings_null =
        {.on_message_begin = 0
                ,.on_header_field = 0
                ,.on_header_value = 0
                ,.on_url = 0
                ,.on_status = 0
                ,.on_body = 0
                ,.on_headers_complete = 0
                ,.on_message_complete = 0
                ,.on_chunk_header = 0
                ,.on_chunk_complete = 0
        };*/

/*struct message{
    char url[MAX_ELEMENT_SIZE];
};

struct message *mess;*/



//char url[MAX_ELEMENT_SIZE];


void initialize_request_parser(http_parser *parser){

    parser = malloc(sizeof(http_parser));

    if(parser == NULL)
        die_with_error("error malloc() for parser");

    http_parser_init(parser, HTTP_REQUEST);

}

int header_field_compare(rcv_msg *msg, const char *value, size_t length){

    if(strncmp(msg->field, "Host", length) == 0){
        //printf("Host parsing\n");
        msg->host = malloc(length*sizeof(char));

        if (msg->host == NULL)
            die_with_error("error catching host");

        strncpy(msg->host, value, length);
        msg->host_length = length;

        return 0;
    }
    if(strncmp(msg->field, "User-Agent", length) == 0){
        msg->user_agent = malloc(length*sizeof(char));

        if (msg->user_agent == NULL)
            die_with_error("error catching host");

        strncpy(msg->user_agent, value, length);
        msg->user_agent_length = length;

        return 0;
    }
    if(strncmp(msg->field, "Accept", length) == 0){
        msg->accept = malloc(length*sizeof(char));

        if (msg->accept == NULL)
            die_with_error("error catching host");

        strncpy(msg->accept, value, length);
        msg->accept_length = length;

        return 0;
    }
    if(strncmp(msg->field, "Accept-Language", length) == 0){
        msg->accept_language = malloc(length*sizeof(char));

        if (msg->accept_language == NULL)
            die_with_error("error catching host");

        strncpy(msg->accept_language, value, length);
        msg->accept_language_length = length;

        return 0;
    }
    if(strncmp(msg->field, "Accept-Encoding", length) == 0){
        msg->accept_encoding = malloc(length*sizeof(char));

        if (msg->accept_encoding == NULL)
            die_with_error("error catching host");

        strncpy(msg->accept_encoding, value, length);
        msg->accept_encoding_length = length;

        return 0;
    }

    if(strncmp(msg->field, "Referer", length) == 0){
        msg->referer = malloc(length*sizeof(char));

        if (msg->referer == NULL)
            die_with_error("error catching host");

        strncpy(msg->referer, value, length);
        msg->referer_length = length;

        return 0;
    }

    if(strncmp(msg->field, "Connection", length) == 0){
        msg->connection = malloc(length*sizeof(char));

        if (msg->connection == NULL)
            die_with_error("error catching host");

        strncpy(msg->connection, value, length);
        msg->connection_length = length;

        return 0;
    }

    return -1;

}

int url_reader(http_parser *p, const char *buf, size_t length){

    //printf("Url: %.*s\n", (int)length, buf);

    rcv_msg *msg = (rcv_msg *) p->data;

    msg->url = malloc(length*sizeof(char));

    strncpy(msg->url, buf, length);

    msg->url_length = length;

    //printf("msg->url: %.*s\n", (int) length, msg->url);

    return 0;

}

int on_message_begin(http_parser* _) {
    (void)_;
    printf("\n***MESSAGE BEGIN***\n\n");
    return 0;
}

int on_message_complete(http_parser* _) {
    (void)_;
    printf("\n***MESSAGE COMPLETE***\n\n");
    return 0;
}

int on_header_field(http_parser *p, const char* at, size_t length) {

    rcv_msg *msg = (rcv_msg *) p->data;

    free(msg->field);

    msg->field = malloc(length*sizeof(char));

    memset(msg->field, '\0', length);

    strncpy(msg->field, at, length);

    //printf("Field: %.*s with length %d\n", (int) length, msg->field, (int) length);

    return 0;
}

int on_header_value(http_parser *p, const char* at, size_t length) {

    rcv_msg *msg = (rcv_msg *) p->data;

    //printf("Field: %s\n", msg->field);

    //printf("Value: %.*s\n", (int) length, at);

    if(header_field_compare(msg, at, length) == -1)
        printf("problems in field reading\n");

    return 0;
}

void parse_data(rcv_msg *msg, char *buf, int rcvd){

    size_t  nparsed;

    http_parser_settings settings;
    http_parser *parser;

    //printf("parser\n");

    memset(&settings, 0, sizeof(settings));

    settings.on_message_begin = on_message_begin;
    settings.on_url = url_reader;
    settings.on_message_complete = on_message_complete;
    settings.on_header_value = on_header_value;
    settings.on_header_field = on_header_field;

    parser = malloc(sizeof(http_parser));

    http_parser_init(parser, HTTP_REQUEST);

    parser->data = msg;

    nparsed = http_parser_execute(parser, &settings, buf, (size_t) rcvd);

    if(nparsed != (size_t) rcvd)
        die_with_error("nparsed != rcv");
}
