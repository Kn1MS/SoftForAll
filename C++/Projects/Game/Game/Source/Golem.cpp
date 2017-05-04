#include "../Include/Golem.hpp"


Golem::Golem(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)
{
	mTexture = textures.get(Textures::Golem);
	mTextureAttack = textures.get(Textures::GolemAttack);
	mTextureDeath = textures.get(Textures::GolemDeath);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 192, 64, 64));
	mSprite.setScale(0.6f, 0.6f);
	dx = 0.05f;
	isEnd = false;
}

void Golem::checkCollisionWithMap(float Dx, float Dy)
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
					isTurned = true;;
				}
			}

			// Если встретили смерть
			if (mLevelObjects[i].name == "death")
			{
				mHitpoints = 0;
			}
		}
}

void Golem::update(float time)
{
	// Притяжение к земле
	dy += 0.0015f * time;
	y += dy * time;
	checkCollisionWithMap(0.f, dy);

	if (!isStarted)
	{
		mSprite.setTexture(mTextureDeath);
		mSprite.setPosition(x + (mWidth / 2.f) - 8.f, y + (mHeight / 2.f) - 4.f);
		mSprite.setTextureRect(sf::IntRect(64 * 5, 64, 64, 64));
		mCurrentDeath = 5.f;
		return;
	}
	else if (!isEnd)
	{
		mCurrentDeath -= 0.005f * time;
		if (mCurrentDeath < 0.f)
		{
			mCurrentDeath = 0.f;
			isEnd = true;
		}
		mSprite.setTexture(mTextureDeath);
		mSprite.setPosition(x + (mWidth / 2.f) - 8.f, y + (mHeight / 2.f) - 4.f);
		mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath), 64, 64, 64));
		return;
	}

	mMoveTimer += time;
	if (mMoveTimer > 2000.f && isTurned)
	{
		dx = -dx;
		mMoveTimer = 0.f;
		isTurned = false;
	}
	else if (mMoveTimer > 1000.f && !isTurned)
	{
		mMoveTimer = 0.f;
		if (mHitpoints <= 0)
			counter++;
	}

	if (life && (mHitpoints > 0))
	{
		if (!isAttacked && !isTurned)
			x += dx * time;
		checkCollisionWithMap(dx, 0.f);

		mSprite.setPosition(x + (mWidth / 2.f) - 0.f, y + (mHeight / 2.f) - 7.f);


		mCurrentFrame += 0.005f * time;
		if (mCurrentFrame > 7.f)
		{
			mCurrentFrame -= 7.f;
		}
		if (isAttacked)
		{
			mCurrentAttack += 0.005f * time;
			if (mCurrentAttack > 7.f)
			{
				mCurrentAttack -= 7.f;
				isAttacked = false;
				isBack = false;
				isHittedOnce = false;
			}
			else if (static_cast<int>(mCurrentAttack) == 4 && !isHittedOnce)
			{
				isHitted = true;
				isHittedOnce = true;
			}
			else {
				isHitted = false;
			}
			mSprite.setTexture(mTextureAttack);
			mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 1.f : -12.f), y + (mHeight / 2.f) - 12.f);
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentAttack), 311, 64, 72));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentAttack), 119, 64, 72));
			}
		}
		else
		{
			mSprite.setTexture(mTexture);
			mCurrentAttack = 0.f;
			if (isTurned)
			{
				mCurrentFrame = 0.f;
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + (dx < 0 ? 0 : 17), (dx < 0 ? 192 : 64), 48, 64));
			}
			if (dx > 0.f && !isTurned)
			{
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame), 192, 48, 64));
			}
			else if (dx < 0.f && !isTurned)
			{
				mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 17, 64, 48, 64));
			}
		}
	}

	if (mHitpoints <= 0)
	{
		mCurrentDeath += 0.0035f * time;
		dx = 0.f;
		dy = 0.f;
		if (mCurrentDeath > 6.f)
		{
			mCurrentDeath = 6.f;
			if (counter == 6)
				life = false;
		}
		mSprite.setTexture(mTextureDeath);
		mSprite.setPosition(x + (mWidth / 2.f) - 6.f, y + (mHeight / 2.f) - 3.f);
		mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath), 0, 64, 64));
	}
}