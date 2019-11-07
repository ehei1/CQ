/*
-----------------------------------------------------------------------------
CQ
Copyright 2006 Lee Ungju, Lee Junho

Also see acknowledgements in credit.txt

This program is free software; you can redistribute it and/or modify it under
uncommerical purpose.
-----------------------------------------------------------------------------
*/
#pragma once
#include "MenuState.h"

class HighScoreState : public MenuState
{
public:
	void enter();
	void exit();

	bool frameStarted( const FrameEvent & );
	void keyPressed(Ogre::KeyEvent* e);
	void onSelected(MenuItemList::iterator item) {};
	static HighScoreState* getInstance() { return &mHighScoreState; }

protected:
	HighScoreState(void) {};

	static HighScoreState mHighScoreState;
};
