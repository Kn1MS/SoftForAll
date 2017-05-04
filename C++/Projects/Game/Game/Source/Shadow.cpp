#include "../Include/Shadow.hpp"


namespace
{
	const ShadowData STable = initializeShadowData();
}

Shadow::Shadow(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, std::string Type)
: Entity(Id, X, Y, width, height, STable.speed, STable.hitpoints, STable.damage)
, counter(0)
, numTelPoint(0)
, mAppearing(0.f)
, mDisappearing(0.f)
, mStayTimer(0.f)
, mDelayTimer(0.f)
, inaccuracy(0.f)
, isTeleported(false)
, isTeleporting(false)
, isDisappeared(false)
, isDisappearing(false)
, isStay(false)
, isNeedStay(false)
, isDelay(false)
, isNeedDelay(false)
, isCalling(false)
, isWithdrawing(false)
, teleporPoints(STable.teleportPoints)
{
	mTexture = textures.get(Textures::Shadow);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(0, 280, mWidth, mHeight));
	dx = 0.03f;
	mSprite.setScale(0.5f, 0.5f);
	mSprite.scale(-1.f, 1.f);
}

void Shadow::appear(float time)
{
	mAppearing += 0.0075f * time;
	if (mAppearing >= 14.f)
	{
		mAppearing = 0.f;
		isTeleporting = false;
		isTeleported = true;
		isHittedOnce = false;
		return;
	}
	mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
	mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(mAppearing) % 4), 70 * (static_cast<int>(mAppearing) / 4), 80, 70));
}

void Shadow::disappear(float time)
{
	mDisappearing += 0.0075f * time;
	if (mDisappearing >= 14.f)
	{
		mDisappearing = 0.f;
		isDisappeared = true;
		isDisappearing = false;
		return;
	}
	mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
	mSprite.setTextureRect(sf::IntRect(80 * ((13 - static_cast<int>(mDisappearing)) % 4), 70 * ((13 - static_cast<int>(mDisappearing)) / 4), 80, 70));
}

void Shadow::update(float time)
{
	if (!isStarted)
	{
		mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
		mSprite.setTextureRect(sf::IntRect(0, 0, 80, 70));
		mCurrentDeath = 0.f;
		return;
	}
	else if (!isEnd)
	{
		mCurrentDeath += 0.0075f * time;
		if (mCurrentDeath > 13.f)
		{
			mCurrentDeath = 13.f;
			isEnd = true;
			//isDelay = true;
		}
		mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
		mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(mCurrentDeath) % 4), 70 * (static_cast<int>(mCurrentDeath) / 4), 80, 70));
		return;
	}

	isCalling = false;
	isWithdrawing = false;
	mMoveTimer += time;
	if (mMoveTimer > 1000.f)
	{
		mMoveTimer = 0.f;
		if (mHitpoints <= 0)
			counter++;
	}

	if (life && (mHitpoints > 0))
	{

		mCurrentFrame += 0.0075f * time;
		if (mCurrentFrame > 4.f)
		{
			mCurrentFrame -= 4.f;
		}

		if (isAttacked && (numTelPoint <= static_cast<int>(teleporPoints.size())) && !isDelay && !isStay)
		{
			if (!isDisappeared && !isDisappearing)
			{
				isDisappearing = true;
			}

			if (isDisappearing)
			{
				disappear(time);
			}
			else if (!isTeleported && !isTeleporting)
			{
				x = teleporPoints[numTelPoint].x;
				y = teleporPoints[numTelPoint].y;
				switch (numTelPoint)
				{
					case 0:
						if (dx > 0.f)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						isCalling = true;
						isNeedDelay = true;
						break;
					case 1:
						if (dx < 0.f)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						isNeedDelay = true;
						break;
					case 2:
						if (isBack)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						isNeedDelay = true;
						break;
					case 3:
						if (isBack)
						{
							dx = -dx;
							mSprite.scale(-1.f, 1.f);
						}
						isWithdrawing = true;
						isNeedStay = true;
						break;
					default:
						dx = 0.03f;
						break;
				}

				numTelPoint++;
				isTeleporting = true;
			}

			if (isTeleporting)
			{
				appear(time);
			}

			if (isTeleported && !isHittedOnce)
			{
				isHitted = true;
				isHittedOnce = true;
				isAttacked = false;

				isTeleported = false;
				isTeleporting = false;
				
				isDisappeared = false;
				isDisappearing = false;

				if (isNeedDelay)
				{
					isNeedDelay = false;
					isDelay = true;
				}

				if (isNeedStay)
				{
					isNeedStay = false;
					isStay = true;
				}
				mCurrentFrame = 0.f;
			}
			else {
				isHitted = false;
			}
		}
		else
		{
			if (isDelay)
			{
				mDelayTimer += 0.0025f * time;
			}

			if (isStay)
			{
				mStayTimer += 0.0005f * time;
				isHitted = false;
			}

			if (mDelayTimer > 2.f)
			{
				mDelayTimer = 0.f;
				isDelay = false;
			}

			if (mStayTimer > 2.f)
			{
				mStayTimer = 0.f;
				isStay = false;
				numTelPoint = 0;
			}

			mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
			mSprite.setTextureRect(sf::IntRect(80 * static_cast<int>(mCurrentFrame), 280, 80, 70));
		}
		mCurrentDeath = 14.f;
		inaccuracy = dx;
	}

	if (mHitpoints <= 0)
	{
		mCurrentDeath -= 0.0075f * time;
		dx = 0.f;
		dy = 0.f;
		if (mCurrentDeath < 0.f)
		{
			mCurrentDeath = 0.f;
			if (counter == 6)
				life = false;
		}
		mSprite.setPosition(x + (mWidth / 2.f) + (inaccuracy > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 8.f);
		mSprite.setTextureRect(sf::IntRect(80 * (static_cast<int>(mCurrentDeath) % 4), 70 * (static_cast<int>(mCurrentDeath) / 4), 80, 70));
	}
}
