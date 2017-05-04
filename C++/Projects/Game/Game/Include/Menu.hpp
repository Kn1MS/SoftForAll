#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


extern sf::Font errorFont;	// Основной шрифт
extern sf::Font font;	// Шрифт для вывода сообщения об ошибках

extern sf::Image heroImage;	// Картинка героя
extern sf::Image heroAttackImage;	// Картинка атаки героя
extern sf::Image heroDeathImage;	// Картинка смерти героя
extern sf::Image ghostEnemyImage;	// Картинка призрака
extern sf::Image ghostEnemyImageDeath;	// Картинка смерти призрака
extern sf::Image golemEnemyImage;	// Картинка голема
extern sf::Image golemEnemyImageAttack;	// Картинка атаки голема
extern sf::Image golemEnemyImageDeath;	// Картинка смерти голема
extern sf::Image soldierEnemyImage;	// Картинка тёмного солдата
extern sf::Image shadowEnemyImage;	// Картинка тени
extern sf::Image goblinEnemyImage;	// Картинка гоблина
extern sf::Image goblinSwordEnemyImage;	// Картинка гоблина-мечника
extern sf::Image minotaurMageEnemyImage;	// Картинка минотавра мага
extern sf::Image dwarfEnemyImage;	// Картинка дварфов

extern sf::Image fireDamageImage;	// Изображение для огня минотавра-мага
extern sf::Image movePlatformImage;	// Картинка платформы
extern sf::Image bulletImage;	// Изображение для пули
extern sf::Image quest_image;	// Картинка миссий

extern sf::Music music;	// Объект музыки
extern sf::SoundBuffer shootBuffer;	// Буфер для звука

extern std::string errorType;

void menu(sf::RenderWindow & window);
bool loadResources();

#endif // MENU_HPP