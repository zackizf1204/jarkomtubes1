
#ifndef COMMON_H
#define COMMON_H

#include <bits/stdc++.h>
#include <sys/socket.h>
#include <unistd.h> // close socket
#include <netdb.h>
#include <arpa/inet.h> // inet_pton
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
/* ASCII CONST */
#define SOH 1				// Start of Header Character 
#define STX 2				// Start of Text Character
#define ETX 3				// End of Text Character 
#define ACK 6				// Acknowledgement
#define NAK 21			// Negatif Acknowledgement
#define Endfile 26	// End of file Character

/* Constant */
#define MAXLEN 1024
#define WINDOWSIZE 12

#define DEFAULT_PORT 21234
#define DELAY 500000

typedef unsigned char Byte;



#endif