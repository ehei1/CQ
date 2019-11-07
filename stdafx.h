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
#pragma message("Save compile time! Huge & common header should included here.\n")

#include <ogre.h>
#include <OgreEventListeners.h>
#include <OgreEventProcessor.h>
#include <OgreKeyEvent.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef _DEBUG
#include <conio.h>
#endif

using namespace Ogre;