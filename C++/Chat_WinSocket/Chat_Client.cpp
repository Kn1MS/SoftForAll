#include "stdafx.h"
#include <winsock.h>
#include <iostream>
#include <thread>
#include <Windows.h>
#include <vector>

#pragma comment(lib,"ws2_32.lib")

#define SIZE 2048

// Функция потока для отправки сообщений.
void sendMessage(int socket_desc)
{
	char message[SIZE];
	memset(message, 0, SIZE);

	while (true) {
		std::cout << "You: ";
		std::cin.getline(message, SIZE);
		// Проверка на отправку пустой строки.
		if (strlen(message) < 1)
			continue;
		// Отправка данных на сервер.
		if (send(socket_desc, message, strlen(message), 0) < 0) {
			std::cout << "\nSend failed!\n";
			break;
		}
	}
}

int main() {
	WSADATA wsa;
	int socket_desc, read_size;
	sockaddr_in server;
	char server_reply[SIZE];

	memset(server_reply, 0, SIZE);
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
	// Задание параметров для сокета.
	server.sin_addr.s_addr = inet_addr("192.168.0.100"); //192.168.43.204; 172.20.157.87
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
	// Подключение к серверу.
	if (connect(socket_desc, (sockaddr*)&server, sizeof(server)) < 0) {
		std::cout << "Connection to server was failed.\n";
		system("pause");
		return 3;
	}
	std::cout << "Connected to server.\n";
	// Создание и отсоединение потока для отправки сообщений.
	std::thread sending(sendMessage, socket_desc);
	sending.detach();
	// В цикле параллельно будем принимать и обрабатывать сообщения от сервера.
	while (true) {
		memset(server_reply, 0, SIZE);
		// Проверка на получение данных.
		if ((read_size = recv(socket_desc, server_reply, SIZE, 0)) <= 0) {
			std::cout << "\nrecv failed\n";
			break;
		}
		// Проверка на получение пустой строки.
		if (strlen(server_reply) < 1)
			continue;
		// Красивости в консоли.
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO buff;
		GetConsoleScreenBufferInfo(h, &buff);
		SetConsoleCursorPosition(h, { 0, buff.dwCursorPosition.Y });

		std::cout << server_reply << "\nYou: ";
	}

	std::cout << "\nServer disconected.\n";
	system("pause");
	// Закрытие сокета и WinsockAPI.
	shutdown(socket_desc, 1);
	//closesocket(socket_desc);
	WSACleanup();
	return 0;
}