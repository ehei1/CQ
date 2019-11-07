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
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include "OgreCEGUIRenderer.h"
#include "SoundManager.h"

#include <conio.h>

//#include <OgrePanelOverlayElement.h> 
#include "StateManager.h"

#include "InputManager.h"
#include "State.h"

StateManager StateManager::mStateManager;

StateManager::StateManager()
{
	mMousePanelOverlay = 0;
	soundMgr = 0;
}

StateManager::~StateManager()
{
	// clean up all the states
	while (!states.empty()) {
		states.back()->exit();
		states.pop_back();
	}

	if (mInputManager)
		delete mInputManager;

	if( soundMgr )
		delete soundMgr;

	/*
	if(mGUIRenderer)
	{
		delete mGUIRenderer;
		mGUIRenderer = 0;
	}

	if(mGUISystem)
	{
		delete mGUISystem;
		mGUISystem = 0;
	}
	*/

	if (mRoot)
		delete mRoot;
};

void StateManager::start(void)
{
	mInitialRepeatKeyDelay = 0.15;
	mContinousKeyRepeatDelay = 0.05;

#ifdef _DEBUG
	mRoot = new Root;
#else
	std::string path;
	getInstallPath( path );

	mRoot = new Root( path + "\\plugins.cfg", "Ogre.cfg", path + "\\Ogre.log" );
#endif
	
	setupResources();
#ifdef _DEBUG
	cprintf( "StateManager::start(void)\n" );
	cprintf( "\t setupResources()\n" );
#endif

	if (!configure()) return;

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// mouse panel
	{
		mMousePanelOverlay = OverlayManager::getSingleton().getByName( "Game/MousePanel" ); // default cursor
		mCursorOverlayElement = OverlayManager::getSingleton().getOverlayElement( "MousePanel/Cursor" );

		// others is hide
		OverlayManager::getSingleton().getOverlayElement( "MousePanel/RotationCursor" )->hide();

		//mMousePanelOverlay->show();
		//showMouseCursor( true );
	}

	mRoot->addFrameListener(this);

	mInputManager = new InputManager(mRoot->getAutoCreatedWindow());
	mInputManager->getEventProcessor()->addKeyListener(this);
	mInputManager->getEventProcessor()->addMouseListener( this );
	mInputManager->getEventProcessor()->addMouseMotionListener( this );

	//mRoot->getAutoCreatedWindow()->addListener( this );
	mRunning = true;

	setAudio();
}

void StateManager::changeState(State* state)
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->exit();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->enter();
}

void StateManager::pushState(State* state)
{
	// pause current state
	if ( !states.empty() ) {
		states.back()->pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->enter();
}

void StateManager::popState()
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->exit();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() ) {
		states.back()->resume();
	}
}

void StateManager::setupResources(void)
{
	ConfigFile cf;
#ifdef _DEBUG
	cf.load( "resources.cfg");
#else
	std::string path;
	getInstallPath( path );

	cf.load( path + "\\resources.cfg");
#endif

	// go through all settings in the file

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String secName, typeName, archName;

	while(seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;

		for(i=settings->begin(); i!=settings->end(); ++i){
			typeName = i->first;
			archName = i->second;
#ifdef _DEBUG
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
#else
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation( path + '/' + archName, typeName, secName);
			//Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
#endif
		}
	}
}

bool StateManager::configure(void)
{
#ifdef _DEBUG
	cprintf( "StateManager::configure(void)\n" );
#endif

	std::string videoMode;
	std::string antiAliasing;
	std::string fullscreen;

	{
#ifdef _DEBUG
		std::ifstream file( "system.cfg" );
#else
		std::string path;
		getInstallPath( path );
		path += "\\system.cfg";
		std::ifstream file( path.c_str() );
#endif

		char buffer[ 80 ];
		file.getline( buffer, 80 );
		videoMode = buffer;

		file.getline( buffer, 80 );
		antiAliasing = buffer;

		file.getline( buffer, 80 );
		fullscreen = buffer;
	}

	RenderSystemList *rsList = mRoot->getAvailableRenderers();

	int c=0;
	bool foundit = false;
	RenderSystem *selectedRenderSystem=0;

#ifdef _DEBUG
	cprintf( "\t rsList->size(): %d\n", rsList->size() );
#endif
	while(c < (int) rsList->size())
	{
		selectedRenderSystem = rsList->at(c);
		const String & rname = selectedRenderSystem->getName();
		if(rname.compare("Direct3D9 Rendering Subsystem")==0)
		{
			foundit=true;
			break;
		}
		c++; // <-- oh how clever
	}

	if(!foundit)
	{
#ifdef _DEBUG
		cprintf( "rendering system not found\n" );
#endif
		return FALSE; //we didn't find it...
	}

	//we found it, we might as well use it!
	mRoot->setRenderSystem(selectedRenderSystem);

	selectedRenderSystem->setConfigOption("Full Screen",fullscreen);  
	selectedRenderSystem->setConfigOption("Video Mode",videoMode);
	selectedRenderSystem->setConfigOption("Anti aliasing",antiAliasing);  

	//and now we need to run through all of it

	// initialise and create a default rendering window
	mWindow = mRoot->initialise(true);
	return true;
}

void StateManager::keyClicked(KeyEvent* e)
{
	// call keyClicked of current state
	states.back()->keyClicked( e );
}

void StateManager::keyPressed(KeyEvent* e)
{
	states.back()->keyPressed( e );

	mRepeatKey = e->getKey();
	mRepeatKeyDelay = mInitialRepeatKeyDelay;

	states.back()->keyRepeated(mRepeatKey);
}

void StateManager::keyReleased(KeyEvent* e)
{
	// call keyReleased of current state
	states.back()->keyReleased( e );

	if (e->getKey() == mRepeatKey)
	{
		mRepeatKey = -1;
	}
}

void StateManager::mouseMoved(MouseEvent *e)
{
	/*
	CEGUI::System::getSingleton().injectMouseMove(
		e->getRelX() * mGUIRenderer->getWidth(), 
		e->getRelY() * mGUIRenderer->getHeight()
	);
	*/
	
	states.back()->mouseMoved( e );
}

void StateManager::mouseDragged(MouseEvent *e)
{
	states.back()->mouseDragged( e );
}

void StateManager::mousePressed(MouseEvent *e)
{
	states.back()->mousePressed( e );
}

void StateManager::mouseReleased(MouseEvent *e)
{
	states.back()->mouseReleased( e );
}

void StateManager::mouseClicked(MouseEvent* e)
{
	states.back()->mouseClicked( e );
}

void StateManager::mouseEntered(MouseEvent* e)
{
	states.back()->mouseEntered( e );
}

void StateManager::mouseExited(MouseEvent* e)
{
	states.back()->mouseExited( e );
}

bool StateManager::frameStarted(const FrameEvent& evt)
{
	// Send key repeated events
	if (mRepeatKey != -1)
	{
		mRepeatKeyDelay -= evt.timeSinceLastFrame;
		while (mRepeatKeyDelay <= 0.0)
		{
			states.back()->keyRepeated(mRepeatKey);
			mRepeatKeyDelay += mContinousKeyRepeatDelay;
		}
	}

	return ( mRunning ) ? states.back()->frameStarted( evt ) : false;
}

bool StateManager::frameEnded(const FrameEvent& evt)
{
	// call frameEnded of current state
	return states.back()->frameEnded( evt );
}

void StateManager::quit()
{
	mRunning = false;
}

void StateManager::showMouseCursor( const bool & isShow )
{
#ifdef _DEBUG
	cprintf( "StateManager::showMouseCursor( %d )\n", isShow );
	cprintf( "\t mMousePanelOverlay: %ld\n", mMousePanelOverlay );
#endif
	if( mMousePanelOverlay )
	{
		( isShow ) ? mMousePanelOverlay->show() : mMousePanelOverlay->hide();
	}
}

void StateManager::changeMouseCursor( const CursorStatus & s )
{
	// all hide
	OverlayManager::getSingleton().getOverlayElement( "MousePanel/Cursor" )->hide();
	OverlayManager::getSingleton().getOverlayElement( "MousePanel/RotationCursor" )->hide();
	
	switch ( s )
	{
	case CS_Moved:
		mCursorOverlayElement = OverlayManager::getSingleton().getOverlayElement( "MousePanel/Cursor" );
		break;

	case CS_Rotated:
		mCursorOverlayElement = OverlayManager::getSingleton().getOverlayElement( "MousePanel/RotationCursor" );
		break;

	default:
		break;
	}

	mCursorOverlayElement->show();
}

void StateManager::saveScreenShot()
{
	mWindow->writeContentsToTimestampedFile( "CQ", ".PNG" );
}

void StateManager::getInstallPath( std::string & path )
{
	/*
	reference: http://wooya510.wordpress.com/2006/06/26//
	*/

	LONG lRet;
	HKEY hKey;
	DWORD dwByte, dwType;
	static char buffer[ 80 ];
	
	lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Software\\ehei Games", 0,
		KEY_ALL_ACCESS, &hKey );
	
	if( ERROR_SUCCESS != lRet )
	{
#ifdef _DEBUG
		cprintf( "Registry faulted! Install again please\n" );
#endif
	}
	
	lRet = RegQueryValueEx( hKey, "InstallPath", 0, &dwType,
		( unsigned char * )buffer, &dwByte );
		
	RegCloseKey( hKey );
	
	if( ERROR_SUCCESS != lRet )
	{
#ifdef _DEBUG
		cprintf( "Registry faulted! Install again please\n" );
#endif
	}

	path = buffer;
}


void StateManager::setAudio()
{
	soundMgr = new SoundManager;
	soundMgr->Initialize();

	ConfigFile cf;
#ifdef _DEBUG
	cf.load("audio.cfg");
#else
	std::string path;
	StateManager::getInstance()->getInstallPath( path );

	cf.load( path + "\\audio.cfg" );
#endif

	ConfigFile::SectionIterator seci = cf.getSectionIterator();
	String secName, typeName, archName;

	while(seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap* settings = seci.getNext();

		if( "Audio" != secName )
			continue;

		for( ConfigFile::SettingsMultiMap::iterator i = settings->begin();
			i != settings->end(); ++i )
		{
			if( "titleMusic" == i->first )
			{
				mAudio.titleMusic = i->second.c_str();
			}
			else if( "startMusic" == i->first )
			{
				mAudio.startMusic = i->second.c_str();
			}
			else if( "clearMusic" == i->first )
			{
				mAudio.clearMusic = i->second.c_str();
			}
			else if( "rankingMusic" == i->first )
			{
				mAudio.rankingMusic = i->second.c_str();
			}
			else if( "gameOverMusic" == i->first )
			{
				mAudio.gameOverMusic = i->second.c_str();
			}
			else if( "congratulationMusic" == i->first )
			{
				mAudio.congratulationMusic = i->second.c_str();
			}
			else if( "clickBallSound" == i->first )
			{
				mAudio.clickBallSound = i->second.c_str();
			}
			else if( "collectAnswerSound" == i->first )
			{
				mAudio.collectAnswerSound = i->second.c_str();
			}
			else if( "factorProblemSound" == i->first )
			{
				mAudio.factorProblemSound = i->second.c_str();
			}
			else if( "rotateContainerSound" == i->first )
			{
				mAudio.rotateContainerSound = i->second.c_str();
			}
			else if( "countingSound" == i->first )
			{
				mAudio.countingSound = i->second.c_str();
			}
		}
	}
}

