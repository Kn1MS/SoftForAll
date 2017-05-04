#ifndef ROCK_HPP
#define ROCK_HPP

#include "Entity.hpp"

#include <SFML/Graphics.hpp>


class Rock : public Entity
{
	public:
		int					size;

	
	public:
							Rock(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int height, std::string type);
	
		void				fall();
		void				checkCollisionWithMap(float Dx, float Dy);
		void				update(float time);
};

#endif // ROCK_HPP
