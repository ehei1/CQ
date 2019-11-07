/*
-----------------------------------------------------------------------------
CQ
Copyright 2006 Lee Ungju, Lee Junho

Also see acknowledgements in credit.txt

This program is free software; you can redistribute it and/or modify it under
uncommerical purpose.
-----------------------------------------------------------------------------
*/
#include "StdAfx.h"
#include "PlayState.h"

#include "PauseState.h"
#include "GameMenuState.h"
#include "SinglePlayState.h"
#include "PrePlayState.h"
#include "PostPlayState.h"
#include "StateManager.h"
#include "GameOverState.h"
#include "RankState.h"
#include "Interface.h"
#include "cqBehindCtl.h"
#include "SoundManager.h"

PlayState PlayState::mPlayState;

//SoundManager * soundMgr;

void PlayState::enter()
{
#ifdef _DEBUG
	_cprintf( "PlayState::enter()\n" );
#endif
	mCache = &( SinglePlayState::getInstance()->mCache );

	const size_t index = SinglePlayState::getInstance()->mStageIndex;
	const std::string file = SinglePlayState::getInstance()->mStageVec[ index ];

#ifdef _DEBUG
	mStage.load( "stage\\" + file );
#else
	std::string path;
	StateManager::getInstance()->getInstallPath( path );
	
	mStage.load( path + "\\stage\\" + file );
#endif

	Interface::getInstance()->setMaxSecond(
		mPlayableTime = static_cast< float >( mStage.value.limitTime )
	);

	mWidth = SinglePlayState::getInstance()->mWidth;
	mHeight = SinglePlayState::getInstance()->mHeight;
	//mTempSceneNode = 0;
	mPrevSelectedSceneNode = mSelectedSceneNode = 0;
	mTime = 0;

	mInputList.clear();
	//mLockedSceneNodeSet.clear();
	mTimerList.clear();
	mSolvedTime.clear();

	mRaySceneQuery = SinglePlayState::getInstance()->mSceneMgr->createRayQuery( Ray() );

	createScene();

	setAnswer();
	setFactor();// run each answer. because number or operator is changed at correct the answer, you should call at each answer 

	Interface::getInstance()->enter();
	Interface::getInstance()->setAnswer( mAnswer );
	//Interface::getInstance()->setFactor( mFactor.back().first, mMatchedFactor );
	Interface::getInstance()->setFactor( mPlusFactor, mMatchedFactor );
	//Interface::getInstance()->setMinusFactor( mFactor.back().second );
	Interface::getInstance()->setMinusFactor( mMinusFactor );
	Interface::getInstance()->setPanelNumber( mAnswer.back() );
	Interface::getInstance()->setPanelBanner( "Answer, please" );

#ifdef _DEBUG
	for( std::map< SceneNode *, std::string >::const_iterator it = mSceneNodeMap.begin();
		mSceneNodeMap.end() != it; ++it )
	{
		Entity * entity = static_cast< Entity * >( it->first->getAttachedObject( 0 ) );
		cprintf( "\t material: %s, value: %s\n", entity->getSubEntity( 0 )->getMaterialName().c_str(), it->second.c_str() );
	}
#endif

	StateManager::getInstance()->pushState( PrePlayState::getInstance() );

	Root::getSingleton().startRendering();
}

void PlayState::exit()
{
#ifdef _DEBUG
	cprintf( "PlayState::exit()\n" );
#endif

	StateManager::getInstance()->showMouseCursor( false );
	SinglePlayState::getInstance()->mSceneMgr->destroyQuery( mRaySceneQuery );

	Interface::getInstance()->exit();
	
	destroyScene();
}

void PlayState::pause()
{
#ifdef _DEBUG
	_cprintf( "PlayState::pause()\n" );
#endif
	Interface::getInstance()->pause();
	StateManager::getInstance()->showMouseCursor( false );
}

void PlayState::resume()
{
#ifdef _DEBUG
	_cprintf( "PlayState::resume()\n" );
#endif
	if( PlayState::Clear == mStatus || 
		PlayState::Resign == mStatus ||
		PlayState::GameOver == mStatus )
	{
		return;
	}

	StateManager::getInstance()->showMouseCursor( true );
	Interface::getInstance()->resume();
}

void PlayState::keyClicked( KeyEvent * e )
{
#ifdef _DEBUG
	_cprintf( "PlayState::KeyClicked()\n" );
#endif
	e->consume();
}

void PlayState::keyReleased( KeyEvent * e )
{
#ifdef _DEBUG
	_cprintf( "PlayState::KeyReleased()\n" );
#endif
	e->consume();
}

bool PlayState::frameStarted( const FrameEvent & evt )
{
	//Root::getSingleton().renderOneFrame();

	switch( mStatus )
	{
	case PlayState::Clear:
		StateManager::getInstance()->popState();
		return true;
	case PlayState::Resign:
	case PlayState::GameOver:
		SinglePlayState::getInstance()->mSinglePlayExit = true;
		StateManager::getInstance()->popState();
		return true;

	case PlayState::Play:
		break;

	case PlayState::Pause:
		return true;
	}

	mTime += evt.timeSinceLastFrame;

	// check reach regeneration time
	if( mTimerList.size() )
	{
		TimerPair & timerPair = mTimerList.front();

		if( 0.0f > timerPair.first )
		{
			// regenrate entity list
			SceneManager * sceneMgr = SinglePlayState::getInstance()->mSceneMgr;

			for( EntityVector::const_iterator it = timerPair.second.begin();
				timerPair.second.end() != it; ++ it )
			{
				Entity * entity = sceneMgr->getEntity( it->first );

				ParticleSystem * pSys = mParticleMap[ entity ];

				if( pSys->isAttached() )
				{
					entity->getParentSceneNode()->detachObject( 1 ); // detach particle
				}

				const std::string value = ( it->second ) ? getRandomNumeric() : getRandomOperator();
				
				entity->setMaterialName(
					SinglePlayState::getInstance()->mMaterialMap[ value ]
				);

				SceneNode * sceneNode = entity->getParentSceneNode();
				mSceneNodeMap[ sceneNode ] = value;
#ifdef _DEBUG
				cprintf( "PlayState::frameStarted()\n" );
				cprintf( "\t new value generated\n" );
				cprintf( "\t value: %s, material: %s\n", value.c_str(), SinglePlayState::getInstance()->mMaterialMap[ value ].c_str() );
#endif
			}

			mTimerList.pop_front();
		}

		for( std::list< TimerPair >::iterator it = mTimerList.begin();
			mTimerList.end() != it; ++ it )
		{
			it->first -= evt.timeSinceLastFrame;
		}
	}

	mPlayableTime -= evt.timeSinceLastFrame;

	if( 0.0f > mPlayableTime )
	{
		mStatus = PlayState::GameOver;
		StateManager::getInstance()->pushState( GameOverState::getInstance() );
		StateManager::getInstance()->pushState( RankState::getInstance() );	
		return true;
	}

	Interface::getInstance()->setSecond( mPlayableTime );
	return true;
}

bool PlayState::frameEnded( const FrameEvent & evt )
{
	return Interface::getInstance()->frameEnded( evt );
}

void PlayState::keyPressed(KeyEvent* e)
{
	// TODO: later, you should change zoom processing with mouse wheel
	
	if( e->getKey() == KC_BACK )
	{
		if( ! mInputList.empty() )
		{
			removeRoundBar( mInputList.back() );
			toggleLock( mInputList.back() );
			//mInputList.pop_back();

			processOperation();
		}
		else
		{
			//Interface::getInstance()->putMessage( "Cannot erase anymore" );
		}
	}
	else if(e->getKey() == KC_Z)
	{
		Camera * camera = SinglePlayState::getInstance()->mCamera;
		
		if( camera->getPosition().z < SinglePlayState::getInstance()->mValue.cameraMinPosZ )
		{
			const float & zoomStep = SinglePlayState::getInstance()->mValue.cameraZoomStep;
			SinglePlayState::getInstance()->mCamera->moveRelative( Vector3( 0, 0, -zoomStep ) );
		}
	}
	else if(e->getKey() == KC_X)
	{
		Camera * camera = SinglePlayState::getInstance()->mCamera;
		
		if( camera->getPosition().z > SinglePlayState::getInstance()->mValue.cameraMaxPosZ )
		{
			const float & zoomStep = SinglePlayState::getInstance()->mValue.cameraZoomStep;
			SinglePlayState::getInstance()->mCamera->moveRelative( Vector3( 0, 0, zoomStep ) );
		}
	}
	else if(e->getKey() == KC_P)
	{
		mStatus = PlayState::Pause;
		StateManager::getInstance()->pushState(PauseState::getInstance());
	}
	else if(e->getKey() == KC_ESCAPE)
	{
		mStatus = PlayState::Pause;
		StateManager::getInstance()->pushState(GameMenuState::getInstance());
	}
	else if(e->getKey() == KC_SYSRQ )
	{
		StateManager::getInstance()->saveScreenShot();
	}
#ifdef _DEBUG
	else if( e->getKey() == KC_SPACE )
	{
		// TODO: it use only test purpose
		StateManager::getInstance()->popState();
	}
#endif

	e->consume();
}

void PlayState::createScene()
{
	SceneManager * sceneMgr = SinglePlayState::getInstance()->mSceneMgr;
	Entity * entity;
	
	sceneMgr->getRootSceneNode()->setVisible( false );

	// set particle for selecting effect
	{
		ParticleSystem * pSys = sceneMgr->createParticleSystem( "SelectedEffectParticle", "MagicWand");
		mParticleSceneNode = sceneMgr->createSceneNode( "SelectedEffectParticleNode" );
		mParticleSceneNode->attachObject( pSys );
	}

	// set container
	{
		mContainerSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode( "ContainerSceneNode" );

		entity = sceneMgr->createEntity( "ContainerEntity", mStage.container.mesh );
		entity->setMaterialName( mStage.container.material );

		mContainerSceneNode->attachObject( entity );
		mContainerSceneNode->scale( mStage.container.scale );
		mContainerSceneNode->setPosition( mStage.container.position );
	}

	// set egg
	{
		mSceneNodeMap.clear();

		mEggSceneNode = mContainerSceneNode->createChildSceneNode( "EggSceneNode" );
		entity = sceneMgr->createEntity( "standardEgg", mStage.egg.mesh );

		std::map< std::string, std::string > & materialMap = SinglePlayState::getInstance()->mMaterialMap;
		std::vector< Stage::EggElement >::const_iterator it = mStage.egg.eggVec.begin();

		for( ; it != mStage.egg.eggVec.end(); ++ it )
		{
			// TODO: Build supprot egg type is whether digit or operator. But, I have no time to modify it. Sorry for it.
			const std::string & material = materialMap[ it->value ]; 
			//const std::string value = ( material.rfind( "Digit" ) == -1 ) ? getRandomOperator() : getRandomNumeric();

			//entity->setMaterialName( materialMap[ value ] );
			entity->setMaterialName( material );

			Entity * eggClone = entity->clone( it->name );

			Ogre::SceneNode * n = mEggSceneNode->createChildSceneNode( it->name + "Node" );
			n->attachObject( eggClone );
			n->setPosition( it->position );
			n->scale( n->getScale() * it->radius / mStage.egg.standardRadius * 0.7 );
			n->_updateBounds();

			Vector3 size = n->_getWorldAABB().getMaximum() - n->_getWorldAABB().getMinimum();

			mRadiusMap[ it->name ] = min( min( abs( size.x ), abs( size.y ) ), abs( size.z ) ) / 2;

			mSceneNodeMap[ n ] = it->value;
			//mSceneNodeMap[ n ] = value;
			mParticleMap[ eggClone ] = sceneMgr->createParticleSystem( it->name + "Particle", "Examples/GreenyNimbus");
		}

		sceneMgr->destroyEntity( entity );

		mEggSceneNode->setPosition( mStage.egg.position );
		mEggSceneNode->scale( mStage.egg.scale );
	}

	// set light
	{
		mLightSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode( "LightSceneNode" );
		std::map< std::string, Stage::Light >::const_iterator it;

		for( it = mStage.lightMap.begin(); it != mStage.lightMap.end(); ++ it )
		{
			Light * l = sceneMgr->createLight( it->first );
			const Stage::Light & data = it->second;

			l->setPosition( data.position );
			l->setDiffuseColour( data.diffuse );
			l->setSpecularColour( data.specular );
			l->setCastShadows( true );

			mLightSceneNode->createChildSceneNode( it->first + "Node" )->attachObject( l );
		}
	}

	// set environment
	{
		sceneMgr->setAmbientLight( mStage.ambientLight );
		sceneMgr->setSkyBox( true, mStage.skybox.material, mStage.skybox.range );
		//sceneMgr->setFog( mStage.fog.mode, mStage.fog.fade, mStage.fog.density, mStage.fog.linear.first, mStage.fog.linear.second );
	}

	// set camera
	{
		Camera * camera = SinglePlayState::getInstance()->mCamera;
		//camera->setPosition( 0, 0, -500 );
		camera->setPosition( mStage.camera.position + Vector3( 0, 0, -500 ) );
		camera->lookAt( mStage.camera.lookAt );
		camera->setNearClipDistance(1);
		camera->setAutoTracking( true, mContainerSceneNode );
	}

	sceneMgr->getRootSceneNode()->setVisible( true );
}

void PlayState::destroyScene()
{
	SceneManager * sceneMgr = SinglePlayState::getInstance()->mSceneMgr;

	// destory particle for selecting effect
	{
		mParticleSceneNode->detachAllObjects();
		sceneMgr->destroyParticleSystem( "SelectedEffectParticle" );
		sceneMgr->destroySceneNode( "SelectedEffectParticleNode" );
	}

	// destroy container
	{
		mContainerSceneNode->detachAllObjects();
		sceneMgr->destroyEntity( "ContainerEntity" );

		sceneMgr->getRootSceneNode()->removeChild( mContainerSceneNode );
		sceneMgr->destroySceneNode( mContainerSceneNode->getName() );
	}

	// destroy egg
	{
		for( size_t i = 0; i < mStage.egg.eggVec.size(); ++ i )
		{
			const Stage::EggElement & e = mStage.egg.eggVec[ i ];

			Entity * entity = sceneMgr->getEntity( e.name );
			SceneNode * sceneNode = entity->getParentSceneNode();
			sceneNode->detachObject( entity );			

			// detach particle
			ParticleSystem * pSys = mParticleMap[ entity ];
			if( pSys->isAttached() )
			{
				sceneNode->detachObject( pSys );
			}

			sceneMgr->destroyParticleSystem( pSys );
			sceneMgr->destroyEntity( entity );
			sceneMgr->destroySceneNode( sceneNode->getName() );
		}
		
		sceneMgr->getRootSceneNode()->removeChild( mEggSceneNode );
		sceneMgr->destroySceneNode( mEggSceneNode->getName() );
	}

	sceneMgr->destroyLight( "MainLight" );

	// delete light
	{
		std::map< std::string, Stage::Light >::const_iterator it = mStage.lightMap.begin();
		const std::string nodeName = "lightNode";

		for( ; it != mStage.lightMap.end(); ++ it )
		{
			Light * light = sceneMgr->getLight( it->first );
			SceneNode * sceneNode = light->getParentSceneNode();
			
			sceneNode->detachObject( light );
			sceneMgr->destroyLight( light );
			sceneMgr->destroySceneNode( sceneNode->getName() );
		}

		sceneMgr->destroySceneNode( mLightSceneNode->getName() );
	}

	mParticleMap.clear();
}

void PlayState::mouseMoved( Ogre::MouseEvent * e )
{
#ifdef _DEBUG
	cprintf( "PlayState::mouseMoved()\n" );
#endif
	if( 0 < e->getRelZ() )
	{
		Camera * camera = SinglePlayState::getInstance()->mCamera;

		if( camera->getPosition().z < SinglePlayState::getInstance()->mValue.cameraMinPosZ )
		{
			const float & zoomStep = SinglePlayState::getInstance()->mValue.cameraZoomStep;
			SinglePlayState::getInstance()->mCamera->moveRelative( Vector3( 0, 0, -zoomStep ) );
		}

		return;
	}
	else if( 0 > e->getRelZ() )
	{
		Camera * camera = SinglePlayState::getInstance()->mCamera;
		
		if( camera->getPosition().z > SinglePlayState::getInstance()->mValue.cameraMaxPosZ )
		{
			const float & zoomStep = SinglePlayState::getInstance()->mValue.cameraZoomStep;
			SinglePlayState::getInstance()->mCamera->moveRelative( Vector3( 0, 0, zoomStep ) );
		}

		return;
	}

	// rotation mode
	if( 0 == e->getX() || 1 == e->getX() ||
		0 == e->getY() || 1 == e->getY() )
	{
		// audio
		if( SoundManager::getSingleton().isValid() ) 
		{
			static int index = SoundManager::getSingleton().CreateStream( StateManager::getInstance()->mAudio.rotateContainerSound );
			static int channel = INVALID_SOUND_CHANNEL;

			SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
			SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
		}

		// cursor position set visible
		{
			float x = e->getX();
			float y = e->getY();

			if( 1 == e->getX() )
			{
				x -= 0.07;
			}
			
			if( 1 == e->getY() )
			{
				y -= 0.1;
			}

			StateManager::getInstance()->changeMouseCursor( StateManager::CS_Rotated );
			StateManager::getInstance()->getMouseCursor()->setPosition( x, y );
		}

		rotateContainer( e->getX() - 0.5f, 0.5f - e->getY() );
		return;
	}

	// select mode
	Ray mouseRay = SinglePlayState::getInstance()->mCamera->getCameraToViewportRay( e->getX(), e->getY() );

	mRaySceneQuery->setRay( mouseRay );
	mRaySceneQuery->setSortByDistance( true );

	RaySceneQueryResult & result = mRaySceneQuery->execute();

	int movableCount = 0;
	//bool isLockedSelect = false;
	//bool isRecalculate = false;

	for( RaySceneQueryResult::iterator itr = result.begin();
		itr != result.end(); ++ itr )
	{
		if( itr->movable && "ContainerEntity" != itr->movable->getName() )
		{
			++ movableCount;

			const Vector3 centre = itr->movable->getParentSceneNode()->getWorldPosition();
			const Vector3 dirVec = mouseRay.getDirection();
			
			// get projection vector
			const Real ratio = ( centre - mouseRay.getOrigin() ).dotProduct( dirVec ) / dirVec.dotProduct( dirVec );

			// get point direct cross
			const Vector3 crossPoint = mouseRay.getOrigin() + ratio * dirVec;

			// get length
			const Vector3 diff = centre - crossPoint;
			const Real dist = ::sqrt( ::pow( diff.x, 2 ) + ::pow( diff.y, 2 ) + ::pow( diff.z, 2 ) );				

			if( dist < mRadiusMap[ itr->movable->getName() ] )
			{
				if( itr->movable->getParentSceneNode() != mSelectedSceneNode )
				{
					if( mSelectedSceneNode )
					{
						removeRoundBar( mSelectedSceneNode );
					}

					addRoundBar( mSelectedSceneNode = itr->movable->getParentSceneNode() );

#ifdef _DEBUG
					cprintf( "\t value: %s\n", mSceneNodeMap[ mSelectedSceneNode ].c_str() );
#endif
				}

				break;
			}
		}
	}

#ifdef _DEBUG
	cprintf( "\t value: %s\n", mSceneNodeMap[ mSelectedSceneNode ].c_str() );
#endif

	if( ! movableCount )
	{
		removeRoundBar( mSelectedSceneNode );
		mSelectedSceneNode = 0;
	}
	
	StateManager::getInstance()->changeMouseCursor( StateManager::CS_Moved );
	StateManager::getInstance()->getMouseCursor()->setPosition( e->getX(), e->getY() );

	e->consume();
}

void PlayState::mousePressed( MouseEvent * e )
{
#ifdef _DEBUG
	cprintf( "PlayState::mousePressed()\n" );
#endif
	e->consume();

	if( ! mSelectedSceneNode )
	{
#ifdef _DEBUG
		cprintf( "\t mSelectedSceneNode is zero\n" );
#endif
		return;
	}

#ifdef _DEBUG
	cprintf( "\t value: %s\n", mSceneNodeMap[ mSelectedSceneNode ].c_str() );

	cprintf( "\t egg: %s\n", mSelectedSceneNode->getAttachedObject( 0 )->getName().c_str() );
#endif

	if( containMinusFactor( mSelectedSceneNode ) )
	{
#ifdef _DEBUG
		cprintf( "\t contain minus factor\n" );
#endif
		// audio
		if( SoundManager::getSingleton().isValid() ) 
		{
			static int index = SoundManager::getSingleton().CreateStream( StateManager::getInstance()->mAudio.factorProblemSound );
			static int channel = INVALID_SOUND_CHANNEL;

			SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
			SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
		}

		Interface::getInstance()->putMessage( "Prevent minus factor!", "Minus Factor" );
		//mSelectedSceneNode = 0;
		return;
	}

	if( mPrevSelectedSceneNode )
	{
		mPrevSelectedSceneNode->removeChild( mParticleSceneNode );
		mPrevSelectedSceneNode = 0;
	}

	//if( 30 < mLockedSceneNodeSet.size() )
	if( 30 < mInputList.size() )
	{
#ifdef _DEBUG
		cprintf( "\t too long operation\n" );
#endif
		Interface::getInstance()->putMessage( "too long operation!" );
		return;
	}

	//	mPrevSelectedSceneNode = 0;
	
	toggleLock( mSelectedSceneNode );
	addRoundBar( mSelectedSceneNode );
	processOperation();

	// audio
	if( SoundManager::getSingleton().isValid() ) 
	{
		static int index = SoundManager::getSingleton().CreateStream( StateManager::getInstance()->mAudio.clickBallSound );
		static int channel = INVALID_SOUND_CHANNEL;

		SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
		SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
	}
}

void PlayState::mouseReleased( MouseEvent * e )
{
	e->consume();
}

void PlayState::addRoundBar( SceneNode * s )
{
	Entity * entity = static_cast< Entity * >( s->getAttachedObject( 0 ) );
	std::string material = entity->getSubEntity( 0 )->getMaterialName();

	entity->setMaterialName( material + "Selected" );
}

void PlayState::removeRoundBar( SceneNode * s )
{
	if( s )
	{
		Entity * entity = static_cast< Entity * >( s->getAttachedObject( 0 ) );
		std::string material = entity->getSubEntity( 0 )->getMaterialName();

		size_t index = material.rfind( "Selected" );
		if( 0 < index && index < material.length() )
		{
			entity->setMaterialName( material.substr( 0, index ) );
		}
	}
}

void PlayState::toggleLock( SceneNode * s )
{
#ifdef _DEBUG
	cprintf( "PlayState::toggleLock( SceneNode * s )\n" );
#endif
	Entity * entity = static_cast< Entity * >( s->getAttachedObject( 0 ) );

	std::string material = static_cast< Entity * >( s->getAttachedObject( 0 ) )->getSubEntity( 0 )->getMaterialName();
	material = material.substr( 0, material.rfind( "Selected" ) );

	//std::string input = SinglePlayState::getInstance()->mValueMap[ material ];

	std::list< SceneNode * >::const_iterator it = mInputList.begin();
	for( ; mInputList.end() != it; ++it )
	{
		if( *it == s )
		{
			break;
		}
	}

	if( mInputList.end() == it )
	//if( mLockedSceneNodeSet.end() == mLockedSceneNodeSet.find( s ) )
	{
#ifdef _DEBUG
		cprintf( "\t set locked\n" );
#endif
		s->addChild( mParticleSceneNode );
		mInputList.push_back( s );
		//mLockedSceneNodeSet.insert( s );
		mPrevSelectedSceneNode = s;

		entity->setMaterialName( material + "Locked" );
	}
	else
	{
#ifdef _DEBUG
		cprintf( "\t set unlocked\n" );
#endif
		//mLockedSceneNodeSet.erase( s );
		mInputList.remove( s );
		
		material = material.substr( 0, material.rfind( "Locked" ) );
		entity->setMaterialName( material );
	}

	//addRoundBar( s );
}

bool PlayState::getOperationResult()
{
	const std::string s = getOperation();

	if( s.empty() )
	{
		Interface::getInstance()->setInputedOperand( " " );
	}
	else
	{
		static std::map< std::string, SinglePlayState::CacheData >::const_iterator it;
		it = mCache->find( s );

		std::string formular;
		std::string score;
		std::string answer;

		if( mCache->end() == it )
		{
#ifdef _DEBUG
			cprintf( "\t saved to cache\n" );
#endif
			const std::string result = getResult( s );
			const size_t index = result.find( "," );
			answer = result.substr( 0, index );
			score = result.substr( index + 1, result.size() );

			makeFormular( formular );

			if( "invalid" == answer )
			{
				formular += "==?|";
			}
			else if( "devideZero" == answer )
			{
				formular += "==?|";
			}
			else if( "overflow" == answer )
			{
				formular += "==!|";
			}
			else
			{
				formular += "==" + answer + '|';
			}

			// put data to cache
#ifdef _DEBUG
			cprintf( "\t stored cache\n" );
#endif
			SinglePlayState::CacheData cache;
			cache.answer = answer;
			cache.formular = formular;
			cache.score = score;

			( *mCache )[ s ] = cache;
		}
		else
		{
#ifdef _DEBUG
			cprintf( "\t used cache\n" );
#endif
			// get data from cache
			const SinglePlayState::CacheData & cache = ( *mCache )[ s ];

			formular = cache.formular;
			answer = cache.answer;
			score = cache.score;
		}

		Interface::getInstance()->setInputedOperand( formular );

		if( -1 == formular.rfind( "?" ) &&
			mAnswer.back() == atoi( answer.c_str() ) )
		{
			// check plus factor
			if( ! containPlusFactor() )
			{
				// audio
				if( SoundManager::getSingleton().isValid() ) 
				{
					static int index = SoundManager::getSingleton().CreateStream( StateManager::getInstance()->mAudio.factorProblemSound );
					static int channel = INVALID_SOUND_CHANNEL;

					SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
					SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
				}

				Interface::getInstance()->putMessage( "Contain plus factor!", "Plus Factor" );
				return false;
			}
#ifdef _DEBUG
			else
			{
				cprintf( "\t MESSAGE: you select plus factor whole\n" );
			}
#endif

			SinglePlayState::getInstance()->mCQ += atof( score.c_str() );

			static char buffer[ 80 ];
			sprintf( buffer, "Collect! CQ +%s", score.c_str() );
			Interface::getInstance()->putMessage( buffer );

			// audio
			if( SoundManager::getSingleton().isValid() ) 
			{
				static int index = SoundManager::getSingleton().CreateStream( StateManager::getInstance()->mAudio.collectAnswerSound );
				static int channel = INVALID_SOUND_CHANNEL;

				SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
				SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
			}

			//mAnswer.pop_back();
			//mFactor.pop_back();
			//setFactor();
			//mMatchedFactor.clear();
			
			//putEffectForCollectAnswer();
			//Interface::getInstance()->setInputedOperand( " " );

			/*
			if( mAnswer.empty() )
			{
				mStatus = PlayState::Clear;
			}
			*/

			return true;
		}
	}

	return false;
}

std::string PlayState::getResult( const std::string & s ) const
{
	//return std::string( ::calc( s.c_str(), true, mTime ) );
	return std::string( ::calc( s.c_str(), true, 1 ) );
}

void PlayState::setAnswer()
{
#ifdef _DEBUG
	cprintf( "PlayState::setAnswer()\n" );
#endif
	if( mAnswer.size() )
	{
		mAnswer.clear();
	}

	int & begin = mStage.value.answerNumericRange.first;
	int & end = mStage.value.answerNumericRange.second;

	if( begin > end )
	{
		std::swap( begin, end );
	}

	const int step = end - begin - 1;

	for( int i = 0; i < mStage.value.answerQuantity; ++ i )
	{
		mAnswer.push_back( begin + getRandom() % step );
	}
}

std::string PlayState::getRandomNumeric()
{
	const int & from = mStage.value.eggNumericRange.first;
	const int & to = mStage.value.eggNumericRange.second;
	static char buffer[ 80 ];

	// CAUTION: If you called srand() in little time, you will receive equal value
	const int index = getRandom() % abs( from - to - 1 );
	
	sprintf( buffer, "%d", from + index );
	return std::string( buffer );
}

std::string PlayState::getRandomOperator()
{
	const std::list< std::string > & l = mStage.operatorData.usingOperator;
	const size_t index = getRandom() % l.size();

	std::list< std::string >::const_iterator it = l.begin();
	std::advance( it, index );

	return *it;
}

void PlayState::makeFormular( std::string & formular )
{
	for( std::list< SceneNode * >::const_iterator it = mInputList.begin();
		mInputList.end() != it; ++ it )
	{
		//const std::string type = ( ( *it ) == mSelectedSceneNode ) ? "]" : "[";
		const std::string type = "[";
		const std::string & value = mSceneNodeMap[ *it ];
		
		formular += type + value + '|';
	}
}

void PlayState::putEffectForCollectAnswer()
{
	if( mAnswer.size() )
	{
		Interface::getInstance()->setPanelBanner( "Collect! More answer." );
		Interface::getInstance()->setPanelNumber( mAnswer.back() );
		//Interface::getInstance()->setFactor( mFactor.back().first, mMatchedFactor );
		Interface::getInstance()->setFactor( mPlusFactor, mMatchedFactor );
		//Interface::getInstance()->setMinusFactor( mFactor.back().second );
		Interface::getInstance()->setMinusFactor( mMinusFactor );
	}

	Interface::getInstance()->decreaseAnswer();
	//Interface::getInstance()->increaseCQPanel( 0.1f );

	// TODO: put audio
}

std::string PlayState::getOperation()
{
	std::string s;
	mMatchedFactor.clear();

	for( std::list< SceneNode * >::const_iterator it = mInputList.begin();
		mInputList.end() != it; ++ it )
	{
		const std::string & value = mSceneNodeMap[ *it ];
		//mMatchedFactor.insert( value );
		std::map< std::string, size_t >::iterator map_it = mMatchedFactor.find( value );
		
		if( mMatchedFactor.end() == map_it )
		{
			mMatchedFactor[ value ] = 1;
		}
		else
		{
			++ map_it->second;
		}

		s += value;
		//s += mSceneNodeMap[ *it ];
	}

	//Interface::getInstance()->setFactor( mFactor.back().first, mMatchedFactor );
	Interface::getInstance()->setFactor( mPlusFactor, mMatchedFactor );
	return s;
}

int PlayState::getRandom() const
{
	static DWORD tick;

	if( tick < GetTickCount() )
	{
		srand( tick = GetTickCount() );
	}

	return rand();
}

void PlayState::getRandomValueMap( Factor & s )
{
#ifdef _DEBUG
	cprintf( "PlayState::getRandomValueMap()\n" );
#endif
	//const std::vector< std::string > & factor = mStage.factorData.factor;
	//const int & quantity = mStage.factorData.quantity;
	//int factorCount = 0;
	//size_t i = 0;

	std::vector< std::string > v;
	const std::vector< Stage::EggElement > & egg = mStage.egg.eggVec;
	const std::vector< std::string > & factor = mStage.factorData.factor;

	/*
	for( size_t eggIndex = 0; eggIndex < egg.size(); ++eggIndex )
	{
#ifdef _DEBUG
		cprintf( "\t it: %s\n", egg[ eggIndex ].value.c_str() );
#endif
		for( size_t i = 0; i < factor.size(); ++i )
		{
#ifdef _DEBUG
			cprintf( "\t factor[ %d ]: %s\n", i, factor[ i ].c_str() );
#endif
			if( factor[ i ] != egg[ eggIndex ].value )
				continue;
	
			v.push_back( egg[ eggIndex ].value );
			break;
		}
	}
	*/
	if( ! mStage.factorData.quantity )
	{
		return;
	}

	for( std::map< SceneNode *, std::string >::const_iterator it = mSceneNodeMap.begin();
		mSceneNodeMap.end() != it; ++it )
	{
		std::vector< std::string >::const_iterator find_it = std::find( factor.begin(), factor.end(), it->second );

		if( factor.end() != find_it )
		{
#ifdef _DEBUG
			cprintf( "\t mSceneNodeMap value: %s\n", find_it->c_str() );
#endif
			v.push_back( *find_it );
		}
	}
	
	if( v.empty() )
	{
		return;
	}

	size_t step;
	if( static_cast< int >( v.size() ) < mStage.factorData.quantity )
	{
		step = 1;
	}
	else
	{
		step = v.size() / mStage.factorData.quantity;
	}

#ifdef _DEBUG
	cprintf( "\t step: %d, factor qty: %d, parent size: %d\n", step, mStage.factorData.quantity, v.size() );
#endif
	srand( GetTickCount() );
	
	for( size_t i = 0; i < v.size(); i += step )
	{
#ifdef _DEBUG
		cprintf( "\t i: %d", i, step );
#endif
		const size_t index = rand() % step;

		if( v.size() <= i + index )
		{
			break;
		}

		const std::string value = v.at( i + index );

		Factor::iterator it = s.find( value );

		if( s.end() == it )
		{
			s[ value ] = 1;
		}
		else
		{
			++ it->second;
		}
	}
}

/*
To protect, blocking problem - player must select plus factor and unselect minus factor, too -
If both plus & minus factor contain 2, player cannot clear game.

Thus, code should assure execution order - getRandomValueMap() -> getRandomValueSet()
*/
void PlayState::getRandomValueSet( MinusFactor & s )
{
#ifdef _DEBUG
	cprintf( "PlayState::getRandomValueSet()\n" );
#endif

	const std::vector< std::string > & factor = mStage.minusFactorData.factor;
	const int & quantity = mStage.minusFactorData.quantity;
	size_t i = 0;

	while( true )
	{
		if( quantity <= ( int ) s.size() || 10000 < ++i ) // 10000: to evade unlimited looping there is counting 
		{
			break;
		}

		const size_t step = getRandom() % factor.size();
		const std::string & value = factor[ step ];

		//Factor::const_iterator it = mFactor.back().first.find( value );
		Factor::const_iterator it = mPlusFactor.find( value );

		//if( mFactor.back().first.end() == it )
		if( mPlusFactor.end() == it )
		{
			s.insert( value );
		}
	}
}


void PlayState::setFactor()
{
	mMatchedFactor.clear();
	mPlusFactor.clear();
	mMinusFactor.clear();

	getRandomValueMap( mPlusFactor ); // plus factor
	getRandomValueSet( mMinusFactor ); // minus factor
#ifdef _DEBUG
	//system( "pause" );
#endif
}

void PlayState::processOperation()
{
	if( getOperationResult() )
	{
		if( mPrevSelectedSceneNode )
		{
			mPrevSelectedSceneNode->removeChild( mParticleSceneNode );
			mPrevSelectedSceneNode = 0;
		}

		mSolvedTime.push_back( mTime );
		mTime = 0;

		mTimerList.push_back( TimerPair() );
		TimerPair & timerPair = mTimerList.front();

		for( std::list< SceneNode * >::const_iterator it = mInputList.begin();
			mInputList.end() != it; ++ it )
		{
			Entity * entity = static_cast< Entity * >( ( * it )->getAttachedObject( 0 ) );
			const std::string & material = entity->getSubEntity( 0 )->getMaterialName();

			timerPair.second.push_back( 
				EntityPair( 
					entity->getName(),
					material.rfind( "Digit" ) < material.size() ? true : false
				)
			);

			entity->setMaterialName( "CalcKing/OperatorQuestion" );

			SceneNode * sceneNode = entity->getParentSceneNode();
			sceneNode->attachObject( mParticleMap[ entity ] );
			mSceneNodeMap[ sceneNode ] = "?";
		}

		timerPair.first = mStage.value.regenerationTime;

		mAnswer.pop_back();

		if( mAnswer.empty() )
		{
			StateManager::getInstance()->pushState( PostPlayState::getInstance() );
			return;
		}

		mInputList.clear();
		//mMatchedFactor.clear();
		setFactor();

		putEffectForCollectAnswer();
		Interface::getInstance()->setInputedOperand( " " );
	}
}

void PlayState::rotateContainer( const float & x, const float & y )
{
	// get axis
	Vector3 axis = Vector3( y, x, 0 );
	axis.normalise();

	// get rotate value
	mEggSceneNode->getParentSceneNode()->rotate( Quaternion( Radian( 0.05 ), axis ), Node::TS_WORLD );
}

void PlayState::keyRepeated( int kc )
{
	if( KC_LEFT == kc )
	{
		//rotateContainer( -0.5, 0 );
		rotateContainer( -1, 0 );
	}
	else if( KC_RIGHT == kc )
	{
		//rotateContainer( 0.5, 0 );
		rotateContainer( 1, 0 );
	}
	else if( KC_UP == kc )
	{
		//rotateContainer( 0, 0.5 );
		rotateContainer( 0, 1 );
	}
	else if( KC_DOWN == kc )
	{
		//rotateContainer( 0, -0.5 );
		rotateContainer( 0, -1 );
	}
}

bool PlayState::containPlusFactor()
{
	std::map< std::string, size_t > factorMap;

	// make factorMap from user's input
	for( std::list< SceneNode * >::const_iterator it = mInputList.begin();
		mInputList.end() != it;	++it )
	{
		std::string & value = mSceneNodeMap[ *it ];
		std::map< std::string, size_t >::iterator map_it = factorMap.find( value );
		
		if( factorMap.end() == map_it )
		{
			factorMap[ value ] = 1;
		}
		else
		{
			++ map_it->second;
		}
	}

	// check
	//const Factor & plusFactor = mFactor.back().first;
	const Factor & plusFactor = mPlusFactor;

	for( Factor::const_iterator it = plusFactor.begin();
		plusFactor.end() != it;
		++it )
	{
		const std::string & value = it->first;
		
		if( it->second > factorMap[ value ] )
		{
			return false;
		}
	}

	return true;
}

bool PlayState::containMinusFactor( SceneNode * sceneNode )
{
	const std::string & value = mSceneNodeMap[ sceneNode ];
	//const MinusFactor & minusFactor = mFactor.back().second; 
	const MinusFactor & minusFactor = mMinusFactor; 
	MinusFactor::const_iterator it = minusFactor.find( value );

	return ( minusFactor.end() == it ) ? false : true;
}