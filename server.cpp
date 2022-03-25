#include "init.hpp"

int main(int argc, char const *argv[]) {
	int serverSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (serverSocket == -1) {
		cout<<"Failed to create socket, ERROR: "<<errno<<endl;
		exit(EXIT_FAILURE);
	};
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(7777);
	if (bind(serverSocket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		cout<<"Failed to bind to port 7777, ERROR: "<<errno<<endl;
		exit(EXIT_FAILURE);
	};
	bool run = 1;
	while (run == 1) {
		scanForClients(serverSocket,sockaddr);
		for (Client a : CLIENTS) {
			recvDataFromClient(a.socket);
		};
	};
	return 0;
};
