#include "../Include/MinotaurMage.hpp"


MinotaurMage::MinotaurMage(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type)
: Enemy(Id, textures, fonts, lvl, X, Y, width, height, Type)
{
	mTexture = textures.get(Textures::MinotaurMage);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(8, 4, mWidth, mHeight));
	mSprite.setScale(0.5f, 0.5f);
	dx = 0.075f;
}

void MinotaurMage::checkCollisionWithMap(float Dx, float Dy)
{
	for (size_t i = 0; i < mLevelObjects.size(); i++)
		// ��������� ����������� � ��������
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

			// ���� ��������� ������
			if (mLevelObjects[i].name == "death")
			{
				mHitpoints = 0;
			}
		}
}

void MinotaurMage::update(float time)
{
	// ���������� � �����
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
		//if (!isAttacked && !isTurned)
		//	x += dx * time;
		checkCollisionWithMap(dx, 0.f);

		/// Animation
		mCurrentFrame += 0.003f * time;
		if (mCurrentFrame > 2.f)
		{
			mCurrentFrame -= 2.f;
		}
		mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f) + 7.f);
		if (isAttacked)
		{
			mCurrentAttack += 0.005f * time;
			if (mCurrentAttack > 5.f)
			{
				mCurrentAttack -= 5.f;
				isAttacked = false;
				isBack = false;
			}
			else if (static_cast<int>(mCurrentAttack) == 4)
			{
				isHitted = true;
			}
			else
			{
				isHitted = false;
			}
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(120 * static_cast<int>(mCurrentAttack) + 8, 4, 80, 90));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(120 * static_cast<int>(mCurrentAttack) + 8, 4, 80, 90));
			}
		}
		else
		{
			mCurrentAttack = 0.f;
			if (dx > 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(120 * static_cast<int>(mCurrentFrame) + 8, 4, 80, 90));
			}
			else if (dx < 0.f)
			{
				mSprite.setTextureRect(sf::IntRect(120 * static_cast<int>(mCurrentFrame) + 8, 4, 80, 90));
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
		mCurrentDeath += 0.005f * time;
		dx = 0.f;
		dy = 0.f;
		if (mCurrentDeath > 11.f)
		{
			mCurrentDeath = 11.f;
			if (counter == 8)
				life = false;
		}
		mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f) + 6.f);
		mSprite.setTextureRect(sf::IntRect(120 * (static_cast<int>(mCurrentDeath) % 5) + 8, 100 * ((static_cast<int>(mCurrentDeath) / 5) + 1) + 4, 105, 90));
	}
}