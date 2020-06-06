#include "server.h"
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

int ServerTalk(SOCKET client);
int ServerAccept(SOCKET listener, int i);
SOCKET ServerListen(int port);

int main() {
	
	int port;
	std::cout << "Enter port # ";
	std::cin >> port;
	WSADATA wsda;
	WORD ver = MAKEWORD(2, 2);
	int startup = WSAStartup(ver, &wsda);
	if (startup != 0) {
		return -1;
	}
	SOCKET listener = ServerListen(port);
	int state = 1;
	int& stateref = state;
	int i = 0;
	int& ri = i;
	while (state) {
		std::cout << "\nListening\n";
		stateref = ServerAccept(listener,i);
		ri++;
	}
	
	closesocket(listener);
	WSACleanup();
	
	return 0;
}

SOCKET ServerListen(int port) {
	std::string ipAddr = "0.0.0.0";
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	SOCKET listener = socket(AF_INET, SOCK_STREAM, 0);
	bind(listener, (sockaddr*)&hint, sizeof(hint));
	listen(listener, SOMAXCONN);

	return listener;

}

int ServerAccept(SOCKET listener, int i) {
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = accept(listener, (sockaddr*)&client, &clientSize);
	/*std::vector<sockaddr_in> client(5);
	std::vector<int> clientSize;
	std::vector<SOCKET> clientSocket;
	clientSocket.push_back(accept(listener, (sockaddr*)client.data(), clientSize.data()+i));
	clientSize.push_back(sizeof(client.at(i)));*/

	std::cout << "\nclientSock address:" << &clientSocket << std::endl;
	std::cout << "\nclient struct addr:" << &client << std::endl;
	std::cout << "\nAccepting Connection\n";
	std::thread clientThread(ServerTalk, clientSocket);
	clientThread.detach();
	std::cout << "\nAccepted\n";
	return 1;
} 

int ServerTalk(SOCKET client) {
	std::cout << &client << "\n";
	std::vector<char> buf(4096);
	while (1) {
		int bytesReceived = recv(client, buf.data(), 4096, 0);
		if (bytesReceived > 0) {
			send(client, buf.data(), bytesReceived + 1, 0);
		}
	}
	return 0;
}