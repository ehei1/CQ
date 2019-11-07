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

class HelpState : public MenuState
{
public:
	void enter();

	void keyPressed(Ogre::KeyEvent* e);

	static HelpState* getInstance() { return &mHelpState; }
	
protected:
	HelpState() { }

	void onSelected( MenuItemList::iterator );

private:
	static HelpState mHelpState;

	size_t mIndex;
	std::deque< OverlayElement * > mOverlayElement;
};