#include "../Include/Goblin.hpp"


Goblin::Goblin(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)
{
	mTexture = textures.get(Textures::Goblin);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(10, 67, mWidth, mHeight));
	mSprite.setScale(0.5f, 0.5f);
	dx = 0.07f;
}

void Goblin::checkCollisionWithMap(float Dx, float Dy)
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

void Goblin::update(float time)
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
	}

	if (life && (mHitpoints > 0))
	{
		if (!isAttacked && !isTurned)
			x += dx * time;
		checkCollisionWithMap(dx, 0.f);

		mCurrentFrame += 0.005f * time;
		if (mCurrentFrame > 6.f)
		{
			mCurrentFrame -= 6.f;
		}

		mSprite.setPosition(x + (mWidth / 2.f) + (static_cast<int>(mCurrentFrame) == 0 ? -5.f : -6.f), y + (mHeight / 2.f) - 5.f);

		if (isAttacked)
		{
			int temp;
			mCurrentAttack += 0.0075f * time;
			if (mCurrentAttack > 7.f)
			{
				mCurrentAttack -= 7.f;
				isAttacked = false;
				isBack = false;
				isHittedOnce = false;
			}
			else if (static_cast<int>(mCurrentAttack) == 3 && !isHittedOnce)
			{
				isHitted = true;
				isHittedOnce = true;
			}
			else
			{
				isHitted = false;
			}
			switch (static_cast<int>(mCurrentAttack))
			{
				case 0:
					temp = 7;
					break;
				case 1:
					temp = 10;
					break;
				case 2:
					temp = 9;
					break;
				case 3:
					temp = 8;
					break;
				case 4:
					temp = 9;
					break;
				case 5:
					temp = 10;
					break;
				case 6:
					temp = 7;
					break;
				default:
					temp = 0;
					break;
			}
			mSprite.setPosition(x + (mWidth / 2.f) + (static_cast<int>(mCurrentFrame) == 0 ? -5.f : -6.f) + (dx > 0 ? - 3.f : 0.f), y + (mHeight / 2.f) - 5.f);
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(64 * temp + 10, 67, 60, 56));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(64 * temp + 10, 195, 60, 56));
			}
		}
		else
		{
			mCurrentAttack = 0.f;
			if (isTurned)
			{
				mCurrentFrame = 7.f;
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 10, (dx > 0 ? 195 : 67), 50, 56));
			}
			if (dx > 0.f && !isTurned)
			{
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 10, 67, 50, 56));
			}
			else if (dx < 0.f && !isTurned)
			{
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 10, 195, 50, 56));
			}
		}
	}

	if (mHitpoints <= 0)
	{
		mMoveTimer += time;
		if (mMoveTimer > 1000.f)
		{
			counter++;
			mMoveTimer = 0.f;
		}
		mCurrentDeath += 0.0035f * time;
		dx = 0.f;
		dy = 0.f;
		if (mCurrentDeath > 2.f)
		{
			mCurrentDeath = 2.f;
			if (counter == 6)
				life = false;
		}
		mSprite.setPosition(x + (mWidth / 2.f) - 5.f, y + (mHeight / 2.f) - 5.f);
		mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath) + 10, 259, 50, 56));
	}
}