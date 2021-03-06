#include "../Include/GameOverState.hpp"
#include "../Include/Utility.hpp"
#include "../Include/Player.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


GameOverState::GameOverState(StateStack& stack, Context context)
: State(stack, context)
, opacity(0)
, mGameOverText()
, mElapsedTime(sf::Time::Zero)
, mPlayerInfo(context.playerInfo)
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mGameOverText.setFont(font);
	if (context.playerInfo->getGameStatus() == PlayerInfo::GameOver)
		mGameOverText.setString(L"���� ��������!");
	else
		mGameOverText.setString(L"������� �������!");

	mGameOverText.setCharacterSize(70);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	if (opacity < 255)
		opacity += 5;

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, opacity));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
}

bool GameOverState::update(sf::Time dt)
{
	// Show state for 5 seconds, after return to menu
	mElapsedTime += dt;
	if (mElapsedTime > sf::seconds(5))
	{
		mPlayerInfo->mShowedDialogs.clear();
		requestStateClear();
		requestStackPush(States::Menu);
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
	return false;
}
