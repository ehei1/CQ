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
#include "InputManager.h"

template<> InputManager* Ogre::Singleton<InputManager>::ms_Singleton = 0;

InputManager::InputManager(Ogre::RenderWindow* window)
{
	mEventProcessor = new Ogre::EventProcessor();
	mEventProcessor->initialise(window);
	mEventProcessor->startProcessingEvents();
	mInputDevice = mEventProcessor->getInputReader();
}

InputManager::~InputManager()
{
	if (mEventProcessor)
		delete mEventProcessor;
}

InputManager* InputManager::getSingletonPtr(void)
{
    return ms_Singleton;
}

InputManager& InputManager::getSingleton(void)
{
	assert(ms_Singleton);
	return *ms_Singleton;
}

