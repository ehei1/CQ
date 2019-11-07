/*
-----------------------------------------------------------------------------
CQ
Copyright 2006 Lee Ungju, Lee Junho

Also see acknowledgements in credit.txt

This program is free software; you can redistribute it and/or modify it under
uncommerical purpose.
-----------------------------------------------------------------------------
*/
#include "stdafx.h"
#include "StateManager.h"
#include "IntroState.h"

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
#ifdef _DEBUG
		::AllocConsole();
		cprintf( "Hello, CalcKing debug console\n" );
#endif
		// Create StateManager object
		try
		{
			// initialize the game and switch to the first state
			StateManager::getInstance()->start();
			StateManager::getInstance()->changeState(IntroState::getInstance());
		} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL );
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}
#ifdef _DEBUG
		::FreeConsole();
#endif
		return 0;
	}

#ifdef __cplusplus
}
#endif
