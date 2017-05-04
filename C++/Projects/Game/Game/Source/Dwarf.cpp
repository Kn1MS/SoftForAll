#include "../Include/Dwarf.hpp"


Dwarf::Dwarf(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type, int dwarfType)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)
, mDwarfType(dwarfType)
, inaccuracy(0.f)
{
	mTexture = textures.get(Textures::Dwarf);
	mSprite.setTexture(mTexture);
	mSprite.setScale(0.5f, 0.5f);
	mSprite.setTextureRect(sf::IntRect(0, 80 * mDwarfType, mWidth, mHeight));
	dx = 0.075f;
	mSprite.scale(-1.f, 1.f);
	if (Id == Type::DwarvenCommander)
		isSpawn = false;
	else
		isSpawn = true;
}

void Dwarf::checkCollisionWithMap(float Dx, float Dy)
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

void Dwarf::update(float time)
{
	// Притяжение к земле
	dy += 0.0015f * time;
	y += dy * time;
	checkCollisionWithMap(0.f, dy);

	if (!isStarted)
	{
		mCurrentDeath += 0.005f * time;
		if (mCurrentDeath > 4.f)
		{
			mCurrentDeath -= 4.f;
		}
		mSprite.setPosition(x + (mWidth / 2.f) + 16.f, y + (mHeight / 2.f) - 12.f);
		mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentDeath) + 16), 80 * mDwarfType, 100, 80));
		mCurrentAttack = 3.f;
		return;
	}
	else if (!isEnd)
	{
		mCurrentAttack -= 0.0025f * time;
		if (mCurrentAttack < 0.f)
		{
			mCurrentAttack = 0.f;
			mCurrentDeath = 0.f;
			isEnd = true;
		}
		int temp;
		if (static_cast<int>(mCurrentAttack) == 3)
		{
			temp = 0;
		}
		else if (static_cast<int>(mCurrentAttack) == 2)
		{
			temp = 14;
		}
		else
		{
			temp = 13;
		}
		mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 12.f);
		mSprite.setTextureRect(sf::IntRect(100 * temp, 80 * mDwarfType, 100, 80));
		return;
	}

	if (isTurned)
		mMoveTimer += time;
	if (mMoveTimer > 2000.f && isTurned)
	{
		dx = -dx;
		mMoveTimer = 0;
		isTurned = false;
		mSprite.scale(-1.f, 1.f);
	}

	if (life && (mHitpoints > 0))
	{
		if (!isAttacked && !isTurned)
			x += dx * time;
		checkCollisionWithMap(dx, 0.f);

		mSprite.setPosition(x + (mWidth / 2) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2) - 12.f);

		mCurrentFrame += 0.005f * time;
		if (mCurrentFrame > 6.f)
		{
			mCurrentFrame -= 6.f;
		}
		if (isAttacked)
		{
			mCurrentAttack += 0.005f * time;
			if (mCurrentAttack > 4.f)
			{
				mCurrentAttack -= 4.f;
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

			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentAttack) + 10), 80 * mDwarfType, 100, 80));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentAttack) + 10), 80 * mDwarfType, 100, 80));
			}
		}
		else
		{
			mCurrentAttack = 0.f;
			if (isTurned)
			{
				mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentFrame) % 4), 80 * mDwarfType, 100, 80));
			}
			if (dx > 0.f && !isTurned)
			{
				mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentFrame) + 4), 80 * mDwarfType, 100, 80));
			}
			else if (dx < 0.f && !isTurned)
			{
				mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentFrame) + 4), 80 * mDwarfType, 100, 80));
			}
		}
		inaccuracy = dx;
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
		mSprite.setPosition(x + (mWidth / 2.f) + (inaccuracy > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 12.f);
		mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentDeath) + (static_cast<int>(mCurrentDeath) == 0? 0 : 13)), 80 * mDwarfType, 100, 80));
	}
}