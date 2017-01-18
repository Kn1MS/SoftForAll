///файл, отвечающий за уровень жизни игрока

class LifeBar
{
public:
	sf::Image image;
	sf::Texture t;
	sf::Sprite s;
	int max;
	sf::RectangleShape bar;

	LifeBar()
	{
		image.loadFromFile("Images/Textures/life.png");
		image.createMaskFromColor(sf::Color(50, 96, 166));
		t.loadFromImage(image);
		s.setTexture(t);
		s.setTextureRect(sf::IntRect(783, 2, 15, 84));

		bar.setFillColor(sf::Color(0, 0, 0));			//черный прямоугольник накладывается сверху и появляется эффект отсутствия здоровья
		max = 100;
	}

	void update(int k)		// k-текущее здоровье
	{
		if (k>0)
			if (k<max)
				bar.setSize(sf::Vector2f(10, (max - k) * 70 / max));			//если не отрицательно и при этом меньше максимума, то устанавливаем новое значение (новый размер) для черного прямоугольника
	}

	void draw(sf::RenderWindow &window)
	{
		sf::Vector2f center = window.getView().getCenter();
		sf::Vector2f size = window.getView().getSize();

		s.setPosition(center.x - size.x / 2 + 10, center.y - size.y / 2 + 10);			//позиция на экране
		bar.setPosition(center.x - size.x / 2 + 14, center.y - size.y / 2 + 14);

		window.draw(s);			//сначала рисуем полоску здоровья
		window.draw(bar);			//поверх неё уже черный прямоугольник, он как бы покрывает её
	}

};