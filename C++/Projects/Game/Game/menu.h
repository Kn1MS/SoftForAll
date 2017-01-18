///файл, отвечающий за главное меню

void menu(sf::RenderWindow & window) 
{
	window.setView(window.getDefaultView());
	sf::Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("Images/Menu/111.png");
	menuTexture2.loadFromFile("Images/Menu/222.png");
	menuTexture3.loadFromFile("Images/Menu/333.png");
	aboutTexture.loadFromFile("Images/Menu/about.png");
	menuBackground.loadFromFile("Images/Menu/Penguins.jpg");
	sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = true;
	int menuNum = 0;
	menu1.setPosition(150, 30);
	menu2.setPosition(150, 90);
	menu3.setPosition(150, 150);
	menuBg.setPosition(150, 0);

	//само меню
	while (isMenu)
	{
		menu1.setColor(sf::Color::White);
		menu2.setColor(sf::Color::White);
		menu3.setColor(sf::Color::White);
		menuNum = 0;
		window.clear(sf::Color(129, 181, 221));

		if (sf::IntRect(100, 30, 300, 50).contains(sf::Mouse::getPosition(window))) { 
			menu1.setColor(sf::Color::Blue); 
			menuNum = 1; 
		}
		if (sf::IntRect(100, 90, 300, 50).contains(sf::Mouse::getPosition(window))) { 
			menu2.setColor(sf::Color::Blue);
			menuNum = 2; 
		}
		if (sf::IntRect(100, 150, 300, 50).contains(sf::Mouse::getPosition(window))) {
			menu3.setColor(sf::Color::Blue); 
			menuNum = 3; 
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (menuNum == 1) 
				isMenu = false;			//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) { 
				window.draw(about); 
				window.display(); 
				while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)); 
			}
			if (menuNum == 3) { 
				window.close(); 
				isMenu = false; 
			}
		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);

		window.display();
	}
}
