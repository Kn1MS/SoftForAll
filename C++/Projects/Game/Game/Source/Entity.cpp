#include "../Include/Entity.hpp"


Entity::Entity(Type::ID Id, float X, float Y, int width, int height, float speed, int hitpoints, int damage, std::string Type)
: x(X)
, y(Y)
, dx(0.f)
, dy(0.f)
, mSpeed(speed)
, mMoveTimer(0.f)
, mCurrentFrame(0.f)
, mCurrentAttack (0.f)
, mCurrentDeath(0.f)
, mWidth(width)
, mHeight(height)
, mHitpoints(hitpoints)
, mDamage(damage)
, life(true)
, isMove(true)
, onGround(false)
, isAttacked(false)
, isHitted(false)
, isHittedOnce(false)
, isStarted(false)
, isBack(false)
, isSpawn(false)
, isEnd(false)
, isEnabled(false)
, isDisabled(false)
, isEnabling(false)
, type(Type)
, mType(Id)
{
	mSprite.setOrigin(static_cast<float>(mWidth) / 2.f, static_cast<float>(mHeight) / 2.f);
}

Entity::~Entity()
{
}


sf::FloatRect Entity::getRect() const
{
	// ������� ��������� ��������������, ��� ��������� � ��������(������, ������)
	return sf::FloatRect(x, y, static_cast<float>(mWidth), static_cast<float>(mHeight));
}