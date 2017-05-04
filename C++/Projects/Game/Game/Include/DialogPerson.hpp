#ifndef DIALOGPERSON_HPP
#define DIALOGPERSON_HPP

#include "Entity.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class DialogPerson : public Entity
{
	public:
		int						dialogNumber;


	public:
								DialogPerson(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string type);
		virtual void			update(float time);
};

#endif // DIALOGPERSON_HPP
