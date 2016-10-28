#Compiles server

server: server.c connection.c error_management.c send_recv.c server_http_parser.c http-parser-master/http_parser.c files.c process_management.c utilities.c thread.c constants.c
	gcc -Wall -Wextra -O2 -pthread -o server server.c connection.c error_management.c send_recv.c server_http_parser.c http-parser-master/http_parser.c files.c process_management.c utilities.c thread.c constants.c

