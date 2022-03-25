#include "init.hpp"

const int MSG_PER_FRAME = 16;

int main() {
	Client client;
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(7777);
	connect(clientSocket,(struct sockaddr*) &server, sizeof(server));
	client = recvInitDataFromServer(clientSocket);
	bool run = 1;
	while (run == 1) {
		bool _enterInput;
		cout << "Enter input?";
		cin >> _enterInput;
		if (_enterInput) {
			Vector2 _v;
			cout << "Enter X axis: ";
			cin >> _v.x;
			cout << "Enter Y axis: ";
			cin >> _v.y;
			client.position = _v;
			sendDataFromClient(clientSocket,{{client.position.x,client.position.y},client.socket});
		};
		for (int i = 0; i < MSG_PER_FRAME; i++) {
			recvDataFromServer(clientSocket);
		};
	};
	return 0;
};
