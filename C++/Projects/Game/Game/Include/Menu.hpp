#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


extern sf::Font errorFont;	// �������� �����
extern sf::Font font;	// ����� ��� ������ ��������� �� �������

extern sf::Image heroImage;	// �������� �����
extern sf::Image heroAttackImage;	// �������� ����� �����
extern sf::Image heroDeathImage;	// �������� ������ �����
extern sf::Image ghostEnemyImage;	// �������� ��������
extern sf::Image ghostEnemyImageDeath;	// �������� ������ ��������
extern sf::Image golemEnemyImage;	// �������� ������
extern sf::Image golemEnemyImageAttack;	// �������� ����� ������
extern sf::Image golemEnemyImageDeath;	// �������� ������ ������
extern sf::Image soldierEnemyImage;	// �������� ������ �������
extern sf::Image shadowEnemyImage;	// �������� ����
extern sf::Image goblinEnemyImage;	// �������� �������
extern sf::Image goblinSwordEnemyImage;	// �������� �������-�������
extern sf::Image minotaurMageEnemyImage;	// �������� ��������� ����
extern sf::Image dwarfEnemyImage;	// �������� �������

extern sf::Image fireDamageImage;	// ����������� ��� ���� ���������-����
extern sf::Image movePlatformImage;	// �������� ���������
extern sf::Image bulletImage;	// ����������� ��� ����
extern sf::Image quest_image;	// �������� ������

extern sf::Music music;	// ������ ������
extern sf::SoundBuffer shootBuffer;	// ����� ��� �����

extern std::string errorType;

void menu(sf::RenderWindow & window);
bool loadResources();

#endif // MENU_HPP