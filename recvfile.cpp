#include "var.h"
#include "Frame.h"
#include "Ack.h"

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

const int BUFFER_SIZE = 2 * WINDOWSIZE;
const int N = 256;

struct arg_struct {
  // nothing, just formality :)
};

struct arg_struct args;

queue<Byte> q[BUFFER_SIZE];
Byte buffer[N];
struct sockaddr_in myaddr;
struct sockaddr_in remaddr;
socklen_t addrlen;
int sockfd;

/* paddr: print the IP address in a standard decimal dotted format */
void paddr(unsigned char *a) {
  printf("%d.%d.%d.%d", a[0], a[1], a[2], a[3]);
}

void sendAck(Frame f, bool success) {
  printf("Mengirim ACK\n");
  Ack ack(f.getFrameNumber()+1, WINDOWSIZE);
  sendto(sockfd, ack.serialize(), 6, 0, (struct sockaddr*) &remaddr, (socklen_t) addrlen);
}

void *run_receive(void*) {
  Frame f;
  while(1) {
    int recvlen = recvfrom(sockfd, buffer, 9, 0, (struct sockaddr*) &remaddr, &addrlen);
    f.unserialize(buffer);
    int num = f.getFrameNumber();
    if(f.isValid()) {
      sendAck(f, true);
      printf("Frame number %d diterima dengan sukses.\n", num);
      q[num].push(f.getData());
    } else {
      sendAck(f, false);
      printf("Frame number %d diterima. Checksum error / format salah.\n", num);
    }
  }
  pthread_exit(NULL);
}

void *run_consume(void*) {
  int pt = 0;
  while(1) {
    while(q[pt].empty()) {
      usleep(DELAY);
    }
    printf("Mengkonsumsi byte : '%c'\n", q[pt].front());

    ofstream log;
    log.open ("sent.txt", ios_base::app);
    log << q[pt].front();
    log.close();
    q[pt].pop();
    pt = (pt + 1) % BUFFER_SIZE;
  }
  pthread_exit(NULL);
}

int argc;
char** argv;

class Receiver {
public:
  Receiver(char* filename, int windowsize, int buffersize, int defport) {
    create_socket();
    initiate_binding(defport);
    bind_socket();

    int ret = pthread_create(&receiver_thread, NULL, &run_receive, (void*) &args); 
    if(ret != 0) {
      puts("Threading receiver gagal");
      exit(0);
    }
    int rec = pthread_create(&consumer_thread, NULL, &run_consume, (void*) &args);
    if(rec != 0) {
      puts("Threading consumer gagal");
      exit(0);
    }
    pthread_exit(NULL);
    close(sockfd);
  }
  ~Receiver() {

  }
private:
  void create_socket() {
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("Tidak dapat membuat socket\n");
      exit(0);
    }
    printf("Socket created with descriptor %d\n", sockfd);
  }

  void initiate_binding(int defport) {
    addrlen = sizeof(remaddr);
    memset((char*) &myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET; 
    inet_pton(AF_INET, "127.0.0.1", &myaddr.sin_addr.s_addr);
    myaddr.sin_port = htons(defport);

    printf("Binding pada alamat ");
    paddr((unsigned char*) &myaddr.sin_addr.s_addr);
    printf(":%d ...\n", defport);
  }

  void bind_socket() {
    if(bind(sockfd, (struct sockaddr*) &myaddr, sizeof(myaddr)) < 0) {
      perror("Binding gagal.");
      exit(0);
    }
    printf("Binding berhasil\n\n");
  }

  int port;
  pthread_t receiver_thread;
  pthread_t consumer_thread;
};

int main(int _argc, char *_argv[]) {
  argc = _argc;
  argv = new char*[argc];
  for(int i = 0; i < argc; i++) {
    argv[i] = new char[strlen(_argv[i])];
    memcpy(argv[i], _argv[i], sizeof(_argv[i]));
  }
  Receiver rec(argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
  return 0;
}