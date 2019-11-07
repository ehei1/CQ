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
#include "GameOverState.h"

#include "MainMenuState.h"
#include "RankState.h"
#include "HighScoreState.h"
#include "StateManager.h"
#include "SinglePlayState.h"

GameOverState GameOverState::mGameOverState;

void GameOverState::enter()
{
	// put any video or image

#ifdef _DEBUG
	_cprintf( "GameOverState::enter()\n" );
#endif
	//StateManager::getInstance()->popState();
	mOverlay = OverlayManager::getSingleton().getByName( "Game/GameOverPanel" );
	mOverlay->show();

	if( SoundManager::getSingleton().isValid() )
	{
		SoundManager::getSingleton().StopAllSounds();

		int index = SoundManager::getSingleton().CreateStream( StateManager::getInstance()->mAudio.gameOverMusic );
		int channel = INVALID_SOUND_CHANNEL;

		SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
		SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
	}
}

void GameOverState::exit()
{
	SoundManager::getSingleton().StopAllSounds();
	mOverlay->hide();
}

void GameOverState::pause()
{
	//OverlayManager::getSingleton().getOverlayElement( "GameOverPanel/MainPanel" )->hide();
	mOverlay->hide();
}

void GameOverState::resume()
{
	//OverlayManager::getSingleton().getOverlayElement( "GameOverPanel/MainPanel" )->hide();
	mOverlay->show();
}

void GameOverState::keyClicked(KeyEvent* e)
{
}

void GameOverState::keyPressed(KeyEvent* e)
{
	//StateManager::getInstance()->pushState( HighScoreState::getInstance() );
	//StateManager::getInstance()->popState();
}

void GameOverState::keyReleased(KeyEvent* e)
{
}

bool GameOverState::frameStarted(const FrameEvent& evt)
{
	static float prevSecond;
	static float curSecond;

	if( 2.0f < curSecond - prevSecond )
	{
		//StateManager::getInstance()->pushState( HighScoreState::getInstance() );
		StateManager::getInstance()->popState();
	}
	else
	{
		curSecond += evt.timeSinceLastFrame;
	}

	return true;
}

bool GameOverState::frameEnded(const FrameEvent& evt)
{
	return true;
}

void GameOverState::runHighScore()
{
}