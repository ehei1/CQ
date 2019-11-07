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
#include "CongratulationState.h"

#include "MainMenuState.h"
#include "RankState.h"
#include "SinglePlayState.h"
#include "StateManager.h"

CongratulationState CongratulationState::mCongratulationState;

void CongratulationState::enter()
{
	mRoot = Root::getSingletonPtr();

	//mSceneMgr = mRoot->createSceneManager( ST_GENERIC, "SinglePlaySceneManager" );
	mSceneMgr = mRoot->createSceneManager( ST_GENERIC );
	mSceneMgr->setShadowTechnique( Ogre::SHADOWDETAILTYPE_TEXTURE ); // TODO: constants are controlled by first.calc. fix Data class
	
	mSceneNode = mSceneMgr->createSceneNode( "SinglePlaySceneNode" );
	mCamera = mSceneMgr->createCamera("SinglePlayCamera");

	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
	mViewport->setBackgroundColour(ColourValue(1.0, 1.0, 0.0));

	// put any video or image

#ifdef _DEBUG
	_cprintf( "CongratulationState::enter()\n" );
#endif
	//StateManager::getInstance()->popState();
	mOverlay = OverlayManager::getSingleton().getByName( "Game/CongratulationPanel" );
	mOverlay->show();

	//mOverlay->show();	
	mRoot->startRendering();

	{
		SoundManager::getSingleton().StopAllSounds();

		int index = SoundManager::getSingleton().CreateLoopedStream( StateManager::getInstance()->mAudio.congratulationMusic );
		int channel = INVALID_SOUND_CHANNEL;

		SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
		SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
	}
}

void CongratulationState::exit()
{
	SoundManager::getSingleton().StopAllSounds();

	OverlayManager::getSingleton().getOverlayElement( "CongratulationPanel/MainText" )->show();
	OverlayManager::getSingleton().getOverlayElement( "CongratulationPanel/StageText" )->show();

	mOverlay->hide();
	
	mSceneMgr->clearScene();
	mSceneMgr->destroyAllCameras();
	//mSceneMgr->destroySceneNode( "SinglePlaySceneNode" );
	mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void CongratulationState::pause()
{	
	OverlayManager::getSingleton().getOverlayElement( "CongratulationPanel/MainText" )->hide();
	OverlayManager::getSingleton().getOverlayElement( "CongratulationPanel/StageText" )->hide();
	//mOverlay->hide();
}

void CongratulationState::resume()
{
	//StateManager::getInstance()->popState();
	StateManager::getInstance()->changeState( MainMenuState::getInstance() );
	//StateManager::getInstance()->changeState( MainMenuState::getInstance() );
}

void CongratulationState::keyClicked(KeyEvent* e)
{
	e->consume();
}

void CongratulationState::keyPressed(KeyEvent* e)
{
	e->consume();
}

void CongratulationState::keyReleased(KeyEvent* e)
{
	e->consume();
}

bool CongratulationState::frameStarted(const FrameEvent& evt)
{
	static float curSecond, prevSecond;

	if( 2.0f < curSecond - prevSecond )
	{
		StateManager::getInstance()->pushState( RankState::getInstance() );
		prevSecond = curSecond;
	}
	else
	{
		curSecond += evt.timeSinceLastFrame;
	}

	//StateManager::getInstance()->changeState( MainMenuState::getInstance() );
	return true;
}

bool CongratulationState::frameEnded(const FrameEvent& evt)
{
	return true;
}
