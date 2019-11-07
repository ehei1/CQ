/****************************************************************************
** Meta object code from reading C++ file 'RendererView.h'
**
** Created: ? 6 22 11:13:08 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "RendererListener.h"
#include "RendererView.h"

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RendererView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const unsigned int qt_meta_data_RendererView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_RendererView[] = {
    "RendererView\0"
};

const QMetaObject RendererView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RendererView,
      qt_meta_data_RendererView, 0 }
};

const QMetaObject *RendererView::metaObject() const
{
    return &staticMetaObject;
}

void *RendererView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RendererView))
	return static_cast<void*>(const_cast<RendererView*>(this));
    return QWidget::qt_metacast(_clname);
}

int RendererView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}

RendererView::RendererView( QWidget * parent )
: QWidget( parent ), 
mRenderWindow( 0 ), mSceneMgr( 0 ), mViewport( 0 ), mRoot( 0 ),
mFrameListener( 0 ),
mTranslateVector( Ogre::Vector3::ZERO ), mDefaultCameraPosition( Ogre::Vector3(0,0,-100) ),
mSceneDetailIndex( 0 ), mMoveScale( 0.5f )
{
	mRoot = new Ogre::Root();	

	Ogre::ConfigFile cf;
	cf.load("resources.cfg");

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
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	mRoot->restoreConfig();
	mRoot->initialise(false);

	this->setFocusPolicy( Qt::StrongFocus ); // you should call to receive keyboard input

	//mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	//showDebugOverlay( true );
}


RendererView::~RendererView()
{
	if( mFrameListener )
	{
		mRoot->removeFrameListener( mFrameListener );
		delete mFrameListener;
	}

	if( mRoot ){
		delete mRoot;
		mRoot = 0;
	}
}

void RendererView::paintEvent(QPaintEvent * evt)
{
    if (mRenderWindow == NULL){
		Ogre::NameValuePairList params;

		params["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)(HWND)winId());

		mRenderWindow = mRoot->createRenderWindow("View", width(), height(), true, &params);	

        // mSceneMgr = mRoot->getSceneManager(Ogre::ST_GENERIC);
		mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "mainSceneManager" );
		rendererViewNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "rendererViewNode" );

        mCamera = mSceneMgr->createCamera("defaultCamera");

        mCamera->setPosition(mDefaultCameraPosition);
		mCamera->lookAt( Ogre::Vector3::ZERO );
        mCamera->setNearClipDistance(1);
		
        mViewport = mRenderWindow->addViewport(mCamera);
		mViewport->setBackgroundColour(Ogre::ColourValue(0.58, 0.65, 0.52, 1));

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(2);

		mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

        startTimer(60);

		mRenderWindow->reposition (x(),y());

		// create frame listener
		mFrameListener = new RendererListener(mRenderWindow, mCamera);
		mRoot->addFrameListener(mFrameListener);
    } 

	update();
}

void RendererView::update()
{
	if(mRenderWindow != NULL){
		//mRoot->_fireFrameStarted();
		//mRenderWindow->update();
		//mRoot->_fireFrameEnded();
		mRoot->renderOneFrame();
		
		//mCamera->moveRelative(mDirection);
		//mCamera->yaw(Radian(angleX));
		//mCamera->pitch(Radian(angleY));		
	}
}
	
void RendererView::timerEvent( QTimerEvent * evt )
{
	update();
}

void RendererView::createScene()
{
	/*
	Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");

	mainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("headNode");
	mainNode->attachObject(ogreHead);
	mainNode->setPosition( Ogre::Vector3::ZERO );
	mainNode->rotate( Ogre::Vector3::UNIT_Y, Ogre::Radian( Ogre::Math::AngleUnitsToRadians( 180 ) ) );
	*/
	
	//mCamera->setPosition(mainNode->getPosition().x, mainNode->getPosition().y, mainNode->getPosition().z - 200);
	//mCamera->setPosition( Ogre::Vector3( 0, 0, -100 ) );
	//mCamera->lookAt( Ogre::Vector3::ZERO );
	//mCamera->lookAt(mainNode->getPosition());
	//mCamera->setAutoTracking( true, this->mSceneMgr->getSceneNode( "headNode" ) );

	// Set ambient light
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	/*
	Ogre::Plane * mPlane = new Ogre::Plane;
	mPlane->d = 0;
	mPlane->normal = Ogre::Vector3::UNIT_Y;
	Ogre::MeshManager::getSingleton().createPlane("BasePlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		*mPlane, 10000, 10000, 1, 1, true, 1, 100, 100, Ogre::Vector3::UNIT_Z);

	Ogre::Entity * plane = mSceneMgr->createEntity("Plane", "BasePlane");
	plane->setMaterialName("BasePlane");

	Ogre::SceneNode * planeNode = rendererViewNode->createChildSceneNode( "planeNode" );
	planeNode->attachObject(plane);
	planeNode->setPosition(Ogre::Vector3(0, -50, 0));

	delete mPlane;
	*/
}

void RendererView::createLight()
{
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	//mSceneMgr->setAmbientLight(Ogre::ColourValue::Blue);
	//mainLight = mSceneMgr->createLight("Main Light");
    //mainLight->setPosition(0, 100, -500);
}

void RendererView::resizeEvent( QResizeEvent * evt )
{
	if (mRenderWindow != NULL){
		mRenderWindow->windowMovedOrResized();
		mCamera->setAspectRatio( Ogre::Real(width()) / Ogre::Real(height()));
	}
}

Ogre::SceneManager * RendererView::getSceneManager() const
{
	return this->mSceneMgr;
}


// is functional?
void RendererView::keyPressEvent( QKeyEvent *k )
{
	/*
		W or Up:  Forward
		S or Down:Backward
		A:        Step left
		D:        Step right
		PgUp:     Move upwards
		PgDown:   Move downwards
		R:        Render mode
		T:        Cycle texture filtering
			   Bilinear, Trilinear, Anisotropic(8)
		P:        Toggle on/off display of camera position / orientation

		J:		  left rotate documentNode
		L:		  right rotate documentNode
		I:	      upward rotate documentNode
		K:		  downward rotate documentNode
	*/
#ifndef NDEBUG
		_cprintf( "remain key input: %d, ", k->count() );
		_cprintf( "input string: %s, ", k->text().constData() );
		_cprintf( "isAutorepeat: %d\n", k->isAutoRepeat() );
#endif

	// TODO: buffered input process correctly
	/*
	if( k->isAutoRepeat() )
	{
		k->ignore();
		return;
	}*/

	switch ( k->key() ) 
	{
		case Qt::Key_W:
			mTranslateVector.z -= mMoveScale;
			break;

		case Qt::Key_S:
			mTranslateVector.z += mMoveScale;
            break;

        case Qt::Key_A:
			mTranslateVector.x -= mMoveScale;
            break;

		case Qt::Key_D:
			mTranslateVector.x += mMoveScale;
            break;

		case Qt::Key_PageUp:
			break;

		case Qt::Key_PageDown:
			break;

		case Qt::Key_R:
			mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
			switch(mSceneDetailIndex) {
				case 0 : mCamera->setPolygonMode(Ogre::PM_SOLID) ; break ;
				case 1 : mCamera->setPolygonMode(Ogre::PM_WIREFRAME) ; break ;
				case 2 : mCamera->setPolygonMode(Ogre::PM_POINTS) ; break ;
			}
			break;

		case Qt::Key_P:
#ifndef NDEBUG
			{
				char buffer[ 80 ];
				const Ogre::Vector3 pos( mCamera->getPosition() );
				sprintf( buffer, "%.3f, %.3f, %.3f", pos.x, pos.y, pos.z );

				_cprintf( "Key P pressed\n" );
				_cprintf( "camera position: %s\n", buffer );
			}
#endif
			this->mCamera->setPosition( mDefaultCameraPosition );
			mTranslateVector = Ogre::Vector3::ZERO;
			break;

		case Qt::Key_I:
			mTranslateVector.y += mMoveScale;
			this->rotateDocumentNode( Ogre::Vector3::UNIT_X );
			break;

		case Qt::Key_K:
			mTranslateVector.y -= mMoveScale;
			this->rotateDocumentNode( Ogre::Vector3::UNIT_X );
			break;

		case Qt::Key_J:
			mTranslateVector.x -= mMoveScale;
			this->rotateDocumentNode( Ogre::Vector3::UNIT_Y );
			break;

		case Qt::Key_L:
			mTranslateVector.x += mMoveScale;
			this->rotateDocumentNode( Ogre::Vector3::UNIT_Y );
			break;
	}

	k->accept();
	moveCamera();
}


void RendererView::moveCamera()
{
	this->mCamera->moveRelative( this->mTranslateVector );
	this->mTranslateVector = Ogre::Vector3::ZERO;
}

void RendererView::rotateDocumentNode( const Ogre::Vector3 & axis )
{
	Ogre::SceneNode * n = this->mSceneMgr->getSceneNode( "documentNode" );
	n->rotate( axis, Ogre::Radian( Ogre::Degree( this->mTranslateVector.length() ) ) );
	
	this->mTranslateVector = Ogre::Vector3::ZERO;
}

void RendererView::updateStats(void)
{
    static Ogre::String currFps = "Current FPS: ";
    static Ogre::String avgFps = "Average FPS: ";
    static Ogre::String bestFps = "Best FPS: ";
    static Ogre::String worstFps = "Worst FPS: ";
    static Ogre::String tris = "Triangle Count: ";

    // update stats when necessary
    try {
        OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
        OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
        OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
        OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

		const Ogre::RenderTarget::FrameStats& stats = this->mRenderWindow->getStatistics();

        guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(stats.avgFPS));
        guiCurr->setCaption(currFps + Ogre::StringConverter::toString(stats.lastFPS));
        guiBest->setCaption(bestFps + Ogre::StringConverter::toString(stats.bestFPS)
			+" "+ Ogre::StringConverter::toString(stats.bestFrameTime)+" ms");
		guiWorst->setCaption(worstFps + Ogre::StringConverter::toString(stats.worstFPS)
			+" "+ Ogre::StringConverter::toString(stats.worstFrameTime)+" ms");

		Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + Ogre::StringConverter::toString(stats.triangleCount));

        Ogre::OverlayElement* guiDbg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
        guiDbg->setCaption(mRenderWindow->getDebugText());
    }
    catch(...)
    {
        // ignore
    }
}
/*
void RendererView::showDebugOverlay(bool show)
{
    if (mDebugOverlay)
    {
        if (show)
        {
            mDebugOverlay->show();
        }
        else
        {
            mDebugOverlay->hide();
        }
    }
}
*/