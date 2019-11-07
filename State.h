/*
-----------------------------------------------------------------------------
CQ
Copyright 2006 Lee Ungju, Lee Junho

Also see acknowledgements in credit.txt

This program is free software; you can redistribute it and/or modify it under
uncommerical purpose.
-----------------------------------------------------------------------------
*/
#ifndef State_H
#define State_H

//#include "Ogre.h"
 
#include "StateManager.h"

class State
{
public:
	virtual void enter() = 0;
	virtual void exit() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void keyClicked(Ogre::KeyEvent* e) { e->consume(); }
	virtual void keyPressed(Ogre::KeyEvent* e) { e->consume(); }
	virtual void keyReleased(Ogre::KeyEvent* e) { e->consume(); }
	virtual void keyRepeated( int ) {}

	virtual void mouseMoved(MouseEvent *e) { e->consume(); }
	virtual void mouseDragged(MouseEvent *e) { e->consume(); }

	// MouseListener overrides
	virtual void mousePressed(MouseEvent *e) { e->consume(); }
	virtual void mouseReleased(MouseEvent *e) { e->consume(); }
	virtual void mouseClicked(MouseEvent* e) { e->consume(); }
	virtual void mouseEntered(MouseEvent* e) { e->consume(); }
	virtual void mouseExited(MouseEvent* e) { e->consume(); }

	virtual bool frameStarted(const Ogre::FrameEvent& evt) { return true; }
	virtual bool frameEnded(const Ogre::FrameEvent& evt) { return true; }

protected:
	State() { }
};

#endif
