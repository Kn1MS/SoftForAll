#include "../Include/Lifebar.hpp"


LifeBar::LifeBar(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, int hitpoints)
: mTexture()
, mTextureEmpty()
, max(hitpoints)
, mType(Id)
, mBossName("", fonts.get(Fonts::Main))
{
	if (Id == Type::BossBar)
	{
		mBossName.setString(L"Страж крепости");
		mTexture = textures.get(Textures::BossBar);
		mSprite.setTexture(mTexture);
		mSpriteEmpty.setTexture(mTexture);
		mSprite.setTextureRect(sf::IntRect(0, 0, 334, 24));
		mSpriteEmpty.setTextureRect(sf::IntRect(0, 24, 334, 24));
		mBossName.setScale(0.5f, 0.5f);
		mSpriteEmpty.setScale(0.5f, 0.5f);
		mSprite.setScale(0.5f, 0.5f);
	}
	else
	{
		mTexture = textures.get(Textures::HealthBar);
		mTextureEmpty = textures.get(Textures::EmptyBar);
		mSprite.setTexture(mTexture);
		mSpriteEmpty.setTexture(mTextureEmpty);
		mSpriteEmpty.setScale(0.5f, 0.5f);
		mSprite.setScale(0.5f, 0.5f);
	}
}

void LifeBar::update(int points)
{
	if (points < 0)
		points = 0;
	if (points < max)
	{
		if (mType == Type::BossBar)
			mSprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(334 * (static_cast<float>(points) / static_cast<float>(max))), 24));
		else
			mSprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(206 * (static_cast<float>(points) / static_cast<float>(max))), 28));
	}
}

void LifeBar::draw(sf::RenderWindow& window)
{
	sf::Vector2f center = window.getView().getCenter();
	sf::Vector2f size = window.getView().getSize();

	if (mType == Type::BossBar)
	{
		mBossName.setPosition(center.x - 50.f, center.y + size.y / 2.f - 60.f);
		mSprite.setPosition(center.x - 70.f, center.y + size.y / 2.f - 40.f);
		mSpriteEmpty.setPosition(center.x - 70.f, center.y + size.y / 2.f - 40.f);
	}
	else
	{
		mSprite.setPosition(center.x - size.x / 2.f + 10.f, center.y - size.y / 2.f + 20.f);
		mSpriteEmpty.setPosition(center.x - size.x / 2.f + 10.f, center.y - size.y / 2.f + 20.f);
	}

	if (mType == Type::BossBar)
		window.draw(mBossName);

	window.draw(mSpriteEmpty);
	window.draw(mSprite);
}