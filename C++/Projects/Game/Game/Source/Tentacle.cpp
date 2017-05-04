#include "../Include/Tentacle.hpp"


Tentacle::Tentacle(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)
, mAppearing(0.f)
, mDisappearing(0.f)
{
	mTexture = textures.get(Textures::Tentacle);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, mWidth, mHeight));
	mSprite.setScale(0.5f, 0.5f);
	dx = 0.f;
}

void Tentacle::checkCollisionWithMap(float Dx, float Dy)
{
	for (size_t i = 0; i < mLevelObjects.size(); i++)
		// ��������� ����������� � ��������
		if (getRect().intersects(mLevelObjects[i].rect))
		{
			// ���� ��������� ������
			if (mLevelObjects[i].name == "death")
			{
				mHitpoints = 0;
			}
		}
}

void Tentacle::appear(float time)
{
	mAppearing += 0.0075f * time;
	if (mAppearing >= 13.f)
	{
		mAppearing = 0.f;
		isEnabled = false;
		return;
	}
	mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
	mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mAppearing) < 5 ? static_cast<int>(mAppearing) : static_cast<int>(mAppearing) - 5),
		90 * (static_cast<int>(mAppearing) < 5 ? 0 : 1), 25, 90));

}

void Tentacle::disappear(float time)
{
	mDisappearing += 0.0075f * time;
	if (mDisappearing >= 13.f)
	{
		mDisappearing = 0.f;
		isDisabled = false;
		return;
	}

	mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
	mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mDisappearing) < 8 ? 7 - static_cast<int>(mDisappearing): 12 - static_cast<int>(mDisappearing)),
		90 * (static_cast<int>(mDisappearing) < 8 ? 1 : 0), 25, 90));
}

void Tentacle::update(float time)
{
	if (!isStarted)
	{
		mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
		mSprite.setTextureRect(sf::IntRect(0, 0, 25, 90));
		mCurrentDeath = 0.f;
		return;
	}
	else if (!isEnd)
	{
		mCurrentDeath += 0.0075f * time;
		if (mCurrentDeath > 12.f)
		{
			mCurrentDeath = 12.f;
			isEnd = true;
			isEnabling = true;
		}
		mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
		mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mCurrentDeath) < 5 ? static_cast<int>(mCurrentDeath) : static_cast<int>(mCurrentDeath) - 5),
			90 * (static_cast<int>(mCurrentDeath) < 5 ? 0 : 1), 25, 90));
		return;
	}

	if (life && (mHitpoints > 0))
	{
		if (isEnabled)
		{
			appear(time);
		}

		if (isDisabled)
		{
			disappear(time);
		}

		if (!isEnabled && !isDisabled && isEnabling)
		{
			mCurrentFrame += 0.0075f * time;
			if (mCurrentFrame > 8.f)
			{
				mCurrentFrame -= 8.f;
			}
			checkCollisionWithMap(dx, 0.f);

			if (isAttacked)
			{
				mCurrentAttack += 0.005f * time;
				if (mCurrentAttack > 2.f)
				{
					mCurrentAttack -= 1.f;
					isAttacked = false;
					isHittedOnce = false;
				}
				else if (static_cast<int>(mCurrentAttack) == 1 && !isHittedOnce)
				{
					isHitted = true;
					isHittedOnce = true;
				}
				else
				{
					isHitted = false;
				}
			}
			else
			{
				mCurrentAttack = 0.f;
			}
			mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
			mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mCurrentFrame) % 8), 90 * 2, 25, 90));
			mCurrentDeath = 0.f;
		}
	}

	if (mHitpoints <= 0)
	{
		mCurrentDeath += 0.0075f * time;
		dx = 0.f;
		dy = 0.f;
		isTurned = false;
		if (mCurrentDeath >= 12.f)
		{
			mCurrentDeath = 12.f;
			life = false;
		}
		mSprite.setPosition(x + (mWidth / 2.f) - 2.f, y + (mHeight / 2.f) - 11.f);
		mSprite.setTextureRect(sf::IntRect(25 * (static_cast<int>(mCurrentDeath) < 8 ? 7 - static_cast<int>(mCurrentDeath) : 12 - static_cast<int>(mCurrentDeath)),
			90 * (static_cast<int>(mCurrentDeath) < 8 ? 1 : 0), 25, 90));
	}
}
