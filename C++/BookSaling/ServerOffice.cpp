#include "stdafx.h"
#include <winsock.h>
#include <iostream>
#include <thread>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <chrono>

#pragma comment(lib,"ws2_32.lib")

#define SIZE 2048
#define COUNT 100


std::vector <int> users;
std::map<std::string, bool> tickets;
std::mutex mut;

// Функция потока для отправки данных о текущем статусе базы данных с билетами.
void sendingData(int client_socket)
{
	char reply[SIZE];
	char buffer[SIZE];
	memset(reply, 0, SIZE);
	memset(buffer, 0, SIZE);

	std::string temp = std::to_string(COUNT);
	const char *temp_c;
	temp_c = temp.c_str();
	strcat_s(buffer, temp_c);
	send(client_socket, buffer, strlen(buffer), 0);
	std::chrono::milliseconds dura(5);
	std::this_thread::sleep_for(dura);
	for (auto it = tickets.begin(); it != tickets.end(); ++it) {
		memset(buffer, 0, SIZE);
		temp.clear();
		temp = it->first;
		if (it->second == true)
			temp += " 1";
		else
			temp += " 0";
		temp_c = temp.c_str();
		strcat_s(buffer, temp_c);
		std::this_thread::sleep_for(dura);
		send(client_socket, buffer, strlen(buffer), 0);
	}
}

// Функция потока для обработки запросов на "покупку" билета.
void processing(int client_socket, char *reply)
{
	char buffer[SIZE];
	memset(buffer, 0, SIZE);

	bool isAccess = true, isFind = false;
	std::string temp;
	temp = reply;
	// Блокируем этот участок функции для того, чтобы два потока не пытались одновременно изменить данные в контейнере.
	std::lock_guard<std::mutex> locker(mut);
	for (auto it = tickets.begin(); it != tickets.end(); ++it) {
		if (temp == it->first) {
			isFind = true;
			if (it->second == true)
				it->second = false;
			else
				isAccess = false;
		}	
	}
	// Обработав запрос, отправляем ответ.
	memset(buffer, 0, SIZE);
	std::string temped;
	if ((isAccess) && (isFind)) {
		temped = "2";
		std::cout << "User " << client_socket << " bought successfully ticket " << reply << std::endl;
		send(client_socket, buffer, strlen(buffer), 0);
	}
	else if (isFind) {
		temped = "1";
		std::cout << "User " << client_socket << " tried to buy sold ticket " << reply << std::endl;
		send(client_socket, buffer, strlen(buffer), 0);
	}
	else {
		temped = "0";
		std::cout << "User " << client_socket << " tried to buy non-existent ticket " << reply << std::endl;
	}
	const char *temp_c;
	temp_c = temped.c_str();
	strcat_s(buffer, temp_c);
	send(client_socket, buffer, strlen(buffer), 0);
}

// Функция потока для приёма и обработки запросов от пользователей.
void adding(int client_socket)
{
	char reply[SIZE];
	char buffer[SIZE];
	memset(reply, 0, SIZE);
	memset(buffer, 0, SIZE);
	sendingData(client_socket);
	while (true) {
		memset(reply, 0, SIZE);
		memset(buffer, 0, SIZE);
		if (recv(client_socket, reply, SIZE, 0) > 0) {
			std::string temp;
			temp = reply;
			if (temp == "Tickets")
				sendingData(client_socket);
			else
				processing(client_socket, reply);
		}
		else {
			std::cout << "\nUser " << client_socket << " disconnected.\n" << std::endl;
			for (unsigned int i = 0; i < users.size(); i++)
				if (users[i] == client_socket)
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

	for (int i = 0; i < COUNT; i++) {
		std::string key = std::to_string(i + 1);
		tickets.insert((std::pair<std::string, bool>(key, true)));
	}
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
	this_server.sin_port = htons(9999);
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
	//closesocket(socket_desc);
	shutdown(socket_desc, 1);
	WSACleanup();
	return 0;
}