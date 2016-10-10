#include <cstdlib>
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <iostream>

#define INT_COUNT 6
#define CHAR_COUNT 8

#define SERVER_MODE_TCP 1
#define CLIENT_MODE_TCP 2
#define SERVER_MODE_UDP 3
#define CLIENT_MODE_UDP 4
#define EXIT 0

#pragma warning(disable : 4996)

int* randomizeInt(const int count)
{
	int* result = new int[count];
	for (int i = 0; i < count; i++) {
		result[i] = rand() % 100;
	}
	return result;
}

char* randomizeChar(const int count)
{
	char* result = new char[count];
	for (int i = 0; i < count; i++) {
		result[i] = 'a' + rand() % 24;
	}
	return result;
}

namespace TCP
{
	void server()
	{
		int dataInt[INT_COUNT];
		char dataChar[CHAR_COUNT];

		struct sockaddr_in local;
		local.sin_family = AF_INET;
		local.sin_port = htons(7500);
		local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cout << "Failed to load lib." << std::endl;
			return;
		}

		int sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			std::cout << "Socket call error." << std::endl;
			return;
		}

		int bind_code, listen_code, accept_code;
		bind_code = bind(
			sock, 
			(struct sockaddr *)&local, 
			sizeof(local)
			);

		if (bind_code < 0) {
			std::cout << "Call bind error." << std::endl;
			return;
		}

		listen_code = listen(sock, 5);
		if (listen_code) {
			std::cout << "Error during listening" << std::endl;
			return;
		}

		accept_code = accept(sock, nullptr, nullptr);
		if (accept_code < 0) {
			std::cout << "Error during accepting" << std::endl;
			return;
		}

		int recv_code, send_code;
		recv_code = recv(accept_code, (char*)dataInt, INT_COUNT * sizeof(int), 0);
		if (recv_code > 0) {
			for (int i = 0; i < INT_COUNT; i++) {
				std::cout << dataInt[i] << " ";
			}
			std::cout << std::endl;
		}
		else {
			std::cout << "Error during receiving" << std::endl;
			return;
		}

		recv_code = recv(accept_code, dataChar, CHAR_COUNT * sizeof(int), 0);
		if (recv_code > 0) {
			for (int i = 0; i < CHAR_COUNT; i++) {
				std::cout << dataChar[i] << " ";
			}
			std::cout << std::endl;
		}
		else {
			std::cout << "Error during receiving" << std::endl;
			return;
		}

		send_code = send(accept_code, (char*)dataInt, INT_COUNT * sizeof(int), 0);
		if (send_code <= 0) {
			std::cout << "Error during sending." << std::endl;
			return;
		}

		send_code = send(accept_code, dataChar, CHAR_COUNT * sizeof(char), 0);
		if (send_code <= 0) {
			std::cout << "Error during sending." << std::endl;
			return;
		}
	}

	void client()
	{
		int* dataInt = randomizeInt(INT_COUNT);
		char* dataChar = randomizeChar(CHAR_COUNT);

		struct sockaddr_in peer;
		peer.sin_family = AF_INET;
		peer.sin_port = htons(7500);
		peer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cout << "Failed to load lib." << std::endl;
			return;
		}

		int sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			std::cout << "Socket call error." << std::endl;
			return;
		}

		int conn_code = connect(
			sock,
			(struct sockaddr *)&peer,
			sizeof(peer)
		    );

		if (conn_code) {
			std::cout << "Connect call error." << std::endl;
			return;
		}

		int send_code, recv_code;
		send_code = send(sock, (char*)dataInt, INT_COUNT * sizeof(int), 0);
		if (send_code <= 0) {
			std::cout << "Error during sending." << std::endl;
			return;
		}

		send_code = send(sock, dataChar, CHAR_COUNT * sizeof(char), 0);
		if (send_code <= 0) {
			std::cout << "Error during sending." << std::endl;
			return;
		}

		recv_code = recv(sock, (char*)dataInt, INT_COUNT * sizeof(int), 0);
		if (recv_code > 0) {
			for (int i = 0; i < INT_COUNT; i++) {
				std::cout << dataInt[i] << " ";
			}
			std::cout << std::endl;
		}
		else {
			std::cout << "Error during receiving" << std::endl;
			return;
		}

		recv_code = recv(sock, dataChar, CHAR_COUNT * sizeof(int), 0);
		if (recv_code > 0) {
			for (int i = 0; i < CHAR_COUNT; i++) {
				std::cout << dataChar[i] << " ";
			}
			std::cout << std::endl;
		}
		else {
			std::cout << "Error during receiving" << std::endl;
			return;
		}
	}
}

namespace UDP
{
	void server()
	{
		int dataInt[INT_COUNT];
		char dataChar[CHAR_COUNT];

		struct sockaddr_in local;
		local.sin_family = AF_INET;
		local.sin_port = htons(7500);
		local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cout << "Failed to load lib." << std::endl;
			return;
		}

		SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock == INVALID_SOCKET) {
			std::cout << "Socket call error." << std::endl;
			return;
		}

		int bind_code, listen_code, accept_code;
		bind_code = bind(
			sock, 
			(struct sockaddr *)&local, 
			sizeof(local)
			);

		struct sockaddr_in client;
		int client_addr_size = sizeof(client);
		int recv_code;
		recv_code = recvfrom(
			sock,
			(char*)dataInt,
			INT_COUNT * sizeof(int),
			0,
			(struct sockaddr *)&client,
			&client_addr_size
			);

		if (recv_code > 0) {
			for (int i = 0; i < INT_COUNT; i++) {
				std::cout << dataInt[i] << " ";
			}
			std::cout << std::endl;
		}
		else {
			std::cout << "Error during receiving" << std::endl;
			return;
		}

		recv_code = recvfrom(
			sock,
			dataChar,
			CHAR_COUNT * sizeof(char),
			0,
			(struct sockaddr *)&client,
			&client_addr_size
			);

		if (recv_code > 0) {
			for (int i = 0; i < CHAR_COUNT; i++) {
				std::cout << dataChar[i] << " ";
			}
			std::cout << std::endl;
		}
		else {
			std::cout << "Error during receiving" << std::endl;
			return;
		}
	}

	void client()
	{
		int* dataInt = randomizeInt(INT_COUNT);
		char* dataChar = randomizeChar(CHAR_COUNT);

		struct sockaddr_in peer;
		peer.sin_family = AF_INET;
		peer.sin_port = htons(7500);
		peer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cout << "Failed to load lib." << std::endl;
			return;
		}

		int sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock < 0) {
			std::cout << "Socket call error." << std::endl;
			return;
		}

		int conn_code = connect(
			sock,
			(struct sockaddr *)&peer,
			sizeof(peer)
		    );

		if (conn_code) {
			std::cout << "Connect call error." << std::endl;
			return;
		}

		int send_code, recv_code;
		send_code = sendto(
			sock,
			(char*)dataInt,
			INT_COUNT * sizeof(int),
			0,
			(struct sockaddr *)&peer,
			sizeof(peer)
			);

		if (send_code <= 0) {
			std::cout << "Error during sending." << std::endl;
			closesocket(sock);
			return;
		}

		send_code = sendto(
			sock,
			dataChar,
			CHAR_COUNT * sizeof(char),
			0,
			(struct sockaddr *)&peer,
			sizeof(peer)
			);

		if (send_code <= 0) {
			std::cout << "Error during sending." << std::endl;
			closesocket(sock);
			return;
		}
	}
}

int main(int argc, char* argv[]) {
	setlocale(LC_CTYPE, "Russian");
	int programMode;
userInput:
	std::cout << "1 - сервер TCP\n2 - клиент TCP\n3 - сервер UDP\n4 - клиент UDP\n0 - Выход\n\n";
	std::cout << "> ";
	std::cin >> programMode;

	switch (programMode)
	{
		case SERVER_MODE_TCP:
			TCP::server();
			break;
		case CLIENT_MODE_TCP:
			TCP::client();
			break;
		case SERVER_MODE_UDP:
			UDP::server();
			break;
		case CLIENT_MODE_UDP:
			UDP::client();
			break;
		case EXIT:
			return 0;
		default:
			system("cls");
			goto userInput;
	}
	system("pause");
	return 0;
}
