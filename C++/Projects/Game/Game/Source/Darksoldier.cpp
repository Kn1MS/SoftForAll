#include "../Include/DarkSoldier.hpp"


DarkSoldier::DarkSoldier(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)
{
	mTexture = textures.get(Textures::DarkSoldier);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(14, 131, mWidth, mHeight));
	mSprite.setScale(0.5f, 0.5f);
	dx = 0.05f;
}

void DarkSoldier::checkCollisionWithMap(float Dx, float Dy)
{
	for (size_t i = 0; i < mLevelObjects.size(); i++)
		// Проверяем пересечение с объектом
		if (getRect().intersects(mLevelObjects[i].rect))
		{
			if (mLevelObjects[i].name == "enemyBorder")
			{
				if (Dy > 0.f)
				{
					y = mLevelObjects[i].rect.top - mHeight;
					dy = 0.f;
					onGround = true;
				}
				if (Dy < 0.f)
				{
					y = mLevelObjects[i].rect.top + mLevelObjects[i].rect.height;
					dy = 0.f;
				}
				if (Dx > 0.f)
				{
					x = mLevelObjects[i].rect.left - mWidth;
					isTurned = true;
				}
				if (Dx < 0.f)
				{
					x = mLevelObjects[i].rect.left + mLevelObjects[i].rect.width;
					isTurned = true;
				}
			}

			// Если встретили смерть
			if (mLevelObjects[i].name == "death")
			{
				mHitpoints = 0;
			}
		}
}

void DarkSoldier::update(float time)
{
	if (isTurned)
		mMoveTimer += time;
	if (mMoveTimer > 2000.f && isTurned)
	{
		dx = -dx;
		mMoveTimer = 0.f;
		isTurned = false;
	}

	if (life && (mHitpoints > 0))
	{
		if (!isAttacked && !isTurned)
		{
			x += dx * time;
			mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f) + 3.f);
		}
		else if (!isTurned)
		{
			mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f) + 5.f);
		}
		else
		{
			mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f) + 5.f);
		}
		checkCollisionWithMap(dx, 0.f);

		// Притяжение к земле
		dy += 0.0015f * time;
		y += dy * time;
		checkCollisionWithMap(0.f, dy);

		mCurrentFrame += 0.005f * time;
		if (mCurrentFrame > 5.f)
		{
			mCurrentFrame -= 5.f;
		}
		if (isAttacked)
		{
			mCurrentAttack += 0.005f * time;
			if (mCurrentAttack > 4.f)
			{
				mCurrentAttack = 0.f;
				isAttacked = false;
				isHitted = true;
				isBack = false;
			}
			else
			{
				isAttacked = false;
				isHitted = false;
			}
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(0 + 14, 3, 40, 60));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(0 + 14, 3, 40, 60));
			}
		}
		else
		{
			mCurrentAttack = 0.f;
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(64 * (isTurned ? 0 : static_cast<int>(mCurrentFrame)) + 14, (isTurned ? 3 : 131), 40, 60));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(64 * (isTurned ? 0 : static_cast<int>(mCurrentFrame)) + 14, (isTurned ? 3 : 195), 40, 60));
			}
		}
	}

	if (mHitpoints <= 0)
	{
		mCurrentDeath += 0.0035f * time;
		dx = 0.f;
		dy = 0.f;
		if (mCurrentDeath > 5.f)
		{
			mCurrentDeath = 5.f;
			life = false;
		}
		mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f) + 5.f);
		mSprite.setTextureRect(sf::IntRect(0 + 14, 3, 40, 60));
	}
}