#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>


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
					if ((result < 0) || (result > 5))
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
	std::cout << "Dictionary by Vasar v 0.1\n" << std::endl;
	std::cout << "1) Add new word" << std::endl;
	std::cout << "2) Delete word/clear dictionary" << std::endl;
	std::cout << "3) Read from file" << std::endl;
	std::cout << "4) Save to file" << std::endl;
	std::cout << "5) Show dictionary\n" << std::endl;
	std::cout << "0) Exit" << std::endl;
}

int main()
{
	std::multimap<std::string, std::string>  dictionary;
	std::string inputfile, outputfile;
	int command;
	bool flag = true;
	while (flag) {
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
		// Добавление нового слова в словарь.
		case 1: {
			std::string temp, str1, str2;
			system("cls");
			std::cout << "Enter word and it definition in format 'word : defunition' [enter 0 for return]: ";
			std::getline(std::cin, temp);
			if (temp == "0") {
				break;
			}
			int pos = temp.find(" : ");
			if (pos == -1) {
				std::cout << "Wrong input data!\n";
				system("pause");
				break;
			}
			str1 = temp.substr(0, pos);
			str2 = temp.substr(pos + 3, temp.length() - 1);
			dictionary.insert(std::pair<std::string, std::string>(str1, str2));
			std::cout << temp << " was added!" << std::endl;
			system("pause");
			break;
		}
		// Удаление/очистка словаря.
		case 2: {
			std::string temp;
			system("cls");
			if (dictionary.empty()) {
				std::cout << "Dictionary is empty!\n";
				system("pause");
				break;
			}
			for (auto it = dictionary.begin(); it != dictionary.end(); ++it)
				std::cout << it->first << " : " << it->second << std::endl;
			std::cout << "Enter word, which you want to delete (enter 1 to clear dictionary) [enter 0 for return]: ";
			std::getline(std::cin, temp);
			if (temp == "0") {
				break;
			}
			else if (temp == "1") {
				dictionary.clear();
				std::cout << "Dictionary was cleared!\n";
				system("pause");
				break;
			}
			else {
				auto itWord = dictionary.begin();
				itWord = dictionary.find(temp);
				if (itWord != dictionary.end()) {
					dictionary.erase(itWord);
					std::cout << temp << " was deleted!\n";
				}
				else
					std::cout << temp << " didn't find in the dictionary!\n";
			}
			system("pause");
			break;
		}
		// Загрузка словаря из файла.
		case 3: {
			system("cls");
			std::cout << "Input file name (example.txt) [enter 0 for return]: ";
			std::getline(std::cin, inputfile);
			if (inputfile == "0") {
				break;
			}
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
			dictionary.clear();
			while (!input.eof()) {
				std::string temp, str1, str2;
				std::getline(input, temp);
				int pos = temp.find(" : ");
				str1 = temp.substr(0, pos);
				str2 = temp.substr(pos + 3, temp.length() - 1);
				dictionary.insert(std::pair<std::string, std::string>(str1, str2));
			}
			std::cout << std::endl;
			for (auto it = dictionary.begin(); it != dictionary.end(); ++it)
				std::cout << it->first << " : " << it->second << std::endl;
			system("pause");
			break;
		}
		// Сохранение словаря в файл.
		case 4: {
			system("cls");
			if (!dictionary.empty()) {
				std::cout << "Output file name (example.txt) [enter 0 for return]: ";
				std::getline(std::cin, outputfile);
				if (outputfile == "0") {
					break;
				}
				std::ofstream output(outputfile);	//output data file
				for (auto it = dictionary.begin(); it != dictionary.end(); ++it)
					output << it->first << " : " << it->second << std::endl;
				std::cout << "Text from dictionary was successfully saved!\n";
			}
			else
				std::cout << "Edit field is empty!\n";
			system("pause");
			break;
		}
		// Вывод словаря в консоль.
		case 5: {
			system("cls");
			if (!dictionary.empty())
				for (auto it = dictionary.begin(); it != dictionary.end(); ++it)
					std::cout << it->first << " : " << it->second << std::endl;
			else
				std::cout << "Dictionary is empty!\n";
			system("pause");
			break;
		}}
	}
    return 0;
}