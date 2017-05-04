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
	std::string							name;	// ���������� name ���� string
	std::string							type;	// type ���� string
	sf::Rect<float>						rect;	// ��� Rect � �������� ����������
	std::map<std::string, std::string>	properties;	// ������ ������������� ������. ���� - ��������� ���, �������� ���� ��������� ���
	sf::Sprite							sprite;	// ������ �������


	int									getPropertyInt(std::string name);	// ����� �������� ������� � ������
	float								getPropertyFloat(std::string name);
	std::string							getPropertyString(std::string name);
};


struct Layer 
{
	int						opacity;	// �������������� ����
	std::vector<sf::Sprite> tiles;	// ������, ��������� �� ������
};


class Level 
{
	private:
		int					width;
		int					height;
		int					tileWidth;
		int					tileHeight;	// ����������, ������� ����� ������� ����� �� tmx-����� �� ������
		int					firstTileID;	// ����� ��������� ������� �����
		sf::Rect<float>		drawingBounds;	// ������ ����� �����, ������� ������
		sf::Texture			tilesetImage;	// �������� �����
		std::vector<Layer>	layers;	// ������ ����
	
	
	public:
		int					levelNumber;	// ����� ������
		std::vector<Object>	objects;	// ������ ���� �������, ������� �������� ����
	
	
	public:
		bool				loadFromFile(std::string filename);	// ���������� false, ���� �� ������� ��������� �������
		Object				getObject(std::string name);
		std::vector<Object> getObjects(std::string name);	// ����� ������ � �������
		std::vector<Object> getAllObjects();	// ����� ��� ������� � �������
		void				drawAll(sf::RenderWindow& window);
		void				draw(sf::RenderWindow& window);
		sf::Vector2i		getTileSize();	// �������� ������ �����
};

#endif // LEVEL_HPP
