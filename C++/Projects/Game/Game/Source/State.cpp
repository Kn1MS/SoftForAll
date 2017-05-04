#include "../Include/State.hpp"
#include "../Include/StateStack.hpp"


State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, PlayerInfo& playerInfo)
: window(&window)
, textures(&textures)
, fonts(&fonts)
, playerInfo(&playerInfo)
{
}

State::State(StateStack& stack, Context context)
: mStack(&stack)
, mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}