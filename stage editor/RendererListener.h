#pragma once
#include "OgreKeyEvent.h"
#include "OgreEventListeners.h"

using namespace Ogre;

class RendererListener : public FrameListener, public KeyListener
{
public:
	RendererListener( Ogre::RenderWindow * win, Ogre::Camera * cam,
		bool useBufferedInputKeys = false, bool useBufferedInputMouse = false );
	~RendererListener(void);

	bool frameStarted(const FrameEvent& evt);
	

protected:
	void updateStats(void);
	bool processUnbufferedKeyInput(const FrameEvent& evt);
	bool processUnbufferedMouseInput(const FrameEvent& evt);
	void moveCamera();
	void showDebugOverlay(bool show);
	bool frameEnded(const FrameEvent& evt);
	void switchMouseMode();
	void switchKeyMode();
	void keyClicked(KeyEvent* e);
	void keyPressed(KeyEvent* e);
	void keyReleased(KeyEvent* e);

	int mSceneDetailIndex ;
    Real mMoveSpeed;
    Degree mRotateSpeed;
    Overlay* mDebugOverlay;
	EventProcessor* mEventProcessor;
    InputReader* mInputDevice;
    Camera* mCamera;
    Vector3 mTranslateVector;
    RenderWindow* mWindow;
    bool mStatsOn;
    bool mUseBufferedInputKeys, mUseBufferedInputMouse, mInputTypeSwitchingOn;
	unsigned int mNumScreenShots;
    float mMoveScale;
    Degree mRotScale;
    // just to stop toggles flipping too fast
    Real mTimeUntilNextToggle ;
    Radian mRotX, mRotY;
    TextureFilterOptions mFiltering;
    int mAniso;
};
