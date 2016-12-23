#include "stdafx.h"
#include <atlstr.h>
#include <atlconv.h>
#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD position = { 0, 0 };
unsigned int line = 0;
std::vector<unsigned int> lineSize;
std::vector <unsigned int>::iterator it;
std::vector<std::string> inFile;
std::vector <std::string>::iterator itStr;
/*CONSOLE_SCREEN_BUFFER_INFO SCRN_INFO;
GetConsoleScreenBufferInfo(hConsole, &SCRN_INFO);
position.X = SCRN_INFO.dwCursorPosition.X;
position.Y = SCRN_INFO.dwCursorPosition.Y;*/


//Функция для получения текущей директории.
std::string exePath() 
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
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
	std::cout << "ConsoleTextEditor by Vasar v 0.5\n" << std::endl;
	std::cout << "1) Create new file" << std::endl;
	std::cout << "2) Read from file" << std::endl;
	std::cout << "3) Save to file" << std::endl;
	std::cout << "4) Show edit field\n" << std::endl;
	std::cout << "5) Compile your C++ file" << std::endl;
	std::cout << "6) Compile your Python file\n" << std::endl;
	std::cout << "7) Start implementation of Python\n" << std::endl;
	std::cout << "0) Exit\n" << std::endl;
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
			}}
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
			for (unsigned int i = 0; i < inFile.size(); i++)
				std::cout << inFile[i] << std::endl;
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
			for (unsigned int i = 0; i < inFile.size(); i++)
				std::cout << inFile[i] << std::endl;
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
			for (unsigned int i = 0; i < inFile.size(); i++)
				std::cout << inFile[i] << std::endl;
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
			for (unsigned int i = 0; i < inFile.size(); i++)
				std::cout << inFile[i] << std::endl;
			SetConsoleCursorPosition(hConsole, position);
			break;
		}}
	}
}

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
			std::cout << "Text from edit field was successfully saved!\n";
		}
		else
			std::cout << "Text from edit field was cleared\n";
		inFile.clear();
		system("pause");
		system("cls");
	}
}

int main()
{
	std::string inputfile, outputfile;
	int command;
	bool flag = true;
	setlocale(LC_ALL, "Russian");
	while (flag) {
		system("cls");
		outMenu();
		command = inputStrToInt("Enter your command: ", 0);
		switch (command) {
		// Выход из программы.
		case 0:
			system("cls");
			saveUnsavedText();
			std::cout << "Goodbye, human!" << std::endl;
			system("pause");
			flag = false;
			break;
		// Создание нового рабочего поля.
		case 1: {
			std::cin.sync();
			std::cin.clear();
			system("cls");
			saveUnsavedText();
			line = 1;
			lineSize.clear();
			lineSize.push_back(0);
			inFile.clear();
			inFile.push_back("");
			consoleDelay();
			std::ofstream tempOut("ConsoleTextEditorTemp.txt");	//output temp file
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
				std::cout << "File was loading!\n";
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
				std::cout << temp << std::endl;
				line++;
				lineSize.push_back(temp.length());
				inFile.push_back(temp);
			}
			consoleDelay();
			std::ofstream tempOut("ConsoleTextEditorTemp.txt");	//output temp file
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
				std::cout << "Text from edit field was successfully saved!\n";
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
				for (unsigned int i = 0; i < inFile.size(); i++)
					std::cout << inFile[i] << std::endl;
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
		// Запуск компилятора Python.
		case 6: {
			system("cls");
			std::string path, pyFile;
			path = "cd " + exePath();
			const char *temp_c;
			temp_c = path.c_str();
			system(temp_c);
			std::cout << "File name (example.py) [enter 0 for return]: ";
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
		}}
	}
	// Удаление временных файлов.
	system("del ConsoleTextEditorTemp.txt");
	return 0;
}