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

#include "State.h"

class MenuState : public State
{
public:
	void enter();
	void exit();
	void pause();
	void resume();

	void keyPressed(KeyEvent* e);

protected:
	MenuState(void) {}

	typedef std::vector< Ogre::OverlayElement * > MenuItemList;
	MenuItemList mMenuItems;
	MenuItemList::iterator mSelectedItem;
	virtual void setItemState(MenuItemList::iterator item, bool selected);
	virtual void onSelected( MenuItemList::iterator ) = 0;

	Ogre::Overlay * mOverlay;

private:
	static MenuState mMenuState;
};
