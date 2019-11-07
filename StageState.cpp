#include "StdAfx.h"
#include "StageState.h"

#include "SinglePlayState.h"
#include "PauseState.h"
#include "MainMenuState.h"
#include "CongratulationState.h"

StageState StageState::mStageState;

void StageState::enter()
{
	const std::string file = SinglePlayState::getInstance()->getNextStage();

#ifdef _DEBUG
	_cprintf( "Stage loading: %s\n", file.c_str() );
#endif
	if( file.empty() )
	{
		// game all clear! change ConglatulationState
		GameManager::getInstance()->changeState( CongratulationState::getInstance() );
	}

	mStage.load( file );

	// build stage on distant place
	// set overlay
	// fade out
	// zooming stage


	// stage init: camera entity, viewport
	// put stage first effect
#ifdef _DEBUG
	_cprintf( "StageState::enter()\n" );
#endif
}

void StageState::exit()
{
#ifdef _DEBUG
	_cprintf( "StageState::exit()\n" );
#endif
}

void StageState::pause()
{
#ifdef _DEBUG
	_cprintf( "StageState::pause()\n" );
#endif
}

void StageState::resume()
{
#ifdef _DEBUG
	_cprintf( "StageState::resume()\n" );
#endif
}

void StageState::keyClicked( KeyEvent * e )
{
}

void StageState::keyReleased( KeyEvent * e )
{
}

bool StageState::frameStarted( const FrameEvent & evt )
{
	return true;
}

bool StageState::frameEnded( const FrameEvent & evt )
{
	return true;
}

void StageState::keyPressed(KeyEvent* e)
{
	if (e->getKey() == KC_P)
	{
		GameManager::getInstance()->pushState(PauseState::getInstance());
#ifdef _DEBUG
		_cprintf("SinglePlayState->PauseState\n");
#endif
	}
	else if (e->getKey() == KC_ESCAPE)
	{
		GameManager::getInstance()->popState();
#ifdef _DEBUG
		_cprintf("SinglePlayState->MainMenuState\n");
#endif
	}
	else
	{
		GameManager::getInstance()->changeState( StageState::getInstance() );
	}
}