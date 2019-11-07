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
#include "GameMenuState.h"
#include "PlayState.h"

GameMenuState GameMenuState::mGameMenuState;

void GameMenuState::enter()
{
	//mResign = false;
	// TODO: it's test. later you should use right overlay name
	mOverlay = OverlayManager::getSingleton().getByName( "Menu/GameMenu" ); 

	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("GameMenu/ResumeGame"));
	mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("GameMenu/Resign"));

	MenuState::enter();
	//mRoot->startRendering();
}

void GameMenuState::onSelected(MenuItemList::iterator item)
{
	const std::string & name = (*item)->getName();

#ifdef _DEBUG
	cprintf( "select menu item: %s\n", name.c_str() );
#endif
	if( name == "GameMenu/ResumeGame" )
	{
		PlayState::getInstance()->mStatus = PlayState::Play;
		StateManager::getInstance()->popState();
	}
	else if( name == "GameMenu/Resign" )
	{
		//mResign = true;
		//PlayState::getInstance()->mStatus = PlayState::Resign;
		//StateManager::getInstance()->popState();

		mMenuItems.clear();
		mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("NotifyResignMenu/No"));
		mMenuItems.push_back(OverlayManager::getSingleton().getOverlayElement("NotifyResignMenu/Yes"));

		mOverlay->hide();
		mOverlay = OverlayManager::getSingleton().getByName( "Menu/ResignMenu" ); 

		MenuState::enter();
	}
	else if( name == "NotifyResignMenu/Yes" )
	{
		PlayState::getInstance()->mStatus = PlayState::Resign;
		StateManager::getInstance()->popState();
	}
	else if( name == "NotifyResignMenu/No" )
	{
		PlayState::getInstance()->mStatus = PlayState::Play;
		StateManager::getInstance()->popState();
	}
}