#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Level.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"

#include <SFML/Graphics.hpp>


class Entity 
{
	public:
		float				x;
		float				y;	// Координаты
		float				dx;	// Ускорение по x
		float				dy;	// Ускорение по y
		float				mSpeed;	// Скорость
		float				mMoveTimer;	// Таймер
		float				mCurrentFrame;	// Таймер анимации
		float				mCurrentAttack;
		float				mCurrentDeath;

		int					mWidth;	// Ширина
		int					mHeight;	// Высота
		int					mHitpoints;	// Количество здоровья
		int					mDamage;

		bool				life; // Жизнь игрока
		bool				isMove;	// Разрешение движения
		bool				onGround;	// Нахождение на земле
		bool				isAttacked;
		bool				isHitted;
		bool				isHittedOnce;
		bool				isStarted;
		bool				isBack;
		bool				isSpawn;
		bool				isEnd;
		bool				isEnabled;
		bool				isDisabled;
		bool				isEnabling;

		std::string			type;	// Тип сущности, получаем из TMX-карты

		sf::Sprite			mSprite;
		sf::Texture			mTexture;
		sf::Texture			mTextureAttack;
		sf::Texture			mTextureDeath;
		Type::ID			mType;
	
		std::vector<Object>	mLevelObjects;	// Вектор объектов карты
	
	
	public:
							Entity(Type::ID Id, float X, float Y, int width, int height, float speed, int hitpoints, int damage, std::string Type = "0");
		virtual				~Entity();
	
		sf::FloatRect		getRect() const;	// Функция получения прямоугольника, его координат и размеров(ширина, высота)
	
		virtual void		update(float time) = 0;
};

#endif // ENTITY_HPP
