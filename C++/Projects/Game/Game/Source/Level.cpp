#include "../Include/Level.hpp"


int Object::getPropertyInt(std::string name)
{
	// ���������� ����� �������� � ������
	return atoi(properties[name].c_str());
}

float Object::getPropertyFloat(std::string name)
{
	return static_cast<float>(strtod(properties[name].c_str(), NULL));
}

std::string Object::getPropertyString(std::string name)
{
	// �������� ��� � ���� ������
	return properties[name];
}

bool Level::loadFromFile(std::string filename)
{
	//��������� ���� � TiXmlDocument
	TiXmlDocument levelFile(filename.c_str());

	if (!levelFile.LoadFile())
	{
		// ���� �� ������� ��������� �����, ������ ������
		std::cout << "Loading level \"" << filename << "\" failed." << std::endl;
		return false;
	}

	// �������� � ����������� map
	TiXmlElement *map;
	map = levelFile.FirstChildElement("map");

	// ������ �����: <map version="1.0" orientation="orthogonal"
	// width="10" height="10" tilewidth="34" tileheight="34">

	// ��������� �� ����� ����� �� ��������, ������� �������� ��� ������ � ������� ���������
	width = atoi(map->Attribute("width"));
	height = atoi(map->Attribute("height"));
	tileWidth = atoi(map->Attribute("tilewidth"));
	tileHeight = atoi(map->Attribute("tileheight"));

	// ����� �������� �������� � ������������� ������� �����
	TiXmlElement *tilesetElement = map->FirstChildElement("tileset");
	firstTileID = atoi(tilesetElement->Attribute("firstgid"));

	// source - ���� �� �������� � ���������� image
	TiXmlElement *image = tilesetElement->FirstChildElement("image");
	std::string imagepath = image->Attribute("source");

	// �������� ��������� �������
	sf::Image img;

	if (!img.loadFromFile(imagepath))
	{
		// ���� �� ������� ��������� �������-������� ������ � �������
		std::cout << "Failed to load tile sheet." << std::endl;
		return false;
	}

	// ��� ����� �����.������ ��� �����
	img.createMaskFromColor(sf::Color(152, 172, 226));
	tilesetImage.loadFromImage(img);
	//�����������
	tilesetImage.setSmooth(false);

	// �������� ���������� �������� � ����� ��������
	int columns = tilesetImage.getSize().x / tileWidth;
	int rows = tilesetImage.getSize().y / tileHeight;

	// ������ �� ��������������� ����������� (TextureRect)
	std::vector<sf::Rect<int>> subRects;

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
		{
			sf::Rect<int> rect;

			rect.top = y * tileHeight;
			rect.height = tileHeight;
			rect.left = x * tileWidth;
			rect.width = tileWidth;

			subRects.push_back(rect);
		}

	// ������ �� ������
	TiXmlElement *layerElement = map->FirstChildElement("layer");
	while (layerElement) 
	{
		Layer layer;

		// ���� ������������ opacity, �� ������ ������������ ����, ����� �� ��������� �����������
		if (layerElement->Attribute("opacity") != NULL)
		{
			float opacity = static_cast<float>(strtod(layerElement->Attribute("opacity"), NULL));
			layer.opacity = 255 * static_cast<int>(opacity);
		}
		else
		{
			layer.opacity = 255;
		}

		// ��������� <data>
		TiXmlElement *layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == NULL)
		{
			std::cout << "Bad map. No layer information found." << std::endl;
		}

		// ��������� <tile> - �������� ������ ������� ����
		TiXmlElement *tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == NULL)
		{
			std::cout << "Bad map. No tile information found." << std::endl;
			return false;
		}

		int x = 0;
		int y = 0;

		while (tileElement)
		{
			int tileGID = atoi(tileElement->Attribute("gid"));
			int subRectToUse = tileGID - firstTileID;

			// ������������� TextureRect ������� �����
			if (subRectToUse >= 0)
			{
				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(subRects[subRectToUse]);
				sprite.setPosition(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight));
				sprite.setColor(sf::Color(255, 255, 255, layer.opacity));

				// ���������� � ���� ������� ������
				layer.tiles.push_back(sprite);
			}

			tileElement = tileElement->NextSiblingElement("tile");

			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				if (y >= height)
					y = 0;
			}
		}

		layers.push_back(layer);

		layerElement = layerElement->NextSiblingElement("layer");
	}



	// ���� ���� ���� ��������
	if (map->FirstChildElement("objectgroup") != NULL)
	{
		// ������ � ���������
		TiXmlElement *objectGroupElement = map->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{
			// ��������� <object>
			TiXmlElement *objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement)
			{
				// �������� ��� ������ - ���, ���, �������, � ��
				std::string objectType;
				if (objectElement->Attribute("type") != NULL)
				{
					objectType = objectElement->Attribute("type");
				}
				std::string objectName;
				if (objectElement->Attribute("name") != NULL)
				{
					objectName = objectElement->Attribute("name");
				}
				int x = atoi(objectElement->Attribute("x"));
				int y = atoi(objectElement->Attribute("y"));

				int width, height;

				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition(static_cast<float>(x), static_cast<float>(y));

				if (objectElement->Attribute("width") != NULL)
				{
					width = atoi(objectElement->Attribute("width"));
					height = atoi(objectElement->Attribute("height"));
				}
				else
				{
					width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
					height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
					sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - firstTileID]);
				}

				// ��������� �������
				Object object;
				object.name = objectName;
				object.type = objectType;
				object.sprite = sprite;

				sf::Rect <float> objectRect;
				objectRect.top = static_cast<float>(y);
				objectRect.left = static_cast<float>(x);
				objectRect.height = static_cast<float>(height);
				objectRect.width = static_cast<float>(width);
				object.rect = objectRect;

				// "����������" �������
				TiXmlElement *properties = objectElement->FirstChildElement("properties");
				if (properties != NULL)
				{
					TiXmlElement *prop = properties->FirstChildElement("property");
					if (prop != NULL)
					{
						while (prop)
						{
							std::string propertyName = prop->Attribute("name");
							std::string propertyValue = prop->Attribute("value");

							object.properties[propertyName] = propertyValue;

							prop = prop->NextSiblingElement("property");
						}
					}
				}


				objects.push_back(object);

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
	{
		std::cout << "No object layers found..." << std::endl;
	}

	return true;
}

Object Level::getObject(std::string name)
{
	Object *result = new Object();
	// ������ ������ ������ � �������� ������
	for (size_t i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
		{
			*result = objects[i];
			return *result;	// objects[i]
		}

	std::cout << "Error! Couldn't find object with this name!" << std::endl;
	return *result;
}

std::vector<Object> Level::getObjects(std::string name)
{
	// ��� ������� � �������� ������
	std::vector<Object> vec;
	for (size_t i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			vec.push_back(objects[i]);

	return vec;
}


std::vector<Object> Level::getAllObjects()
{
	return objects;
}


sf::Vector2i Level::getTileSize()
{
	return sf::Vector2i(tileWidth, tileHeight);
}

void Level::drawAll(sf::RenderWindow& window)
{
	// ������ ��� ����� (������� �� ������!)
	for (size_t layer = 0; layer < layers.size(); layer++)
		for (size_t tile = 0; tile < layers[layer].tiles.size(); tile++)
			window.draw(layers[layer].tiles[tile]);
}

void Level::draw(sf::RenderWindow& window)
{
	sf::Vector2f center = window.getView().getCenter();
	sf::Vector2f size = window.getView().getSize();
	sf::FloatRect viewRect = sf::FloatRect(center.x - (size.x / 2.f) - 100.f, center.y - (size.y / 2.f) - 100.f, 
									   	   center.x + (size.x / 2.f) + 100.f, center.y + (size.y / 2.f) + 100.f);

	for (size_t layer = 0; layer < layers.size(); layer++)
		for (size_t tile = 0; tile < layers[layer].tiles.size(); tile++)
		{
			if (viewRect.contains(layers[layer].tiles[tile].getPosition()))
				window.draw(layers[layer].tiles[tile]);
		}
}
