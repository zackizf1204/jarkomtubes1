#ifndef __ACK_H
#define __ACK_H

#include "var.h"

using namespace std;

class Ack {
public:   
  Ack() {}
  Ack(int nextFrameNumber, int windowsize) {
    ack = ACK;
    next_frame_number = nextFrameNumber;
    advwindowsize = windowsize;
    checksum = 0;
    checksum = getChecksum(serialize());
  }
  bool isValid() {
    return (checksum==getChecksum(serialize()) && ack == ACK);
  }
  Byte* serialize() {
    Byte* temp = new Byte[7];
    temp[0]=ack;
    temp[1]=(next_frame_number >> 24) & 0xFF;
    temp[2]=(next_frame_number >> 16) & 0xFF;
    temp[3]=(next_frame_number >> 8) & 0xFF;
    temp[4]=next_frame_number & 0xFF;
    temp[5]=advwindowsize;
    temp[6]=checksum;
    return temp;
  }
  void unserialize(Byte* c) {
    ack = c[0];
    next_frame_number = c[1] << 24 | c[2] << 16 | c[3] << 8 | c[4];
    advwindowsize = c[5];
    checksum = c[6];
  }
  Byte getAck() {
    return ack;
  }
  unsigned int getFrameNumber() {
    return next_frame_number;
  }

private:
  // return checksum computed based on data
  Byte getChecksum(Byte* BitString) {
   unsigned long long x = 0;
    Byte f1, f2, f3, f4;

    f1=(next_frame_number >> 24) & 0xFF;
    f2=(next_frame_number >> 16) & 0xFF;
    f3=(next_frame_number >> 8) & 0xFF;
    f4=next_frame_number & 0xFF;

    x = ack + f1 + f2 + f3 + f4 + advwindowsize;
    x = x % 256;
    return (Byte)x;
  }

  Byte ack;
  unsigned int advwindowsize;
  unsigned int next_frame_number;
  Byte checksum;
};

#endif
