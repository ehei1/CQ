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
#include "MainMenuState.h"

#include "HelpState.h"
#include "HighScoreState.h"
#include "SinglePlayState.h"
#include "CreditState.h"
#include "OptionState.h"
#include "SoundManager.h"
#include "StateManager.h"

MainMenuState MainMenuState::mMainMenuState;

void MainMenuState::enter()
{
#ifdef _DEBUG
	cprintf( "MainMenuState::enter()\n" );
#endif

	if( mRank.empty() )
	{
		loadRank();
	}

	mRoot = Root::getSingletonPtr();
	//mSceneMgr = mRoot->createSceneManager( ST_GENERIC, "MainMenuSceneManager" );
	mSceneMgr = mRoot->createSceneManager( ST_GENERIC );
	mSceneMgr->setShadowTechnique( Ogre::SHADOWDETAILTYPE_TEXTURE );

	mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "MainMenuSceneNode" );
	mCamera = mSceneMgr->createCamera( "MainMenuCamera" );

	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);

	// music
	{
		SoundManager::getSingleton().StopAllSounds();

		int index = SoundManager::getSingleton().CreateLoopedStream( StateManager::getInstance()->mAudio.titleMusic );
		int channel = INVALID_SOUND_CHANNEL;

		SoundManager::getSingleton().PlaySound( index , mSceneNode, &channel);
	}

	mOverlay = OverlayManager::getSingleton().getByName( "Menu/MainMenu" );
	
	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("MainMenu/SinglePlay"));
	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("MainMenu/Help"));
	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("MainMenu/HighScore"));
	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("MainMenu/Option"));
	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("MainMenu/Credit"));
	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("MainMenu/Quit"));

	MenuState::enter();
}

void MainMenuState::exit()
{
	SoundManager::getSingleton().StopAllSounds();

	saveRank();

	MenuState::exit();

	mSceneMgr->clearScene();
	mSceneMgr->destroyAllCameras();
	mRoot->getAutoCreatedWindow()->removeAllViewports();
	//mRoot->destroySceneManager( mSceneMgr );

	//TitleState::getInstance()->clear();
#ifdef _DEBUG
	cprintf( "MainMenuState::exit()\n" );
#endif
}

void MainMenuState::pause()
{	
	mOverlay->hide();
	/*
	mSceneMgr->getRootSceneNode()->removeChild( mSceneNode );
	mRoot->getAutoCreatedWindow()->removeAllViewports();
	*/
	
#ifdef _DEBUG
	cprintf( "MainMenuState::pause()\n" );
#endif
}

void MainMenuState::resume()
{	
	/*
	mSceneMgr->getRootSceneNode()->addChild( mSceneNode );
	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
	*/
	mOverlay->show();
	
#ifdef _DEBUG
	cprintf( "MainMenuState::resume()\n" );
#endif
}

void MainMenuState::onSelected(MenuItemList::iterator item)
{
	const std::string & name = (*item)->getName();

#ifdef _DEBUG
	cprintf( "select menu item: %s\n", name.c_str() );
#endif
	if( name == "MainMenu/SinglePlay" )
	{
		StateManager::getInstance()->changeState( SinglePlayState::getInstance() );
	}
	else if( name == "MainMenu/HighScore" )
	{
		StateManager::getInstance()->pushState( HighScoreState::getInstance() );
	}
	else if( name == "MainMenu/Help" )
	{
		StateManager::getInstance()->pushState( HelpState::getInstance() );
	}
	else if( name == "MainMenu/Option" )
	{
		StateManager::getInstance()->pushState( OptionState::getInstance() );
	}
	else if( name == "MainMenu/Credit" )
	{
		StateManager::getInstance()->pushState( CreditState::getInstance() );
	}
	else if( name == "MainMenu/Quit")
	{
		
#ifdef _DEBUG
		_cprintf( "you selected quit\n" );
#endif
		StateManager::getInstance()->quit();
	}
}

void MainMenuState::loadRank()
{
#ifdef _DEBUG
	std::ifstream file( "rank.dat" );
#else
	std::string path;
	StateManager::getInstance()->getInstallPath( path );
	path += "\\rank.dat";

	std::ifstream file( path.c_str() );
#endif

	std::string line;
	
	while( file >> line )
	{
		if( line.empty() )
		{
			break;
		}

		const size_t index = line.find( "=" );
		const std::string name = line.substr( 0, index );
		const std::string cq = line.substr( index + 1, line.size() );

		mRank.push_back( Result( name, atof( cq.c_str() ) ) );

#ifdef _DEBUG
		cprintf( "\t name: %s, score: %0.1f\n", name.c_str(), atof( cq.c_str() ) );
#endif
	}
}

void MainMenuState::saveRank()
{
#ifdef _DEBUG
	std::ofstream file( "rank.dat" );
#else
	std::string path;
	StateManager::getInstance()->getInstallPath( path );
	path += "\\rank.dat";

	std::ofstream file( path.c_str() );
#endif

	for( std::list< Result >::const_iterator it = mRank.begin();
		mRank.end() != it; ++ it )
	{
		file << it->first << '=' << it->second << std::endl;
	}
}

bool MainMenuState::frameStarted( const FrameEvent & evt )
{
	mSceneNode->yaw( Radian( Degree( 1 ) ) );

	return MenuState::frameStarted( evt );
}