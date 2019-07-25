#include "pch.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include "defaultSocketConfiguration.h"
#include "WinSockServer.h"
#include <sstream>
#include <iomanip>
#pragma warning(disable: 4996)


const int max_connections = 100;


SOCKET Connections[max_connections];
int Counter = 0;

struct REQUEST {
	char message[256];
	char username[256];
};
void ClientHandler(int index) {
	std::string SERVER_MASSAGE_S = "[SERVER: broadcast] ";
	std::string CLIENT_MASSAGE;
	std::string RESULT_MESSAGE;
	char user_request_message[256];
	while (true) {
		recv(Connections[index], user_request_message, sizeof(user_request_message), NULL);
		for (int i = 0; i < Counter; i++) {
			if (i == index) { continue; }
			else { 
				CLIENT_MASSAGE = user_request_message;
				RESULT_MESSAGE = SERVER_MASSAGE_S + CLIENT_MASSAGE; //only client message send
				char* response = new char[RESULT_MESSAGE.size()];
				strcpy(response, CLIENT_MASSAGE.c_str());
				send(Connections[i], user_request_message, sizeof(user_request_message), NULL);
			}
		}
	}
}

int main(int argc, char* argv[]) {
	WSAData wsaData;
	SOCKADDR_IN addr;
	SOCKET newConnection;
	WORD DLLVersion = MAKEWORD(2, 1);
	std::cout << "[Initialize dll'ls] Starting...\n";
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "[Initialize] Error.\n" << std::endl;
		exit(1);
	}
	else { std::cout << "[Initialize dll'ls] Complete.\n"; }
	int sizeofaddr = sizeof(addr);
	std::cout << "[Initialize Socket] Starting...\n" << std::endl;
	addr.sin_addr.s_addr = inet_addr(sockConfigure.net_properties.localhost);
	addr.sin_port = htons(sockConfigure.net_properties.port);
	addr.sin_family = AF_INET;
	std::cout << "[Initialize Socket] Socket run\n" << std::endl;
	std::cout << "[INFO] address: " << sockConfigure.net_properties.localhost << std::endl;
	std::cout << "[INFO] port: " << sockConfigure.net_properties.port << std::endl;
	std::cout << "[INFO] max clients: " << sockConfigure.additional_properties.max_client_count << std::endl;
	std::cout << "[INFO] Waiting for connections...\n";
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
	for (int i = 0; i < sockConfigure.additional_properties.max_client_count; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client Connected!\n";
			char msg[256] = "[SERVER] Connection established.";
			send(newConnection, msg, sizeof(msg), NULL);

			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}
	system("pause");
	return 0;
}
