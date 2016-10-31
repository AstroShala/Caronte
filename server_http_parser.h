//
// Created by simone_mancini on 19/10/16.
//

#ifndef WEBSERVER_HTTP_PARSER_H
#define WEBSERVER_HTTP_PARSER_H

#include "http-parser-master/http_parser.h"
#include "typedef.h"

struct url_field {
    const char *name;
    const char *url;
    int is_connect;
    struct http_parser_url u;
    int rv;
};

void parser_version_control(void);

void initialize_request_parser(http_parser *parser);

void parse_data(rcv_msg *msg, char *buf, int rcvd);

void dump_url(const char *buf, struct http_parser_url *pUrl);

#endif //WEBSERVER_HTTP_PARSER_H
