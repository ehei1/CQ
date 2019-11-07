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

class GameOverState : public State
{
public:
	void enter();
	void exit();

	void pause();
	void resume();

	void keyClicked(Ogre::KeyEvent* e);
	void keyPressed(Ogre::KeyEvent* e);
	void keyReleased(Ogre::KeyEvent* e);
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	static GameOverState* getInstance() { return &mGameOverState; }
protected:
	GameOverState() { }

private:
	static GameOverState mGameOverState;

	void runHighScore();

	Overlay * mOverlay;
	bool mPass;
};