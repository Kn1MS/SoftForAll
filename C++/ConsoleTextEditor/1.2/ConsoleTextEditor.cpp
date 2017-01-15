#include "stdafx.h"
#include <atlstr.h>
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


std::string VERSION = "1.2";
std::map<std::string, bool> SETTINGS = { { "Confirmation of action", false }, { "Syntax highlighting", false }};
std::vector<std::string> WORDS_BLUE = { "using", "namespace", "int", "bool", "char", "unsigned", "if", "else", "auto", "return", "while", "for", "void", "do", "break", "try", "throw", "catch", "default", "switch", "case", "sizeof", "const", "true", "false", "nullptr", "string", "vector", "map", "HANDLE", "size_type", "COORD", "WORD", "deque", "queue", "#include", "#define", "#ifndef", "#endif", "#pragma once" }; //11
std::map<int, std::string> coordColor;
std::map<int, std::string> coordLineComment;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD position = { 0, 0 };
unsigned int line = 0;
std::vector<unsigned int> lineSize;
std::vector <unsigned int>::iterator it;
std::vector<std::string> inFile;
std::vector <std::string>::iterator itStr;


// Функция для вывода цветных символов.
void output(char symbol, int color, int color1 = 0)
{
	SetConsoleTextAttribute(hConsole, (WORD)(color1 << 4 | color));
	putchar(symbol);
	SetConsoleTextAttribute(hConsole, (WORD)(color1 << 4 | 15));
}

//Функция для получения текущей директории.
std::string exePath() 
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

// Функция для поиска слов и раскрашивания их в поле редактирования.
void paintKeyWords(unsigned int numChangeLine)
{
	coordColor.clear();
	coordLineComment.clear();
	for (unsigned int i = 0; i < WORDS_BLUE.size(); i++) {
		int pos = inFile[numChangeLine].find(WORDS_BLUE[i]);
		while (pos != -1) {
			coordColor.insert((std::pair<int, std::string>(pos, WORDS_BLUE[i])));
			pos = inFile[numChangeLine].find(WORDS_BLUE[i], pos + WORDS_BLUE[i].length());
			//std::cout << inFile[numChangeLine].substr(0, pos);
			//for (unsigned int j = 0; j < WORDS_BLUE[i].length(); j++)
				//output(WORDS_BLUE[i][j], 11);
			//std::cout << inFile[numChangeLine].substr(pos + WORDS_BLUE[i].length(), inFile[numChangeLine].length() - 1) << std::endl;
		}
	}
	int pos = inFile[numChangeLine].find("//");
	if (pos != -1) {
		coordLineComment.insert((std::pair<int, std::string>(pos, inFile[numChangeLine].substr(pos, inFile[numChangeLine].length() - 1))));
	}
	int last = 0;
	if (coordColor.empty()) {
		if (!coordLineComment.empty()) {
			auto itCom = coordLineComment.begin();
			for (int i = last; i < itCom->first; i++)
				putchar(inFile[numChangeLine][i]);
			for (unsigned int i = itCom->first; i < inFile[numChangeLine].length(); i++)
				output(inFile[numChangeLine][i], 10);
			last = inFile[numChangeLine].length();
			std::cout << inFile[numChangeLine].substr(last, inFile[numChangeLine].length() - 1) << std::endl;
		}
		else 
			std::cout << inFile[numChangeLine] << std::endl;
	}
	else {
		if (!coordLineComment.empty()) {
			auto it = coordColor.begin();
			auto itCom = coordLineComment.begin();
			if (it->first < itCom->first) {
				while (it->first < itCom->first) {
					for (int i = last; i < it->first; i++)
						putchar(inFile[numChangeLine][i]);
					for (unsigned int j = 0; j < it->second.length(); j++)
						output(it->second[j], 11);
					last = it->first + it->second.length();
					++it;
					if (it == coordColor.end())
						break;
				}
				for (int i = last; i < itCom->first; i++)
					putchar(inFile[numChangeLine][i]);
				for (unsigned int i = itCom->first; i < inFile[numChangeLine].length(); i++)
					output(inFile[numChangeLine][i], 10);
				last = inFile[numChangeLine].length();
			}
			else {
				for (int i = last; i < itCom->first; i++)
					putchar(inFile[numChangeLine][i]);
				for (unsigned int i = itCom->first; i < inFile[numChangeLine].length(); i++)
					output(inFile[numChangeLine][i], 10);
				last = inFile[numChangeLine].length();
			}
		}
		else {
			for (auto it = coordColor.begin(); it != coordColor.end(); ++it) {
				for (int i = last; i < it->first; i++)
					putchar(inFile[numChangeLine][i]);
				//std::cout << inFile[numChangeLine].substr(last, it->first);
				for (unsigned int j = 0; j < it->second.length(); j++)
					output(it->second[j], 11);
				last = it->first + it->second.length();
			}
		}
		std::cout << inFile[numChangeLine].substr(last, inFile[numChangeLine].length() - 1) << std::endl;
	}
}

// Функция вывода редактируемого поля в консоль с учётом конфига настроек.
void consoleOut()
{
	if (!inFile.empty()) {
		auto it = SETTINGS.find("Syntax highlighting");
		if (it != SETTINGS.end()) {
			if (it->second)
				for (unsigned int i = 0; i < inFile.size(); i++)
					paintKeyWords(i);
			else
				for (unsigned int i = 0; i < inFile.size(); i++)
					std::cout << inFile[i] << std::endl;
		}
		else
			for (unsigned int i = 0; i < inFile.size(); i++)
				std::cout << inFile[i] << std::endl;
	}
	else {
		std::cout << "Edit field is empty!\n";
		system("pause");
	}
}

// Функция проверки правильности вода чисел.
int inputStrToInt(std::string mes, int command)
{
	std::string str;
	int minus = 0, result = 0;
	bool check = true, flag = true;
	while (flag) {
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
		if (check == false)
			return -1;
		else {
			try {
				result = std::stoi(str);
				switch (command) {
					case 0:
						break;
					case 1:
						if ((result < 0) || (result > 9))
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

// Функция безопастного перевода string в int.
int strToInt(std::string str, int command)
{
	int minus = 0, result = 0;
	bool check = true, flag = true;
	while (flag) {
		minus = 0; result = 0;
		check = true;
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
		if (check == false)
			return -1;
		else {
			try {
				result = std::stoi(str);
				switch (command) {
					case 0:
						break;
					case 1:
						if ((result < 0) || (result > 9))
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
	std::cout << "ConsoleTextEditor by Vasar v " + VERSION + '\n' << std::endl;
	std::cout << "1) Create new file" << std::endl;
	std::cout << "2) Read from file" << std::endl;
	std::cout << "3) Save to file" << std::endl;
	std::cout << "4) Show edit field\n" << std::endl;
	std::cout << "5) Compile your C++ file" << std::endl;
	std::cout << "6) Run your Python file" << std::endl;
	std::cout << "7) Start implementation of Python\n" << std::endl;
	std::cout << "8) Settings" << std::endl;
	std::cout << "9) About program" << std::endl;
	std::cout << "0) Exit\n" << std::endl;
}

// Вывод раздела настроек программы.
void outSettings()
{
	std::cout << "ConsoleTextEditor by Vasar v " + VERSION + '\n' << std::endl;
	for (auto it = SETTINGS.begin(); it != SETTINGS.end(); ++it)
		std::cout << it->first << " = " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << "1) Change settings" << std::endl;
	std::cout << "2) Load settings from file" << std::endl;
	std::cout << "3) Save settings to file\n" << std::endl;
	std::cout << "0) Return to menu\n" << std::endl;
}

// Вывод раздела "О программе".
void outAbout()
{
	std::cout << "ConsoleTextEditor by Vasar v " + VERSION + '\n' << std::endl;
	std::cout << "---------------------------------------------------------------------------------" << std::endl;
	std::cout << "| Name of the program: ConsoleTextEditor (CTE)                                  |" << std::endl;
	std::cout << "| Release date: 23.12.2016                                                      |" << std::endl;
	std::cout << "| Version of the program: " + VERSION + "                                                   |" << std::endl;
	std::cout << "| Developer of the program: Vasily 'Vasar' Vasilyev                             |" << std::endl;
	std::cout << "| You can send your comments and suggestions to the e-mail Vasar007@yandex.ru   |" << std::endl;
	std::cout << "| (©) Copyright - All rights reserved, V. V. Vasilyev, 2017                     |" << std::endl;
	std::cout << "---------------------------------------------------------------------------------" << std::endl << std::endl;
}

// Функция, убирающая из строки пустые символы '\0'.
void zip(std::string &str)
{
	std::string temp = str;
	unsigned int k = 0;
	for (unsigned int i = 0; i < str.length(); i++)
		if (str[i] != '\0') {
			temp[k] = str[i];
			k++;
		}
	str = temp.substr(0, k);
}

// Функция для обработки нажатий в консоли.
void consoleDelay()
{
	bool flag = true;
	position = { 0,0 };
	SetConsoleCursorPosition(hConsole, position);
	while (flag) {
		int ch = _getch();
		switch (ch) {
			// Нажатие клавиш-стрелок, перемещение каретки по консоли.
			case 224: {
				int sym = _getch();
				switch (sym) {
					// Нажата клавиша вверх.
					case 72: {
						if (position.Y > 0) {
							position.Y--;
							if (position.X > lineSize[position.Y])
								position.X = lineSize[position.Y];
							SetConsoleCursorPosition(hConsole, position);
						}
						break;
					}
					// Нажата клавиша вниз.
					case 80: {
						if (position.Y < line - 1) {
							position.Y++;
							if (position.X > lineSize[position.Y])
								position.X = lineSize[position.Y];
							SetConsoleCursorPosition(hConsole, position);
						}
						break;
					}
					// Нажата клавиша влево.
					case 75: {
						if (position.X > 0)
							position.X--;
						else if (position.Y > 0) {
							position.Y--;
							position.X = lineSize[position.Y];
						}
						SetConsoleCursorPosition(hConsole, position);
						break;
					}
					// Нажата клавиша вправо.
					case 77: {
						if (position.X < lineSize[position.Y])
							position.X++;
						else if (position.Y < line - 1) {
							position.X = 0;
							position.Y++;
						}
						SetConsoleCursorPosition(hConsole, position);
						break;
					}
					default: {
						break;
					}
				}
				break;
			}
			// Нажата клавиша Enter.
			case 13: {
				std::string temped = inFile[position.Y].substr(position.X, inFile[position.Y].length());
				inFile[position.Y].erase(position.X, inFile[position.Y].length());
				lineSize[position.Y] = position.X;
				it = lineSize.begin() + position.Y + 1;
				itStr = inFile.begin() + position.Y + 1;
				lineSize.insert(it, temped.length());
				inFile.insert(itStr, temped);
				position.X = 0;
				position.Y++;
				line++;
				system("cls");
				consoleOut();
				SetConsoleCursorPosition(hConsole, position);
				break;
			}
			// Нажата клавиша Esc.
			case 27: {
				flag = false;
				break;
			}
			// Нажата клавиша Backspace.
			case 8: {
				if ((position.X > 0) && (lineSize[position.Y] > 0)) {
					lineSize[position.Y]--;
					position.X--;
					std::string temped = inFile[position.Y];
					if (temped[position.X] == '\t') {
						lineSize[position.Y] -= 3;
						position.X -= 3;
					}
					temped[position.X] = '\0';
					zip(temped);
					inFile[position.Y] = temped;
				}
				else if ((position.X == 0) && (position.Y > 0)) {
					it = lineSize.begin() + position.Y;
					itStr = inFile.begin() + position.Y;
					position.X = lineSize[position.Y - 1];
					lineSize[position.Y - 1] += lineSize[position.Y];
					lineSize.erase(it);
					inFile[position.Y - 1] += inFile[position.Y];
					inFile.erase(itStr);
					position.Y--;
					line--;
				}
				system("cls");
				consoleOut();
				SetConsoleCursorPosition(hConsole, position);
				break;
			}
			// Нажата клавиша Tab.
			case 9: {
				std::string temped;
				ch = 32;
				temped = (char)ch;
				inFile[position.Y].insert(position.X, temped);
				lineSize[position.Y]++;
				position.X++;
			}
			// Нажата клавиша Space.
			case 32: {
				std::string temped;
				temped = (char)ch;
				inFile[position.Y].insert(position.X, temped);
				lineSize[position.Y]++;
				position.X++;
				system("cls");
				consoleOut();
				SetConsoleCursorPosition(hConsole, position);
				break;
			}
			// Нажата любая другая клавиша.
			default: {
				std::string temped;
				temped = (char)ch;
				inFile[position.Y].insert(position.X, temped);
				lineSize[position.Y]++;
				position.X++;
				system("cls");
				consoleOut();
				//for (unsigned int i = 0; i < position.Y; i++)
				//	std::cout << inFile[i] << std::endl;
				//paintKeyWords(position.Y);
				//if (position.Y < line - 1)
				//	for (unsigned int i = position.Y + 1; i < inFile.size(); i++)
				//		std::cout << inFile[i] << std::endl;
				SetConsoleCursorPosition(hConsole, position);
				break;
			}
		}
	}
}

// Функция для сохранения текста при создании нового файла.
void saveUnsavedText()
{
	std::string outputfile;
	while (!inFile.empty()) {
		std::cout << "Warning, you can lose current text. Do you want to continue (Press Y or N)?" << std::endl;
		std::string answer;
		do {
			std::getline(std::cin, answer);
		} while ((answer != "Y") && (answer != "y") && (answer != "N") && (answer != "n"));
		if ((answer == "Y") || (answer == "y")) {
			std::cout << "Output file name (example.txt) [enter 0 for return]: ";
			std::getline(std::cin, outputfile);
			if (outputfile == "0")
				break;
			std::ofstream output(outputfile);	//output data file
			for (unsigned int i = 0; i < inFile.size(); i++)
				output << inFile[i] << std::endl;
			std::cout << "Text from edit field was successfully saved.\n";
		}
		else
			std::cout << "Text from edit field was cleared.\n";
		inFile.clear();
		system("pause");
		system("cls");
	}
}

// Функция для чтения конфига.
void readConfig(std::string &configname)
{
	std::ifstream inputSet(configname);	//config file
	auto it = SETTINGS.begin();
	if ((inputSet.is_open()) && (it != SETTINGS.end())) {
		std::cout << "Config-file was read." << std::endl;
		while ((!inputSet.eof()) && (it != SETTINGS.end())) {
			std::string temp;
			std::getline(inputSet, temp);
			int pos = temp.find(it->first);
			if (pos != -1) {
				int pos2 = temp.find("=");
				if (pos2 != -1) {
					std::string value = temp.substr(pos2 + 2, temp.length() - 1);
					if (value == "1")
						it->second = true;
					else if (value == "0")
						it->second = false;
				}
			}
			++it;
		}
	}
}

// Функция для сохранения текущего конфига.
void saveConfig(std::string &configname)
{
	std::ofstream outputSet(configname);	//config file
	for (auto it = SETTINGS.begin(); it != SETTINGS.end(); ++it)
		outputSet << it->first << " = " << it->second << std::endl;
	std::cout << "File was successfully saved." << std::endl;
}

// Функция для подтверждения действия с учётом текущего конфига настроек.
void confirmAct()
{
	auto it = SETTINGS.find("Confirmation of action");
	if (it != SETTINGS.end())
		if (it->second)
			saveUnsavedText();
}

int main()
{
	std::string inputfile, outputfile, configname = "configCTE.ini";
	
	int command;
	bool flag = true;
	
	setlocale(LC_ALL, "Russian");
	readConfig(configname);
	while (flag) {
		system("cls");
		outMenu();
		command = inputStrToInt("Enter your command: ", 0);
		switch (command) {
			// Выход из программы.
			case 0: {
				system("cls");
				confirmAct();
				std::cout << "All unsaved temp files will be deleted! They are in current working folder.\n" << std::endl;
				std::cout << "Goodbye, human!" << std::endl;
				system("pause");
				flag = false;
				break;
			}
			// Создание нового рабочего поля.
			case 1: {
				std::cin.sync();
				std::cin.clear();
				system("cls");
				confirmAct();
				line = 1;
				lineSize.clear();
				lineSize.push_back(0);
				inFile.clear();
				inFile.push_back("");
				consoleDelay();
				std::ofstream tempOut("CTEtemp.txt");	//output temp file
				if (!inFile.empty())
					for (unsigned int i = 0; i < inFile.size(); i++)
						tempOut << inFile[i] << std::endl;
				break;
			}
			// Загрузка файла в поле редактирования.
			case 2: {
				system("cls");
				std::cout << "Input file name (example.txt) [enter 0 for return]: ";
				std::getline(std::cin, inputfile);
				if (inputfile == "0")
					break;
				std::ifstream input(inputfile);	//input data file
				if (!input.is_open()) {
					std::cout << "ER01: Input error! Could not open file " << inputfile << "." << std::endl;;
					system("pause");
					break;
				}
				else {
					std::cout << "File was loading.\n";
					system("pause");
				}
				std::cin.sync();
				std::cin.clear();
				system("cls");
					line = 0;
				lineSize.clear();
				inFile.clear();
				while (!input.eof()) {
					std::string temp;
					std::getline(input, temp);
					int pos = temp.find('\t');
					while (pos != -1) {
						temp.replace(pos, 1, "  ");
						pos = temp.find('\t');
					}
					line++;
					lineSize.push_back(temp.length());
					inFile.push_back(temp);
				}
				consoleOut();
				consoleDelay();
				std::ofstream tempOut("CTEtemp.txt");	//output temp file
				if (!inFile.empty())
					for (unsigned int i = 0; i < inFile.size(); i++)
						tempOut << inFile[i] << std::endl;
				break;
			}
			// Сохранение файла из рабочего поля.
			case 3: {
				system("cls");
				if (!inFile.empty()) {
					std::cout << "Output file name (example.txt) [enter 0 for return]: ";
					std::getline(std::cin, outputfile);
					if (outputfile == "0")
						break;
					std::ofstream output(outputfile);	//output data file
					for (unsigned int i = 0; i < inFile.size(); i++)
						output << inFile[i] << std::endl;
					std::cout << "Text from edit field was successfully saved.\n";
				}
				else
					std::cout << "Edit field is empty!\n";
				system("pause");
				break;
			}
			// Вывод рабочего поля.
			case 4: {
				system("cls");
				if (!inFile.empty()) {
					consoleOut();
					consoleDelay();
				}
				else {
					std::cout << "Edit field is empty!\n";
					system("pause");
				}
				break;
			}
			// Запуск компилятора C++.
			case 5: {
				system("cls");
				std::ofstream tempOut("CTEtemp.cpp");	//output temp file
				if (!inFile.empty())
					for (unsigned int i = 0; i < inFile.size(); i++)
						tempOut << inFile[i] << std::endl;
				std::string path;
				path = "cd " + exePath();
				std::cout << "cd + Current working path:\n\n" << path << std::endl << std::endl;
				CA2W pszWide(path.c_str());
				OpenClipboard(0);
				EmptyClipboard();
				HGLOBAL hStrMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(pszWide));
				void* pStrMem = GlobalLock(hStrMem);
				memcpy(pStrMem, pszWide, sizeof(pszWide));
				GlobalUnlock(pStrMem);
				SetClipboardData(CF_UNICODETEXT, hStrMem);
				CloseClipboard();
				system("start Compiler.lnk");
				system("pause");
				break;
			}
			// Запуск файла Python.
			case 6: {
				system("cls");
				std::ofstream tempOut("CTEtemp.py");	//output temp file
				if (!inFile.empty())
					for (unsigned int i = 0; i < inFile.size(); i++)
						tempOut << inFile[i] << std::endl;
				std::string path, pyFile;
				path = "cd " + exePath();
				const char *temp_c;
				temp_c = path.c_str();
				system(temp_c);
				std::cout << "Enter file name of python program (example.py) [enter 0 for return]: ";
				std::getline(std::cin, pyFile);
				if (pyFile == "0")
					break;
				std::cout << "\nOutput:" << std::endl;
				temp_c = pyFile.c_str();
				system(temp_c);
				std::cout << std::endl;
				system("pause");
				break;
			}
			// Запуск интерпретатора Python.
			case 7: {
				system("cls");
				std::string path, pyFile;
				path = "cd " + exePath();
				const char *temp_c;
				temp_c = path.c_str();
				system(temp_c);
				std::cout << "Press CTR+Z for exit" << std::endl;
				system("start py");
				system("pause");
				break;
			}
			// Открытие раздела настроек программы.
			case 8: {
				bool flagSet = true;
				while (flagSet) {
					system("cls");
					outSettings();
					int commandSet = inputStrToInt("Enter your command: ", 0);
					switch (commandSet) {
						// Возвращение в меню.
						case 0: {
							flagSet = false;
							break;
						}
						// Изменение какой-либо настройки.
						case 1: {
							std::string changeSet;
							std::cout << "Enter number of setting and new value (0 or 1), for example: '2=0', [enter 0 for return]: ";
							std::getline(std::cin, changeSet);
							if (changeSet == "0")
								break;
							int pos = changeSet.find("=");
							std::string numSet = changeSet.substr(0, pos);
							std::string newSet = changeSet.substr(pos + 1, changeSet.length() - 1);
							int numberSet = strToInt(numSet, 0) - 1;
							int newerSet = strToInt(newSet, 0);
							auto it = SETTINGS.begin();
							for (it = SETTINGS.begin(); ((it != SETTINGS.end()) && (numberSet > 0)); ++it)
								numberSet--;
							if (it != SETTINGS.end())
								if (newerSet == 1)
									it->second = true;
								else if (newerSet == 0)
									it->second = false;
							break;
						}
						// Загрузка настроек из файла.
						case 2: {
							system("cls");
							std::string nameInSet;
							std::cout << "Enter config-file name (configCTE.ini) [enter 0 for return]: ";
							std::getline(std::cin, nameInSet);
							if (nameInSet == "0")
								break;
							readConfig(nameInSet);
							system("pause");
							break;
						}
						// Сохранение настроек в файл.
						case 3: {
							system("cls");
							std::string nameOutSet;
							std::cout << "Enter config-file name (configCTE.ini) [enter 0 for return]: ";
							std::getline(std::cin, nameOutSet);
							if (nameOutSet == "0")
								break;
							saveConfig(nameOutSet);
							system("pause");
							break;
						}
					}
				}
				break;
			}
			// Открытие раздела "О программе".
			case 9: {
				system("cls");
				outAbout();
				system("pause");
				break;
			}
		}
	}
	// Удаление временных файлов.
	system("del CTEtemp.txt");
	system("del CTEtemp.cpp");
	system("del CTEtemp.py");
	return 0;
}