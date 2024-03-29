#ifndef __FRAME_H
#define __FRAME_H

#include "var.h"
#include <bits/stdc++.h>

using namespace std;

class Frame {
public:
  Frame() {}
  Frame(int frameNumber, Byte Data) {
    soh = SOH;
    stx = STX;
    etx = ETX;
    frame_number = frameNumber;
    data = Data;
    checksum = 0;
    checksum = getChecksum(serialize());
  }

  // Mengecek checksum
  bool isValid() {
    return (checksum==getChecksum(serialize()) && soh==SOH && stx==STX && etx==ETX);
  }

  // mengubah frame menjadi string agar siap dikirim
  Byte* serialize() {
    Byte* temp = new Byte[9];
    temp[0]=soh;
    temp[1]=(frame_number >> 24) & 0xFF;
    temp[2]=(frame_number >> 16) & 0xFF;
    temp[3]=(frame_number >> 8) & 0xFF;
    temp[4]=frame_number & 0xFF;
    temp[5]=stx;
    temp[6]=data;
    temp[7]=etx;
    temp[8]=checksum;
    //cout << temp[1] << ' ' << temp[2] << ' ' << temp[3] << ' ' << temp[4] << endl;
    return temp;
  }

  // mengubah string yang diterima menjadi objek frame
  void unserialize(Byte* c) {
    //cout << c[1] << ' ' << c[2] << ' ' << c[3] << ' ' << c[4] << endl;
    frame_number = (c[1] << 24) | (c[2] << 16) | (c[3] << 8) | c[4];
    checksum = c[8];
    data = c[6];
    soh = c[0];
    stx=c[5];
    etx=c[7];
  }

  unsigned int getFrameNumber() {
    return frame_number;
  }

  Byte getData() {
    return data;
  }

  Byte getChecksum() {
    return checksum;
  }

private:
  // return checksum computed based on data
  Byte getChecksum(Byte* BitString) {
    unsigned long long x = 0;
    Byte f1, f2, f3, f4;

    f1=(frame_number >> 24) & 0xFF;
    f2=(frame_number >> 16) & 0xFF;
    f3=(frame_number >> 8) & 0xFF;
    f4=frame_number & 0xFF;

    x = soh + f1 + f2 + f3 + f4 + stx + data + stx;
    x = x % 256;
    return (Byte)x;
  }

  Byte soh;
  Byte stx;
  Byte etx;
  unsigned int frame_number;
  Byte checksum;
  Byte data;
};

#endif
