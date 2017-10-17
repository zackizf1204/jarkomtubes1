
#include "Frame.h"
#include "Ack.h"
#include <chrono>

#include <pthread.h>

const long long TIMEOUT = 3000; // millisecond

int socketfd; // socket handler
struct sockaddr_in transmitterAddr;
struct sockaddr_in receiverAddr; 

typedef struct {
	Frame frame;
	chrono::system_clock::time_point lastSent;
	Byte ack;
} rQElement;

// buffer for retransmission, set chrono::system_clock::now() every transmit
deque<rQElement> retransmissionQueue;


class Transmitter {
	public:
		Transmitter(char* host, int port, char* filename) {
			/* Construct the transmiter sockaddr structure */
			memset(&transmitterAddr, 0, sizeof(transmitterAddr));
			transmitterAddr.sin_family = AF_INET;
			transmitterAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			transmitterAddr.sin_port = htons(0);

			/* Create the socket */
			if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
				perror("Failed to create socket");
			}

			if (bind(socketfd, (struct sockaddr *) &transmitterAddr, sizeof(transmitterAddr)) < 0) {
				perror("Failed to bind socket");
			}

			/* Construct the receiver sockaddr structure */
			memset(&receiverAddr, 0, sizeof(receiverAddr));
			receiverAddr.sin_family = AF_INET;
			inet_pton(AF_INET, host, &receiverAddr.sin_addr.s_addr);
			receiverAddr.sin_port = htons(port);

			messageParsing(filename);
		}

		void send() {

			socklen_t addrlen = sizeof(receiverAddr);
			for (auto it : frames) {
					if (!sendto(socketfd, it.serialize(), 9, 0, (struct sockaddr *)&receiverAddr, addrlen)) {
						perror("sendto failed");
					} 
					else {
						printf("Paket %d dikirim\n", it.getFrameNumber());
					}

			}
		}

		~Transmitter() {
			close(socketfd);
		}

	private:
		// array of frame
		std::vector<Frame> frames;
		void messageParsing(char* filename) {
			std::ifstream infile;
			infile.open(filename);

			char currentChar;
			int counter = 0;
			int checksum;
			while (infile.get(currentChar)) {
				Frame frame(counter % (2 * WINDOWSIZE), currentChar);
				frames.push_back(frame);
				counter++; 
			}
		}

		// div antar dua waktu
		long long timeDiv(chrono::system_clock::time_point t1, chrono::system_clock::time_point t2) {
			return chrono::duration_cast<chrono::milliseconds>(t1 - t2).count();
		}
};

int main(int argc, char* argv[]) {
	if (argc > 3) {
		Transmitter transmitter(argv[1], atoi(argv[2]), argv[3]);
		transmitter.send();
	}
	else {
		printf("Usage : transmitter [receiver IP] [receiver port] [message.txt]\n");
	}
}
