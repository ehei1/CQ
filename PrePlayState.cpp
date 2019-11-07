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
#include "PrePlayState.h"

#include "MainMenuState.h"
#include "SinglePlayState.h"
#include "PlayState.h"
#include "Stage.h"
#include "SoundManager.h"

PrePlayState PrePlayState::mPrePlayState;

void PrePlayState::enter()
{
#ifdef _DEBUG
	_cprintf( "PrePlayState::enter()\n" );
#endif
	static char buffer[ 80 ];
	sprintf( buffer, "Stage %d", SinglePlayState::getInstance()->mStageIndex + 1 );

	OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "StartPanel/StageText" );
	element->setCaption( buffer );

	mOverlay = OverlayManager::getSingleton().getByName( "Game/StartPanel" );
	mOverlay->show();

	{
		SoundManager::getSingleton().StopAllSounds();

		int index = SoundManager::getSingleton().CreateLoopedStream( PlayState::getInstance()->mStage.music );
		int channel = INVALID_SOUND_CHANNEL;

		SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
		SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
	}
}

void PrePlayState::exit()
{
	mOverlay->hide();
	PlayState::getInstance()->mStatus = PlayState::Play;

	// set overlay
}

void PrePlayState::pause()
{
}

void PrePlayState::resume()
{
}

void PrePlayState::keyClicked(KeyEvent* e)
{
}

void PrePlayState::mousePressed( MouseEvent * e )
{
	skip();
	e->consume();
}

void PrePlayState::keyPressed(KeyEvent* e)
{
	skip();
	e->consume();
}

void PrePlayState::keyReleased(KeyEvent* e)
{
	e->consume();
}

bool PrePlayState::frameStarted(const FrameEvent& evt)
{
	SceneNode * sceneNode = PlayState::getInstance()->mContainerSceneNode;
	sceneNode->yaw( Radian( Degree( 1 ) ) );

	Camera * camera = SinglePlayState::getInstance()->mCamera;
	camera->moveRelative( Vector3( 0, 0, -5 ) );

	if( camera->getPosition().z > PlayState::getInstance()->mStage.camera.position.z )
	{
		StateManager::getInstance()->popState();
	}
	
	return true;
}

bool PrePlayState::frameEnded(const FrameEvent& evt)
{
	return true;
}

void PrePlayState::skip()
{
	Camera * camera = SinglePlayState::getInstance()->mCamera;
	camera->setPosition( PlayState::getInstance()->mStage.camera.position );
	
	StateManager::getInstance()->popState();
}