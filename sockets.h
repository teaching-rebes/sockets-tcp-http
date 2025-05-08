#ifndef SOCKETS_H
#define SOCKETS_H

#include <iostream>
typedef int SOCKET;

void terminate_with_error(std::string error_msg);
void send_string(SOCKET socket, std::string string_to_send);
std::string receive_string(SOCKET socket);

#endif // SOCKETS_H