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

class InputManager : public Ogre::Singleton<InputManager>
{
public:
	InputManager(Ogre::RenderWindow* rwindow);
	virtual ~InputManager();
	inline Ogre::InputReader* getInputDevice() const { return mInputDevice; }
	inline Ogre::EventProcessor* getEventProcessor() const { return mEventProcessor; }
	static InputManager& getSingleton(void);
	static InputManager* getSingletonPtr(void);
private:
	Ogre::EventProcessor* mEventProcessor;
	Ogre::InputReader* mInputDevice;
};