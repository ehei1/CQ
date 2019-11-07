/*
-----------------------------------------------------------------------------
CQ
Copyright 2006 Lee Ungju, Lee Junho

Also see acknowledgements in credit.txt

This program is free software; you can redistribute it and/or modify it under
uncommerical purpose.
-----------------------------------------------------------------------------
*/
#include "StdAfx.h"
#include "OptionState.h"

#include "MainMenuState.h"

OptionState OptionState::mOptionState;

void OptionState::enter()
{
#ifdef _DEBUG
	cprintf( "OptionState::enter()\n" );
#endif
	mOverlay = OverlayManager::getSingleton().getByName( "Menu/OptionMenu" );

	// for test. you should change current value
	//mVideoModeIndex = mAntiAliasingIndex = 0;

	OverlayElement * element;
	ConfigOptionMap & configOptionMap = Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions();

#ifdef _DEBUG
	std::ifstream file( "system.cfg" );
#else
	std::string path;
	StateManager::getInstance()->getInstallPath( path);
	path += "\\system.cfg"; 
	std::ifstream file( path.c_str() );
#endif

	static char buffer[ 80 ];

	file.getline( buffer, 80 );
	const std::string videoMode = buffer;

	file.getline( buffer, 80 );
	const std::string antiAliasing = buffer;

	file.getline( buffer, 80 );
	const std::string fullScreen = buffer;

	// set video mode
	{
		const StringVector & mode = configOptionMap[ "Video Mode" ].possibleValues;
		//const String & curMode = configOptionMap[ "Video Mode" ].currentValue;

		for( size_t i = 0; i < mode.size(); ++ i )
		{
			if( videoMode == mode[ i ] )
			{
				element = OverlayManager::getSingleton().getOverlayElement( "OptionMenu/VideoModeOption" );
				element->setCaption( "<< " + mode[ i ] + " >>" );
				mVideoModeIndex = static_cast< int >( i );
				break;
			}
		}
	}

	// set anti aliasing
	{
		const StringVector & mode = configOptionMap[ "Anti aliasing" ].possibleValues;
		//const String & curMode = configOptionMap[ "Anti aliasing" ].currentValue;

		for( size_t i = 0; i < mode.size(); ++ i )
		{
			if( antiAliasing == mode[ i ] )
			{
				element = OverlayManager::getSingleton().getOverlayElement( "OptionMenu/AntiAliasingOption" );
				element->setCaption( "<< " + mode[ i ] + " >>" );
				mAntiAliasingIndex = static_cast< int >( i );
				break;
			}
		}
	}

	// set full screen
	{
		const StringVector & mode = configOptionMap[ "Full Screen" ].possibleValues;
		const String & curMode = configOptionMap[ "Full Screen" ].currentValue;

		for( size_t i = 0; i < mode.size(); ++ i )
		{
			if( fullScreen == mode[ i ] )
			{
				element = OverlayManager::getSingleton().getOverlayElement( "OptionMenu/FullScreenOption" );
				element->setCaption( "<< " + mode[ i ] + " >>" );
				mFullScreenIndex = static_cast< int >( i );
				break;
			}
		}
	}

	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("OptionMenu/VideoModeTitle"));
	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("OptionMenu/AntiAliasingTitle"));
	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("OptionMenu/FullScreenTitle"));

	MenuState::enter();
}

void OptionState::keyPressed(Ogre::KeyEvent *e)
{
	MenuState::keyPressed( e );

	if( ::KC_ESCAPE == e->getKey() )
	{
		StateManager::getInstance()->popState();
		return;
	}
	else if( ::KC_RETURN == e->getKey() )
	{
		ConfigOptionMap & configOptionMap = Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions();
		const StringVector & videoMode = configOptionMap[ "Video Mode" ].possibleValues;
		const StringVector & antiAliasing = configOptionMap[ "Anti aliasing" ].possibleValues;
		const StringVector & fullScreen = configOptionMap[ "Full Screen" ].possibleValues;

#ifdef _DEBUG
		std::ofstream file( "system.cfg" );
#else
		std::string path;
		StateManager::getInstance()->getInstallPath( path );
		path += "\\system.cfg";
		std::ofstream file( path.c_str() ); 
#endif

		file << videoMode[ mVideoModeIndex ] << std::endl;
		file << antiAliasing[ mAntiAliasingIndex ] << std::endl;
		file << fullScreen[ mFullScreenIndex ] << std::endl;

		StateManager::getInstance()->popState();
		return;
	}

	int step = 0;

	if( ::KC_LEFT == e->getKey() )
	{
		step = -1;
	}
	else if( ::KC_RIGHT == e->getKey() )
	{
		step = 1;
	}
	else
	{
		//MenuState::keyPressed( e );
		return;
	}

	if( "OptionMenu/VideoModeTitle" == ( *MenuState::mSelectedItem )->getName() )
	{
		ConfigOptionMap & configOptionMap = Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions();
		const StringVector & mode = configOptionMap[ "Video Mode" ].possibleValues;

		mVideoModeIndex += step;

		if( 0 > mVideoModeIndex )
		{
			mVideoModeIndex = static_cast< int >( mode.size() - 1 );
		}
		else if( mode.size() == mVideoModeIndex )
		{
			mVideoModeIndex = 0;
		}

		OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "OptionMenu/VideoModeOption" );
		element->setCaption( "<< " + mode[ mVideoModeIndex] + " >>" );
	}
	else if( "OptionMenu/AntiAliasingTitle" == ( *MenuState::mSelectedItem )->getName() )
	{
		ConfigOptionMap & map = Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions();
		const StringVector & mode = map[ "Anti aliasing" ].possibleValues;

		mAntiAliasingIndex += step;

		if( 0 > mAntiAliasingIndex )
		{
			mAntiAliasingIndex = static_cast< int >( mode.size() - 1 );
		}
		else if( mode.size() == mAntiAliasingIndex )
		{
			mAntiAliasingIndex = 0;
		}

		OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "OptionMenu/AntiAliasingOption" );
		element->setCaption( "<< " + mode[ mAntiAliasingIndex ] + " >>" );
	}
	else if( "OptionMenu/FullScreenTitle" == ( *MenuState::mSelectedItem )->getName() )
	{
		ConfigOptionMap & map = Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions();
		const StringVector & mode = map[ "Full Screen" ].possibleValues;

		mFullScreenIndex += step;

		if( 0 > mFullScreenIndex )
		{
			mFullScreenIndex = static_cast< int >( mode.size() - 1 );
		}
		else if( mode.size() == mFullScreenIndex )
		{
			mFullScreenIndex = 0;
		}

		OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "OptionMenu/FullScreenOption" );
		element->setCaption( "<< " + mode[ mFullScreenIndex ] + " >>" );
	}

}

void OptionState::exit()
{
	MenuState::exit();
}

bool OptionState::frameStarted( const Ogre::FrameEvent & evt )
{
	static float prevSecond;
	static float curSecond;
	static OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "OptionMenu/Message" );

	if( 0.5f < curSecond - prevSecond )
	{
		element->isVisible() ? element->hide() : element->show();
		prevSecond = curSecond;
	}
	else
	{
		curSecond += evt.timeSinceLastFrame;
	}

	return true;
}