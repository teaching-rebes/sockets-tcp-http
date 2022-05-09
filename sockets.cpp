#include "sockets.h"
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <winsock.h>

#define BUFFERLEN 100000

using namespace std;

#define WIN32_LEAN_AND_MEAN

void terminate_with_error(string error_msg) {

  cerr << "Terminating due to error: " << error_msg << endl;
  WSACleanup();
  exit(1);
}

void check_winsock_version() {
  WORD wVersionRequested;
  WSADATA wsaData;
  wVersionRequested = MAKEWORD(1, 1);
  if (WSAStartup(wVersionRequested, &wsaData)) {
    double version;
    version = HIBYTE(wsaData.wVersion) / 10.;
    version += LOBYTE(wsaData.wVersion);
    if (version < 1.1) {
      terminate_with_error("Invalid winsock version. Must use version >= 1.1.");
    }
  }
}

SOCKET connect_to_server(string hostname, int port) {
  SOCKET sock;
  struct sockaddr_in address;
  struct hostent *host;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    terminate_with_error("Socket cannot be created.");
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  cout << "Connecting to host: " << hostname << endl;
  if (!(host = gethostbyname(hostname.c_str())))
    terminate_with_error("Cannot resolve hostname to ip address.");
  address.sin_addr.s_addr = *((unsigned long *)host->h_addr);
  if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
    terminate_with_error("Cannot connect to remote host.");

  return sock;
}

void send_string(SOCKET socket, string string_to_send) {
  send(socket, string_to_send.c_str(), string_to_send.length(), 0);
}

string receive_string(SOCKET socket) {
  char buffer[BUFFERLEN];

  int nr_received = recv(socket, buffer, BUFFERLEN - 1, 0);
  buffer[nr_received] = 0; // indicate the end of the string in buffer
  return string(buffer);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Beispiel 1: Aktuelle Zeit vom Zeitserver über einen TCP Socket abfragen
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example_receive_time() {
  SOCKET sock;

  sock = connect_to_server("time.nist.gov", 13);
  string response = receive_string(sock);
  cout << response << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Beispiel 2: Informationen über eine Domain über WHOIS Server abfragen
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example_request_domain_info(string hostname) {
  SOCKET sock;

  sock = connect_to_server("whois.internic.net", 43);
  send_string(sock, hostname + "\r\n");
  string response;
  do {
    response = receive_string(sock);
    cout << response;
  } while (response.length() > 0);

  // response = receive_string(sock);
  cout << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Implementieren Sie hier eine Methode um einen HTTP GET Request zu
// senden und die Response auszugeben!
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void example_http_request(string server, string ressource) {
  SOCKET sock;

  sock = connect_to_server(server, 80);
  send_string(sock, "GET " + ressource + " HTTP/1.1\r\n");
  send_string(sock, "Host: " + server + "\r\n");
  send_string(sock, "\r\n\r\n");

  string response;
  do {
    response = receive_string(sock);
    cout << response;
  } while (response.length() > 0);

  cout << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main method
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

  check_winsock_version();

  // example_receive_time();
  // example_request_domain_info("google.com");
  example_http_request("nicht-wahr.de", "/");

  WSACleanup();
}
