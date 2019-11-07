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
#include "MenuState.h"

void MenuState::enter()
{
	mSelectedItem = mMenuItems.begin();

	for (MenuItemList::iterator i = mMenuItems.begin(); i != mMenuItems.end(); ++i)
	{
		setItemState(i, i == mSelectedItem);
	}

	if( mOverlay )
	{
		mOverlay->show();
	}

	//mRoot->startRendering();
	//Root::getSingleton().startRendering();
}

void MenuState::exit()
{
	mMenuItems.clear();
	if( mOverlay )
	{
		mOverlay->hide();
		mOverlay = 0;
	}
}

void MenuState::pause()
{
}

void MenuState::resume()
{
}

void MenuState::keyPressed(KeyEvent* e)
{
	// Handle keys common to all menu states
	switch ( e->getKey() )
	{
	case KC_RETURN:
	case KC_NUMPADENTER:
		onSelected(mSelectedItem);
		break;

	case KC_UP:
		setItemState(mSelectedItem, false);
		if (mSelectedItem != mMenuItems.begin())
			--mSelectedItem;
		else
			mSelectedItem = mMenuItems.end() - 1;

		setItemState(mSelectedItem, true);
		break;

	case KC_DOWN:
		setItemState(mSelectedItem, false);
		if (++mSelectedItem == mMenuItems.end())
			mSelectedItem = mMenuItems.begin();

		setItemState(mSelectedItem, true);
		break;
	}

	e->consume();
}

void MenuState::setItemState(MenuItemList::iterator item, bool selected)
{
	OverlayElement* element = *item;

	// Apply hardcoded menu item colours
	if (selected)
	{
		element->setParameter("colour_top", "1.0 1.0 0.0");
		element->setParameter("colour_bottom", "0.8 0.8 0.0");
	}
	else
	{
		element->setParameter("colour_top", "1.0 1.0 1.0");
		element->setParameter("colour_bottom", "0.8 0.8 0.8");
	}
}
