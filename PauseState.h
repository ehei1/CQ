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

class PauseState : public MenuState
{
public:
	void enter();

	void keyPressed(Ogre::KeyEvent* e);

	static PauseState* getInstance() { return &mPauseState; }
	
protected:
	PauseState() { }

	void onSelected( MenuItemList::iterator );

private:
	static PauseState mPauseState;
};