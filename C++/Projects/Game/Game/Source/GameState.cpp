#include "../Include/GameState.hpp"


GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mWorld(*context.window, *context.textures, *context.fonts, context.playerInfo)
, mPlayerInfo(*context.playerInfo)
{
	mPlayerInfo.setGameStatus(PlayerInfo::GameRunning);
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);

	if(!mWorld.hasAlivePlayer())
	{
		mPlayerInfo.setGameStatus(PlayerInfo::GameOver);
		requestStackPush(States::GameOver);
	}
	else if(mWorld.hasPlayerReachedEnd())
	{
		mPlayerInfo.setGameStatus(PlayerInfo::LevelComplete);
		requestStackPush(States::GameOver);
	}

	mWorld.handleEvent();

	if (mPlayerInfo.mDialogNumber != 0)
	{
		requestStackPush(States::Dialog);
	}

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	mWorld.handleEvent();

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}