#ifndef PARALLELTASK_HPP
#define PARALLELTASK_HPP

#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "PlayerInfo.hpp"

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>


class ParallelTask
{
	private:
		sf::Thread			mThread;
		sf::Thread			mThreadClock;
		bool				mFinished;
		bool				mFinishedLoading;
		sf::Clock			mElapsedTime;
		sf::Mutex			mMutex;

		TextureHolder&		mTextures;

		PlayerInfo*			mPlayerInfo;
	
	
	private:
		void				runTask();
		void				loadTextures();


	public:
		explicit 			ParallelTask(TextureHolder& textures, PlayerInfo* playerInfo);
		void				execute();
		bool				isFinished();
		float				getCompletion();
};

#endif // PARALLELTASK_HPP
