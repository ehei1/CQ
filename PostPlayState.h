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

class PostPlayState : public State
{
public:
	void enter();
	void exit();

	void pause();
	void resume();

	void keyClicked(Ogre::KeyEvent* e);
	void keyPressed(Ogre::KeyEvent* e);
	void keyReleased(Ogre::KeyEvent* e);
	void mousePressed( Ogre::MouseEvent * e );
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	static PostPlayState* getInstance() { return &mPostPlayState; }
	
protected:
	PostPlayState() {}

private:
	static PostPlayState mPostPlayState;

	Overlay * mOverlay;

	float mAverageTimeCount;
	float mRemainedTimeCount;
	float mAverageTime;
	float mRemainedTime;

	bool mNextStage;

	void skip();
};