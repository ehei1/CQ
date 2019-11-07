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

class IntroState : public State
{
public:
	void enter();
	void exit();

	void pause();
	void resume();

	//void keyClicked(Ogre::KeyEvent* e);
	void keyPressed(Ogre::KeyEvent* e);
	//void keyReleased(Ogre::KeyEvent* e);

	void mousePressed( Ogre::MouseEvent * e );
	void mouseMoved( Ogre::MouseEvent * e );

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	static IntroState* getInstance() { return &mIntroState; }
protected:
	IntroState() { }

	Ogre::Root *mRoot;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Viewport* mViewport;
	Ogre::InputReader* mInputDevice;
	Ogre::Camera* mCamera;
	bool exitGame;
private:
	static IntroState mIntroState;
};