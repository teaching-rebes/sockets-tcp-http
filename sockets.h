#ifndef SOCKETS_H
#define SOCKETS_H

#include <iostream>
#include <winsock.h>

void terminate_with_error(std::string error_msg);
void check_winsock_version();
void send_string(SOCKET socket, std::string string_to_send);
std::string receive_string(SOCKET socket);

#endif // SOCKETS_H