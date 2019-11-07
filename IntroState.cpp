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
#include "IntroState.h"

#include "MainMenuState.h"

IntroState IntroState::mIntroState;

void IntroState::enter()
{
	exitGame = false;

	mInputDevice = InputManager::getSingletonPtr()->getInputDevice();
	mRoot = Root::getSingletonPtr();

	//mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "IntroSceneManager" );
	mSceneMgr = mRoot->createSceneManager( ST_GENERIC );
	mSceneMgr->setShadowTechnique( Ogre::SHADOWDETAILTYPE_TEXTURE );

	mCamera = mSceneMgr->createCamera("IntroCamera");
	mCamera->setPosition( 0, 0, 300 );
	mCamera->lookAt( Ogre::Vector3::ZERO );

	mViewport = mRoot->getAutoCreatedWindow()->addViewport(mCamera);
	mViewport->setBackgroundColour(ColourValue( 0.5, 0.5, 0.5 ));


	//StateManager::getInstance()->mGUIRenderer->setTargetSceneManager( mSceneMgr );
	//StateManager::getInstance()->mGUIRenderer->doRender();
	//Overlay * overlay = OverlayManager::getSingleton().getByName( "Game/MousePanel" );
	//overlay->show();

#ifdef _DEBUG
	{
		Light* l = mSceneMgr->createLight("MainLight");
		l->setPosition(20,80,50);
		l->setCastShadows( true );
	
		mSceneMgr->setAmbientLight( Ogre::ColourValue( 0.5, 0.5, 0.5 ) );

		Entity * entity = mSceneMgr->createEntity( "test", "defaultContainer.mesh" );
		entity->setMaterialName( "CalcKing/DigitOne" );

		mSceneMgr->getRootSceneNode()->attachObject( entity );
		//mSceneMgr->setFog( FogMode::FOG_EXP2, ColourValue::White );
		//entity->setMaterialName( "CalcKing/DigitOneSelected" );
		//entity->setMaterialName( "CalcKing/OperatorQuestion" );

		//ParticleSystem* pSys5 = mSceneMgr->createParticleSystem("Aureola", "Examples/GreenyNimbus");
        //mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(pSys5);

		// Set nonvisible timeout
		//ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);

		//Ogre::mater
		/*
		Pass * barPass = entity->getSubEntity( 0 )->getMaterial()->getTechnique( 0 )->createPass();
		TextureUnitState * barTexture = barPass->createTextureUnitState();
		//static TextureUnitState barTexture( barPass );

		barTexture->setTextureName( "roundBar.png" );
		barTexture->setAnimatedTextureName( "roundBar.png", 22, 2.5f );
		barTexture->setEnvironmentMap( true, TextureUnitState::ENV_PLANAR );
		barTexture->setScrollAnimation( 0, 0 );

		barPass->setLightingEnabled( false );
		barPass->setAmbient( ColourValue::White );
		barPass->setDiffuse( ColourValue::White );
		barPass->setSceneBlending( SBT_TRANSPARENT_ALPHA );
		*/

	}
#endif
	/*
#ifdef _DEBUG
	cprintf("IntroState enter\n");
#endif
	//mSceneNode->attachObject( l );

	
	*/
	

	/*
	Light* l = mSceneMgr->createLight("PlayLight");
	l->setType( Light::LT_SPOTLIGHT );
	l->setPosition(1000,1200,500);
	l->setDiffuseColour( 0.5, 0.5, 0.5 );
	l->setSpecularColour( 1, 1, 1 );
	l->setCastShadows( true );
	l->setSpotlightRange( Radian( Degree( 40 ) ), Radian( Degree( 60 ) ) );
	l->setDirection( Vector3::ZERO );	
	*/

	//mSceneNode->attachObject( l );
	
	mRoot->startRendering();
}

void IntroState::exit()
{
	// remove added material
	//Entity * entity = mSceneMgr->getEntity( "test" );
	//entity->getSubEntity( 0 )->getTechnique()->getPass( 1 )->getTextureUnitState( 0 )->setScrollAnimation( 0, 0 );
	//entity->getSubEntity( 0 )->getTechnique()->removePass( 1 );
	mSceneMgr->clearScene();
	mSceneMgr->destroyAllCameras();
	mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause()
{
}

void IntroState::resume()
{
}

void IntroState::keyPressed(KeyEvent* e)
{
	StateManager::getInstance()->changeState( MainMenuState::getInstance() );
}

bool IntroState::frameStarted(const FrameEvent& evt)
{
	StateManager::getInstance()->changeState( MainMenuState::getInstance() );
	return true;
}

bool IntroState::frameEnded(const FrameEvent& evt)
{
	return true;
}

void IntroState::mousePressed( Ogre::MouseEvent * e )
{
	StateManager::getInstance()->changeState( MainMenuState::getInstance() );

	e->consume();
}

void IntroState::mouseMoved( Ogre::MouseEvent * e )
{
	//StateManager::getInstance()->mouseMoved
	//e->consume();
}