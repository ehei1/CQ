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

class GameMenuState : public MenuState
{
public:
	void enter();

	static GameMenuState* getInstance() { return &mGameMenuState; }
	
protected:
	GameMenuState() { }

	void onSelected( MenuItemList::iterator );

private:
	static GameMenuState mGameMenuState;

	//bool mResign;
};