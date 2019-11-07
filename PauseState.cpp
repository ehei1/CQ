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
#include "PauseState.h"

PauseState PauseState::mPauseState;

void PauseState::enter()
{
	mOverlay = OverlayManager::getSingleton().getByName( "Menu/PauseMenu" );

	MenuState::enter();

	//mRoot->startRendering();
}

void PauseState::keyPressed(Ogre::KeyEvent* e)
{
	StateManager::getInstance()->popState();
}

void PauseState::onSelected( MenuItemList::iterator )
{
}