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

#define MAX_HEADER_LINES 10
#define MAX_ELEMENT_SIZE 2048
#define MAX_CHUNKS 16

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

typedef struct{
    int fd_sock;
    char *url_buf;
    int url_buf_len;
} custom_data_t;

static custom_data_t *custom_data;
static http_parser *parser;
//char url[MAX_ELEMENT_SIZE];
struct http_parser_url url_parser;

void parser_version_control(void){
    unsigned long version;
    unsigned major;
    unsigned minor;
    unsigned patch;

    version = http_parser_version();
    major = (version >> 16) & 255;
    minor = (version >> 8) & 255;
    patch = version & 255;
    printf("http_parser v%u.%u.%u (0x%06lx)\n", major, minor, patch, version);

    printf("sizeof(http_parser) = %u\n", (unsigned int)sizeof(http_parser));

    static const char *all_methods[] = {
            "GET",
            "HEAD",
            0
    };

    const char **this_method;
    char buf[1024];
    for (this_method = all_methods; *this_method; this_method++) {
        char inner_buf[200];
        sprintf(inner_buf, "%s / HTTP/1.1\r\n\r\n", *this_method);
        strcat(buf, inner_buf);
    }

    printf("%s\n", buf);
}


void initialize_request_parser(http_parser *parser){

    parser = malloc(sizeof(http_parser));

    if(parser == NULL)
        die_with_error("error malloc() for parser");

    http_parser_init(parser, HTTP_REQUEST);

}
size_t
strlncat(char *dst, size_t len, const char *src, size_t n)
{
    size_t slen;
    size_t dlen;
    size_t rlen;
    size_t ncpy;

    slen = strnlen(src, n);
    dlen = strnlen(dst, len);

    if (dlen < len) {
        rlen = len - dlen;
        ncpy = slen < rlen ? slen : (rlen - 1);
        memcpy(dst + dlen, src, ncpy);
        dst[dlen + ncpy] = '\0';
    }

    assert(len > slen + dlen);
    return slen + dlen;
}

int url_reader(http_parser *p, const char *buf, size_t length){

    assert(p == parser);

    printf("Url: %.*s\n", (int)length, buf);

    custom_data = malloc(sizeof(custom_data_t));

    if(custom_data == NULL)
        die_with_error("error malloc() custom_data");

    custom_data->url_buf = malloc(length*sizeof(char));

    strncpy(custom_data->url_buf, buf, length);

    custom_data->url_buf_len = length;

    printf("Url: %.*s\n", (int)length, custom_data->url_buf);

    return 0;

}

void dump_url(const char *buf, struct http_parser_url *pUrl) {
    unsigned int i;

    printf("\tfield_set: 0x%x, port: %u\n", pUrl->field_set, pUrl->port);
    for (i = 0; i < UF_MAX; i++) {
        if ((pUrl->field_set & (1 << i)) == 0) {
            printf("\tfield_data[%u]: unset\n", i);
            continue;
        }

        printf("\tfield_data[%u]: off: %u, len: %u, part: %.*s\n",
               i,
               pUrl->field_data[i].off,
               pUrl->field_data[i].len,
               pUrl->field_data[i].len,
               buf + pUrl->field_data[i].off);
    }
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

/*int on_header_value(http_parser* _, const char* at, size_t length) {
    (void)_;
    printf("Header value: %.*s\n", (int)length, at);
    return 0;
}*/

void parse_data(char *dest, char *buf, int rcvd){

    size_t  nparsed;

    http_parser_settings settings;

    memset(&settings, 0, sizeof(settings));

    settings.on_message_begin = on_message_begin;
    settings.on_url = url_reader;
    settings.on_message_complete = on_message_complete;
    //settings.on_header_value = on_header_value;

    parser = malloc(sizeof(http_parser));

    http_parser_init(parser, HTTP_REQUEST);

    nparsed = http_parser_execute(parser, &settings, buf, rcvd);

    if(nparsed != (size_t) rcvd)
        die_with_error("nparsed != rcv");

    strncpy(dest, custom_data->url_buf, custom_data->url_buf_len);
}
