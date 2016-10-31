//
//Definition of all server constants and structures.
//

#ifndef WEBSERVER_CONSTANTS_H
#define WEBSERVER_CONSTANTS_H

#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <wchar.h>
#include <netinet/in.h>
#include <stdbool.h>


/* size of buffer for log line  */
const size_t BUFFER_LOG;
/*  path of images' directory    */
const char *PATH;
/*  path of log file    */
const char *LOG_PATH;
/*  path of server's database of images */
const size_t MAXLINE;
/* default protocol port number */
const in_port_t DEFAULT_PORT;
/*  successfully HTTP request   */
const char *HTTP_OK;
/*  error in serving HTTP request */
const char *HTTP_BAD_REQUEST;
/*  not found file requested    */
const char *HTTP_NOT_FOUND;
/*  HTML page to show not-found error    */
const char *HTML_404;
/*  HTML page to show bad-request error    */
const char *HTML_400;
/*  HTML page to show not supported protocol HTTP 1.0 error    */
const char *HTML_NOT_SUPPORTED;
/*  version HTTP protocol 1.1   */
const char *HTTP_1;
/*  version HTTP protocol 1.0   */
const char *HTTP_0;
/*  HTTP request Accept line    */
const char *ACCEPT;
/*  home page of server images to show to the client    */
const char *INDEX;

/*  Image adapted struct   */
typedef struct conv_img {
    char             original_name[256];
    unsigned long    name_code; // result hash function
    size_t           width;
    size_t           height;
    size_t           colors;
    size_t           quality; // quality factor * 100
    char             type[5];
    size_t           length;
    char             last_modified[21]; // date in SQL format
} conv_img;

/*  Device characteristics and capabilities struct */
typedef struct device {
    char id[100];
    size_t width;
    size_t height;
    long colors;
    bool jpg;
    bool png;
    bool gif;
} device ;

/* Log file line struct */
typedef struct log
{
    char ip_host[16];   // IP address of the client
    char date[30];      // current date and time
    char reqLine[300];  // HTTP request line
    char status[50];    // HTTP response status
    size_t size;        // number of bytes sent
} log;

/* Image struct */
typedef struct
{
    const char *base_path;

    float quality;      //From Accept

    int max_height;     //From libwurfl-master
    int max_width;      //From libwurfl-master
    int colors;         //From libwurfl-master
    char *extension;    //From libwurfl-master

} image_info_t;

#endif //WEBSERVER_CONSTANTS_H
