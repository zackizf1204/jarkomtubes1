receiver : Receiver.cpp
	g++ -std=c++11 Receiver.cpp -o receiver -lpthread 

transmitter : Transmitter.cpp
	g++ -std=c++11 Transmitter.cpp -o transmitter -lpthread

recvfile : recvfile.cpp
	g++ -std=c++11 recvfile.cpp -o recvfile -lpthread -fpermissive

sendfile : sendfile.cpp
	g++ -std=c++11 sendfile.cpp -o sendfile -lpthread -fpermissive
