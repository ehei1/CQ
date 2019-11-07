#include "stdafx.h"
#include "ui_MainWindow.h"
#include "ui_About.h"
#include "RendererView.h"
#include "Document.h"
#include "Data.h"

#include "MainWindow.h"
#include "moc_MainWindow.cpp" // moc MainWindow.h -o moc_MainWindow.cpp. you should run moc when MainWindow.h is modified


MainWindow::MainWindow(void) :
document( NULL )
{
}

MainWindow::~MainWindow(void)
{
	if( document )
		delete document;
}


void MainWindow::setupUi( QMainWindow * mainWindow )
{
	Ui_MainWindow::setupUi( mainWindow );

	mWindow = mainWindow;
	// renderer dock widget
	/*
	rendererDock = new ::QDockWidget( "Renderer", 0 );
	rendererView = new RendererView( rendererDock ); //renderer view + docking
	rendererDock->setWidget( rendererView );

	mainWindow->setCentralWidget( rendererDock );
	*/
	rendererView = new RendererView( mainWindow ); //renderer view + docking
	mainWindow->setCentralWidget( rendererView );

	// property dock widget
	this->scrollArea = new ::QScrollArea( Ui_MainWindow::propertyDockWidget );
	this->scrollArea->setWidget( Ui_MainWindow::tabWidget );
	Ui_MainWindow::propertyDockWidget->setWidget( this->scrollArea );

	mainWindow->addDockWidget( Qt::LeftDockWidgetArea, Ui_MainWindow::propertyDockWidget );

	// message dock widget
	mainWindow->addDockWidget( Qt::BottomDockWidgetArea, Ui_MainWindow::messageDockWidget );

	mainWindow->show();

	this->textEditMessage->insertPlainText( "== CalcKing Builder 0.90a wrote by Lee Ungju ehei0001@gmail.com ==\n" );
	this->textEditMessage->insertPlainText( "== This program is under free license. ==\n" );
	this->textEditMessage->insertPlainText( "Loading default document...\n" );

	document = new ::Document( rendererView->getSceneManager() );
	
	this->textEditMessage->insertPlainText( "Loading material...\n" );

	SetMaterialProperty();

	this->textEditMessage->insertPlainText( "Setting editor properties...\n" );

	SetGUI();

	this->textEditMessage->insertPlainText( "Connecting Qt signal with slot...\n" );

	// connect signals with slots
	// NOTICE: you cannot use 'this->' in signal. If you did, Signal-Slot sysem isn't functional

	// menu
	{
		QObject::connect( action_New, SIGNAL( triggered() ), this, SLOT( ShowNewDialog() ) );
		QObject::connect( action_Open, SIGNAL( triggered() ), this, SLOT( ShowOpenDialog() ) );
		QObject::connect( action_Save, SIGNAL( triggered() ), this, SLOT( ShowSaveDialog() ) );
		QObject::connect( action_Save_As, SIGNAL( triggered() ), this, SLOT( ShowSaveAsDialog() ) );
		QObject::connect( action_Quit, SIGNAL( triggered() ), this, SLOT( ShowQuitDialog() ) );

		QObject::connect( action_Property_Editor, SIGNAL( triggered() ), this, SLOT( ShowPropertyEditor() ) );
		QObject::connect( action_Message, SIGNAL( triggered() ), this, SLOT( ShowMessage() ) );
		QObject::connect( action_Default_Layout, SIGNAL( triggered() ), this, SLOT( ShowDefaultLayout() ) );

		QObject::connect( action_About, SIGNAL( triggered() ), this, SLOT( ShowAboutDialog() ) );
	}
	
	// property editor
	{
		// general
		QObject::connect( mixEggPushButton, SIGNAL( clicked() ), this, SLOT( MixEgg() ) );
		QObject::connect( numericRateHorizontalSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetNumericData( int ) ) );
		QObject::connect( LimitTimeLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetLimitTime( const QString & ) ) );
		QObject::connect( AnswerQuantityLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetAnswerQuantity( const QString & ) ) );

		// object
		QObject::connect( containerXSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetContainerX( int ) ) );
		QObject::connect( containerYSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetContainerY( int ) ) );
		QObject::connect( containerZSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetContainerZ( int ) ) );
		QObject::connect( containerAngleSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetContainerAngle( int ) ) );

		QObject::connect( containerMoveRadioButton, SIGNAL( clicked() ), this, SLOT( SetContainerMove() ) );
		QObject::connect( containerRotateRadioButton, SIGNAL( toggled( bool ) ), this, SLOT( SetContainerRotate() ) );
		QObject::connect( containerScaleRadioButton, SIGNAL( clicked() ), this, SLOT( SetContainerScale() ) );

		QObject::connect( containerXLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetContainerXFromText( const QString & ) ) );
		QObject::connect( containerYLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetContainerYFromText( const QString & ) ) );
		QObject::connect( containerZLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetContainerZFromText( const QString & ) ) );
		QObject::connect( containerAngleLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetContainerAngleFromText( const QString & ) ) );

		QObject::connect( eggXSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetEggX( int ) ) );
		QObject::connect( eggYSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetEggY( int ) ) );
		QObject::connect( eggZSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetEggZ( int ) ) );
		QObject::connect( eggAngleSlider, SIGNAL( currentIndexChanged( int ) ), this, SLOT( SetEggAngle( int ) ) );

		QObject::connect( eggXLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetEggXFromText( const QString & ) ) );
		QObject::connect( eggYLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetEggYFromText( const QString & ) ) );
		QObject::connect( eggZLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetEggZFromText( const QString & ) ) );
		QObject::connect( eggAngleLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetEggAngleFromText( const QString & ) ) );

		QObject::connect( eggMoveRadioButton, SIGNAL( clicked() ), this, SLOT( SetEggMove() ) );
		QObject::connect( eggRotateRadioButton, SIGNAL( clicked() ), this, SLOT( SetEggRotate() ) );
		QObject::connect( eggScaleRadioButton, SIGNAL( clicked() ), this, SLOT( SetEggScale() ) );

		// world
		QObject::connect( containerReplacePushButton, SIGNAL( clicked() ), this, SLOT( ReplaceContainer() ) );
		QObject::connect( containerMaterialComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( ReplaceContainerMaterial( int ) ) );

		QObject::connect( skyboxComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( ReplaceSkybox( int ) ) );
		QObject::connect( eggReplacePushButton, SIGNAL( clicked() ), this, SLOT( ReplaceEgg() ) );
		QObject::connect( renderTypeComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( SetRenderState( int ) ) );
		QObject::connect( cameraComboBox, SIGNAL ( currentIndexChanged( int ) ), this, SLOT( SetCameraPosition( int ) ) );
		QObject::connect( setCameraPosPushButton, SIGNAL( clicked() ), this, SLOT( SetDefaultCameraPostion() ) );
		QObject::connect( cameraGoStartPosPushButton, SIGNAL( clicked() ), this, SLOT( GoCameraStartPosition() ) );

		QObject::connect( cameraPosXSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetCameraXPosition( int ) ) );
		QObject::connect( cameraPosYSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetCameraYPosition( int ) ) );
		QObject::connect( cameraPosZSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetCameraZPosition( int ) ) );


		

		// light: ambient
		QObject::connect( ambientLightRSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetAmbientLightRed( int ) ) );
		QObject::connect( ambientLightGSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetAmbientLightGreen( int ) ) );
		QObject::connect( ambientLightBSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetAmbientLightBlue( int ) ) );

		QObject::connect( ambientLightRLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetAmbientLightRedFromText( const QString & ) ) );
		QObject::connect( ambientLightGLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetAmbientLightGreenFromText( const QString & ) ) );
		QObject::connect( ambientLightBLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetAmbientLightBlueFromText( const QString & ) ) );
		
		// light
		QObject::connect( LightXSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetLightX( int ) ) );
		QObject::connect( LightYSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetLightY( int ) ) );
		QObject::connect( LightZSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetLightZ( int ) ) );
		QObject::connect( lightPowerSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetLightPower( int ) ) );

		QObject::connect( lightXLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetLightXFromText( const QString & ) ) );
		QObject::connect( lightYLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetLightYFromText( const QString & ) ) );
		QObject::connect( lightZLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetLightZFromText( const QString & ) ) );
		QObject::connect( lightPowerLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetLightPowerFromText( const QString & ) ) );

		QObject::connect( lightDiffuseRadioButton, SIGNAL( clicked() ), this, SLOT( SetLightDiffuse() ) );
		QObject::connect( lightSpecularRadioButton, SIGNAL( clicked() ), this, SLOT( SetLightSpecular() ) );
		QObject::connect( lightPositionRadioButton, SIGNAL( clicked() ), this, SLOT( SetLightPosition() ) );
		QObject::connect( lightDirectionRadioButton, SIGNAL( clicked() ), this, SLOT( SetLightDirection() ) );

		// material
		QObject::connect( fogModeComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( SetFogType( int ) ) );
		QObject::connect( fogDensitySlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetFogDensity( int ) ) );
		QObject::connect( fogDensityLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetFogDensityFromText( const QString & ) ) );
		QObject::connect( fogLinearStartLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetFogLinearStart( const QString & ) ) );
		QObject::connect( fogLinearEndLineEdit, SIGNAL( textChanged( const QString & ) ), this, SLOT( SetFogLinearEnd( const QString & ) ) );
		QObject::connect( fogFadeColorRedSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetFogFadeColorRed( int ) ) );
		QObject::connect( fogFadeColorGreenSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetFogFadeColorGreen( int ) ) );
		QObject::connect( fogFadeColorBlueSlider, SIGNAL( valueChanged( int ) ), this, SLOT( SetFogFadeColorBlue( int ) ) );

		QObject::connect( materialAddPushButton, SIGNAL( clicked() ), this, SLOT( UseMaterial() ) );
		QObject::connect( materialDeletePushButton, SIGNAL( clicked() ), this, SLOT( UnuseMaterial() ) );

		// widget // not functional // you should override DockWidget...
		QObject::connect( propertyDockWidget, SIGNAL( hide() ), this, SLOT( ClosePropertyEditor() ) );
		QObject::connect( messageDockWidget, SIGNAL( close() ), this, SLOT( CloseMessage() ) );
	}

	this->textEditMessage->insertPlainText( "Loading complete.\n" );
	this->textEditMessage->insertHtml( "<font color=red>Welcome to Builder for CalcKing</font><br>" );
	this->textEditMessage->insertHtml( "<font color=gray>Hint: Defalut setting loaded from first.calc. If you change default value, you should edit it.</font><br>" );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}


void MainWindow::SetGeneralProperty()
{
	char buffer[ 80 ];
	const Data & d = document->data;

	// set value
	{
		// set question range
		sprintf( buffer, "%d", d.valueData.eggNumericRange.first );
		this->numericRangeQuestionFromLineEdit->setText( buffer );

		sprintf( buffer, "%d", d.valueData.eggNumericRange.second );
		this->numericRangeQuestionToLineEdit->setText( buffer );

		// set answer range
		sprintf( buffer, "%d", d.valueData.answerNumericRange.first );
		this->numericRangeAnswerFromLineEdit->setText( buffer );

		sprintf( buffer, "%d", d.valueData.answerNumericRange.second );
		this->numericRangeAnswerToLineEdit->setText( buffer );

		// set answer qty
		sprintf( buffer, "%d", d.valueData.answerQuantity );
		this->AnswerQuantityLineEdit->setText( buffer );
	}
	
	// set operator
	this->sumCheckBox->setCheckState( ( d.operatorData.sum ) ? Qt::Checked : Qt::Unchecked );
	this->subtractCheckBox->setCheckState( ( d.operatorData.subtract ) ? Qt::Checked : Qt::Unchecked );
	this->multiplyCheckBox->setCheckState( ( d.operatorData.multiply ) ? Qt::Checked : Qt::Unchecked );
	this->bracketCheckBox->setCheckState( ( d.operatorData.bracket ) ? Qt::Checked : Qt::Unchecked );
	this->squareCheckBox->setCheckState( ( d.operatorData.square ) ? Qt::Checked : Qt::Unchecked );
	this->factorialCheckBox->setCheckState( ( d.operatorData.factorial ) ? Qt::Checked : Qt::Unchecked );
	this->invertCheckBox->setCheckState( ( d.operatorData.invert ) ? Qt::Checked : Qt::Unchecked );
	this->modCheckBox->setCheckState( ( d.operatorData.mod ) ? Qt::Checked : Qt::Unchecked );

	// set numeric rate, qty of numeric & operation
	sprintf( buffer, "%f", d.valueData.numericRate );
	//this->NumericRateLineEdit->setText( buffer );
	this->numericRateHorizontalSlider->setValue( static_cast< int >( atof( buffer ) * 100 ) );
	
	const int numericQty = d.eggData.egg.size() * d.valueData.numericRate;
	sprintf( buffer, "%d", numericQty );
	this->numericQtyTextLabel->setText( buffer );

	sprintf( buffer, "%d", d.eggData.egg.size() - numericQty );
	this->operationQtyTextLabel->setText( buffer );

	// set limit time
	sprintf( buffer, "%d", d.valueData.limitTime );
	this->LimitTimeLineEdit->setText( buffer );
}

void MainWindow::SetWorldProperty()
{
	const Data & d = document->data;
	char buffer[ 80 ];

	// skybox
	const int index = this->skyboxComboBox->findText( d.skyboxData.material.c_str() );
	this->skyboxComboBox->setCurrentIndex( index );

#ifndef NDEBUG
	_cprintf(" current skybox: %s\n", d.skyboxData.material.c_str() );
#endif
	// set container
	this->containerMeshLineEdit->setText( d.containerData.mesh.c_str() );

	// set egg
	this->eggMeshLineEdit->setText( d.eggData.mesh.c_str() );

	// set camera
	{
		this->cameraPosXSlider->setValue( static_cast< int >( d.cameraData.position.x ) );
		this->cameraPosYSlider->setValue( static_cast< int >( d.cameraData.position.y ) );
		this->cameraPosZSlider->setValue( static_cast< int >( d.cameraData.position.z ) );

		sprintf( buffer, "%0.1f", d.cameraData.position.x );
		this->cameraPosXLineEdit->setText( buffer );
		sprintf( buffer, "%0.1f", d.cameraData.position.y );
		this->cameraPosYLineEdit->setText( buffer );
		sprintf( buffer, "%0.1f", d.cameraData.position.z );
		this->cameraPosZLineEdit->setText( buffer );
	}
}

void MainWindow::SetLightProperty()
{
	const int sliderValueMax = this->LightXSlider->maximum(); // 100000: magic const, this is slider bar max value
	char buffer[ 80 ];
	const Data & d = document->data;

	// set ambient light
	{
		this->ambientLightRSlider->setValue( static_cast< int >( sliderValueMax * d.ambientLight.r ) );
		this->ambientLightGSlider->setValue( static_cast< int >( sliderValueMax * d.ambientLight.g ) );
		this->ambientLightBSlider->setValue( static_cast< int >( sliderValueMax * d.ambientLight.b ) );

		sprintf( buffer, "%0.3f", d.ambientLight.r );
		this->ambientLightRLineEdit->setText( buffer );
		sprintf( buffer, "%0.3f", d.ambientLight.g );
		this->ambientLightGLineEdit->setText( buffer );
		sprintf( buffer, "%0.3f", d.ambientLight.b );
		this->ambientLightBLineEdit->setText( buffer );

#ifndef NDEBUG
		_cprintf( "ambient rgb: %d %d %d\n", static_cast< int >( sliderValueMax * d.ambientLight.r ), static_cast< int >( sliderValueMax * d.ambientLight.g ), static_cast< int >( sliderValueMax * d.ambientLight.b ) );

#endif
	}

	// regist all light to combox box
	std::map< std::string, Data::LightData >::const_iterator it;

	for( it = d.lightMap.begin(); it != d.lightMap.end(); ++ it )
	{
		this->lightSelectComboBox->addItem( it->first.c_str() );
	}

	this->lightSelectComboBox->setCurrentIndex( 0 );

	// set property of first registed light
	{
		const Data::LightData & l =  d.lightMap.begin()->second;	

		// set position
		this->lightDiffuseRadioButton->setChecked( true );
		this->LightXSlider->setValue( static_cast< int >( l.diffuse.r * sliderValueMax ) );  // 100000: magic const, this is slider bar max value
		this->LightYSlider->setValue( static_cast< int >( l.diffuse.g * sliderValueMax ) );
		this->LightZSlider->setValue( static_cast< int >( l.diffuse.b * sliderValueMax ) );
		
		sprintf( buffer, "%0.3f", l.diffuse.r );
		this->lightXLineEdit->setText( buffer );
		sprintf( buffer, "%0.3f", l.diffuse.g );
		this->lightYLineEdit->setText( buffer );
		sprintf( buffer, "%0.3f", l.diffuse.b );
		this->lightZLineEdit->setText( buffer );

		// set power
		this->lightPowerSlider->setValue( static_cast< int >( sliderValueMax * l.power ) ); // 100000: magic const, this is slider bar max value
		sprintf( buffer, "%0.3f", l.power );
		this->lightPowerLineEdit->setText( buffer );
		
		// set type: current no use
		switch( l.type )
		{
		case Ogre::Light::LT_DIRECTIONAL:
			this->lightTypeComboBox->setCurrentIndex( 1 );
			break;
		case Ogre::Light::LT_POINT:
			this->lightTypeComboBox->setCurrentIndex( 0 );
			break;
		case Ogre::Light::LT_SPOTLIGHT:
			this->lightTypeComboBox->setCurrentIndex( 2 );
			break;
		}
	}
}

void MainWindow::SetObjectProperty()
{
	//char buffer[ 80 ];
	const Data & d = document->data;

	// set container property
	this->containerMoveRadioButton->setChecked( true );
	this->containerXSlider->setValue( static_cast< int >( this->document->data.containerData.position.x ) );
	this->containerYSlider->setValue( static_cast< int >( this->document->data.containerData.position.y ) );
	this->containerZSlider->setValue( static_cast< int >( this->document->data.containerData.position.z ) );


	// set egg property
	this->eggMoveRadioButton->setChecked( true );
	this->eggXSlider->setValue( static_cast< int >( this->document->data.eggData.position.x ) );
	this->eggYSlider->setValue( static_cast< int >( this->document->data.eggData.position.y ) );
	this->eggZSlider->setValue( static_cast< int >( this->document->data.eggData.position.z ) );
}

void MainWindow::SetMaterialProperty()
{
	Ogre::MaterialManager::ResourceMapIterator i = Ogre::MaterialManager::getSingletonPtr()->getResourceIterator();
	
	while( i.hasMoreElements() )
	{
		Ogre::Resource * r = i.getNext().get();

		const std::string & material( r->getName() );
		const size_t length = material.size() - 1;

		if( length > material.find( "SkyBox" ) )
		{
			static int index = 0;

			this->skyboxComboBox->insertItem( index ++, material.c_str() );
		}
		else
		{
			static int materialIndex = 0;

			this->materialAllListWidget->insertItem( materialIndex, material.c_str() );
			this->containerMaterialComboBox->insertItem( materialIndex , material.c_str() );
			++ materialIndex;
		}
	}

	const std::string & defaultSkybox = this->document->data.skyboxData.material;

	// set current container material
	this->containerMaterialComboBox->setCurrentIndex(
		this->containerMaterialComboBox->findText( QString( this->document->data.containerData.material.c_str() ) )
	);

	// set default skybox on combobox
	if( this->skyboxComboBox->maxCount() )
	{
		this->skyboxComboBox->setCurrentIndex(
			this->skyboxComboBox->findText( QString( defaultSkybox.c_str() ) )
		);
	}

	// set fog type
	{
		int i = 0;
		this->fogModeComboBox->insertItem( i++, "FOG_NONE" );
		this->fogModeComboBox->insertItem( i++, "FOG_LINEAR" );
		this->fogModeComboBox->insertItem( i++, "FOG_EXP" );
		this->fogModeComboBox->insertItem( i++, "FOG_EXP2" );
	}
}

void MainWindow::ReplaceContainer()
{
	//textEditMessage->insertPlainText( tr( "It did.\n" ) );
	QString f = QFileDialog::getOpenFileName(
		this, tr("Open Container File"), QDir::currentPath() + "/media/models", tr("Ogre Mesh Files (*.mesh)")
	);

	if( ! f.toStdString().size() )
		return;

	QStringList strList = f.split( "/" );
	QList<QString>::const_iterator i = strList.constEnd();
	--i;

	document->replaceContainer( ( *i ).toStdString() );
	this->ReplaceContainerMaterial( 0 );

	this->textEditMessage->insertPlainText( "Container replaced with \"" + f + "\"\n" );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::ReplaceSkybox( int i )
{
#ifndef NDEBUG
	_cprintf( "new skybox selected: %s\n", skyboxComboBox->itemText( i ).toStdString().c_str() );
#endif
	document->replaceSkybox( skyboxComboBox->itemText( i ).toStdString() );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::ReplaceEgg()
{
	QString f = QFileDialog::getOpenFileName(
		this, tr("Open Egg File"), QDir::currentPath() + "/media/models", tr("Ogre Mesh Files (*.mesh)")
	);

#ifndef NDEBUG
	_cprintf( "file name: %s\n", f.toStdString().c_str() );

#endif
	if( ! f.toStdString().size() )
		return;

	QStringList strList = f.split( "/" );
	QList<QString>::const_iterator i = strList.constEnd();
	--i;

	document->replaceEgg( ( *i ).toStdString() );

	this->textEditMessage->insertPlainText( "Egg replaced with \"" + f + "\"\n" );
	this->textEditMessage->insertHtml( "<font color=gray>Hint: eggs are defined by sub entites property in mesh. Builder use their radius, position. You should remind mesh file including eggs.</font><br>" );
}

void MainWindow::MixEgg()
{
#ifndef NDEBUG
	_cprintf( "* * MainWindow::MixEgg() * *\n" );
#endif
	// set inputed constant
	this->document->data.valueData.numericRate = 0.01f * this->numericRateHorizontalSlider->value();
	this->document->data.valueData.eggNumericRange.first = static_cast< int >( ::atoi( this->numericRangeQuestionFromLineEdit->text().toStdString().c_str() ) );
	this->document->data.valueData.eggNumericRange.second = static_cast< int >( ::atoi( this->numericRangeQuestionToLineEdit->text().toStdString().c_str() ) );

	this->textEditMessage->insertPlainText( "Collecting current general properties...\n" );
	// set using operator
	{
		std::list< std::string > & l = this->document->data.operatorData.usingOperator;
		l.clear();
#ifndef NDEBUG
		_cprintf( "sumCheckBox: %d\n", ! abs( sumCheckBox->text().compare( "true" ) ) );
#endif
		if( Qt::Checked == sumCheckBox->checkState() )
		{
			l.push_back( "+" );
		}

		if( Qt::Checked == subtractCheckBox->checkState() )
		{
			l.push_back( "-" );
		}

		if( Qt::Checked == multiplyCheckBox->checkState() )
		{
			l.push_back( "*" );
		}

		if( Qt::Checked == bracketCheckBox->checkState() )
		{
			l.push_back( "(" );
			l.push_back( ")" );
		}

		if( Qt::Checked == squareCheckBox->checkState() )
		{
			l.push_back( "^" );
		}

		if( Qt::Checked == modCheckBox->checkState() )
		{
			l.push_back( "%" );
		}

		if( Qt::Checked == factorialCheckBox->checkState() )
		{
			l.push_back( "!" );
		}

		if( Qt::Checked == invertCheckBox->checkState() )
		{
			l.push_back( "~" );
		}			
	}

	this->textEditMessage->insertPlainText( "Mixing all egg's value...\n" );
	document->mixEgg();

	this->textEditMessage->insertPlainText( "Eggs mixing complete.\n" );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::SetNumericData( int i )
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetNumericData()\n" );
#endif

	char buffer[ 80 ];

	const float rate = 0.01f * i;
	const size_t total = this->document->data.eggData.egg.size();
	const size_t numQty = static_cast< size_t >( this->document->data.eggData.egg.size() * rate );
	const size_t opQty = total - numQty;

	sprintf( buffer, "%d", numQty );
	this->numericQtyTextLabel->setText( buffer );

	sprintf( buffer, "%d", opQty );
	this->operationQtyTextLabel->setText( buffer );

#ifndef NDEBUG
	_cprintf( "num qty: %d, op qty: %d\n", numQty, opQty );
#endif
}

void MainWindow::UseMaterial()
{
	// find selected item
	const int index = this->materialAllListWidget->currentRow();
	
	if( -1 < index )
	{
		QListWidgetItem* item = this->materialAllListWidget->item( index );

		// insert use listview
		this->materialUsingListWidget->addItem( item );

		// delete all listview
		this->materialAllListWidget->takeItem( index );

		this->textEditMessage->insertPlainText( "Material selected to use: " + item->text() + "\n" );
	}	
}

void MainWindow::UnuseMaterial()
{
	const int index = this->materialUsingListWidget->currentRow();
	
	if( -1 < index )
	{
		QListWidgetItem* item = this->materialUsingListWidget->item( index );

		// insert use listview
		this->materialAllListWidget->addItem( item );

		// delete all listview
		this->materialUsingListWidget->takeItem( index );

		this->textEditMessage->insertPlainText( "Material selected to unuse: " + item->text() + "\n" );

		QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
		scrollBar->setValue(scrollBar->maximum());
	}
}

void MainWindow::SetRenderState( int i )
{
	Ogre::Camera * c = this->rendererView->getSceneManager()->getCamera( "defaultCamera" );

	switch( i )
	{
	case 0:
		c->setPolygonMode(Ogre::PM_SOLID);
		this->textEditMessage->insertPlainText( "Render state changed to solid mode.\n" );
		break;
	case 1:
		c->setPolygonMode(Ogre::PM_WIREFRAME);
		this->textEditMessage->insertPlainText( "Render state changed to wire frame mode.\n" );
		break;
	case 2:
		c->setPolygonMode(Ogre::PM_POINTS);
		this->textEditMessage->insertPlainText( "Render state changed to points mode.\n" );
		break;
	}

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::SetCameraPosition( int i )
{
	const float interval = 100.0f;
	Ogre::Camera * c = this->rendererView->getSceneManager()->getCamera( "defaultCamera" );

	char buffer[ 80 ];
	switch( i )
	{
	case 0: // Left
		c->setPosition( interval, 0, 0 );
		sprintf( buffer, "%0.0f, %0.0f, %0.0f", interval, 0, 0 );
		this->textEditMessage->insertPlainText( "Camera moved to left.(" + QString( buffer ) + ")\n" );
		break;
	case 1: // Right
		c->setPosition( -interval, 0, 0 );
		sprintf( buffer, "%0.0f, %0.0f, %0.0f", -interval, 0, 0 );
		this->textEditMessage->insertPlainText( "Camera moved to right.(" + QString( buffer ) + ")\n" );
		break;
	case 2: // Front
		c->setPosition( 0, 0, -interval );
		sprintf( buffer, "%0.0f, %0.0f, %0.0f", 0, 0, -interval );
		this->textEditMessage->insertPlainText( "Camera moved to front.(" + QString( buffer ) + ")\n" );
		break;
	case 3: // Back
		c->setPosition( 0, 0, interval );
		sprintf( buffer, "%0.0f, %0.0f, %0.0f", 0, 0, interval );
		this->textEditMessage->insertPlainText( "Camera moved to back.(" + QString( buffer ) + ")\n" );
		break;
	case 4: // Upon
		c->setPosition( 0, interval, interval );
		sprintf( buffer, "%0.0f, %0.0f, %0.0f", 0, interval, interval );
		this->textEditMessage->insertPlainText( "Camera moved to upon.(" + QString( buffer ) + ")\n" );
		break;
	case 5: // Below
		c->setPosition( 0, -interval, -interval );
		sprintf( buffer, "%0.0f, %0.0f, %0.0f", 0, -interval, -interval );
		this->textEditMessage->insertPlainText( "Camera moved to below.(" + QString( buffer ) + ")\n" );
		break;
	}

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::SetDefaultCameraPostion()
{
	Ogre::Camera * c = this->rendererView->getSceneManager()->getCamera( "defaultCamera" );
	this->document->data.cameraData.position = c->getPosition();

	char buffer[ 80 ];
	sprintf( buffer, "%0.0f, %0.0f, %0.0f", c->getPosition().x, c->getPosition().y, c->getPosition().z );
	this->textEditMessage->insertPlainText( "It saved for default start position in game.(" + QString( buffer ) + ")\n" );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());

#ifndef NDEBUG
	Ogre::Vector3 & p = this->document->data.cameraData.position;

	_cprintf( "MainWindow::SetDefaultCameraPostion()\n" );
	_cprintf( "%f %f %f\n", p.x, p.y, p.z );
#endif
}

void MainWindow::RunAsCheckedContainerButton()
{
	if( this->containerMoveRadioButton->isChecked() )
	{
		this->document->moveContainer();
	}
	else if( this->containerRotateRadioButton->isChecked() )
	{
		this->document->rotateContainer();
	}
	else if( this->containerScaleRadioButton->isChecked() )
	{
		this->document->scaleContainer();
	}
}

void MainWindow::RunAsCheckedEggButton()
{
	if( this->eggMoveRadioButton->isChecked() )
	{
		this->document->moveEgg();
	}
	else if( this->eggRotateRadioButton->isChecked() )
	{
		this->document->rotateEgg();
	}
	else if( this->eggScaleRadioButton->isChecked() )
	{
		this->document->scaleEgg();
	}
}

void MainWindow::RunAsCheckedLightButton()
{
	const int index = lightSelectComboBox->currentIndex();
	const std::string name = lightSelectComboBox->itemText( index ).toStdString();

	if( this->lightDiffuseRadioButton->isChecked() )
	{
		document->setLightDiffuse( name );
	}
	else if( this->lightSpecularRadioButton->isChecked() )
	{
		document->setLightSpecular( name );
	}
	else if( this->lightDirectionRadioButton->isChecked() )
	{
		document->setLightDirection( name );
	}
	else if( this->lightPositionRadioButton->isChecked() )
	{
		document->setLightPosition( name );
	}
}

// object
void MainWindow::SetContainerX( int i )
{
	char buffer[ 80 ];
	sprintf( buffer, "%d", i );

	if( this->containerMoveRadioButton->isChecked() )
	{
		document->data.containerData.position.x = i;
		this->containerXLineEdit->setText( buffer );
	}
	else if( this->containerRotateRadioButton->isChecked() )
	{
		document->data.containerData.rotate.x = i;
		this->containerXLineEdit->setText( buffer );
	}
	else if( this->containerScaleRadioButton->isChecked() )
	{
		const float ratio = ( float ) i / 100; // 100: value 'i' change to percentage
		document->data.containerData.scale.x = ratio;

		sprintf( buffer, "%0.3f", ratio );
		this->containerXLineEdit->setText( buffer );

		if( containerConstrainedCheckBox->isChecked() )
		{	
			document->data.containerData.scale = Ogre::Vector3( ratio, ratio, ratio );
			
			this->containerYSlider->setSliderPosition( i );
			this->containerYSlider->setValue( i );
			this->containerYLineEdit->setText( buffer );

			this->containerZSlider->setSliderPosition( i );
			this->containerZSlider->setValue( i );
			this->containerZLineEdit->setText( buffer );
		}
	}

	RunAsCheckedContainerButton();
}

void MainWindow::SetContainerY( int i )
{
	char buffer[ 80 ];
	sprintf( buffer, "%d", i );

	if( this->containerMoveRadioButton->isChecked() )
	{
		document->data.containerData.position.y = i;
		this->containerYLineEdit->setText( buffer );
	}
	else if( this->containerRotateRadioButton->isChecked() )
	{
		document->data.containerData.rotate.y = i;
		this->containerYLineEdit->setText( buffer );
	}
	else if( this->containerScaleRadioButton->isChecked() )
	{
		const float ratio = ( float ) i / 100; // 100: value 'i' change to percentage
		document->data.containerData.scale.y = ratio;

		sprintf( buffer, "%0.3f", ratio );
		this->containerYLineEdit->setText( buffer );
		
		if( containerConstrainedCheckBox->isChecked() )
		{
			document->data.containerData.scale = Ogre::Vector3( ratio, ratio, ratio );

			this->containerXSlider->setSliderPosition( i );
			this->containerXSlider->setValue( i );
			this->containerXLineEdit->setText( buffer );

			this->containerZSlider->setSliderPosition( i );
			this->containerZSlider->setValue( i );
			this->containerZLineEdit->setText( buffer );
		}
	}
	
	RunAsCheckedContainerButton();
}

void MainWindow::SetContainerZ( int i )
{
	char buffer[ 80 ];
	sprintf( buffer, "%d", i );

	if( this->containerMoveRadioButton->isChecked() )
	{
		document->data.containerData.position.z = i;
		this->containerZLineEdit->setText( buffer );
	}
	else if( this->containerRotateRadioButton->isChecked() )
	{
		document->data.containerData.rotate.z = i;
		this->containerZLineEdit->setText( buffer );
	}
	else if( this->containerScaleRadioButton->isChecked() )
	{
		const float ratio = ( float ) i / 100; // 100: value 'i' change to percentage
		document->data.containerData.scale.z = ratio;

		sprintf( buffer, "%0.3f", ratio );
		this->containerZLineEdit->setText( buffer );

		if( containerConstrainedCheckBox->isChecked() )
		{
			document->data.containerData.scale = Ogre::Vector3( ratio, ratio, ratio );

			this->containerYSlider->setSliderPosition( i );
			this->containerYSlider->setValue( i );
			this->containerYLineEdit->setText( buffer );

			this->containerXSlider->setSliderPosition( i );
			this->containerXSlider->setValue( i );
			this->containerXLineEdit->setText( buffer );
		}
	}	
	
	RunAsCheckedContainerButton();
}

void MainWindow::SetContainerAngle( int i )
{
	document->data.containerData.rotate.w = Ogre::Radian( Ogre::Degree( i ) ).valueRadians();

	if( containerAngleSlider->hasFocus() )
	{
		RunAsCheckedContainerButton();
	}
}

void MainWindow::SetEggX( int i )
{
	char buffer[ 80 ];
	sprintf( buffer, "%d", i );

	if( this->eggMoveRadioButton->isChecked() )
	{
		document->data.eggData.position.x = i;
		eggXLineEdit->setText( buffer );
	}
	else if( this->eggRotateRadioButton->isChecked() )
	{
		document->data.eggData.rotate.x = i;
		eggXLineEdit->setText( buffer );
	}
	else if( this->eggScaleRadioButton->isChecked() )
	{
		const float ratio = static_cast< float >( i ) / 100; // 100: value 'i' change to percentage
		document->data.eggData.scale.x = ratio;

		sprintf( buffer, "%0.3f", ratio );
		this->eggXLineEdit->setText( buffer );

		if( eggConstrainedCheckBox->isChecked() )
		{
			document->data.eggData.scale = Ogre::Vector3( ratio, ratio, ratio );

			this->eggYSlider->setSliderPosition( i );
			this->eggYSlider->setValue( i );
			this->eggYLineEdit->setText( buffer );

			this->eggZSlider->setSliderPosition( i );
			this->eggZSlider->setValue( i );
			this->eggZLineEdit->setText( buffer );
		}
	}
	
	RunAsCheckedEggButton();
}

void MainWindow::SetEggY( int i )
{
	char buffer[ 80 ];
	sprintf( buffer, "%d", i );

	if( this->eggMoveRadioButton->isChecked() )
	{
		document->data.eggData.position.y = i;
		eggYLineEdit->setText( buffer );
	}
	else if( this->eggRotateRadioButton->isChecked() )
	{
		document->data.eggData.rotate.y = i;
		eggYLineEdit->setText( buffer );
	}
	else if( this->eggScaleRadioButton->isChecked() )
	{
		const float ratio = ( float ) i / 100; // 100: value 'i' change to percentage
		document->data.eggData.scale.y = ratio;

		sprintf( buffer, "%0.3f", ratio );
		this->eggYLineEdit->setText( buffer );

		if( eggConstrainedCheckBox->isChecked() )
		{
			document->data.eggData.scale = Ogre::Vector3( ratio, ratio, ratio );

			this->eggXSlider->setSliderPosition( i );
			this->eggXSlider->setValue( i );
			this->eggXLineEdit->setText( buffer );

			this->eggZSlider->setSliderPosition( i );
			this->eggZSlider->setValue( i );
			this->eggZLineEdit->setText( buffer );
		}
	}
	
	RunAsCheckedEggButton();
}

void MainWindow::SetEggZ( int i )
{
	char buffer[ 80 ];
	sprintf( buffer, "%d", i );

	if( this->eggMoveRadioButton->isChecked() )
	{
		document->data.eggData.position.z = i;
		eggZLineEdit->setText( buffer );
	}
	else if( this->eggRotateRadioButton->isChecked() )
	{
		document->data.eggData.rotate.z = i;
		eggZLineEdit->setText( buffer );
	}
	else if( this->eggScaleRadioButton->isChecked() )
	{
		const float ratio = ( float ) i / 100; // 100: value 'i' change to percentage
		document->data.eggData.scale.z = ratio;

		sprintf( buffer, "%0.3f", ratio );
		this->eggZLineEdit->setText( buffer );

		if( eggConstrainedCheckBox->isChecked() )
		{
			document->data.eggData.scale = Ogre::Vector3( ratio, ratio, ratio );

			this->eggYSlider->setSliderPosition( i );
			this->eggYSlider->setValue( i );
			this->eggYLineEdit->setText( buffer );

			this->eggXSlider->setSliderPosition( i );
			this->eggXSlider->setValue( i );
			this->eggXLineEdit->setText( buffer );
		}
	}

	RunAsCheckedEggButton();
}

void MainWindow::SetEggAngle( int i )
{
	document->data.eggData.rotate.w = Ogre::Radian( Ogre::Degree( i ) ).valueRadians();

	if( eggAngleSlider->hasFocus() )
	{
		RunAsCheckedEggButton();
	}
}

void MainWindow::SetCameraXPosition( int i ) // input from lineEdit, slide
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetCameraXPosition( %d )\n", i );
#endif
	//document->data.cameraData.position.x = i;
	//document->moveCamera();
	Ogre::Camera * c = this->rendererView->getSceneManager()->getCamera( "defaultCamera" );
	Ogre::Vector3 p = c->getPosition();
	p.x = i;
	c->setPosition( p );

	char buffer[ 80 ];
	sprintf( buffer, "%d", i );
	cameraPosXLineEdit->setText( buffer );
}

void MainWindow::SetCameraYPosition( int i ) // input from lineEdit, slide
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetCameraYPosition( %d )\n", i );
#endif
	//document->data.cameraData.position.y = i;
	//document->moveCamera();
	Ogre::Camera * c = this->rendererView->getSceneManager()->getCamera( "defaultCamera" );
	Ogre::Vector3 p = c->getPosition();
	p.y = i;
	c->setPosition( p );

	char buffer[ 80 ];
	sprintf( buffer, "%d", i );
	cameraPosYLineEdit->setText( buffer );
}

void MainWindow::SetCameraZPosition( int i ) // input from lineEdit, slide
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetCameraZPosition( %d )\n", i );
#endif
	//document->data.cameraData.position.z = i;
	//document->moveCamera();
	Ogre::Camera * c = this->rendererView->getSceneManager()->getCamera( "defaultCamera" );
	Ogre::Vector3 p = c->getPosition();
	p.z = i;
	c->setPosition( p );

	char buffer[ 80 ];
	sprintf( buffer, "%d", i );
	cameraPosZLineEdit->setText( buffer );
}

// light
void MainWindow::SetAmbientLightRed( int i ) // input from lineEdit, slide
{
	const float color = ( float ) i / this->ambientLightRSlider->maximum();
	document->data.ambientLight.r = color;
	document->setAmbientLight();

	char buffer[ 80 ];
	sprintf( buffer, "%0.3f", color );
	ambientLightRLineEdit->setText( buffer );
#ifndef NDEBUG
	_cprintf( "MainWindow::SetAmbientLightRed( %d )\n", i );
	_cprintf( "\t color: %f buffer: %s\n", color, buffer );
	_cprintf( "\t minimum: %d maximum: %d\n", this->ambientLightRSlider->minimum(), this->ambientLightRSlider->maximum() );
#endif

}

void MainWindow::SetAmbientLightGreen( int i ) // input from lineEdit, slide
{
	const float color = ( float ) i / this->ambientLightRSlider->maximum();
	document->data.ambientLight.g = color;
	document->setAmbientLight();

	char buffer[ 80 ];
	sprintf( buffer, "%0.3f", color );
	ambientLightGLineEdit->setText( buffer );
}

void MainWindow::SetAmbientLightBlue( int i ) // input from lineEdit, slide
{
	const float color = ( float ) i / this->ambientLightRSlider->maximum();
	document->data.ambientLight.b = color;
	document->setAmbientLight();	

	char buffer[ 80 ];
	sprintf( buffer, "%0.3f", color );
	ambientLightBLineEdit->setText( buffer );
}

void MainWindow::SetLightX( int i )
{
	char buffer[ 80 ];
	const int index = lightSelectComboBox->currentIndex();
	const std::string name = lightSelectComboBox->itemText( index ).toStdString();

	if( this->lightDiffuseRadioButton->isChecked() )
	{
		float color = ( float ) i / this->LightXSlider->maximum();
		document->data.lightMap[ name ].diffuse.r = color;

		sprintf( buffer, "%0.3f", color );
	}
	else if( this->lightSpecularRadioButton->isChecked() )
	{
		float color = ( float ) i / this->LightXSlider->maximum();
		document->data.lightMap[ name ].specular.r = color;

		sprintf( buffer, "%0.3f", color );
	}
	else if( this->lightPositionRadioButton->isChecked() )
	{
		document->data.lightMap[ name ].position.x = i;
		sprintf( buffer, "%d", i );
	}
	else if( this->lightDirectionRadioButton->isChecked() )
	{
		document->data.lightMap[ name ].direction.x = i;
		sprintf( buffer, "%d", i );
	}

	lightXLineEdit->setText( buffer );

	//if( LightXSlider->hasFocus() )
	{
		RunAsCheckedLightButton();
	}
}

void MainWindow::SetLightY( int i )
{
	char buffer[ 80 ];
	const int index = lightSelectComboBox->currentIndex();
	const std::string name = lightSelectComboBox->itemText( index ).toStdString();

	if( this->lightDiffuseRadioButton->isChecked() )
	{
		float color = ( float ) i / this->LightYSlider->maximum();
		document->data.lightMap[ name ].diffuse.g = color;

		sprintf( buffer, "%0.3f", color );
	}
	else if( this->lightSpecularRadioButton->isChecked() )
	{
		float color = ( float ) i / this->LightYSlider->maximum();
		document->data.lightMap[ name ].specular.b = color;

		sprintf( buffer, "%0.3f", color );
	}
	else if( this->lightPositionRadioButton->isChecked() )
	{
		document->data.lightMap[ name ].position.y = i;
		sprintf( buffer, "%d", i );
	}
	else if( this->lightDirectionRadioButton->isChecked() )
	{
		document->data.lightMap[ name ].direction.y = i;
		sprintf( buffer, "%d", i );
	}

	lightYLineEdit->setText( buffer );

	//if( LightYSlider->hasFocus() )
	{
		RunAsCheckedLightButton();
	}
}

void MainWindow::SetLightZ( int i )
{
	char buffer[ 80 ];
	const int index = lightSelectComboBox->currentIndex();
	const std::string name = lightSelectComboBox->itemText( index ).toStdString();

	if( this->lightDiffuseRadioButton->isChecked() )
	{
		float color = ( float ) i / this->LightZSlider->maximum();
		document->data.lightMap[ name ].diffuse.b = color;

		sprintf( buffer, "%0.3f", color );
	}
	else if( this->lightSpecularRadioButton->isChecked() )
	{
		float color = ( float ) i / this->LightZSlider->maximum();
		document->data.lightMap[ name ].specular.b = color;

		sprintf( buffer, "%0.3f", color );
	}
	else if( this->lightPositionRadioButton->isChecked() )
	{
		document->data.lightMap[ name ].position.z = i;
		sprintf( buffer, "%d", i );
	}
	else if( this->lightDirectionRadioButton->isChecked() )
	{
		document->data.lightMap[ name ].direction.z = i;
		sprintf( buffer, "%d", i );
	}

	lightZLineEdit->setText( buffer );

	//if( LightZSlider->hasFocus() )
	{
		RunAsCheckedLightButton();
	}
}

void MainWindow::SetLightPower( int i )
{
	this->textEditMessage->insertPlainText( "You cannot changed light power. It will do later.\n" );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
	// later
}

void MainWindow::SetContainerMove()
{
	char buffer[ 80 ];
	const int min = -300;
	const int max = 300;

	containerXSlider->setMaximum( max );
	containerYSlider->setMaximum( max );
	containerZSlider->setMaximum( max );

	containerXSlider->setMinimum( min );
	containerYSlider->setMinimum( min );
	containerZSlider->setMinimum( min );

	const Ogre::Vector3 & pos = document->data.containerData.position;

	containerXSlider->setValue( pos.x );
	containerYSlider->setValue( pos.y );
	containerZSlider->setValue( pos.z );

	containerXSlider->setSliderPosition( pos.x );
	containerYSlider->setSliderPosition( pos.y );
	containerZSlider->setSliderPosition( pos.z );

	sprintf( buffer, "%d", pos.x );
	containerXLineEdit->setText( buffer );
	sprintf( buffer, "%d", pos.y );
	containerYLineEdit->setText( buffer );
	sprintf( buffer, "%d", pos.z );
	containerZLineEdit->setText( buffer );

	this->containerAngleLabel->setEnabled( false );
	this->containerAngleSlider->setEnabled( false );
	this->containerAngleLineEdit->setEnabled( false );
	containerConstrainedCheckBox->setEnabled( false );
}

void MainWindow::SetContainerRotate()
{
	char buffer[ 80 ];
	const int min = -300;
	const int max = 300;

	containerXSlider->setMaximum( max );
	containerYSlider->setMaximum( max );
	containerZSlider->setMaximum( max );

	containerXSlider->setMinimum( min );
	containerYSlider->setMinimum( min );
	containerZSlider->setMinimum( min );

	const Ogre::Quaternion & q = document->data.containerData.rotate;

	containerXSlider->setValue( q.x );
	containerYSlider->setValue( q.y );
	containerZSlider->setValue( q.z );

	containerXSlider->setSliderPosition( q.x );
	containerYSlider->setSliderPosition( q.y );
	containerZSlider->setSliderPosition( q.z );

	sprintf( buffer, "%d", q.x );
	containerXLineEdit->setText( buffer );
	sprintf( buffer, "%d", q.y );
	containerYLineEdit->setText( buffer );
	sprintf( buffer, "%d", q.z );
	containerZLineEdit->setText( buffer );

	this->containerAngleLabel->setEnabled( true );
	this->containerAngleSlider->setEnabled( true );
	this->containerAngleLineEdit->setEnabled( true );
	containerConstrainedCheckBox->setEnabled( false );
}

void MainWindow::SetContainerScale()
{
	char buffer[ 80 ];
	const int min = 0;
	const int max = 300;

	containerXSlider->setMaximum( max );
	containerYSlider->setMaximum( max );
	containerZSlider->setMaximum( max );

	containerXSlider->setMinimum( min );
	containerYSlider->setMinimum( min );
	containerZSlider->setMinimum( min );

	const Ogre::Vector3 & scale = document->data.containerData.scale;

	containerXSlider->setValue( static_cast< int >( scale.x * 100 ) );
	containerYSlider->setValue( static_cast< int >( scale.y * 100 ) );
	containerZSlider->setValue( static_cast< int >( scale.z * 100 ) );

	containerXSlider->setSliderPosition( static_cast< int >( scale.x * 100 ) );
	containerYSlider->setSliderPosition( static_cast< int >( scale.y * 100 ) );
	containerZSlider->setSliderPosition( static_cast< int >( scale.z * 100 ) );

	sprintf( buffer, "%0.3f", scale.x );
	containerXLineEdit->setText( buffer );
	sprintf( buffer, "%0.3f", scale.y );
	containerYLineEdit->setText( buffer );
	sprintf( buffer, "%0.3f", scale.z );
	containerZLineEdit->setText( buffer );

	this->containerAngleLabel->setEnabled( false );
	this->containerAngleSlider->setEnabled( false );
	this->containerAngleLineEdit->setEnabled( false );
	containerConstrainedCheckBox->setEnabled( true );
}

void MainWindow::SetEggMove()
{
	char buffer[ 80 ];
	const int min = -300;
	const int max = 300;

	eggXSlider->setMaximum( max );
	eggYSlider->setMaximum( max );
	eggZSlider->setMaximum( max );

	eggXSlider->setMinimum( min );
	eggYSlider->setMinimum( min );
	eggZSlider->setMinimum( min );

	const Ogre::Vector3 & v = document->data.eggData.position;

	eggXSlider->setValue( v.x );
	eggYSlider->setValue( v.y );
	eggZSlider->setValue( v.z );

	eggXSlider->setSliderPosition( v.x );
	eggYSlider->setSliderPosition( v.y );
	eggZSlider->setSliderPosition( v.z );

	sprintf( buffer, "%d", v.x );
	eggXLineEdit->setText( buffer );
	sprintf( buffer, "%d", v.y );
	eggYLineEdit->setText( buffer );
	sprintf( buffer, "%d", v.z );
	eggZLineEdit->setText( buffer );

	this->eggAngleLabel->setEnabled( false );
	this->eggAngleSlider->setEnabled( false );
	this->eggAngleLineEdit->setEnabled( false );
	eggConstrainedCheckBox->setEnabled( false );
}

void MainWindow::SetEggRotate()
{
    char buffer[ 80 ];
	const int min = -300;
	const int max = 300;

	eggXSlider->setMaximum( max );
	eggYSlider->setMaximum( max );
	eggZSlider->setMaximum( max );

	eggXSlider->setMinimum( min );
	eggYSlider->setMinimum( min );
	eggZSlider->setMinimum( min );

	const Ogre::Quaternion & q = document->data.eggData.rotate;

	eggXSlider->setValue( q.x );
	eggYSlider->setValue( q.y );
	eggZSlider->setValue( q.z );

	eggXSlider->setSliderPosition( q.x );
	eggYSlider->setSliderPosition( q.y );
	eggZSlider->setSliderPosition( q.z );

	sprintf( buffer, "%d", q.x );
	eggXLineEdit->setText( buffer );
	sprintf( buffer, "%d", q.y );
	eggYLineEdit->setText( buffer );
	sprintf( buffer, "%d", q.z );
	eggZLineEdit->setText( buffer );

	this->eggAngleLabel->setEnabled( true );
	this->eggAngleSlider->setEnabled( true );
	this->eggAngleLineEdit->setEnabled( true );
	eggConstrainedCheckBox->setEnabled( false );
}

void MainWindow::SetEggScale()
{
    char buffer[ 80 ];
	const int min = 0;
	const int max = 300;

	eggXSlider->setMaximum( max );
	eggYSlider->setMaximum( max );
	eggZSlider->setMaximum( max );

	eggXSlider->setMinimum( min );
	eggYSlider->setMinimum( min );
	eggZSlider->setMinimum( min );

	const Ogre::Vector3 & v = document->data.eggData.scale;

	eggXSlider->setValue( static_cast< int >( v.x * 100 ) );
	eggYSlider->setValue( static_cast< int >( v.y * 100 ) );
	eggZSlider->setValue( static_cast< int >( v.z * 100 ) );

	eggXSlider->setSliderPosition( static_cast< int >( v.x * 100 ) );
	eggYSlider->setSliderPosition( static_cast< int >( v.y * 100 ) );
	eggZSlider->setSliderPosition( static_cast< int >( v.z * 100 ) );

	sprintf( buffer, "%0.3f", v.x );
	eggXLineEdit->setText( buffer );
	sprintf( buffer, "%0.3f", v.y );
	eggYLineEdit->setText( buffer );
	sprintf( buffer, "%0.3f", v.z );
	eggZLineEdit->setText( buffer );

	this->eggAngleLabel->setEnabled( false );
	this->eggAngleSlider->setEnabled( false );
	this->eggAngleLineEdit->setEnabled( false );
	eggConstrainedCheckBox->setEnabled( true );
}

void MainWindow::SetLightDiffuse()
{
	lightXLabel->setText( "r" );
	lightYLabel->setText( "g" );
	lightZLabel->setText( "b" );

	LightXSlider->setMinimum( 0 );
	LightXSlider->setMaximum( 100000 );
	LightYSlider->setMinimum( 0 );
	LightYSlider->setMaximum( 100000 );
	LightZSlider->setMinimum( 0 );
	LightZSlider->setMaximum( 100000 );

	const int index = lightSelectComboBox->currentIndex();
	const std::string name = lightSelectComboBox->itemText( index ).toStdString();
	const Data::LightData & data = document->data.lightMap[ name ];

	LightXSlider->setValue( static_cast< int >( data.diffuse.r * LightXSlider->maximum() ) );
	LightYSlider->setValue( static_cast< int >( data.diffuse.g * LightXSlider->maximum() ) );
	LightZSlider->setValue( static_cast< int >( data.diffuse.b * LightXSlider->maximum() ) );

	char buffer[ 80 ];
	sprintf( buffer, "%0.3f", data.diffuse.r );
	lightXLineEdit->setText( buffer );
	sprintf( buffer, "%0.3f", data.diffuse.g );
	lightYLineEdit->setText( buffer );
	sprintf( buffer, "%0.3f", data.diffuse.b );
	lightZLineEdit->setText( buffer );
}

void MainWindow::SetLightSpecular()
{
	lightXLabel->setText( "r" );
	lightYLabel->setText( "g" );
	lightZLabel->setText( "b" );

	LightXSlider->setMinimum( 0 );
	LightXSlider->setMaximum( 100000 );
	LightYSlider->setMinimum( 0 );
	LightYSlider->setMaximum( 100000 );
	LightZSlider->setMinimum( 0 );
	LightZSlider->setMaximum( 100000 );

	const int index = lightSelectComboBox->currentIndex();
	const std::string name = lightSelectComboBox->itemText( index ).toStdString();
	const Data::LightData & data = document->data.lightMap[ name ];

	LightXSlider->setValue( static_cast< int >( data.specular.r * LightXSlider->maximum() ) );
	LightYSlider->setValue( static_cast< int >( data.specular.g * LightXSlider->maximum() ) );
	LightZSlider->setValue( static_cast< int >( data.specular.b * LightXSlider->maximum() ) );

	char buffer[ 80 ];
	sprintf( buffer, "%0.3f", data.specular.r );
	lightXLineEdit->setText( buffer );
	sprintf( buffer, "%0.3f", data.specular.g );
	lightYLineEdit->setText( buffer );
	sprintf( buffer, "%0.3f", data.specular.b );
	lightZLineEdit->setText( buffer );
}

void MainWindow::SetLightDirection()
{
	lightXLabel->setText( "x" );
	lightYLabel->setText( "y" );
	lightZLabel->setText( "z" );

	LightXSlider->setMinimum( -1000 );
	LightXSlider->setMaximum( 1000 );
	LightYSlider->setMinimum( -1000 );
	LightYSlider->setMaximum( 1000 );
	LightZSlider->setMinimum( -1000 );
	LightZSlider->setMaximum( 1000 );

	const int index = lightSelectComboBox->currentIndex();
	const std::string name = lightSelectComboBox->itemText( index ).toStdString();
	const Data::LightData & data = document->data.lightMap[ name ];

	LightXSlider->setValue( data.direction.x );
	LightYSlider->setValue( data.direction.y );
	LightZSlider->setValue( data.direction.z );

	char buffer[ 80 ];
	sprintf( buffer, "%d", data.direction.x );
	lightXLineEdit->setText( buffer );
	sprintf( buffer, "%d", data.direction.y );
	lightYLineEdit->setText( buffer );
	sprintf( buffer, "%d", data.direction.z );
	lightZLineEdit->setText( buffer );
}

void MainWindow::SetLightPosition()
{
	lightXLabel->setText( "x" );
	lightYLabel->setText( "y" );
	lightZLabel->setText( "z" );

	LightXSlider->setMinimum( -1000 );
	LightXSlider->setMaximum( 1000 );
	LightYSlider->setMinimum( -1000 );
	LightYSlider->setMaximum( 1000 );
	LightZSlider->setMinimum( -1000 );
	LightZSlider->setMaximum( 1000 );

	const int index = lightSelectComboBox->currentIndex();
	const std::string name = lightSelectComboBox->itemText( index ).toStdString();
	const Data::LightData & data = document->data.lightMap[ name ];

	LightXSlider->setValue( data.position.x );
	LightYSlider->setValue( data.position.y );
	LightZSlider->setValue( data.position.z );

	char buffer[ 80 ];
	sprintf( buffer, "%d", data.position.x );
	lightXLineEdit->setText( buffer );
	sprintf( buffer, "%d", data.position.y );
	lightYLineEdit->setText( buffer );
	sprintf( buffer, "%d", data.position.z );
	lightZLineEdit->setText( buffer );
}


void MainWindow::SetContainerXFromText(const QString & s)
{
	if( ! this->containerXLineEdit->hasFocus() )
		return;

#ifndef NDEBUG
	_cprintf( "MainWindow::SetContainerXFromText(const QString & s)\n" );
#endif

	int value = 0;

	if( containerMoveRadioButton->isChecked() )
	{
		// min max check
		value = atoi( s.toStdString().c_str() );
	}
	else if( containerRotateRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}
	else if( containerScaleRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * 100;	
	}

	if( containerXSlider->minimum() <= value && containerXSlider->maximum() >= value )
	{
		this->containerXSlider->setSliderPosition( value );
		this->containerXSlider->setValue( value );
		SetContainerX( value );
	}

	//RunAsCheckedContainerButton();
}
void MainWindow::SetContainerYFromText(const QString & s)
{
	if( ! this->containerYLineEdit->hasFocus() )
		return;

#ifndef NDEBUG
	_cprintf( "MainWindow::SetContainerYFromText(const QString & s)\n" );
#endif

	int value = 0;

	if( containerMoveRadioButton->isChecked() )
	{
		// min max check
		value = atoi( s.toStdString().c_str() );
	}
	else if( containerRotateRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}
	else if( containerScaleRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * 100;	
	}

	if( containerYSlider->minimum() <= value && containerYSlider->maximum() >= value )
	{
		this->containerYSlider->setSliderPosition( value );
		this->containerYSlider->setValue( value );
		SetContainerY( value );
	}

	//RunAsCheckedContainerButton();
}

void MainWindow::SetContainerZFromText(const QString & s)
{
	if( ! this->containerZLineEdit->hasFocus() )
		return;

#ifndef NDEBUG
	_cprintf( "MainWindow::SetContainerZFromText(const QString & s)\n" );
#endif

	int value = 0;

	if( containerMoveRadioButton->isChecked() )
	{
		// min max check
		value = atoi( s.toStdString().c_str() );
	}
	else if( containerRotateRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}
	else if( containerScaleRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * 100;	
	}

	if( containerZSlider->minimum() <= value && containerZSlider->maximum() >= value )
	{
		this->containerZSlider->setSliderPosition( value );
		this->containerZSlider->setValue( value );
		SetContainerZ( value );
	}

	//RunAsCheckedContainerButton();
}

void MainWindow::SetContainerAngleFromText( const QString & s )
{
	if( ! this->containerAngleLineEdit->hasFocus() )
		return;

	const int value = atoi( s.toStdString().c_str() );

	if( containerAngleSlider->minimum() <= value && containerAngleSlider->maximum() >= value )
	{
		this->containerAngleSlider->setSliderPosition( value );
		this->containerAngleSlider->setValue( value );
		SetContainerAngle( value );
	}

	//RunAsCheckedContainerButton();
}

void MainWindow::SetEggXFromText( const QString & s )
{
	if( ! this->eggXLineEdit->hasFocus() )
		return;

	int value = 0;

	if( eggMoveRadioButton->isChecked() )
	{
		// min max check
		value = atoi( s.toStdString().c_str() );
	}
	else if( eggRotateRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}
	else if( eggScaleRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * 100;
	}

	if( eggXSlider->minimum() <= value && eggXSlider->maximum() >= value )
	{
		this->eggXSlider->setSliderPosition( value );
		this->eggXSlider->setValue( value );
		SetEggX( value );
	}

	//RunAsCheckedEggButton();
}

void MainWindow::SetEggYFromText( const QString & s )
{
	if( ! this->eggYLineEdit->hasFocus() )
		return;

	int value = 0;

	if( eggMoveRadioButton->isChecked() )
	{
		// min max check
		value = atoi( s.toStdString().c_str() );
	}
	else if( eggRotateRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}
	else if( eggScaleRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * 100;
	}

	if( eggYSlider->minimum() <= value && eggYSlider->maximum() >= value )
	{
		this->eggYSlider->setSliderPosition( value );
		this->eggYSlider->setValue( value );
		SetEggY( value );
	}

	//RunAsCheckedEggButton();
}

void MainWindow::SetEggZFromText( const QString & s )
{
	if( ! this->eggZLineEdit->hasFocus() )
		return;

	int value = 0;

	if( eggMoveRadioButton->isChecked() )
	{
		// min max check
		value = atoi( s.toStdString().c_str() );
	}
	else if( eggRotateRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}
	else if( eggScaleRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * 100;	
	}

	if( eggZSlider->minimum() <= value && eggZSlider->maximum() >= value )
	{
		this->eggZSlider->setSliderPosition( value );
		this->eggZSlider->setValue( value );
		SetEggZ( value );
	}

	//RunAsCheckedEggButton();
}

void MainWindow::SetEggAngleFromText( const QString & s )
{
	if( ! this->eggAngleLineEdit->hasFocus() )
		return;

	const int value = atoi( s.toStdString().c_str() );

	if( eggAngleSlider->minimum() <= value && eggAngleSlider->maximum() >= value )
	{
		this->eggAngleSlider->setSliderPosition( value );
		this->eggAngleSlider->setValue( value );
		SetEggAngle( value );
	}

	//RunAsCheckedEggButton();
}

void MainWindow::SetCameraXPositionFromText( const QString & s )
{
	const int value = atoi( s.toStdString().c_str() );

	if( cameraPosXSlider->minimum() <= value && cameraPosXSlider->maximum() >= value )
	{
		this->cameraPosXSlider->setSliderPosition( value );
		this->cameraPosXSlider->setValue( value );
		SetCameraXPosition( value );
	}
}

void MainWindow::SetCameraYPositionFromText( const QString & s )
{
	const int value = atoi( s.toStdString().c_str() );

	if( cameraPosYSlider->minimum() <= value && cameraPosYSlider->maximum() >= value )
	{
		this->cameraPosYSlider->setSliderPosition( value );
		this->cameraPosYSlider->setValue( value );
		SetCameraYPosition( value );
	}
}

void MainWindow::SetCameraZPositionFromText( const QString & s )
{
	const int value = atoi( s.toStdString().c_str() );

	if( cameraPosZSlider->minimum() <= value && cameraPosZSlider->maximum() >= value )
	{
		this->cameraPosZSlider->setSliderPosition( value );
		this->cameraPosZSlider->setValue( value );
		SetCameraZPosition( value );
	}
}

void MainWindow::SetAmbientLightRedFromText( const QString & s )
{
	if( ! this->ambientLightRLineEdit->hasFocus() )
		return;

	const float value = atof( s.toStdString().c_str() );

	if( ambientLightRSlider->minimum() <= value && ambientLightRSlider->maximum() >= value )
	{
		this->ambientLightRSlider->setSliderPosition( value * ambientLightRSlider->maximum() );
		this->ambientLightRSlider->setValue( value * ambientLightRSlider->maximum() );
		SetAmbientLightRed( value );
	}
}

void MainWindow::SetAmbientLightGreenFromText( const QString & s )
{
	if( ! this->ambientLightGLineEdit->hasFocus() )
		return;

	const float value = atof( s.toStdString().c_str() );

	if( ambientLightGSlider->minimum() <= value && ambientLightGSlider->maximum() >= value )
	{
		this->ambientLightGSlider->setSliderPosition( value * ambientLightGSlider->maximum() );
		this->ambientLightGSlider->setValue( value * ambientLightGSlider->maximum() );
		SetAmbientLightRed( value );
	}
}

void MainWindow::SetAmbientLightBlueFromText( const QString & s )
{
	if( ! this->ambientLightBLineEdit->hasFocus() )
		return;

	const float value = atof( s.toStdString().c_str() );

	if( ambientLightBSlider->minimum() <= value && ambientLightBSlider->maximum() >= value )
	{
		this->ambientLightBSlider->setSliderPosition( value * ambientLightBSlider->maximum() );
		this->ambientLightBSlider->setValue( value * ambientLightBSlider->maximum() );
		SetAmbientLightBlue( value );
	}

	RunAsCheckedLightButton();
}

void MainWindow::SetLightXFromText( const QString & s )
{
	if( ! this->lightXLineEdit->hasFocus() )
		return;

	int value = 0;

	if( lightDiffuseRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * LightXSlider->maximum();
	}
	else if( lightSpecularRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * LightXSlider->maximum();
	}
	else if( lightPositionRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}
	else if( lightDirectionRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}

	if( LightXSlider->minimum() <= value && LightXSlider->maximum() >= value )
	{
		this->LightXSlider->setSliderPosition( value );
		this->LightXSlider->setValue( value );
		SetLightX( value );
	}

	//RunAsCheckedLightButton();
}

void MainWindow::SetLightYFromText( const QString & s )
{
	if( ! this->lightYLineEdit->hasFocus() )
		return;

	int value = 0;

	if( lightDiffuseRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * LightYSlider->maximum();
	}
	else if( lightSpecularRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * LightYSlider->maximum();
	}
	else if( lightPositionRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}
	else if( lightDirectionRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}

	if( LightYSlider->minimum() <= value && LightYSlider->maximum() >= value )
	{
		this->LightYSlider->setSliderPosition( value );
		this->LightYSlider->setValue( value );
		SetLightY( value );
	}

	//RunAsCheckedLightButton();
}

void MainWindow::SetLightZFromText( const QString & s )
{
	if( ! this->lightZLineEdit->hasFocus() )
		return;

	int value = 0;

	if( lightDiffuseRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * LightZSlider->maximum();
	}
	else if( lightSpecularRadioButton->isChecked() )
	{
		value = atof( s.toStdString().c_str() ) * LightZSlider->maximum();
	}
	else if( lightPositionRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}
	else if( lightDirectionRadioButton->isChecked() )
	{
		value = atoi( s.toStdString().c_str() );
	}

	if( LightZSlider->minimum() <= value && LightZSlider->maximum() >= value )
	{
		this->LightZSlider->setSliderPosition( value );
		this->LightZSlider->setValue( value );
		SetLightZ( value );
	}

	//RunAsCheckedLightButton();
}

void MainWindow::SetLightPowerFromText( const QString & s )
{
	if( ! this->lightPowerLineEdit->hasFocus() )
		return;

	const int value = atof( s.toStdString().c_str() ) * lightPowerSlider->maximum();

	if( lightPowerSlider->minimum() <= value && lightPowerSlider->maximum() >= value )
	{
		this->lightPowerSlider->setSliderPosition( value );
		this->lightPowerSlider->setValue( value );
		SetLightPower( value );
	}
}

void MainWindow::ShowAboutDialog()
{
#ifndef NDEBUG
	_cprintf( "MainWindow::ShowAboutDialog()\n" );
#endif

	QDialog d;
	Ui_aboutDialog ui;
	ui.setupUi( & d );

	d.exec();
}

int MainWindow::MessageBox( const std::string & message, 
						   const std::string & yes_ment, const std::string & no_ment, 
						   const std::string & title)
{
	QMessageBox mb( title.c_str(),
		message.c_str(),
		QMessageBox::Information,
		QMessageBox::Yes | QMessageBox::Default,
		QMessageBox::No,
		QMessageBox::Cancel | QMessageBox::Escape);
	mb.setButtonText(QMessageBox::Yes, yes_ment.c_str() );
	mb.setButtonText(QMessageBox::No, no_ment.c_str() );

	return mb.exec();
}

void MainWindow::ShowNewDialog()
{
	// check modify
	if( this->document->isModified() )
	{
		// question whether save or not
		int result = MessageBox( "File Modified. Do you want save this?", "&Yes", "&Discard" );

		switch( result )
		{
		case 0:
			// call save dialog
			ShowSaveDialog();
			break;
		case 1:
			break;
		case 2:
			return;
			break;
		}
	}

	this->document->New();
	
	SetGUI();

	this->textEditMessage->insertPlainText( "Default setting loaded.\n" );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::ShowOpenDialog()
{
	if( this->document->isModified() )
	{
		int result = MessageBox( "File Modified. Do you want save this?", "&Yes", "&Discard" );

		switch( result )
		{
		case 0:
			ShowSaveDialog();
			break;
		case 1:
			break;
		case 2:
			return;
			break;
		}
	}
	
	QString f = QFileDialog::getOpenFileName(
		this, tr("Open Stage File"), QDir::currentPath() + "/stage/*.calc", tr("CalcKing Stage Files (*.calc)" ) // , & QString( "calc")
	);

#ifndef NDEBUG
	_cprintf( "file name: %s\n", f.toStdString().c_str() );
#endif

	if( f.toStdString().empty() )
		return;

	// open stage file
	this->document->Load( f.toStdString() );
	SetGUI();

	QStringList strList = f.split( "/" );
	QList<QString>::const_iterator i = strList.constEnd();
	--i;

	mWindow->setWindowTitle( *i + " - Builder" );

	this->textEditMessage->insertPlainText( "File opened \"" + f + "\"\n" );

	char buffer[ 80 ];
	sprintf( buffer, "standard radius: %f", this->document->data.eggData.standardRadius );
	this->textEditMessage->insertPlainText( "\"" + QString( buffer ) + "\"\n" );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::ShowSaveDialog()
{
	if( document->data.fileName.empty() )
	{	
		QString f = QFileDialog::getSaveFileName(
			this, tr("Save Stage File"), QDir::currentPath() + "/stage/*.calc", tr("CalcKing Stage Files (*.calc)")
		);

		if( ! f.isEmpty() )
		{
			this->document->SaveAs( f.toStdString() + ".calc" );
		}

		return;
	}

	this->document->Save();
	this->textEditMessage->insertPlainText( "File saved.\n" );
}

void MainWindow::ShowSaveAsDialog()
{
	QString f = QFileDialog::getSaveFileName(
		this, tr("Save Stage File"), QDir::currentPath() + "/stage/*.calc", tr("CalcKing Stage Files (*.calc)") //, & QString( "*.calc" ) 
	);

	if( f.isEmpty() )
		return;

	std::string name = f.contains( ".calc" ) ? f.toStdString() : f.toStdString() + ".calc";

	if( hasFile( name ) )
	{
		int result = MessageBox( "Do you want overwrite it?", "Overwrite" );

		switch( result )
		{
		case 0:
			break;
		case 1:
			ShowSaveAsDialog();
			break;
		case 2:
			return;
			break;
		}
	}

	this->document->SaveAs( name );

	QStringList strList = f.split( "/" );
	QList<QString>::const_iterator i = strList.constEnd();
	--i;

	mWindow->setWindowTitle( *i + " - Builder" );

	this->textEditMessage->insertPlainText( "File saved as \"" + f + "\"\n" );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::ShowQuitDialog()
{
	if( this->document->isModified() )
	{
		// question whether save or not
		int result = MessageBox( "File Modified. Do you want save this?", "&Yes", "&Discard" );

		switch( result )
		{
		case 0:
			// call save dialog
			ShowSaveDialog();
			break;
		case 1:
			this->close();
			break;
		case 2:
			break;
		}
	}
}

void MainWindow::ShowPropertyEditor()
{
#ifndef NDEBUG
	_cprintf( "MainWindow::ShowPropertyEditor()\n" );
#endif
	const bool isChecked = this->action_Property_Editor->isChecked();

	( isChecked ) ? propertyDockWidget->show() : propertyDockWidget->hide();
}

void MainWindow::ShowMessage()
{
	const bool isChecked = this->action_Message->isChecked();

	( isChecked ) ? messageDockWidget->show() : messageDockWidget->hide();
}

void MainWindow::ShowDefaultLayout()
{
	this->propertyDockWidget->setVisible( true );
	this->messageDockWidget->setVisible( true );
	
	this->action_Property_Editor->setChecked( true );
 	this->action_Message->setChecked( true );

	messageDockWidget->show();
	propertyDockWidget->show();

	this->textEditMessage->insertPlainText( "Default layout loaded.\n" );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

bool MainWindow::hasFile( const std::string & name )
{
	std::ifstream file( name.c_str() );

	return file.is_open();
}

void MainWindow::SetGUI()
{
	SetGeneralProperty();
	SetWorldProperty();
	SetLightProperty();
	SetFogProperty();
	//SetMaterialProperty();
	SetObjectProperty();
}

void MainWindow::SetLimitTime( const QString & s )
{
	 document->data.valueData.limitTime = atoi( s.toStdString().c_str() );
}

void MainWindow::SetAnswerQuantity( const QString & s )
{
	document->data.valueData.answerQuantity = atoi( s.toStdString().c_str() );
}

void MainWindow::SetFogProperty()
{
	char buffer[ 80 ];
	const Data & d = document->data;

	// fog type is registed in setMaterialProperty()

	std::string mode;
	switch( d.fogData.mode )
	{
		case Ogre::FOG_LINEAR:
			mode = "FOG_LINEAR";
			break;
		case Ogre::FOG_EXP:
			mode = "FOG_EXP";
			break;
		case Ogre::FOG_EXP2:
			mode = "FOG_EXP2";
			break;
		case Ogre::FOG_NONE:
			mode = "FOG_NONE";
			break;
	}

	// set combobox
	int index = this->fogModeComboBox->findText( mode.c_str() );
	this->fogModeComboBox->setCurrentIndex( index );

	// set density
	{
		sprintf( buffer, "%0.5f", d.fogData.density );
		this->fogDensityLineEdit->setText( buffer );

		const int max = this->fogDensitySlider->maximum();
		this->fogDensitySlider->setSliderPosition( d.fogData.density * max );
		this->fogDensitySlider->setValue( d.fogData.density * max );
	}

	// set linear
	sprintf( buffer, "%0.3f", d.fogData.linear.first );
	this->fogLinearStartLineEdit->setText( buffer );
	sprintf( buffer, "%0.3f", d.fogData.linear.second );
	this->fogLinearEndLineEdit->setText( buffer );

	// set fade color slider
	{
		const int max = this->fogFadeColorRedSlider->maximum();

		sprintf( buffer, "%0.3f", d.fogData.fade.r );
		this->fogFadeColorRedLabel->setText( buffer );
		sprintf( buffer, "%0.3f", d.fogData.fade.g );
		this->fogFadeColorGreenLabel->setText( buffer );
		sprintf( buffer, "%0.3f", d.fogData.fade.b );
		this->fogFadeColorBlueLabel->setText( buffer );

		this->fogFadeColorRedSlider->setSliderPosition( d.fogData.fade.r * max );
		this->fogFadeColorRedSlider->setValue( d.fogData.fade.r * max );
		this->fogFadeColorGreenSlider->setSliderPosition( d.fogData.fade.g * max );
		this->fogFadeColorGreenSlider->setValue( d.fogData.fade.g * max );
		this->fogFadeColorBlueSlider->setSliderPosition( d.fogData.fade.b * max );		
		this->fogFadeColorBlueSlider->setValue( d.fogData.fade.b * max );
	}
}

void MainWindow::SetFogType( int i )
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetFogType( %d )\n", i );
#endif
	Data::FogData & f = this->document->data.fogData;
	std::string name = this->fogModeComboBox->currentText().toStdString();

	if( name == "FOG_NONE" )
	{
		f.mode = Ogre::FOG_NONE;
	}
	else if( name == "FOG_LINEAR" )
	{
		f.mode = Ogre::FOG_LINEAR;
	}
	else if( name == "FOG_EXP" )
	{
		f.mode = Ogre::FOG_EXP;
	}
	else if( name == "FOG_EXP2" )
	{
		f.mode = Ogre::FOG_EXP2;
	}

	this->document->setFog();
}

void MainWindow::SetFogDensity( int i )
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetFogDensity( %d )\n", i );
#endif
	Data::FogData & f = this->document->data.fogData;
	f.density = ( float ) i / this->fogDensitySlider->maximum();
	
	if( this->fogDensitySlider->hasFocus() )
	{
		char buffer[ 80 ];
		sprintf( buffer, "%0.5f", f.density );
		this->fogDensityLineEdit->setText( buffer );
	}

	this->document->setFog();
}

void MainWindow::SetFogLinearStart( const QString & s )
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetFogLinearStart( %s )\n", s.toStdString().c_str() );
#endif
	Data::FogData & f = this->document->data.fogData;
	f.linear.first = atof( s.toStdString().c_str() );

	this->document->setFog();
}

void MainWindow::SetFogLinearEnd( const QString & s )
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetFogLinearEnd( %s )\n", s.toStdString().c_str() );
#endif
	Data::FogData & f = this->document->data.fogData;
	f.linear.second = atof( s.toStdString().c_str() );

	this->document->setFog();
}

void MainWindow::SetFogFadeColorRed( int i )
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetFogFadeColorRed( %d )\n", i );
#endif
	Data::FogData & f = this->document->data.fogData;
	f.fade.r = ( float ) i / this->fogFadeColorRedSlider->maximum();

	char buffer[ 80 ];
	sprintf( buffer, "%0.3f", f.fade.r );
	this->fogFadeColorRedLabel->setText( buffer );

	this->document->setFog();
}

void MainWindow::SetFogFadeColorGreen( int i )
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetFogFadeColorGreen( %d )\n", i );
#endif
	Data::FogData & f = this->document->data.fogData;
	f.fade.g = ( float ) i / this->fogFadeColorGreenSlider->maximum();

	char buffer[ 80 ];
	sprintf( buffer, "%0.3f", f.fade.g );
	this->fogFadeColorGreenLabel->setText( buffer );

	this->document->setFog();
}

void MainWindow::SetFogFadeColorBlue( int i )
{
#ifndef NDEBUG
	_cprintf( "MainWindow::SetFogFadeColorBlue( %d )\n", i );
#endif
	Data::FogData & f = this->document->data.fogData;
	f.fade.b = ( float ) i / this->fogFadeColorBlueSlider->maximum();

	char buffer[ 80 ];
	sprintf( buffer, "%0.3f", f.fade.b );
	this->fogFadeColorBlueLabel->setText( buffer );

	this->document->setFog();
}

void MainWindow::SetFogDensityFromText(const QString & s )
{
	if( ! this->fogDensityLineEdit->hasFocus() )
		return;

	const int max = this->fogDensitySlider->maximum();
	Data::FogData & f = this->document->data.fogData;
	f.density = atof( s.toStdString().c_str() );

	this->fogDensitySlider->setSliderPosition( f.density * max );
	this->fogDensitySlider->setValue( f.density * max );

	this->document->setFog();
}

void MainWindow::GoCameraStartPosition()
{
	document->moveCamera();

	char buffer[ 80 ];
	sprintf( buffer, "%0.0f, %0.0f, %0.0f", document->data.cameraData.position.x, document->data.cameraData.position.y, document->data.cameraData.position.z );
	this->textEditMessage->insertPlainText( "Camera moved starting position in game.( " + QString( buffer ) + ")\n" );

	QScrollBar *scrollBar = this->textEditMessage->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}

void MainWindow::ClosePropertyEditor()
{
	this->action_Property_Editor->setChecked( false );
}

void MainWindow::CloseMessage()
{
	this->action_Message->setChecked( false );
}

void MainWindow::ReplaceContainerMaterial( int i )
{
	std::string name = this->containerMaterialComboBox->currentText().toStdString();
	
	Ogre::SceneManager * m = this->rendererView->getSceneManager();
	Ogre::Entity * e = m->getEntity( "defaultContainer" );
	e->setMaterialName( name );

	document->data.containerData.material = name;
}