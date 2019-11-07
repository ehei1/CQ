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
#include "Stage.h"
#include "SinglePlayState.h"

//class Stage;
class PrePlayState;
class PostPlayState;

class PlayState : public State
{
public:
	friend class PrePlayState;
	friend class PostPlayState;

	enum Status
	{
		Play, Pause, Resign, Clear, GameOver, TimeOver
	} mStatus;

	void enter();
	void exit();

	void pause();
	void resume();

	void keyClicked(Ogre::KeyEvent* e);
	void keyPressed(Ogre::KeyEvent* e);
	void keyReleased(Ogre::KeyEvent* e);
	void keyRepeated( int );

	void mouseMoved( Ogre::MouseEvent * e );
	void mousePressed( Ogre::MouseEvent * e );
	void mouseReleased( Ogre::MouseEvent * e );

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	static PlayState * getInstance() { return &mPlayState; }
	
protected:
	PlayState() {}

	Stage mStage;

private:
	static PlayState mPlayState;

	void createScene();
	void destroyScene();
	void addRoundBar( SceneNode * );
	void removeRoundBar( SceneNode * );
	void toggleLock( SceneNode * );
	bool getOperationResult();
	std::string getOperation();
	std::string getResult( const std::string & ) const;
	void setAnswer();
	void setFactor();
	int getRandom() const;
	std::string getRandomNumeric();
	std::string getRandomOperator();
	void makeFormular( std::string & );
	void putEffectForCollectAnswer();
	void processOperation();
	void rotateContainer( const float &, const float & );

	SceneNode * mContainerSceneNode;
	SceneNode * mEggSceneNode;
	SceneNode * mLightSceneNode;
	
	int mWidth;
	int mHeight;
	float mPlayableTime;
	float mTime;

	RaySceneQuery * mRaySceneQuery;
	
	SceneNode * mSelectedSceneNode;
	SceneNode * mPrevSelectedSceneNode;
	//SceneNode * mTempSceneNode; // previous selected scene node
	SceneNode * mParticleSceneNode;
	//std::set< SceneNode * > mLockedSceneNodeSet; // player cannot select twice equal egg. this flag is for using
	std::map< std::string, float > mRadiusMap;
	std::list< SceneNode * > mInputList;

	typedef std::pair< std::string, bool > EntityPair; // first: entity name, second: type( digit or operator )
	typedef std::vector< EntityPair > EntityVector; // true: digit, false: operator
	typedef std::pair< double, EntityVector > TimerPair; // first: remained time, second: EntityVector
	std::list< TimerPair > mTimerList; // it's same
	
	std::vector< int > mAnswer;
	std::vector< float > mSolvedTime;
	std::map< Entity *, ParticleSystem * > mParticleMap; // key: entity pointer, value: particle system pointer
	std::map< SceneNode *, std::string > mSceneNodeMap; // key: sceneNode, value: value
	std::map< std::string, SinglePlayState::CacheData > * mCache;
	
	typedef std::map< std::string, size_t > Factor;
	Factor mPlusFactor;
	typedef std::set< std::string > MinusFactor;
	//std::vector< MinusFactor > mMinusFactor;
	MinusFactor mMinusFactor;
	Factor mMatchedFactor;

	void getRandomValueMap( Factor & );
	void getRandomValueSet( MinusFactor & );
	bool containPlusFactor();
	bool containMinusFactor( SceneNode * );
};