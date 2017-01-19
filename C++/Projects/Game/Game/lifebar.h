///����, ���������� �� ������� ����� ������

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

		bar.setFillColor(sf::Color(0, 0, 0));			//������ ������������� ������������� ������ � ���������� ������ ���������� ��������
		max = 100;
	}

	void update(int k)		// k-������� ��������
	{
		if (k>0)
			if (k<max)
				bar.setSize(sf::Vector2f(10, (max - k) * 70 / max));			//���� �� ������������ � ��� ���� ������ ���������, �� ������������� ����� �������� (����� ������) ��� ������� ��������������
	}

	void draw(sf::RenderWindow &window)
	{
		sf::Vector2f center = window.getView().getCenter();
		sf::Vector2f size = window.getView().getSize();

		s.setPosition(center.x - size.x / 2 + 10, center.y - size.y / 2 + 10);			//������� �� ������
		bar.setPosition(center.x - size.x / 2 + 14, center.y - size.y / 2 + 14);

		window.draw(s);			//������� ������ ������� ��������
		window.draw(bar);			//������ �� ��� ������ �������������, �� ��� �� ��������� �
	}

};