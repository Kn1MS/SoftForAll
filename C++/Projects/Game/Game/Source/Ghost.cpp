#include "../Include/Ghost.hpp"


Ghost::Ghost(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)

{
	mTexture = textures.get(Textures::Ghost);
	mTextureDeath = textures.get(Textures::GhostDeath);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 0, mWidth, mHeight));
	mSprite.scale(-1.f, 1.f);
	mSprite.setScale(0.5f, 0.5f);
	dx = 0.1f;
}

void Ghost::checkCollisionWithMap(float Dx, float Dy)
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

void Ghost::update(float time)
{
	// Притяжение к земле
	dy += 0.0015f * time;
	y += dy * time;
	checkCollisionWithMap(0.f, dy);

	if (isTurned)
		mMoveTimer += time;
	if (mMoveTimer > 2000.f && isTurned)
	{
		dx = -dx;
		mMoveTimer = 0.f;
		isTurned = false;
		mSprite.scale(-1.f, 1.f);
	}

	if (life && (mHitpoints > 0))
	{
		if (!isAttacked && !isTurned)
		{
			x += dx * time;
			mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 25.f : -25.f), y + (mHeight / 2.f));
		}
		else if (!isTurned)
		{
			mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 25.f : -25.f), y + (mHeight / 2.f));
		}
		else
		{
			mSprite.setPosition(x + (mWidth / 2.f) + (dx < 0 ? 25.f : -25.f), y + (mHeight / 2.f));
		}
		checkCollisionWithMap(dx, 0.f);

		mCurrentFrame += 0.005f * time;
		if (mCurrentFrame > 3.f)
		{
			mCurrentFrame -= 3.f;
		}
		if (isAttacked)
		{
			mCurrentAttack += 0.005f * time;
			if (mCurrentAttack > 10.f)
			{
				mCurrentAttack -= 10.f;
				isAttacked = false;
				isBack = false;
				isHittedOnce = false;
			}
			else if (static_cast<int>(mCurrentAttack) == 8 && !isHittedOnce)
			{
				isHitted = true;
				isHittedOnce = true;
			}
			else
			{
				isHitted = false;
				isBack = true;
			}
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(102 * static_cast<int>(mCurrentAttack), 0, 102, 90));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(102 * static_cast<int>(mCurrentAttack), 0, 102, 90));
			}
		}
		else
		{
			mCurrentAttack = 0.f;
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(102 * static_cast<int>(mCurrentFrame), 0, 102, 90));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(102 * static_cast<int>(mCurrentFrame), 0, 102, 90));
			}
		}
	}

	if (mHitpoints <= 0)
	{
		mCurrentDeath += 0.0035f * time;
		dx = 0.f;
		dy = 0.f;
		isTurned = false;
		if (mCurrentDeath > 6.f)
		{
			mCurrentDeath = 6.f;
			life = false;
		}
		mSprite.setTexture(mTextureDeath);
		mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f) + 35.f);
		mSprite.setTextureRect(sf::IntRect(64 * (static_cast<int>(mCurrentDeath) < 3 ? static_cast<int>(mCurrentDeath) : static_cast<int>(mCurrentDeath) + 5), 0, 64, 50));
	}
}