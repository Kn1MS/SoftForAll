#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "View.hpp"
#include "DataTables.hpp"
#include "PlayerInfo.hpp"


class Player :public Entity {
public:
	enum State
	{
		left,
		right,
		up,
		down,
		jump,
		stay
	};


public:
	float			mStayTimer; // Таймер анимации
	float			onPlatform;
	float			shootTimer;
	float			jumpTimer;
	float			mBeforeJump;
	float			mAfterJump;
	float			mFallingHeight;
				
	int				counter;
	int				dialogNumber;
				
	bool			isShoot; 
	bool			canShoted; 
	bool			isJumped;
	bool			isLeft; 
	bool			isRight;
	bool			shooted;
	bool			isShooting;
	bool			doubleJump; 
	bool			canDoubleJump; 
	bool			pressJump;
	bool			isRichedEnd;
	bool			mHasStartedFirstBoss;
	bool			mGotKey;
	bool			mActivatedGate;
				
	State			mState;

	PlayerInfo*		mPlayerInfo;


public:
					Player(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, PlayerInfo* playerInfo);

	void			control(float time);
	void			checkCollisionWithMap(float Dx, float Dy);	// Функция взаимодейтсвия с картой
	virtual void	update(float time);	// Функция "оживления" класса

};

#endif // PLAYER_HPP
