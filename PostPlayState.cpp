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
#include "PostPlayState.h"

#include "MainMenuState.h"
#include "PlayState.h"
#include "Interface.h"
#include "SoundManager.h"

PostPlayState PostPlayState::mPostPlayState;

void PostPlayState::enter()
{
#ifdef _DEBUG
	_cprintf( "PostPlayState::enter()\n" );
#endif

	PlayState::getInstance()->mStatus = PlayState::Clear;

	mOverlay = OverlayManager::getSingleton().getByName( "Game/ResultPanel" );

	float total = 0;
	const std::vector< float > & time = PlayState::getInstance()->mSolvedTime;

	for( std::vector< float >::const_iterator it = time.begin(); time.end() != it; ++ it )
	{
		total += *it;
	}

	mAverageTime = total / time.size();
	mRemainedTime = PlayState::getInstance()->mPlayableTime;

	static char buffer[ 80 ];

	mNextStage = false;
	mRemainedTimeCount = 0;
	mAverageTimeCount = 0;
	
	OverlayElement * element;
	element = OverlayManager::getSingleton().getOverlayElement( "ResultPanel/RemainedTimeNumber" );
	element->setCaption( " " );
	element = OverlayManager::getSingleton().getOverlayElement( "ResultPanel/SolvedTimeNumber" );
	element->setCaption( " " );
	
	mOverlay->show();

	{
		SoundManager::getSingleton().StopAllSounds();

		int index = SoundManager::getSingleton().CreateLoopedStream( StateManager::getInstance()->mAudio.clearMusic );
		int channel = INVALID_SOUND_CHANNEL;

		SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
		SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
	}
}

void PostPlayState::exit()
{
	SoundManager::getSingleton().StopAllSounds();
	mOverlay->hide();
}

void PostPlayState::pause()
{
}

void PostPlayState::resume()
{
}

void PostPlayState::keyClicked(KeyEvent* e)
{
	// goto play
	e->consume();
}

void PostPlayState::keyPressed(KeyEvent* e)
{
	skip();
	e->consume();
}

void PostPlayState::mousePressed( MouseEvent * e )
{
	skip();
	e->consume();
}

void PostPlayState::keyReleased(KeyEvent* e)
{
	e->consume();
}

bool PostPlayState::frameStarted(const FrameEvent& evt)
{
	SceneNode * sceneNode = PlayState::getInstance()->mContainerSceneNode;
	sceneNode->yaw( Radian( Degree( 1 ) ) );

	if( mNextStage )
	{
		StateManager::getInstance()->popState();
		return true;
	}

	static float prevTime, curTime;

	if( 0.05 < curTime - prevTime )
	{
		prevTime = curTime;
	}
	else
	{
		curTime += evt.timeSinceLastFrame;
		return true;
	}
	
	if( mAverageTime == mAverageTimeCount &&
		mRemainedTime == mRemainedTimeCount )
	{
		mNextStage = true;
		return true;
	}
	else
	{
		static char buffer[ 80 ];
		OverlayElement * element;

		sprintf( buffer, "%0.1fsec", mRemainedTimeCount );
		element = OverlayManager::getSingleton().getOverlayElement( "ResultPanel/RemainedTimeNumber" );
		element->setCaption( buffer );

		sprintf( buffer, "%0.1fsec", mAverageTimeCount );
		element = OverlayManager::getSingleton().getOverlayElement( "ResultPanel/SolvedTimeNumber" );
		element->setCaption( buffer );

		if( SoundManager::getSingleton().isValid() ) 
		{
			static int index = SoundManager::getSingleton().CreateStream( StateManager::getInstance()->mAudio.countingSound );
			static int channel = INVALID_SOUND_CHANNEL;

			SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
			SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
		}
	}

	if( mRemainedTime <= mRemainedTimeCount )
	{
		mRemainedTimeCount = mRemainedTime;
	}
	else
	{
		mRemainedTimeCount += 0.1;
	}

	if( mAverageTime <= mAverageTimeCount )
	{
		mAverageTimeCount = mAverageTime;
	}
	else
	{
		mAverageTimeCount += 0.1;
	}

	return true;
}

bool PostPlayState::frameEnded(const FrameEvent& evt)
{
	//return Interface::getInstance()->frameEnded( evt );
	return true;
}

void PostPlayState::skip()
{
	if( mNextStage )
	{
		StateManager::getInstance()->popState();	
	}
	else
	{
		static char buffer[ 80 ];
		
		OverlayElement * element;
		
		sprintf( buffer, "%0.1fsec", mRemainedTime );
		element = OverlayManager::getSingleton().getOverlayElement( "ResultPanel/RemainedTimeNumber" );
		element->setCaption( buffer );

		sprintf( buffer, "%0.1fsec", mAverageTime );
		element = OverlayManager::getSingleton().getOverlayElement( "ResultPanel/SolvedTimeNumber" );
		element->setCaption( buffer );

		mNextStage = true;
	}
}