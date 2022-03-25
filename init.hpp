#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <set>

using namespace std;

struct Vector2 {
	int x;
	int y;
};

struct Client {
	Vector2 position;
	int socket;
	bool operator<(const Client& _other) const {
		return socket < _other.socket;
	};
};

set<Client> CLIENTS;

void sendDataFromServer(int _socket, Client _sentClient) {
	cout<<"Sending position: "<<_sentClient.position.x<<","<<_sentClient.position.y<<" from socket: "<<_sentClient.socket<<endl;
	send(_socket,&_sentClient,sizeof(_sentClient),0);
};

Client recvInitDataFromServer(int _socket) {
	Client _recvClient;
	recv(_socket,&_recvClient,sizeof(_recvClient),0);
	cout<<"My socket is: "<<_recvClient.socket<<endl;
	return _recvClient;
};

void recvDataFromServer(int _socket) {
	Client _recvClient;
	int _recv = recv(_socket,&_recvClient,sizeof(_recvClient),MSG_DONTWAIT | MSG_MORE);
	if (_recv != -1) {
		cout<<"Received position: "<<_recvClient.position.x<<","<<_recvClient.position.y<<" from socket: "<<_recvClient.socket<<endl;
	};
};

void scanForClients(int _serverSocket, sockaddr_in _sockaddr) {
	if (listen(_serverSocket,10) < 0) {
		cout<<"Failed to listen. ERROR: "<<errno<<endl;
		exit(EXIT_FAILURE);
	};
	auto addrlen = sizeof(_sockaddr);
	int _connection = accept4(_serverSocket, (struct sockaddr*)&_sockaddr, (socklen_t*)&addrlen, SOCK_NONBLOCK);
	if (_connection < 0) {
		return;
	}
	else {
		cout<<"Connection received"<<endl;
		Client _client = {{0,0},_connection};
		CLIENTS.insert(_client);
		sendDataFromServer(_connection,_client);
	};
};

Client recvDataFromClient(int _socket) {
	Client _recvClient;
	if (recv(_socket,&_recvClient,sizeof(_recvClient),MSG_DONTWAIT | MSG_MORE) != -1) {
		cout<<"Received position: "<<_recvClient.position.x<<","<<_recvClient.position.y<<" from socket: "<<_recvClient.socket<<endl;
		for (Client a : CLIENTS) {
			if (a.socket == _recvClient.socket) {
				a = _recvClient;
			}
			else {
				sendDataFromServer(a.socket,{{_recvClient.position.x,_recvClient.position.y},_recvClient.socket});
			};
		};
	};
	return _recvClient;
};

void sendDataFromClient(int _socket, Client _sentClient) {
	send(_socket,&_sentClient,sizeof(_sentClient),0);
};

