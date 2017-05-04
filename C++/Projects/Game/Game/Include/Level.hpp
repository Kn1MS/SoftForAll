#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <SFML/Graphics.hpp>
#include "../TinyXML/tinyxml.h"


struct Object 
{
	std::string							name;	// Переменная name типа string
	std::string							type;	// type типа string
	sf::Rect<float>						rect;	// Тип Rect с нецелыми значениями
	std::map<std::string, std::string>	properties;	// Создаём ассоциативный массив. Ключ - строковый тип, значение тоже строковый тип
	sf::Sprite							sprite;	// Спрайт объекта


	int									getPropertyInt(std::string name);	// Номер свойства объекта в списке
	float								getPropertyFloat(std::string name);
	std::string							getPropertyString(std::string name);
};


struct Layer 
{
	int						opacity;	// Непрозрачность слоя
	std::vector<sf::Sprite> tiles;	// Вектор, состоящий из тайлов
};


class Level 
{
	private:
		int					width;
		int					height;
		int					tileWidth;
		int					tileHeight;	// Переменные, которые будет хранить даные из tmx-файла об уровне
		int					firstTileID;	// Узнаём положение первого тайла
		sf::Rect<float>		drawingBounds;	// Размер части карты, которую рисуем
		sf::Texture			tilesetImage;	// Текстура карты
		std::vector<Layer>	layers;	// Массив слоёв
	
	
	public:
		int					levelNumber;	// Номер уровня
		std::vector<Object>	objects;	// Массив типа Объекты, который прописан выше
	
	
	public:
		bool				loadFromFile(std::string filename);	// Возвращает false, если не удалось загрузить уровень
		Object				getObject(std::string name);
		std::vector<Object> getObjects(std::string name);	// Выдаём объект в уровень
		std::vector<Object> getAllObjects();	// Выдаём все объекты в уровень
		void				drawAll(sf::RenderWindow& window);
		void				draw(sf::RenderWindow& window);
		sf::Vector2i		getTileSize();	// Получаем размер тайла
};

#endif // LEVEL_HPP
