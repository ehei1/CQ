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

class HighScoreState;
class RankState;

class MainMenuState : public MenuState
{
public:
	friend class HighScoreState;
	friend class RankState;

	void enter();
	void exit();
	void pause();
	void resume();

	bool frameStarted( const FrameEvent & );

	static MainMenuState* getInstance() { return & mMainMenuState; }
protected:
	MainMenuState() {}
	virtual void onSelected(MenuItemList::iterator item);

	Ogre::Root *mRoot;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Viewport* mViewport;
	Ogre::InputReader* mInputDevice;
	Ogre::Camera* mCamera;
	Ogre::SceneNode * mSceneNode;

private:
	static MainMenuState mMainMenuState;

	void loadRank();
	void saveRank();

	typedef std::pair< std::string, float > Result; // key: name, value: cq
	std::list< Result > mRank;
};