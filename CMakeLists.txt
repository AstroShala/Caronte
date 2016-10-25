cmake_minimum_required(VERSION 3.6)
project(WebServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES server.c error_management.c error_management.h connection.c connection.h send_recv.c send_recv.h server_http_parser.c server_http_parser.h files.c files.h http-parser-master/http_parser.c http-parser-master/http_parser.h process_management.c process_management.h typedef.c typedef.h)
add_executable(WebServer ${SOURCE_FILES} server.c error_management.c error_management.h connection.c connection.h send_recv.c send_recv.h server_http_parser.c server_http_parser.h files.c files.h http-parser-master/http_parser.c http-parser-master/http_parser.h process_management.c process_management.h typedef.c typedef.h)