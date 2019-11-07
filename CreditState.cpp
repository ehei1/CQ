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
#include "CreditState.h"

#include "MainMenuState.h"

CreditState CreditState::mCreditState;

void CreditState::enter()
{
#ifdef _DEBUG
	cprintf( "CreditState::enter()\n" );
#endif
	mOverlay = OverlayManager::getSingleton().getByName( "Menu/CreditMenu" );

	OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "CreditMenu/ScrollPanel" );
	element->setTop( 0.5 );

	/*
	OverlayContainer * container = static_cast< OverlayContainer * >(
		OverlayManager::getSingleton().getOverlayElement( "HighScorePanel/Rank" )
	);
	*/
	
	MenuState::enter();
}

void CreditState::keyPressed(Ogre::KeyEvent *e)
{
	StateManager::getInstance()->popState();
}

void CreditState::exit()
{
	MenuState::exit();
}

bool CreditState::frameStarted( const Ogre::FrameEvent & evt )
{
	static float prevSecond;
	static float curSecond;
	static OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "CreditMenu/ScrollPanel" );
	static bool isShow;

	if( 0.05f < curSecond - prevSecond )
	{
		if( element->getTop() < -3.5 )
		{
			StateManager::getInstance()->popState();
			return true;
		}

		element->setTop( element->getTop() - 0.003 );
		//element->setTop( element->getTop() - 0.006 );
		prevSecond = curSecond;
	}
	else
	{
		curSecond += evt.timeSinceLastFrame;
	}

	return true;
}