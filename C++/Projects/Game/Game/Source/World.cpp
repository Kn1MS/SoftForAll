#include "../Include/World.hpp"
#include "../Include/Projectile.hpp"
#include "../Include/Pickup.hpp"
#include "../Include/Foreach.hpp"
#include "../Include/Ghost.hpp"
#include "../Include/Golem.hpp"
#include "../Include/DarkSoldier.hpp"
#include "../Include/Shadow.hpp"
#include "../Include/Goblin.hpp"
#include "../Include/MinotaurMage.hpp"
#include "../Include/Dwarf.hpp"
#include "../Include/MovingPlatform.hpp"
#include "../Include/Bullet.hpp"
#include "../Include/Utility.hpp"
#include "../Include/Rock.hpp"
#include "../Include/Gate.hpp"
#include "../Include/DialogPerson.hpp"
#include "../Include/Tentacle.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <limits>


World::World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, PlayerInfo* playerInfo)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures(textures)
, mFonts(fonts)
, mLevel(nullptr)
, mWorldBounds(0.f, 0.f, 13072.f, 3200.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mLevelNumber(1)
, mPosition()
, mScrollSpeed(0.f)
, mPlayerHero(nullptr)
, mPlayerInfo(playerInfo)
, mLifeBar(nullptr)
, mShadowBoss()
, mEnemySpawnPoints()
, mEnemies()
, mMusic()
, mSoundBuffer()
, mCasualSoundBuffer()
, mDarkSoundBuffer()
, mSound()
{
	mSoundBuffer.loadFromFile("Media/Sounds/firebol3.ogg");
	mCasualSoundBuffer.loadFromFile("Media/Sounds/music.ogg");
	mDarkSoundBuffer.loadFromFile("Media/Sounds/DarkMusic.ogg");

	mSound.setBuffer(mSoundBuffer);

	mMusic.openFromFile("Media/Sounds/music.ogg");
	mMusic.play();
	mMusic.setLoop(true);

	buildScene();
	mWorldView.reset(sf::FloatRect(0, 0, 1280, 720));
	mWorldView.zoom(0.5f);

	objects = mLevel->getObjects("solid");
}

void World::loadLevel(int& levelNumber)
{
	mLevelNumber = levelNumber;
	switch (mLevelNumber)
	{
		case 1:
			mLevel->loadFromFile("Level1.tmx"); // Нужно будет положить в Levels/
			break;
		case 2:
			mLevel->loadFromFile("map2.tmx");
			break;
		case 3:
			mLevel->loadFromFile("map3.tmx");
			break;
		default:
			std::cout << "Error! Out of range level." << std::endl;
			mLevelNumber = 1;
			break;
	}
}


void World::update(sf::Time time)
{
	// Передаём координаты игрока в функцию управления камерой
	setPlayerCoordinateForView(mPlayerHero->x, mPlayerHero->y);

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end();)
	{
		// Вызываем функцию update для объектов
		(*it)->update(static_cast<float>(time.asMilliseconds()));
		// Если объект "мёртв"
		if (!(*it)->life)
		{
			it = entities.erase(it);
			//delete (*it);
		}
		else
		{
			++it;
		}
	}

	if (mPlayerHero->isShoot && !mPlayerHero->shooted)
	{
		mPlayerHero->isShoot = false;
		mPlayerHero->shooted = true;
	}
	else if (mPlayerHero->isShooting)
	{
		mPlayerHero->isShooting = false;
		// Если выстрелили, то появляется пуля, enum передаем как int
		entities.push_back(new Bullet(Type::ID::AlliedBullet, mTextures, mFonts, *mLevel, 
			mPlayerHero->x + (mPlayerHero->mWidth / 2.f), mPlayerHero->y + 9.f, 7, 7, mPlayerHero->mState));
		mSound.play();	// Играем звук пули
	}

	if (mShadowBoss.isActive)
	{
		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentaclesStatic.begin(); it != mShadowBoss.mTentaclesStatic.end();)
		{
			(*it)->update(static_cast<float>(time.asMilliseconds()));
			if (!(*it)->life)
			{
				it = mShadowBoss.mTentaclesStatic.erase(it);
				//delete (*it);
			}
			else
			{
				++it;
			}
		}

		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentacles.begin(); it != mShadowBoss.mTentacles.end();)
		{
			(*it)->update(static_cast<float>(time.asMilliseconds()));
			if (!(*it)->life)
			{
				it = mShadowBoss.mTentacles.erase(it);
				//delete (*it);
			}
			else
			{
				++it;
			}
		}

		mShadowBoss.mShadowLifeBar->update(mShadowBoss.mShadow->mHitpoints);
		mShadowBoss.mShadow->update(static_cast<float>(time.asMilliseconds()));
	}

	// Обновляем полосу здоровья
	mLifeBar->update(mPlayerHero->mHitpoints);

	mPlayerHero->update(static_cast<float>(time.asMilliseconds()));

	// Проверка столкновений
	handleCollisions(static_cast<float>(time.asMilliseconds()));

	mPlayerInfo->mDialogNumber = 0;
	if (mPlayerHero->dialogNumber != 0)
	{
		mPlayerInfo->showDialog(mPlayerHero->dialogNumber);
	}
	else
	{
		mPlayerInfo->mDialogNumber = 0;
	}
}

void World::draw()
{
	mWindow.setView(mWorldView);

	// Рисуем новую карту
	mLevel->draw(mWindow);

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		/*
		sf::FloatRect rect = (*it)->getRect();
		sf::RectangleShape shape;
		shape.setPosition(sf::Vector2f(rect.left, rect.top));
		shape.setSize(sf::Vector2f(rect.width, rect.height));
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Green);
		shape.setOutlineThickness(1.f);
		mWindow.draw(shape);
		*/
		mWindow.draw((*it)->mSprite);
	}

	if (mShadowBoss.isActive)
	{
		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentaclesStatic.begin(); it != mShadowBoss.mTentaclesStatic.end(); ++it)
		{
			/*
			sf::FloatRect rect = (*it)->getRect();
			sf::RectangleShape shape;
			shape.setPosition(sf::Vector2f(rect.left, rect.top));
			shape.setSize(sf::Vector2f(rect.width, rect.height));
			shape.setFillColor(sf::Color::Transparent);
			shape.setOutlineColor(sf::Color::Green);
			shape.setOutlineThickness(1.f);
			mWindow.draw(shape);
			*/
			mWindow.draw((*it)->mSprite);
		}

		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentacles.begin(); it != mShadowBoss.mTentacles.end(); ++it)
		{
			/*
			sf::FloatRect rect = (*it)->getRect();
			sf::RectangleShape shape;
			shape.setPosition(sf::Vector2f(rect.left, rect.top));
			shape.setSize(sf::Vector2f(rect.width, rect.height));
			shape.setFillColor(sf::Color::Transparent);
			shape.setOutlineColor(sf::Color::Green);
			shape.setOutlineThickness(1.f);
			mWindow.draw(shape);
			*/
			mWindow.draw((*it)->mSprite);
		}
		/*
		sf::FloatRect srect = mShadowBoss.mShadow->getRect();
		sf::RectangleShape sshape;
		sshape.setPosition(sf::Vector2f(srect.left, srect.top));
		sshape.setSize(sf::Vector2f(srect.width, srect.height));
		sshape.setFillColor(sf::Color::Transparent);
		sshape.setOutlineColor(sf::Color::Green);
		sshape.setOutlineThickness(1.f);
		mWindow.draw(sshape);
		*/

		mShadowBoss.mShadowLifeBar->draw(mWindow);

		mWindow.draw(mShadowBoss.mShadow->mSprite);
	}
	/*
	sf::FloatRect rect = mPlayerHero->getRect();
	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);
	mWindow.draw(shape);
	*/
	mLifeBar->draw(mWindow);

	mWindow.draw(mPlayerHero->mSprite);
}

bool World::hasAlivePlayer() const
{
	return mPlayerHero->life;
}

bool World::hasPlayerReachedEnd() const
{
	return mPlayerHero->isRichedEnd;
}

void World::setPlayerCoordinateForView(float x, float y)
{
	float tempX = x, tempY = y;

	/*
	if (x < 640.f)
		tempX = 640.f;

	if (y < 100.f)
		tempY = 100.f;

	if (y > 2000.f)
		tempY = 2000.f;

	if (x > 2560.f)
		tempX = 2560.f;*/

	mWorldView.setCenter(tempX, tempY);
}

void World::buildScene()
{
	// Инициализация уровня
	mLevel = new Level();
	loadLevel(mLevelNumber);

	// Добавление игрока
	Object playerObj = mLevel->getObject("player");
	mPlayerHero = new Player(Type::Archer, mTextures, mFonts, *mLevel, playerObj.rect.left, playerObj.rect.top, 20, 30, mPlayerInfo);
	mPlayerInfo->setPlayer(mPlayerHero);
	mLifeBar = new LifeBar(Type::HealthBar, mTextures, mFonts, mPlayerHero->mHitpoints);

	Object shadowObj = mLevel->getObject("bossShadow");
	mShadowBoss.mShadow = new Shadow(Type::Shadow, mTextures, mFonts, *mLevel, shadowObj.rect.left, shadowObj.rect.top, 40, 35, shadowObj.type);
	mShadowBoss.mShadowLifeBar = new LifeBar(Type::BossBar, mTextures, mFonts, mShadowBoss.mShadow->mHitpoints);
	for (int i = 0; i < 4; i++)
	{
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 12013.f + 13.f * i, 994.f, 13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 12061.f + 13.f * i, 1073.f, 13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 12285.f + 13.f * i, 1073.f, 13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 12333.f + 13.f * i, 994.f, 13, 45, "0"));
		mShadowBoss.mTentaclesStatic.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, 12173.f + 13.f * i, 946.f, 13, 45, "0"));
	}

	// Добавление врагов
	addObjects();
}

void World::addObjects()
{
	// Все объкты врага на tmx-карте будут храниться в векторе
	std::vector<Object> e = mLevel->getObjects("enemyGhost");

	// Проходимся по элементам вектора с врагами
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Ghost(Type::Ghost, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 51, 36, e[i].type));
	}

	e = mLevel->getObjects("enemyGolem");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Golem(Type::Golem, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 45, 38, e[i].type));
	}

	e = mLevel->getObjects("enemySoldier");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new DarkSoldier(Type::DarkSoldier,  mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 20, 30, e[i].type));
	}

	e = mLevel->getObjects("enemyGoblin");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Goblin(Type::Goblin,  mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 25, 28, e[i].type));
	}

	e = mLevel->getObjects("enemyMinotaurMage");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new MinotaurMage(Type::MinotaurMage, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 40, 45, e[i].type));
	}

	e = mLevel->getObjects("enemyDwarf");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Dwarf(Type::Dwarf, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 40, 35, e[i].type, 0));
	}

	e = mLevel->getObjects("enemyDwarvenArcher");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Dwarf(Type::DwarfArcher, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 40, 35, e[i].type, 1));
	}

	e = mLevel->getObjects("enemyDwarvenCommander");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Dwarf(Type::DwarvenCommander, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 40, 35, e[i].type, 2));
	}

	e = mLevel->getObjects("enemyDwarfA");
	for (size_t i = 0; i < e.size(); i++)
	{
		Entity* dwarf = new Dwarf(Type::Dwarf, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 40, 35, e[i].type, 0);
		dwarf->isStarted = true;
		entities.push_back(dwarf);
	}

	e = mLevel->getObjects("enemyDwarvenArcherA");
	for (size_t i = 0; i < e.size(); i++)
	{
		Entity* dwarf = new Dwarf(Type::DwarfArcher, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 40, 35, e[i].type, 1);
		dwarf->isStarted = true;
		entities.push_back(dwarf);
	}

	e = mLevel->getObjects("enemyDwarvenCommanderA");
	for (size_t i = 0; i < e.size(); i++)
	{
		Entity* dwarf = new Dwarf(Type::DwarvenCommander, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 40, 35, e[i].type, 2);
		dwarf->isStarted = true;
		entities.push_back(dwarf);
	}

	e = mLevel->getObjects("enemyDwarvenCommanderM");
	for (size_t i = 0; i < e.size(); i++)
	{
		Entity* dwarf = new Dwarf(Type::DwarvenCommander, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 40, 35, e[i].type, 2);
		dwarf->isEnabling = true;
		entities.push_back(dwarf);
	}

	e = mLevel->getObjects("enemyTentacle");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 13, 45, e[i].type));
	}

	// Добавляем падающие камни, не враг
	e = mLevel->getObjects("rock");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Rock(Type::Rock, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 16, e[i].type));
	}

	// Добавляем врата, не враг
	e = mLevel->getObjects("gateO");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Gate(Type::OpeningGate, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 16, e[i].type));
	}

	// Добавляем врата, не враг
	e = mLevel->getObjects("gateC");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Gate(Type::ClosingGate, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 16, e[i].type));
	}

	// Добавляем врата, не враг
	e = mLevel->getObjects("gateOC");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new Gate(Type::OpenClosingGate, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 16, e[i].type));
	}

	// Добавляем врата, не враг
	e = mLevel->getObjects("dialogPerson");
	for (size_t i = 0; i < e.size(); i++)
	{
		switch(std::stoi(e[i].type))
		{
			case 1:
				entities.push_back(new DialogPerson(Type::Oswald, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 16, 20, "2"));
				break;
			case 2:
				entities.push_back(new DialogPerson(Type::Heinrich, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 29, 32, "6"));
				break;
			default:
				std::cout << "Invalid type dialog person" << std::endl;
				break;
		}
	}

	// Добавляем платформы, не враг
	e = mLevel->getObjects("movingPlatform");
	for (size_t i = 0; i < e.size(); i++)
	{
		entities.push_back(new MovingPlatform(Type::MovingPlatform, mTextures, mFonts, *mLevel, e[i].rect.left, e[i].rect.top, 48, 11));
	}
}

void World::addEnemy(Type::ID type, float relX, float relY)
{
	SpawnPoint spawn(type, relX, relY);
	mEnemySpawnPoints.push_back(spawn);
}

void World::guideMissiles()
{
	
}


void World::spawnEnemies()
{

}

void World::destroyEntitiesOutsideView()
{

}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::handleCollisions(float time)
{
	mPlayerHero->onPlatform = 0.f;

	/// Если активировали первого босса
	if (mPlayerHero->mHasStartedFirstBoss && !mShadowBoss.isFinished)
	{
		mShadowBoss.isActive = true;

		if (mShadowBoss.mShadow->mType == Type::ID::Shadow)
		{
			// Если тень активирована
			if (!mShadowBoss.mShadow->isStarted)
			{
				mShadowBoss.mShadow->isStarted = true;
				//mMusic.pause();
				//mDarkMusic.play();
			}
			else if (mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
			{
				if (!mShadowBoss.mShadow->isBack && ((mPlayerHero->x - mShadowBoss.mShadow->x > 0.f && mShadowBoss.mShadow->dx < 0.f)
					|| (mPlayerHero->x - mShadowBoss.mShadow->x < 0.f && mShadowBoss.mShadow->dx > 0.f)))
				{
					mShadowBoss.mShadow->isBack = true;
					mShadowBoss.mShadow->dx = -mShadowBoss.mShadow->dx;
					mShadowBoss.mShadow->mSprite.scale(-1.f, 1.f);
				}
				else
				{
					mShadowBoss.mShadow->isBack = false;
				}
				mShadowBoss.mShadow->isAttacked = true;
				if (mShadowBoss.mShadow->isHitted && !mShadowBoss.mShadow->isStay)
				{
					mPlayerHero->mHitpoints -= mShadowBoss.mShadow->mDamage;
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, mPlayerHero->x - 50.f, mShadowBoss.mShadow->y - 10.f, 13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, mPlayerHero->x - 25.f, mShadowBoss.mShadow->y - 10.f, 13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, mPlayerHero->x, mShadowBoss.mShadow->y - 10.f, 13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, mPlayerHero->x + 25.f, mShadowBoss.mShadow->y - 10.f, 13, 45, "1"));
					mShadowBoss.mTentacles.push_back(new Tentacle(Type::Tentacle, mTextures, mFonts, *mLevel, mPlayerHero->x + 50.f, mShadowBoss.mShadow->y - 10.f, 13, 45, "1"));
					mShadowBoss.mShadow->isHitted = false;
					std::cout << "Shadow ATTACK\n";
				}
			}
		}

		if (mShadowBoss.mShadow->getRect().intersects(mPlayerHero->getRect()) && mShadowBoss.mShadow->isDelay)
		{
			mPlayerHero->mHitpoints = 0;
		}

		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentaclesStatic.begin(); it != mShadowBoss.mTentaclesStatic.end(); ++it)
		{
			if (mShadowBoss.mShadow->isCalling)
			{
				if (!(*it)->isStarted)
					(*it)->isStarted = true;
				else
					(*it)->isEnabled = true;
				(*it)->isEnabling = true;
			}
			else if (mShadowBoss.mShadow->isWithdrawing)
			{
				(*it)->isDisabled = true;
				(*it)->isEnabling = false;
			}

			if (mShadowBoss.isFinished)
			{
				(*it)->mHitpoints = 0;
			}

			if ((*it)->getRect().intersects(mPlayerHero->getRect()) && mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
			{
				(*it)->isAttacked = true;
				if ((*it)->isHitted)
				{
					mPlayerHero->mHitpoints -= (*it)->mDamage;
					(*it)->isHitted = false;
					std::cout << "Hit\n";
				}
			}
		}

		for (std::list<Tentacle*>::iterator it = mShadowBoss.mTentacles.begin(); it != mShadowBoss.mTentacles.end(); ++it)
		{
			if (mShadowBoss.mShadow->isWithdrawing)
				(*it)->mHitpoints = 0;
			else
			{
				(*it)->isStarted = true;
				(*it)->isEnabling = true;

				if ((*it)->getRect().intersects(mPlayerHero->getRect()) && mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
				{
					(*it)->isAttacked = true;
					if ((*it)->isHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->isHitted = false;
						std::cout << "Hit\n";
					}
				}
			}
		}
	}
	else if (mShadowBoss.isFinished && !mShadowBoss.mShadow->life)
	{
		mShadowBoss.isActive = false;
		mPlayerInfo->mQuests[3] = true;
		//mDarkMusic.pause();
		mMusic.play();
	}


	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		/// Если игрок столкнулся с объектом movingplatform
		if (((*it)->mType == Type::ID::MovingPlatform) && ((*it)->getRect().intersects(mPlayerHero->getRect())))
		{
			// При этом игрок находится в состоянии после прыжка, т.е падает вниз
			if ((mPlayerHero->dy > 0.f) || (mPlayerHero->onGround == false))
				// Если игрок находится выше платформы, т.е это его ноги минимум (т.к. мы уже проверяли что он столкнулся с платформой)
				if (mPlayerHero->y + mPlayerHero->mHeight < (*it)->y + (*it)->mHeight)
				{
					mPlayerHero->y = (*it)->y - mPlayerHero->mHeight + 3.f;
					mPlayerHero->x += (*it)->dx * time;
					mPlayerHero->dy = 0;
					mPlayerHero->onPlatform = (*it)->dx;
					// То выталкиваем игрока так, чтобы он как бы стоял на платформе
					mPlayerHero->onGround = true;
				}
		}

		/// Особые атаки врагов
		/// Особая атака минотавра-мага
		if (((*it)->mType == Type::ID::MinotaurMage) && ((*it)->mHitpoints > 0))
		{
			//isFired = false;
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), static_cast<float>((*it)->mHeight));
			int distance = 0;
			//(*it)->isAttacked = false;
			while (distance < 10)
			{
				if (findPlayer.intersects(mPlayerHero->getRect()) && mPlayerHero->life)
				{
					(*it)->isAttacked = true;
					break;
				}
				else
				{
					if ((*it)->dx > 0.f)
					{
						findPlayer.left += 30.f;
					}
					else
					{
						findPlayer.left -= 30.f;
					}
				}
				distance++;
			}
			if ((*it)->isAttacked)
			{
				(*it)->mCurrentFrame = 0.f;
				(*it)->mMoveTimer += 0.005f * time;
				if ((*it)->mMoveTimer > 24.f)
					(*it)->mMoveTimer -= 24.f;
				//fireDamage.setPosition(mPlayerHero->x - 49.f, mPlayerHero->y - 185.f);
				//fireDamage.setTextureRect(sf::IntRect(128 * (int((*it)->mMoveTimer) % 8), 256 * (int((*it)->mMoveTimer) / 8), 128, 256));
				//fireDamage.setScale(0.5f, 0.5f);
				//isFired = true;
				if ((*it)->isHitted)
				{
					// Нанесение урона
					mPlayerHero->mHitpoints -= 5;
					//p.dx = 3.f * (*it)->dx;
					//p.dy = -0.5f;	// Игрок отскакивает от врага
					//(*it)->isBack = false;
					(*it)->isHitted = false;
					(*it)->mMoveTimer = 0.f;
					std::cout << "Hit\n";
				}
			}
		}

		/// Особая атака Гнома-лучника
		if (((*it)->mType == Type::ID::DwarfArcher) && ((*it)->mHitpoints > 0))
		{
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), static_cast<float>((*it)->mHeight));
			int distance = 0;
			while (distance < 10) {
				if (findPlayer.intersects(mPlayerHero->getRect()) && mPlayerHero->life)
				{
					(*it)->isAttacked = true;
					break;
				}
				else
				{
					if ((*it)->dx > 0.f)
					{
						findPlayer.left += 30.f;
					}
					else
					{
						findPlayer.left -= 30.f;
					}
				}
				distance++;
			}
			if ((*it)->isAttacked)
			{
				(*it)->mCurrentFrame = 0.f;
				if ((*it)->isHitted)
				{
					entities.push_back(new Bullet(Type::ID::EnemyBullet, mTextures, mFonts, *mLevel,
						(*it)->x + ((*it)->mWidth / 2.f), (*it)->y + ((*it)->mHeight / 2.f) + 5.f, 7, 7, (*it)->dx > 0.f ? 1 : 0));
					mSound.play();	// Играем звук пули
					(*it)->isHitted = false;
					std::cout << "Shoot\n";
				}
			}
		}

		/// Падение камней
		if (((*it)->mType == Type::ID::Rock) && (!(*it)->isAttacked))
		{
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), static_cast<float>((*it)->mHeight));
			int distance = 0;
			while (distance < 20) {
				if (findPlayer.intersects(mPlayerHero->getRect()) && mPlayerHero->life)
				{
					(*it)->isAttacked = true;
					break;
				}
				else
				{
					findPlayer.top += 30.f;

				}
				distance++;
			}
		}

		/// Закрытие врат
		if (((*it)->mType == Type::ID::ClosingGate) && (!(*it)->isAttacked))
		{
			sf::FloatRect findPlayer((*it)->x, (*it)->y, static_cast<float>((*it)->mWidth), static_cast<float>((*it)->mHeight));
			int distance = 0;
			while (distance < 10) {
				if (findPlayer.intersects(mPlayerHero->getRect()) && mPlayerHero->life)
				{
					(*it)->isAttacked = true;
					break;
				}
				else
				{
					findPlayer.top += 30.f;
				}
				distance++;
			}
		}

		/// Открытие врат
		if (((*it)->mType == Type::ID::OpeningGate) && (!(*it)->isStarted))
		{
			if (((*it)->type == "3") && mPlayerInfo->mQuests[0])
				(*it)->isStarted = true;

			if (((*it)->type == "4") && mPlayerHero->mActivatedGate)
				(*it)->isStarted = true;

			if (((*it)->type == "5") && mPlayerInfo->mQuests[3])
				(*it)->isStarted = true;
		}

		/// Урон по боссу
		if (mShadowBoss.mShadow->getRect().intersects((*it)->getRect()) && ((*it)->mType == Type::ID::AlliedBullet)
			&& mShadowBoss.mShadow->life && (mShadowBoss.mShadow->mHitpoints > 0) && mShadowBoss.isActive && mShadowBoss.mShadow->isStay)
		{
			mShadowBoss.mShadow->mHitpoints -= (*it)->mDamage;
			if (mShadowBoss.mShadow->mHitpoints <= 0)
				mShadowBoss.isFinished = true;
			(*it)->life = false;
		}

		/// Если прямоугольник спрайта объекта пересекается с игроком
		if ((*it)->getRect().intersects(mPlayerHero->getRect()))
		{
			/// Если столкнулись с призраком
			if ((*it)->mType == Type::ID::Ghost)
			{
				if (mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->isBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->isBack = true;
						(*it)->dx = -(*it)->dx;
						(*it)->mSprite.scale(-1.f, 1.f);
					}
					(*it)->isAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->isHitted)
					{
						// Наносит урон
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						// Игрок отскакивает от врага
						mPlayerHero->dy = -0.1f;
						//p.mSpeed = 0.5f * (*it)->dx;
						//(*it)->isBack = false;
						(*it)->isHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// Если столкнулись с големом
			if ((*it)->mType == Type::ID::Golem)
			{
				// Если голем активирован
				if (!(*it)->isStarted)
				{
					(*it)->isStarted = true;
				}
				else if (mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->isBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->isBack = true;
						(*it)->dx = -(*it)->dx;
					}
					(*it)->isAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->isHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->isHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// Если мы столкнулись с тёмным солдатом
			if ((*it)->mType == Type::ID::DarkSoldier)
			{
				if (mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->isBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->isBack = true;
						(*it)->dx = -(*it)->dx;
					}
					// Враг останавливается
					(*it)->isAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->isHitted)
					{
						// Солдат телепортирует игрока
						mPlayerHero->x = ((*it)->dx > 0 ? 1.2f : 0.8f) * (*it)->x;
						// Игрок отскакивает от врага
						mPlayerHero->y = 0.9f * (*it)->y;
						mPlayerHero->dy = -0.25f;
						(*it)->isHitted = false;
						std::cout << "Hit\n";
					}
					//p.x = 0.97f * ((*it)->dx > 0? (*it)->x + 60.f : (*it)->x);
					//p.dy = -0.5f;	// Игрок отскакивает от врага
				}
			}

			/// Если столкнулись с гоблином
			if ((*it)->mType == Type::ID::Goblin)
			{
				if (mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->isBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->isBack = true;
						(*it)->dx = -(*it)->dx;
					}
					(*it)->isAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->isHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->isHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// Если столкнулись с минотавром-магом
			if ((*it)->mType == Type::ID::MinotaurMage)
			{
				if (mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->isBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->isBack = true;
						(*it)->dx = -(*it)->dx;
						(*it)->mSprite.scale(-1.f, 1.f);
					}
					(*it)->isAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->isHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->isHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// Если столкнулись с дварфом
			if ((*it)->mType == Type::ID::Dwarf)
			{
				// Если дварф активирован
				if (!(*it)->isStarted)
				{
					(*it)->isStarted = true;
				}
				else if (mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->isBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->isBack = true;
						(*it)->dx = -(*it)->dx;
						(*it)->mSprite.scale(-1.f, 1.f);
					}
					(*it)->isAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->isHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->isHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// Если столкнулись с дварфом-лучником
			if ((*it)->mType == Type::ID::DwarfArcher)
			{
				// Если дварф-лучник активирован
				if (!(*it)->isStarted)
				{
					(*it)->isStarted = true;
				}
				else if (mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->isBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->isBack = true;
						(*it)->dx = -(*it)->dx;
						(*it)->mSprite.scale(-1.f, 1.f);
					}
					(*it)->isAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->isHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->isHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// Если столкнулись с командиром дварфов
			if ((*it)->mType == Type::ID::DwarvenCommander)
			{
				// Если дварф активирован
				if (!(*it)->isStarted)
				{
					(*it)->isStarted = true;
					// Создаём рядом ещё двух гномов обычного типа, если сагрили командира гномов
					if (!(*it)->isSpawn)
					{
						if (mPlayerHero->dx >= 0.f)
						{
							Entity* dwarf = new Dwarf(Type::ID::Dwarf, mTextures, mFonts, *mLevel, (*it)->x - 30.f, (*it)->y, 50, 40, (*it)->type, 0);
							dwarf->isStarted = true;
							entities.push_back(dwarf);
							dwarf = new Dwarf(Type::ID::DwarfArcher, mTextures, mFonts, *mLevel, (*it)->x + 30.f, (*it)->y, 50, 40, (*it)->type, 1);
							dwarf->isStarted = true;
							dwarf->isEnabling = false;
							dwarf->dx *= -1.f;
							dwarf->mSprite.scale(-1.f, 1.f);
							entities.push_back(dwarf);
						}
						else
						{
							Entity* dwarf = new Dwarf(Type::ID::Dwarf, mTextures, mFonts, *mLevel, (*it)->x - 30.f, (*it)->y, 50, 40, (*it)->type, 0);
							dwarf->isStarted = true;
							dwarf->dx *= -1.f;
							dwarf->mSprite.scale(-1.f, 1.f);
							entities.push_back(dwarf);
							dwarf = new Dwarf(Type::ID::DwarfArcher, mTextures, mFonts, *mLevel, (*it)->x + 30.f, (*it)->y, 50, 40, (*it)->type, 1);
							dwarf->isStarted = true;
							dwarf->isEnabling = false;
							entities.push_back(dwarf);
						}
						(*it)->isSpawn = true;
					}
				}
				else if (mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
				{
					if (!(*it)->isBack && ((mPlayerHero->x - (*it)->x > 0.f && (*it)->dx < 0.f)
						|| (mPlayerHero->x - (*it)->x < 0.f && (*it)->dx > 0.f)))
					{
						(*it)->isBack = true;
						(*it)->dx = -(*it)->dx;
						(*it)->mSprite.scale(-1.f, 1.f);
					}
					(*it)->isAttacked = true;
					(*it)->mCurrentFrame = 0.f;
					if ((*it)->isHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->isHitted = false;
						std::cout << "Hit\n";
					}
				}
			}

			/// Если столкнулись с щупальцем
			if ((*it)->mType == Type::ID::Tentacle)
			{
				// Если щупальце активировано
				if (!(*it)->isStarted)
				{
					(*it)->isStarted = true;
				}
				else if (mPlayerHero->life && (mPlayerHero->mHitpoints > 0))
				{
					(*it)->isAttacked = true;
					if ((*it)->isHitted)
					{
						mPlayerHero->mHitpoints -= (*it)->mDamage;
						(*it)->isHitted = false;
						std::cout << "Hit\n";
					}
				}
			}
		}

		/// Если столкнулись два объекта: герой и пуля
		if ((mPlayerHero->getRect().intersects((*it)->getRect()))
			&& ((*it)->mType == Type::ID::EnemyBullet) && (mPlayerHero->mHitpoints > 0))
		{
			mPlayerHero->mHitpoints -= (*it)->mDamage;
			(*it)->life = false;
		}

		/// Если столкнулись два объекта: герой и камень
		if ((mPlayerHero->getRect().intersects((*it)->getRect()))
			&& ((*it)->mType == Type::ID::Rock) && (*it)->isAttacked && !(*it)->isEnd && (mPlayerHero->mHitpoints > 0))
		{
			mPlayerHero->mHitpoints = 0;
		}

		/// Если столкнулись два объекта: герой и врата
		if ((mPlayerHero->getRect().intersects((*it)->getRect()))
			&& ((*it)->mType == Type::ID::OpeningGate || (*it)->mType == Type::ID::ClosingGate || (*it)->mType == Type::ID::OpenClosingGate) 
			&& (mPlayerHero->mHitpoints > 0))
		{
			mPlayerHero->mHitpoints = 0;
		}

		/// Если столкнулись два объекта: герой и диалоговый персонаж
		if ((mPlayerHero->getRect().intersects((*it)->getRect()))
			&& ((*it)->mType == Type::ID::Oswald || (*it)->mType == Type::ID::Heinrich) && !(*it)->isEnd && (mPlayerHero->mHitpoints > 0))
		{
			(*it)->isEnd = true;
			mPlayerHero->dialogNumber = std::stoi((*it)->type);

			switch ((*it)->mType)
			{
				case Type::Oswald:
					mPlayerInfo->mQuests[0] = true;
					break;
				case Type::Heinrich:
					mPlayerInfo->mQuests[2] = true;
					break;
				default:
					std::cout << "Invalid dialog person type" << std::endl;
					break;
			}
		}

		/// Добавляем упавший камень в твёрдные объекты для проверки коллизии
		if (((*it)->mType == Type::ID::Rock) && (*it)->isEnd && !(*it)->isSpawn && (mPlayerHero->mHitpoints > 0))
		{
			(*it)->isSpawn = true;

			tempObject = new Object();
			tempObject->name = "solid";
			tempObject->type = (*it)->type;
			tempObject->sprite = (*it)->mSprite;

			sf::Rect <float> objectRect;
			objectRect.top = (*it)->y;
			objectRect.left = (*it)->x;
			objectRect.height = static_cast<float>((*it)->mHeight);
			objectRect.width = static_cast<float>((*it)->mWidth);
			tempObject->rect = objectRect;
			mLevel->objects.push_back(*tempObject);
			mPlayerHero->mLevelObjects.push_back(*tempObject);
		}

		/// Добавляем закрытые врата в смертельные объекты для проверки коллизии
		if (((*it)->mType == Type::ID::ClosingGate) && (*it)->isEnd && !(*it)->isSpawn && (mPlayerHero->mHitpoints > 0))
		{
			(*it)->isSpawn = true;

			tempObject = new Object();
			tempObject->name = "death";
			tempObject->type = (*it)->type;
			tempObject->sprite = (*it)->mSprite;

			sf::Rect <float> objectRect;
			objectRect.top = (*it)->y;
			objectRect.left = (*it)->x;
			objectRect.height = static_cast<float>((*it)->mHeight);
			objectRect.width = static_cast<float>((*it)->mWidth);
			tempObject->rect = objectRect;
			mLevel->objects.push_back(*tempObject);
			mPlayerHero->mLevelObjects.push_back(*tempObject);
		}

		/// Проверка столкновений объектов между собой
		for (std::list<Entity*>::iterator it2 = it; it2 != entities.end(); ++it2)
		{
			// При этом это должны быть разные объекты
			if ((*it) != (*it2))
			{
				/// Если столкнулись два объекта: враг и камень
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->mType != Type::ID::Rock)
					&& ((*it2)->mType == Type::ID::Rock) && (*it2)->isAttacked && !(*it2)->isEnd && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints = 0;
					(*it)->isStarted = true;
					(*it)->isEnd = true;
					(*it)->isSpawn = true;
				}

				/// Если столкнулись два объекта: пуля и камень || врата
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it2)->mType == Type::ID::AlliedBullet || (*it2)->mType == Type::ID::EnemyBullet)
					&& ((*it)->mType == Type::ID::Rock || (*it)->mType == Type::ID::OpeningGate || (*it)->mType == Type::ID::ClosingGate || (*it)->mType == Type::ID::OpenClosingGate))
				{
					(*it2)->life = false;
				}

				/// Если столкнулись два объекта: призрак и пуля
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->mType == Type::ID::Ghost)
					&& ((*it2)->mType == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->life = false;
				}

				/// Если столкнулись два объекта: голем и пуля
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->mType == Type::ID::Golem)
					&& ((*it2)->mType == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0) && ((*it)->isEnd))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->life = false;
				}

				/// Если столкнулись два объекта: тёмный солдат и пуля
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->mType == Type::ID::DarkSoldier)
					&& ((*it2)->mType == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->life = false;
				}

				/// Если столкнулись два объекта: тень и пуля
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->mType == Type::ID::Shadow)
					&& ((*it2)->mType == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0) && ((*it)->isEnd))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->life = false;
				}

				/// Если столкнулись два объекта: гоблин и пуля
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->mType == Type::ID::Goblin)
					&& ((*it2)->mType == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->life = false;
				}

				/// Если столкнулись два объекта: минотавр-маг и пуля
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->mType == Type::ID::MinotaurMage)
					&& ((*it2)->mType == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->life = false;
				}

				/// Если столкнулись два объекта: дварф и пуля
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->mType == Type::ID::Dwarf)
					&& ((*it2)->mType == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0) && ((*it)->isEnd))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->life = false;
				}

				/// Если столкнулись два объекта: дварф-лучник и пуля
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->mType == Type::ID::DwarfArcher)
					&& ((*it2)->mType == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0) && ((*it)->isEnd))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->life = false;
				}

				/// Если столкнулись два объекта: командир дварфов и пуля
				if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->mType == Type::ID::DwarvenCommander)
					&& ((*it2)->mType == Type::ID::AlliedBullet) && ((*it)->mHitpoints > 0) && ((*it)->isEnd))
				{
					(*it)->mHitpoints -= (*it2)->mDamage;
					(*it2)->life = false;

					if ((*it)->isEnabling && (*it)->mHitpoints <= 0)
					{
						mPlayerInfo->mQuests[1] = true;
						mPlayerHero->dialogNumber = 4;
					}
				}
			}
		}
	}
}

void World::handleEvent()
{
		
}
