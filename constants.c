//
//Definition of all server constants.
//

#include "constants.h"

/*  path of images' directory    */
const char *PATH =               "/home/luigi/Scrivania/Caronte/Images/";
/*  path of log file    */
const char *LOG_PATH =           "/home/luigi/Scrivania/Caronte/Log/log.txt";
/* default protocol port number */
const in_port_t DEFAULT_PORT =   23000;
/* size of buffer for writing lines on log file  */
const size_t BUFFER_LOG =        50;
/*  line length */
const size_t MAXLINE =           1024;
/*  successfully HTTP request   */
const char *HTTP_OK =           "HTTP/1.1 200 OK";
/*  error in serving HTTP request */
const char *HTTP_BAD_REQUEST =  "HTTP/1.1 400 Bad Request";
/*  not found file requested    */
const char *HTTP_NOT_FOUND =    "HTTP/1.1 404 Not Found";
/*  HTML page to show not-found error    */
const char *HTML_404 =          "<html><body><h1>Error 404: Page Not Found </h1></body></html>";
/*  HTML page to show bad-request error    */
const char *HTML_400 =          "<html><body><h1>Error 400: Bad Request </h1></body></html>";
/*  HTML page to show not supported protocol HTTP 1.0 error    */
const char *HTML_NOT_SUPPORTED ="<html><body><h1>Error: Not Supported Protocol HTTP 1.0 </h1></body></html>";
/*  version HTTP protocol 1.1   */
const char *HTTP_1 =            "HTTP/1.1";
/*  version HTTP protocol 1.0   */
const char *HTTP_0 =            "HTTP/1.0";
/*  HTTP request Accept line    */
const char *ACCEPT =            "Accept:";
/*  home page of server images to show to the client    */
const char *INDEX =             "index";
