#ifndef SHADOW_HPP
#define SHADOW_HPP

#include "Entity.hpp"


class Shadow : public Entity 
{
	public:
		int							counter;
		int							numTelPoint;

		float						mAppearing;
		float						mDisappearing;
		float						mStayTimer;
		float						mDelayTimer;

		float						inaccuracy;

		bool						isTeleported;
		bool						isTeleporting;
		bool						isDisappeared;
		bool						isDisappearing;
		bool						isStay;
		bool						isNeedStay;
		bool						isDelay;
		bool						isNeedDelay;
		bool						isCalling;
		bool						isWithdrawing;

		std::vector<TeleportPoint>	teleporPoints;


	public:
									Shadow(Type::ID Id, const TextureHolder& textures, const FontHolder& fonts, Level& lvl, float X, float Y, int width, int height, std::string Type = "0");

		void						appear(float time);
		void						disappear(float time);
		virtual void				update(float time);
};

#endif // SHADOW_HPP
