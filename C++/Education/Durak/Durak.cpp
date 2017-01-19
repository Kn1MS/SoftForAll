#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <Windows.h>

#define COUNT 36

std::string deck2[] = { "6", "7", "8", "9", "10", "�", "�", "�", "�",
						"6", "7", "8", "9", "10", "�", "�", "�", "�",
						"6", "7", "8", "9", "10", "�", "�", "�", "�", 
						"6", "7", "8", "9", "10", "�", "�", "�", "�" };
std::string deck[] = {  "6 �","7 �","8 �","9 �","10 �","� �","� �","� �","� �",
						"6 �","7 �","8 �","9 �","10 �","� �","� �","� �","� �",
						"6 �","7 �","8 �","9 �","10 �","� �","� �","� �","� �",
						"6 �","7 �","8 �","9 �","10 �","� �","� �","� �","� �" };
bool card[COUNT], step = true, isGame = true, deep = false;
std::vector<int> cardPlayer1, cardPlayer2, extra;
int cosIr = 0;
HANDLE hCons;

void changeColor(int val) {
	if (val == -1) {
		SetConsoleTextAttribute(hCons, (WORD)((0 << 4) | 7));	// DEFAULT
		return;
	}
	if (val < 9) {
		SetConsoleTextAttribute(hCons, (WORD)((0 << 4) | 12));	// RED
	}
	else if (val < 18) {
		SetConsoleTextAttribute(hCons, (WORD)((0 << 4) | 14));	// YELLOW
	}
	else if (val < 27) {
		SetConsoleTextAttribute(hCons, (WORD)((0 << 4) | 2));	// GREEN
	}
	else {
		SetConsoleTextAttribute(hCons, (WORD)((0 << 4) | 3));	// CYAN
	}
}

void discardCard(int num1, int num2, std::vector<int> &cardPlayer1, std::vector<int> &cardPlayer2)
{
	std::cout << "����!" << std::endl;
	auto it = cardPlayer1.begin();
	for (unsigned int i = 0; i < cardPlayer1.size(); i++)
		if (cardPlayer1[i] == cardPlayer1[num1])
			break;
		else
			++it;
	cardPlayer1.erase(it);
	it = cardPlayer2.begin();
	for (unsigned int i = 0; i < cardPlayer2.size(); i++)
		if (cardPlayer2[i] == cardPlayer2[num2])
			break;
		else
			++it;
	cardPlayer2.erase(it);
	if (extra.size() > 0) {
		if (cardPlayer1.size() < 6) {
			cardPlayer1.push_back(extra[extra.size() - 1]);
			extra.pop_back();
		}
		if (cardPlayer2.size() < 6) {
			if (extra.size() > 0) {
				cardPlayer2.push_back(extra[extra.size() - 1]);
				extra.pop_back();
			}
		}
	}
	if ((cardPlayer1.size() == 0) || (cardPlayer2.size() == 0)) {
		isGame = false;
	}
	if (!deep)
		step = !step;
	else
		deep = false;
}

void takeCard(int num1, int num2, std::vector<int> &cardPlayer1, std::vector<int> &cardPlayer2)
{
	std::cout << "�����!" << std::endl;
	cardPlayer2.push_back(cardPlayer1[num1]);
	auto it = cardPlayer1.begin();
	for (unsigned int i = 0; i < cardPlayer1.size(); i++)
		if (cardPlayer1[i] == cardPlayer1[num1])
			break;
		else
			++it;
	if (it != cardPlayer1.end()) {
		cardPlayer1.erase(it);
		if (extra.size() > 0) {
			cardPlayer1.push_back(extra[extra.size() - 1]);
			extra.pop_back();
		}
	}
	if ((cardPlayer1.size() == 0) || (cardPlayer2.size() == 0)) {
		isGame = false;
	}
}

void makeStep(int num1, int num2, std::vector<int> &cardPlayer1, std::vector<int> &cardPlayer2)
{
	std::cout << std::endl << std::endl << std::endl << std::endl;
	changeColor(cardPlayer1[num1]);
	std::cout << "                 " << deck[cardPlayer1[num1]] << std::endl << std::endl;
	changeColor(cardPlayer2[num2]);
	std::cout << "                 " << deck[cardPlayer2[num2]] << std::endl << std::endl;

	changeColor(-1);
	int k1 = 1, k2 = 1;
	if (cardPlayer1[num1] / 9 > 0)
		k1 = (cardPlayer1[num1] / 9);
	if (cardPlayer2[num2] / 9 > 0)
		k2 = (cardPlayer2[num2] / 9);

	if ((cardPlayer1[num1] / 9 == cosIr) && (cardPlayer2[num2] / 9 == cosIr)) {
		if (cardPlayer2[num2] % 9 > cardPlayer1[num1] % 9) {
			discardCard(num1, num2, cardPlayer1, cardPlayer2);
		}
		else {
			takeCard(num1, num2, cardPlayer1, cardPlayer2);
		}
	}
	else if ((cardPlayer1[num1] / 9 == cosIr) && (cardPlayer2[num2] / 9 != cosIr)) {
		takeCard(num1, num2, cardPlayer1, cardPlayer2);
	}
	else if ((cardPlayer1[num1] / 9 != cosIr) && (cardPlayer2[num2] / 9 == cosIr)) {
		discardCard(num1, num2, cardPlayer1, cardPlayer2);
	}
	else if ((cardPlayer1[num1] / 9 != cosIr) && (cardPlayer2[num2] / 9 != cosIr)) {
		if ((cardPlayer2[num2] % 9) > (cardPlayer1[num1] % 9) && (k1 == k2)) {
			discardCard(num1, num2, cardPlayer1, cardPlayer2);
		}
		else {
			takeCard(num1, num2, cardPlayer1, cardPlayer2);
		}
	}
}

int main()
{
	hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(time(NULL));
	cosIr = rand() % 4;
	std::string cosF;
	if (cosIr == 0)
		cosF = "�";
	else if (cosIr == 1)
		cosF = "�";
	else if (cosIr == 2)
		cosF = "�";
	else if (cosIr == 3)
		cosF = "�";
	for (int i = 0; i < COUNT; i++)
		card[i] = true;
	setlocale(LC_ALL, "Russian");

	for (int i = 0; i < 6; i++) {
		int index = rand() % 36;
		while (!card[index]) {
			index = rand() % 36;
		}
		cardPlayer1.push_back(index);
		card[index] = false;
	}
	for (int i = 0; i < 6; i++) {
		int index = rand() % 36;
		while (!card[index]) {
			index = rand() % 36;
		}
		cardPlayer2.push_back(index);
		card[index] = false;
	}
	for (int i = 0; i < COUNT; i++) {
		if (card[i])
			extra.push_back(i);
		card[i] = false;
	}
	
	bool isBot = false;
	int number1, number2 = 0, numb = 0;
	std::cout << "�����. ����� 1 ������, ����� 2 �����. ������ ����� ����� 1.\n";
	std::cout << "PvP or PvE (0 or 1): ";
	std::cin >> numb;
	if (numb == 1)
		isBot = true;
	while (isGame) {
		system("cls");
		for (unsigned int i = 0; i < cardPlayer1.size(); i++) {
			changeColor(cardPlayer1[i]);
			std::cout << deck[cardPlayer1[i]] << "      ";
		}
		std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
		changeColor(cosIr * 9);
		std::cout << extra.size() << "   " << cosF;
		std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
		for (unsigned int i = 0; i < cardPlayer2.size(); i++) {
			changeColor(cardPlayer2[i]);
			std::cout << deck[cardPlayer2[i]] << "      ";
		}
		changeColor(-1);
		std::cout << std::endl << std::endl;

		if (!isBot) {
			if (step) {
				std::cout << "����� ����� ������ 1: ";
				std::cin >> number1;
				number1--;
				std::cout << "����� ����� ������ 2 [0 = ���]: ";
				std::cin >> number2;
				if (number2 == 0) {
					takeCard(number1, number2, cardPlayer1, cardPlayer2);
					continue;
				}
				number2--;
				makeStep(number1, number2, cardPlayer1, cardPlayer2);
			}
			else {
				std::cout << "����� ����� ������ 2: ";
				std::cin >> number2;
				number2--;
				std::cout << "����� ����� ������ 1 [0 = ���]: ";
				std::cin >> number1;
				if (number1 == 0) {
					takeCard(number2, number1, cardPlayer2, cardPlayer1);
					continue;
				}
				number1--;
				makeStep(number2, number1, cardPlayer2, cardPlayer1);
			}
			system("pause");
		}
		else {
			if (step) {
				std::cout << "����� ����� ������: ";
				std::cin >> number1;
				number1--;
				number2 = rand() % cardPlayer2.size();
				makeStep(number1, number2, cardPlayer1, cardPlayer2);
			}
			else {
				number2 = rand() % cardPlayer2.size();
				std::cout << "��� ������ ����� " << number2 + 1 << std::endl;
				std::cout << "����� ����� ������ [0 = ���]: ";
				std::cin >> number1;
				if (number1 == 0) {
					takeCard(number2, number1, cardPlayer2, cardPlayer1);
					continue;
				}
				number1--;
				makeStep(number2, number1, cardPlayer2, cardPlayer1);
			}
			system("pause");
		}
	}
	if ((cardPlayer1.size() == 0) && (cardPlayer2.size() == 0))
		std::cout << "�����" << std::endl;
	else if ((cardPlayer1.size() == 0) && (cardPlayer2.size() != 0))
		std::cout << "������ ������ 1" << std::endl;
	else if ((cardPlayer1.size() != 0) && (cardPlayer2.size() == 0))
		std::cout << "������ ������ 2" << std::endl;
	system("pause");
	return 0;
}