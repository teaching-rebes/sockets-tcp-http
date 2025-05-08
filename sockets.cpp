// Note: This is the Linux/Ubuntu version. It cannot be used in Windows.

#include "sockets.h"
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFERLEN 100000

using namespace std;

void terminate_with_error(string error_msg) {

  cerr << "Terminating due to error: " << error_msg << endl;
  exit(EXIT_FAILURE);
}

// Connect to the server
// returns socket file descriptor which can be used to access the socket lateron
SOCKET connect_to_server(string hostname, int port) {
  SOCKET sock;
  struct sockaddr_in address;
  struct hostent *host;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    terminate_with_error("Socket cannot be created.");
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  cout << "Connecting to host: " << hostname << endl;
  if (!(host = gethostbyname(hostname.c_str())))
    terminate_with_error("Cannot resolve hostname to ip address.");
  address.sin_addr.s_addr = *((unsigned long *)host->h_addr);
  if (connect(sock, (struct sockaddr *)&address, sizeof(address))<0)
    terminate_with_error("Cannot connect to remote host.");

  return sock;
}

void send_string(SOCKET socketfd, string string_to_send) {
  send(socketfd, string_to_send.c_str(), string_to_send.length(), 0);
}

string receive_string(SOCKET socketfd) {
  char buffer[BUFFERLEN];

  ssize_t nr_received = recv(socketfd, buffer, BUFFERLEN - 1, 0);
  buffer[nr_received] = 0; // indicate the end of the string in buffer
  return string(buffer);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Beispiel 1: Aktuelle Zeit vom Zeitserver über einen TCP Socket abfragen
//
// Hintergrundinformationen (nicht wichtig, aber interessant):
// * Es handelt sich um einen der aeltesten Dienste im Internet.
// * Es wird das Daytime Protocol (Port 13) genutzt, siehe
// https://en.wikipedia.org/wiki/Daytime_Protocol
// * Original Publikation mit Infos zum vom NIST genutzten Format:
// https://tf.nist.gov/general/pdf/1046.pdf
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void example_receive_time() {
  SOCKET sock;

  sock = connect_to_server("time.nist.gov", 13);
  string response = receive_string(sock);
  cout << response << endl;

  close(sock);
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

  close(sock);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Implementieren Sie hier eine Methode um einen HTTP GET Request zu
// senden und die Response auszugeben!
//
// Stellen Sie die Anfrage dabei einen Server, der unverschlüsselt per HTTP
// antwortet, z.B. die URLs "http://httpforever.com" oder
// "http://info.cern.ch/hypertext/WWW/TheProject.html"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main method
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

  example_receive_time();
  // example_request_domain_info("google.com");

}
