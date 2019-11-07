#pragma once
#include "State.h"
#include "Stage.h"

class StageState : public State
{
public:
	void enter();
	void exit();

	void pause();
	void resume();

	void keyClicked(Ogre::KeyEvent* e);
	void keyPressed(Ogre::KeyEvent* e);
	void keyReleased(Ogre::KeyEvent* e);
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	static StageState* getInstance() { return &mStageState; }
	
protected:
	StageState() { }

	Stage mStage;

private:
	static StageState mStageState;
};