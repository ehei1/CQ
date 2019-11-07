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

#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <OgreCEGUIRenderer.h>

#include "InputManager.h"
#include "SoundManager.h"
//#include <OgrePanelOverlayElement.h> 

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

class State;

class StateManager : public Ogre::FrameListener, public Ogre::KeyListener, 
	public Ogre::MouseListener, public Ogre::MouseMotionListener
{
public:
	enum CursorStatus
	{
		CS_Moved, CS_Rotated, CS_Pressed, CS_Released
	} mCursorStatus;
	
	~StateManager();
	void start(void);
	void changeState(State* state);
	void pushState(State* state);
	void popState();
	//void popState( State * state ); // after run defined state, exit current state
	void quit();

	static StateManager * getInstance() { return & mStateManager; }
	void showMouseCursor( const bool & ); // for overlay mouse cursor
	OverlayElement * getMouseCursor() { return mCursorOverlayElement; }
	void StateManager::changeMouseCursor( const CursorStatus & );
	void saveScreenShot();
	void getInstallPath( std::string & );
	//void setMousePosition( const float &, const float & );

	//CEGUI::OgreCEGUIRenderer * getGUIRenderer() { return mGUIRenderer; }
	struct Audio
	{
		std::string titleMusic;
		std::string startMusic;
		std::string clearMusic;
		std::string rankingMusic;
		std::string gameOverMusic;
		std::string congratulationMusic;

		std::string clickBallSound;
		std::string collectAnswerSound;
		std::string factorProblemSound;
		std::string rotateContainerSound;
		std::string countingSound;
	} mAudio;

protected:
	StateManager();
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mWindow;
	Ogre::EventProcessor* mEventProcessor;
	InputManager* mInputManager;

	void setupResources(void);
	bool configure(void);
	void keyClicked(Ogre::KeyEvent* e);
	void keyPressed(Ogre::KeyEvent* e);
	void keyReleased(Ogre::KeyEvent* e);

	void mouseMoved(MouseEvent *e);
	void mouseDragged(MouseEvent *e);
	// MouseListener overrides
	void mousePressed(MouseEvent *e);
	void mouseReleased(MouseEvent *e);
	void mouseClicked(MouseEvent* e);
	void mouseEntered(MouseEvent* e);
	void mouseExited(MouseEvent* e);

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	bool mRunning;
	void setAudio();

	SoundManager * soundMgr;

private:
	std::vector<State*> states;
	
	static StateManager mStateManager;

	Ogre::Overlay * mMousePanelOverlay;
	Ogre::OverlayElement * mCursorOverlayElement;

	int mRepeatKey;
	float mInitialRepeatKeyDelay;
	float mContinousKeyRepeatDelay;
	float mRepeatKeyDelay;
};