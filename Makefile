all : sendfile.cpp recvfile.cpp
	g++ -pthread -o sendfile sendfile.cpp -std=c++11
	g++ -pthread -o recvfile recvfile.cpp -std=c++11
