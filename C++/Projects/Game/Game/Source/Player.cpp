#include "../Include/Player.hpp"


namespace
{
	const std::vector<EntityData> Table = initializeHeroData();
}

Player::Player(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, PlayerInfo* playerInfo)
: Entity(Id, X, Y, width, height, Table[Id].speed, Table[Id].hitpoints, Table[Id].damage)
, mStayTimer(0.f)
, onPlatform(0.f)
, shootTimer(0.f)
, jumpTimer(0.f)
, mBeforeJump(0.f)
, mAfterJump(0.f)
, mFallingHeight(0.f)
, counter(0)
, dialogNumber(0)
, isShoot(false)
, canShoted(true)
, isJumped(false)
, isLeft(false)
, isRight(false)
, shooted(false)
, isShooting(false)
, doubleJump(false)
, canDoubleJump(false)
, pressJump(false)
, isRichedEnd(false)
, mHasStartedFirstBoss(false)
, mGotKey(false)
, mActivatedGate(false)
, mState(stay)
, mPlayerInfo(playerInfo)
{
	// Инициализируем и получаем все объекты для взаимодействия персонажа с картой
	mLevelObjects = lvl.getAllObjects();
	mTexture = textures.get(Textures::Archer);
	mSprite.setTexture(mTexture);
	mSprite.setTextureRect(sf::IntRect(77, 1104, 100, 110));
	mSprite.setScale(0.33f, 0.33f);
}

void Player::control(float time)
{
	if (!canShoted)
	{
		shootTimer += time;
		if (shootTimer > 750.f)
		{
			shootTimer = 0.f;
			canShoted = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::Fire)) && canShoted)
	{
		isShoot = true;
		canShoted = false;
	}
	else
	{
		shootTimer = 0.f;
	}

	if (canDoubleJump)
	{
		jumpTimer += time;
		if (jumpTimer > 333.f)
		{
			jumpTimer = 0.f;
			doubleJump = true;
			canDoubleJump = false;
		}
	}
	else if (!onGround && !pressJump)
	{
		canDoubleJump = true;
		pressJump = true;
	}
	else
	{
		jumpTimer = 0.f;
	}

	if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveUp)))
	{
		if (onGround)
		{
			//mState = jump;
			dy = -0.35f;

			// Прыжок != на земле
			onGround = false;
			canDoubleJump = true;
			pressJump = true;
		}
		else if (doubleJump)
		{
			//mState = jump;
			dy = -0.5f;

			// Прыжок != на земле
			onGround = false;
			doubleJump = false;
		}
	}


	if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveDown)))
	{
		//mState = down;
		//mSpeed = 0.1;
	}

	if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveLeft)))
	{
		mState = left;
		mSpeed = 0.1f;
		isLeft = true;
		mMoveTimer += time;
	}
	else if (isLeft)
	{
		isLeft = false;
		if (mMoveTimer < 100.f)
		{
			mCurrentFrame = static_cast<int>(mCurrentFrame) + 0.5f;
		}
		mMoveTimer = 0.f;
	}

	if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveRight)))
	{
		mState = right;
		mSpeed = 0.1f;
		isRight = true;
		mMoveTimer += time;
	}
	else if (isRight)
	{
		isRight = false;
		if (mMoveTimer < 100.f)
		{
			mCurrentFrame = static_cast<int>(mCurrentFrame) + 0.5f;
		}
		mMoveTimer = 0.f;
	}

	if (sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveRight)) &&
		sf::Keyboard::isKeyPressed(mPlayerInfo->getAssignedKey(PlayerInfo::MoveLeft)))
	{
		mSpeed = 0.f;
	}
}

void Player::checkCollisionWithMap(float Dx, float Dy)
{
	dialogNumber = 0;

	for (size_t i = 0; i < mLevelObjects.size(); i++)
		// Проверяем пересечение с объектом
		if (getRect().intersects(mLevelObjects[i].rect))
		{
			// Если встретили препятствие
			if (mLevelObjects[i].name == "solid" || mLevelObjects[i].name == "rock")
			{	
				if (Dy > 0.f)
				{
					y = mLevelObjects[i].rect.top - mHeight;
					dy = 0.f;
					onGround = true;
					doubleJump = false;
					canDoubleJump = false;
					pressJump = false;

					if (mBeforeJump != 0.f)
						mAfterJump = y;

					mFallingHeight = mAfterJump - mBeforeJump;

					if (mFallingHeight > 100.f)
						mHitpoints -=  static_cast<int>(0.4f * (mFallingHeight - 100.f));

					mBeforeJump = 0.f;
					mAfterJump = 0.f;
				}
				if (Dy < 0.f)
				{
					y = mLevelObjects[i].rect.top + mLevelObjects[i].rect.height;
					dy = 0.f;
				}
				if (Dx > 0.f)
				{
					x = mLevelObjects[i].rect.left - mWidth;
				}
				if (Dx < 0.f)
				{
					x = mLevelObjects[i].rect.left + mLevelObjects[i].rect.width;
				}
			}

			// Если встретили смерть
			if (mLevelObjects[i].name == "death")
			{	
				mHitpoints = 0;
			}

			// Если встретили диалог
			if (mLevelObjects[i].name == "dialogMessage")
			{
				dialogNumber = std::stoi(mLevelObjects[i].type);
				if (std::stoi(mLevelObjects[i].type) == 3 && mGotKey)
				{
					mActivatedGate = true;
					dialogNumber = 5;
				}
			}

			// Если встретили конец уровня
			if (mLevelObjects[i].name == "end")
			{
				isRichedEnd = true;
			}

			// Если встретили конец уровня
			if (mLevelObjects[i].name == "boss")
			{
				switch (std::stoi(mLevelObjects[i].type))
				{
					case 1:
						mHasStartedFirstBoss = true;
						break;
					case 2:
						break;
					case 3:
						break;
					default:
						mHasStartedFirstBoss = false;
						break;
				}
			}
		}
		// Если мы не пересекаемся не с какими прямоугольниками и dy > 0, то onGround = false 
		else if (dy > 0.f)
		{
			onGround = false;

			if (mBeforeJump == 0.f)
				mBeforeJump = y;
		}
}

void Player::update(float time)
{
	if (mPlayerInfo->mQuests[1])
		mGotKey = true;

	switch (mType)
	{
		case Type::ID::Archer:
			if (life && (mHitpoints > 0) && !isRichedEnd)
			{
				control(time);
			}

			// В зависимости от направления
			switch (mState)
			{
				case left:
					dx = -mSpeed;
					break;
				case right:
					dx = mSpeed;
					break;
				case up:

					break;
				case down:
					dx = 0.f;
					break;
				case jump:

					break;
				case stay:

					break;
				default:
					mState = stay;
					break;
			}

			if (onPlatform != 0.f)
			{
				if (dx > 0.f)
					x += dx * time + (onPlatform > 0.f ? 0.f : -onPlatform) * time;
				else if (dx < 0.f)
					x += dx * time + (onPlatform > 0.f ? -onPlatform : 0.f) * time;
			}
			else if (!shooted)
			{
				x += dx * time;
			}

			// Обрабатываем столкновения по x
			checkCollisionWithMap(dx, 0.f);
			y += dy * time;
		
			// Обрабатываем столкновения по y
			checkCollisionWithMap(0.f, dy);

			// Обнуляем скорость, чтобы объект остановился
			if (!isMove)
				mSpeed = 0.f;
			mSprite.setPosition(x + (mWidth / 2.f) - 13.f, y + (mHeight / 2.f) - 10.f);

			if (mSpeed > 0.f)
				mSpeed -= 0.0085f;
			else if (mSpeed < 0.f)
				mSpeed = 0.f;

			if (mBeforeJump == 0.f && dy > 0.f && !onGround)
				mBeforeJump = y;

			// Притяжение к земле
			dy += 0.0015f * time;


			if ((dx < 0.05f) && (dx > -0.05f))
			{
				dx = 0.f;
			}

			if (life && (mHitpoints > 0))
			{
				if (onGround)
					mCurrentFrame += 0.005f * time;

				if (mCurrentFrame > 4.f)
					mCurrentFrame -= 4.f;

				if (shooted)
				{
					mCurrentAttack += 0.0035f * time;
					if (mCurrentAttack > 2.f)
					{
						mCurrentAttack = 0.f;
						shooted = false;
						return;
					}
					else if ((mCurrentAttack > 1.7f) && (mCurrentAttack < 1.76f))
					{
						isShooting = true;
					}

					if (dx > 0.f)
					{
						mSprite.setTextureRect(sf::IntRect(256 * (static_cast<int>(mCurrentAttack) + 4) + 77, 1104, 100, 110));
					}
					else if (dx < 0.f)
					{
						mSprite.setTextureRect(sf::IntRect(256 * (static_cast<int>(mCurrentAttack) + 4) + 77, 77, 100, 110));
					}
					else if (dx == 0.f)
					{
						mSprite.setTextureRect(sf::IntRect(256 * (static_cast<int>(mCurrentAttack) + 4) + 77, (((mState == right) || (mState == stay)) ? 1104 : 77), 100, 110));
						mCurrentFrame = 0.f;
					}
				}
				else
				{
					if (!onGround)
					{
						isJumped = true;
						if (dx > 0.f)
						{
							mSprite.setTextureRect(sf::IntRect(256 * 3 + 77, 1104, 100, 110));
						}
						else if (dx == 0.f)
						{
							mSprite.setTextureRect(sf::IntRect(256 * 3 + 77, (((mState == right) || (mState == stay)) ? 1104 : 77), 100, 110));
						}
						else
						{
							mSprite.setTextureRect(sf::IntRect(256 * 3 + 77, 77, 100, 110));
						}
					}
					else if (dx > 0.f)
					{
						mSprite.setTextureRect(sf::IntRect(256 * static_cast<int>(mCurrentFrame) + 77, 1104, 100, 110));
					}
					else if (dx < 0.f)
					{
						mSprite.setTextureRect(sf::IntRect(256 * static_cast<int>(mCurrentFrame) + 77, 77, 100, 110));
					}
					else if ((dx == 0.f) && isJumped)
					{
						mSprite.setTextureRect(sf::IntRect(256 * 0 + 77, (((mState == right) || (mState == stay)) ? 1104 : 77), 100, 110));
						isJumped = false;
						mCurrentFrame = 0.f;
					}
					if (dx == 0.f)
					{
						mStayTimer += time;
						if (mStayTimer > 150.f)
						{
							isJumped = true;
							mStayTimer = 0.f;
						}
					}
					else
					{
						mStayTimer = 0.f;
					}
				}
			}

			if (mHitpoints <= 0)
			{
				mCurrentDeath += 0.003f * time;
				dx = 0.f;
				if (y > 2350.f)
					dy = 0.f;
				if (mCurrentDeath > 2.f)
				{
					mCurrentDeath = 2.f;
					mMoveTimer += time;
					if (mMoveTimer > 1000.f)
					{
						mMoveTimer = 0.f;
						counter++;
					}
					if (counter == 2)
						life = false;
				}
				mSprite.setPosition(x + (mWidth / 2.f) - 14.f, y + (mHeight / 2.f) - 10.f);
				if (static_cast<int>(mCurrentDeath) == 0)
				{
					mSprite.setTextureRect(sf::IntRect(256 * 0 + 77, (mState == left ? 77 : 1104), 100, 110));
				}
				else if (static_cast<int>(mCurrentDeath) == 1)
				{
					mSprite.setTextureRect(sf::IntRect(256 * 6 + 77, (mState == left ? 77 : 1104), 100, 110));
				}
				else if (static_cast<int>(mCurrentDeath) == 2)
				{
					mSprite.setTextureRect(sf::IntRect(256 * 7 + 77, (mState == left ? 1869 : 848), 120, 110)); //77 - 1104
				}
			}

			break;

		default:
			std::cout << "Invalid hero type!" << std::endl;
			break;
	}
}