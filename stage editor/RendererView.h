#pragma once
#pragma message("Compiling RendererView.h - this should happen just once per project.\n")
#include "RendererListener.h"

class RendererView :
	public QWidget
{
	Q_OBJECT

public:
	RendererView( QWidget * parent = 0 );
	~RendererView();
	void paintEvent( QPaintEvent * );
	//void showEvent( QShowEvent * );
	void resizeEvent( QResizeEvent * );
	void timerEvent( QTimerEvent * );
	void update();
	void createScene();
	void createLight();
	Ogre::SceneManager * getSceneManager() const;
	void keyPressEvent( QKeyEvent *k );
	void moveCamera();
	void rotateDocumentNode( const Ogre::Vector3 & );

protected:
	void updateStats();
	void showDebugOverlay( bool );

	Ogre::RenderWindow* mRenderWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	Ogre::Root* mRoot;
	//Ogre::SceneNode* mainNode;
	Ogre::SceneNode * rendererViewNode; // generated entity from rendererView should attach only this node
	Ogre::Light* mainLight;
	Ogre::Entity * mainEntity;

	Ogre::Vector3 mTranslateVector; // vector for camera movement
	const float mMoveScale;
	const Ogre::Vector3 mDefaultCameraPosition;
	int mSceneDetailIndex; // look at moveCamera()
	RendererListener * mFrameListener;
	//Ogre::Overlay * mDebugOverlay;
};