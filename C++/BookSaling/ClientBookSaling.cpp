#include "stdafx.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <winsock.h>
#include <Windows.h>

#pragma comment(lib,"ws2_32.lib")

#define SIZE 1024

std::map<std::string, bool> tickets;

// Функция проверки правильности вода чисел.
int inputStrToInt(std::string mes, int command)
{
	std::string str;
	int minus = 0, result = 0;
	bool check = true, flag = true;
	while (flag)
	{
		minus = 0; result = 0;
		check = true;
		str.clear();
		std::cout << mes;
		std::getline(std::cin, str);
		for (unsigned int i = 0; i < str.size(); i++) {
			if (((str[i] < '0') | (str[i] > '9')) & (str[0] != '-')) {
				check = false;
				break;
			}
			if (str[i] == '-') {
				minus++;
				if (minus > 1) {
					check = false;
					break;
				}
			}
		}
		if (check == false) {
			return -1;
		}
		else {
			try {
				result = std::stoi(str);
				switch (command) {
				case 0:
					break;
				case 1:
					if ((result < 0) || (result > 4))
						throw 1;
					break;
				case 2:
					if (result <= 0)
						throw 2;
					break;
				}
				return result;
			}
			catch (...) {
				return -1;
			}
		}
	}
	return 0;
}

// Вывод меню.
void outMenu()
{
	std::cout << "Booking-office by Vasar v 0.1\n" << std::endl;
	std::cout << "1) Buy a ticket" << std::endl;
	std::cout << "2) Show information about tickets" << std::endl;
	std::cout << "0) Exit" << std::endl;
}

void check(int socket_desc)
{
	int read_size, count, pos;
	char server_reply[SIZE];
	std::string buf, key, value;
	while (true) {
		memset(server_reply, 0, SIZE);

		if ((read_size = recv(socket_desc, server_reply, SIZE, 0)) <= 0) {
			std::cout << "\nrecv failed\n";
			break;
		}

		buf = server_reply;
		count = std::stoi(buf);
		tickets.clear();
		for (int i = 0; i < count; i++) {
			memset(server_reply, 0, SIZE);
			if ((read_size = recv(socket_desc, server_reply, SIZE, 0)) <= 0) {
				std::cout << "\nrecv failed\n";
				break;
			}
			buf = server_reply;
			pos = buf.find(" ");
			key = buf.substr(0, pos);
			value = buf.substr(pos + 1, buf.length());
			pos = buf.find(" ");
			if (value == "1")
				tickets.insert((std::pair<std::string, bool>(key, true)));
			else
				tickets.insert((std::pair<std::string, bool>(key, false)));
		}
		std::cout << "\nData base was updated." << std::endl;
		break;
	}
}

int main()
{
	int command;
	bool flag = true;
	// Переменные необходимые для подключения к серверу.
	WSADATA wsa;
	int socket_desc, read_size;
	sockaddr_in server;
	char message[SIZE];
	memset(message, 0, SIZE);
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
	std::cout << "Socket created\n";
	// Задание параметров для сокета.
	server.sin_addr.s_addr = inet_addr("192.168.0.100"); //192.168.43.204; 172.20.157.87
	server.sin_family = AF_INET;
	server.sin_port = htons(9999);
	// Подключение к серверу.
	if (connect(socket_desc, (sockaddr*)&server, sizeof(server)) < 0) {
		std::cout << "Connection to server was failed.\n";
		system("pause");
		return 3;
	}
	std::cout << "Connected.\n";
	std::cout << "Data base is updating. Please, waiting..." << std::endl;
	// Создание потока для получение первичной информации о билетах.
	std::thread checking(check, socket_desc);
	checking.join();

	while (flag) {
		memset(message, 0, SIZE);
		system("cls");
		outMenu();
		command = inputStrToInt("Enter your command: ", 0);
		switch (command) {
		// Выход из программы.
		case 0:
			system("cls");
			std::cout << "Goodbye, human!" << std::endl;
			system("pause");
			flag = false;
			break;
		// Создание нового потока и осуществление процесса покупки билета.
		case 1: {
			system("cls");
			memset(message, 0, SIZE);
			// Обновляем данные о билетах.
			if (tickets.empty()) {
				std::cout << "Data base is empty. Please, try to connect later." << std::endl;
				break;
			}
			else {
				char mes[] = "Tickets";
				send(socket_desc, mes, strlen(mes), 0);
				std::cout << "Data base is updating. Please, waiting..." << std::endl;
				std::thread checknow(check, socket_desc);
				checknow.join();
			}
			// Выводим информацию о билетах.
			for (auto it = tickets.begin(); it != tickets.end(); ++it) {
				if (it->second == true)
					std::cout << it->first << " : access" << std::endl;
				else
					std::cout << it->first << " : sold" << std::endl;
			}
			std::cout << std::endl;
		lab:
			// Красивости в консоли.
			std::cout << "Choose your seat: ";
			std::cin.getline(message, SIZE);
			if (strlen(message) < 1) {
				HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO buff;
				GetConsoleScreenBufferInfo(h, &buff);
				if (buff.dwCursorPosition.Y > 0)
					SetConsoleCursorPosition(h, { 0, buff.dwCursorPosition.Y - 1});
				goto lab;
			}
			// Локальная проверка для того, чтобы лишний раз не отправлять запрос на сервер.
			bool isFind = false, isAccess = true;
			std::string comp;
			comp = message;
			for (auto it = tickets.begin(); it != tickets.end(); ++it) {
				if (comp == it->first) {
					isFind = true;
					if (it->second == false) {
						isAccess = false;
						std::cout << "\nError! You tried to buy sold ticket." << std::endl;
						system("pause");
						break;
					}
				}
			}
			// Отсеиваем "пустые" запросы.
			if (!isFind) {
				std::cout << "\nError! You tried to buy non-existent ticket." << std::endl;
				system("pause");
				break;
			}
			if (!isAccess)
				break;
			// Отправляем запрос на сервер.
			if (send(socket_desc, message, strlen(message), 0) < 0) {
				std::cout << "\nSend failed.\n";
				system("pause");
				flag = false;
			}
			memset(message, 0, SIZE);
			// Получаем ответ от сервера.
			if ((read_size = recv(socket_desc, message, SIZE, 0)) <= 0) {
				std::cout << "\nrecv failed\n";
				system("pause");
				break;
			}
			// Выводим полученную информацию на экран.
			std::string temp;
			temp = message;
			if (temp == "2") {
				std::cout << "\nYou bought this ticket successfully." << std::endl;
			}
			else if (temp == "1") {
				std::cout << "\nError! You tried to buy sold ticket." << std::endl;
			}
			else if (temp == "0") {
				std::cout << "\nError! You tried to buy non-existent ticket." << std::endl;
			}
			else
				std::cout << "\nError! Unexpected answer. Please, try again later." << std::endl;
			system("pause");
			break;
		}
		// Вывод информации о билетах.
		case 2: {
			system("cls");
			char mes[] = "Tickets";
			if (send(socket_desc, mes, strlen(mes), 0) < 0) {
				std::cout << "\nSend failed.\n";
				system("pause");
				flag = false;
			}
			// Обновляем данные о билетах.
			std::cout << "Data base is updating. Please, waiting..." << std::endl;
			std::thread checknew(check, socket_desc);
			checknew.join();
			// Выводим информацию о билетах.
			for (auto it = tickets.begin(); it != tickets.end(); ++it) {
				if (it->second == true)
					std::cout << it->first << " : access" << std::endl;
				else
					std::cout << it->first << " : sold" << std::endl;
			}
			system("pause");
			break;
		}}
	}
	// Закрытие сокета и WinsockAPI.
	shutdown(socket_desc, 1);
	//closesocket(socket_desc);
	WSACleanup();
	std::cout << "Server disconected.\n";
	system("pause");
	return 0;
}