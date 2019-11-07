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

// control SinglePlayState with listed class
class PlayState;
class GameOverState;
class CongratulationState;
class PauseState;
class PrePlayState;
class PostPlayState;
class GameMenuState;
class HighScoreState;
class RankState;
class Inteface;

class SinglePlayState : public State
{
	friend class PlayState;
	friend class GameOverState;
	friend class CongratulationState;
	friend class PauseState;
	friend class PrePlayState;
	friend class PostPlayState;
	friend class GameMenuState;
	friend class HighScoreState;
	friend class RankState;
	friend class Interface;

public:
	void enter();
	void exit();

	void pause();
	void resume();

	void keyClicked(KeyEvent* e);
	void keyPressed(KeyEvent* e);
	void keyReleased(KeyEvent* e);
	bool frameStarted(const FrameEvent& evt);
	bool frameEnded(const FrameEvent& evt);
	//std::string getNextStage();

	static SinglePlayState* getInstance() { return &mSinglePlayState; }

protected:
	SinglePlayState() {}

	void restoreConfig();
	void setMaterialMap( ConfigFile::SettingsMultiMap * );
	void setStageVec( ConfigFile::SettingsMultiMap * );
	void setValue(ConfigFile::SettingsMultiMap * );
	void loadCache();
	void saveCache();
	void loadRank();
	void saveRank();

	Root *mRoot;
	SceneManager* mSceneMgr;
	Viewport* mViewport;
	InputReader* mInputDevice;
	Camera* mCamera;
	SceneNode * mSceneNode;

private:
	static SinglePlayState mSinglePlayState;
	std::map< std::string, std::string > mMaterialMap; // key: value, value: material name
	std::map< std::string, std::string > mValueMap; // key: material name, value: value
	std::vector< std::string > mStageVec;

	struct Value
	{
		float regenerationTime;
		float cameraMinPosZ;
		float cameraMaxPosZ;
		float cameraZoomStep;
	} mValue;

	size_t mStageIndex;
	bool mSinglePlayExit;

	float mWidth;
	float mHeight;

	struct CacheData
	{
		std::string answer;
		std::string score;
		std::string formular;
	};

	std::map< std::string, CacheData > mCache; // key: operation, value: result
	float mCQ; // gotted score
};