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
#include "HelpState.h"

HelpState HelpState::mHelpState;

void HelpState::enter()
{
	mOverlay = OverlayManager::getSingleton().getByName( "Menu/HelpMenu" );

	mOverlayElement.push_back( OverlayManager::getSingleton().getOverlayElement( "HelpMenu/Step1Container" ) );
	mOverlayElement.push_back( OverlayManager::getSingleton().getOverlayElement( "HelpMenu/Step2Container" ) );
	mOverlayElement.push_back( OverlayManager::getSingleton().getOverlayElement( "HelpMenu/Step3Container" ) );
	mOverlayElement.push_back( OverlayManager::getSingleton().getOverlayElement( "HelpMenu/Step4Container" ) );
	
	for( size_t i = 0; i < mOverlayElement.size(); ++ i )
	{
		mOverlayElement[ i ]->hide();
	}

	mOverlayElement.front()->show();

	MenuState::enter();
}

void HelpState::keyPressed(Ogre::KeyEvent* e)
{
	if( ::KC_ESCAPE == e->getKey() )
	{
		StateManager::getInstance()->popState();
	}
	else
	{
		mOverlayElement.front()->hide();
		mOverlayElement.pop_front();

		if( mOverlayElement.empty() )
		{
			StateManager::getInstance()->popState();
		}
		else
		{
			mOverlayElement.front()->show();
		}
	}

	e->consume();
}

void HelpState::onSelected( MenuItemList::iterator )
{
}