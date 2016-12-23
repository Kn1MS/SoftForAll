#include "stdafx.h"
#include <winsock.h>
#include <iostream>
#include <thread>
#include <Windows.h>
#include <string>
#include <vector>

#pragma comment(lib,"ws2_32.lib")

#define SIZE 2048


std::vector <int> users;

// Функция потока для рассылки сообщений всем подключённым пользователям.
void sending(int client_socket, char *reply)
{
	std::cout << "User " << reply << std::endl;
	for (unsigned int i = 0; i < users.size(); i++)
		if (client_socket != users[i])
			send(users[i], reply, strlen(reply), 0);
}

// Функция потока для приёма и обработки данных от присоединённых пользователей.
void adding(int client_sock)
{
	char reply[SIZE];
	char buffer[SIZE];
	while (true) {
		memset(reply, 0, SIZE);
		memset(buffer, 0, SIZE);
		if (recv(client_sock, reply, SIZE, 0) > 0) {
			// Дополняем принятое сообщение информацией о том, с какого клиента она была получена.
			std::string user;
			user = reply;
			user.insert(0, std::to_string(client_sock) + " : ");
			const char *user_c;
			user_c = user.c_str();
			if (strlen(reply) < 1)
				continue;
			else
				strcat_s(buffer, user_c);
			// Рассылаем всем пользователям.
			sending(client_sock, buffer);
		}
		// В случае отключения исключаем данные сокета из контейнера.
		else {
			std::cout << "\nUser " << client_sock << " disconnected.\n" << std::endl;
			for (unsigned int i = 0; i < users.size(); i++)
				if (users[i] == client_sock)
					users.erase(users.begin() + i);
			break;
		}
	}
}

int main() 
{
	sockaddr_in this_server, new_client;
	WSADATA wsa;
	int socket_desc, client_sock, c = sizeof(sockaddr_in);
	// Инициализация WinsockAPI.
	std::cout << "Initialising Winsock...\n";
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cout << "Failed. Error Code : " << WSAGetLastError() << "\n";
		system("pause");
		return 1;
	}
	std::cout << "Initialised.\n";
	// Инициализация самого сокета.
	if ((socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		std::cout << "Could not create socket.\n";
		system("pause");
		return 2;
	}
	std::cout << "Socket created.\n";
	// Задание параметров сокета.
	this_server.sin_family = AF_INET;
	this_server.sin_addr.S_un.S_addr = INADDR_ANY;
	this_server.sin_port = htons(8888);
	// Привязка к определенному адресу и порту.
	if (bind(socket_desc, (sockaddr*)&this_server, sizeof(this_server)) < 0) {
		std::cout << "Bind failed." << std::endl;
		return 3;
	}
	// Включаем "слушающий" режим для приёма входящих соединений.
	listen(socket_desc, 3);
	std::cout << "Waiting for connection..." << std::endl;
	// Ждём запроса на соединение.
	while (client_sock = accept(socket_desc, (sockaddr*)&new_client, &c))
	{
		std::cout << "\nNew connection!\n";
		users.push_back(client_sock);
		std::thread newuser(adding, client_sock);
		newuser.detach();
		if (client_sock < 0) {
			perror("Cannot accept the connection.");
			system("pause");
			break;
		}
	}
	// Закрытие сокета и WinsockAPI.
	shutdown(socket_desc, 1);
	//closesocket(socket_desc);
	WSACleanup();
	return 0;
}