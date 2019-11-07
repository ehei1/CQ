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
#include "SinglePlayState.h"

#include "IntroState.h"
#include "PlayState.h"
#include "MainMenuState.h"
#include "GameOverState.h"
#include "CongratulationState.h"

SinglePlayState SinglePlayState::mSinglePlayState;

void SinglePlayState::enter()
{
#ifdef _DEBUG
	cprintf("SinglePlayState enter\n");
#endif
	restoreConfig();

	mStageIndex = 0;
	mCQ = 0;
	mSinglePlayExit = false;

	mRoot = Root::getSingletonPtr();

	mSceneMgr = mRoot->createSceneManager( ST_GENERIC );
	mSceneMgr->setShadowTechnique( SHADOWDETAILTYPE_TEXTURE ); // TODO: constants are controlled by first.calc. fix Data class
	
	mSceneNode = mSceneMgr->createSceneNode( "SinglePlaySceneNode" );
	mCamera = mSceneMgr->createCamera("SinglePlayCamera");

	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
	mViewport->setBackgroundColour(ColourValue(0.0, 1.0, 0.0));

	mWidth = SinglePlayState::getInstance()->mRoot->getAutoCreatedWindow()->getWidth();
	mHeight = SinglePlayState::getInstance()->mRoot->getAutoCreatedWindow()->getHeight();

	//mRoot->renderOneFrame();
	mRoot->startRendering();
}

void SinglePlayState::exit()
{ 
#ifdef _DEBUG
	cprintf("SinglePlayState exit\n");
#endif
	mSceneMgr->clearScene();
	mSceneMgr->destroyAllCameras();
	mRoot->getAutoCreatedWindow()->removeAllViewports();

	saveCache();
}

void SinglePlayState::pause()
{
#ifdef _DEBUG
	cprintf("SinglePlayState pause\n");
#endif
	mSceneMgr->getRootSceneNode()->removeChild( mSceneNode );
}

void SinglePlayState::resume()
{
#ifdef _DEBUG
	cprintf("SinglePlayState resume\n");
#endif
	mSceneMgr->getRootSceneNode()->addChild( mSceneNode );

	if( mSinglePlayExit )
	{
		StateManager::getInstance()->changeState( MainMenuState::getInstance() );
		return;
	}
	
	if( PlayState::GameOver == PlayState::getInstance()->mStatus ||
		PlayState::Resign == PlayState::getInstance()->mStatus || 
		PlayState::TimeOver == PlayState::getInstance()->mStatus )
	{
		mSinglePlayExit = true;
		return;
	}

	if( mStageIndex >= mStageVec.size() - 1 )
	{
		mSinglePlayExit = true;
		StateManager::getInstance()->changeState( CongratulationState::getInstance() );
		return;
	}

	++ mStageIndex;
	StateManager::getInstance()->pushState( PlayState::getInstance() );
}

void SinglePlayState::keyClicked(KeyEvent* e)
{
	e->consume();
}

void SinglePlayState::keyPressed(KeyEvent* e)
{
#ifdef _DEBUG
	cprintf( "SinglePlayState::keyPressed()\n" );
#endif
}

void SinglePlayState::keyReleased(KeyEvent* e)
{
	e->consume();
}

bool SinglePlayState::frameStarted(const FrameEvent& evt)
{
	StateManager::getInstance()->pushState( PlayState::getInstance() );
	return true;
}

bool SinglePlayState::frameEnded(const FrameEvent& evt)
{
	return true;
}

void SinglePlayState::restoreConfig()
{
#ifdef _DEBUG
	cprintf( "SinglePlayState::restoreConfig()\n" );
#endif
	if( mMaterialMap.size() || mStageVec.size() )
	{
		return;
	}

	loadCache();
	
	ConfigFile cf;
#ifdef _DEBUG
	cf.load("CQ.cfg");
#else
	std::string path;
	StateManager::getInstance()->getInstallPath( path );

	cf.load( path + "\\CQ.cfg" );
#endif

	ConfigFile::SectionIterator seci = cf.getSectionIterator();
	String secName, typeName, archName;
	while(seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap* settings = seci.getNext();

		if( "Material" == secName )
		{
			setMaterialMap( settings );
		}
		else if( "Stage" == secName )
		{
			setStageVec( settings );
		}
		else if( "Value" == secName )
		{
			setValue( settings );
		}
	}
}

void SinglePlayState::setValue(ConfigFile::SettingsMultiMap * s)
{
	for( ConfigFile::SettingsMultiMap::iterator i = s->begin();
		i != s->end(); ++i )
	{
		if( "regenerationTime" == i->first )
		{
			mValue.regenerationTime = atof( i->second.c_str() );
		}
		else if( "cameraMinPosZ" == i->first )
		{
			mValue.cameraMinPosZ = atof( i->second.c_str() );
		}
		else if( "cameraMaxPosZ" == i->first )
		{
			mValue.cameraMaxPosZ = atof( i->second.c_str() );
		}
		else if( "cameraZoomStep" == i->first )
		{
			mValue.cameraZoomStep = atof( i->second.c_str() );
		}
	}
}

void SinglePlayState::setMaterialMap(ConfigFile::SettingsMultiMap * s)
{
	for( ConfigFile::SettingsMultiMap::iterator i = s->begin();
		i != s->end(); ++i )
	{
		mMaterialMap[ i->first ] = i->second;
		mValueMap[ i->second ] = i->first;
	}
}

void SinglePlayState::setStageVec(ConfigFile::SettingsMultiMap * s)
{
	for( ConfigFile::SettingsMultiMap::iterator i = s->begin();
		i != s->end(); ++i )
	{
		mStageVec.push_back( i->second );
	}
}

void SinglePlayState::loadCache()
{
#ifdef _DEBUG
	std::ifstream file( "CQ.cache" );
#else
	std::string path;
	StateManager::getInstance()->getInstallPath( path );
	path += "\\CQ.cache";

	std::ifstream file( path.c_str() );
#endif

	std::string line;

	mCache.clear();
	
	while( file )
	{
		file >> line;
		
		size_t index = line.find( "=" );
		const std::string operation = line.substr( 0, index );
		std::string result = line.substr( index + 1, line.size() );

		CacheData cache;
		index = result.find( "," );
		cache.answer = result.substr( 0, index );

		result = result.substr( index + 1, result.size() );
		index = result.find( "," );
		cache.formular = result.substr( 0, index );
		cache.score = result.substr( index + 1, result.size() );

#ifdef _DEBUG
		cprintf( "\n" );
		cprintf( "\t answer: %s\n", cache.answer.c_str() );
		cprintf( "\t formular: %s\n", cache.formular.c_str() );
		cprintf( "\t score: %s\n", cache.score.c_str() );
#endif
		mCache[ operation ] = cache;
	}
}

void SinglePlayState::saveCache()
{
#ifdef _DEBUG
	std::ofstream file( "CQ.cache" );
#else
	std::string path;
	StateManager::getInstance()->getInstallPath( path );
	path += "\\CQ.cache";

	std::ofstream file( path.c_str() );
#endif

	for( std::map< std::string, CacheData >::const_iterator it = mCache.begin();
		mCache.end() != it; ++ it )
	{
		file << it->first << '=' << it->second.answer << ',' << it->second.formular << ',' << it->second.score << std::endl;
	}
}