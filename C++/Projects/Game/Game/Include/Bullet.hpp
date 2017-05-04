#ifndef BULLET_HPP
#define BULLET_HPP

#include "Entity.hpp"


class Bullet : public Entity
{
	public:
		float			targetX;
		float			targetY;	// Отвечают за направление движения пули к цели
		float			vx;
		float			vy;
		float			distance;

		bool			isMouse;
	
	
	public:
	
						Bullet(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, float tX, float tY);	// При клике мышкой
										    
						Bullet(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level &lvl, float X, float Y, int width, int height, int dir);	// При нажатии space
	
		virtual void	update(float time);
};

#endif // BULLET_HPP
