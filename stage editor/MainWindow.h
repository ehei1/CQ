#pragma once
#pragma message("Compiling MainWindow.h - this should happen just once per project.\n")
#include "ui_MainWindow.h"
#include "RendererView.h"
#include "Document.h"

class MainWindow :
	public Ui_MainWindow
{
	Q_OBJECT

public:
	MainWindow(void);
	~MainWindow(void);
	virtual void setupUi( QMainWindow * );

protected:
	RendererView * rendererView;
	QDockWidget * rendererDock;
	::QScrollArea * scrollArea;
	QMainWindow * mWindow;
	::Document * document;

	void SetGeneralProperty();
	void SetWorldProperty();
	void SetLightProperty();
	void SetMaterialProperty();
	void SetFogProperty();
	void SetObjectProperty();
	std::vector< std::string > skyboxList;

	void RunAsCheckedContainerButton();
	void RunAsCheckedEggButton();
	void RunAsCheckedLightButton();

	int MessageBox( const std::string & message, 
		const std::string & yes_ment = "&Yes", const std::string & no_ment = "&Discard", 
		const std::string & title = "Builder");	
	bool hasFile( const std::string & );
	void SetGUI();

private slots:
	// CAUTION: when you add slot function, you should run moc compiling again.
	// menu
	void ShowNewDialog();
	void ShowOpenDialog();
	void ShowSaveDialog();
	void ShowSaveAsDialog();
	void ShowQuitDialog();

	void ShowPropertyEditor();
	void ShowMessage();
	void ShowDefaultLayout();
	
	void ShowAboutDialog();

	// general
	void MixEgg();
	void SetNumericData( int ); // by slide bar action
	void SetLimitTime( const QString & );
	void SetAnswerQuantity( const QString & );

	// object
	void SetContainerX( int );
	void SetContainerY( int );
	void SetContainerZ( int );
	void SetContainerAngle( int );

	void SetContainerMove();
	void SetContainerRotate();
	void SetContainerScale();

	void SetContainerXFromText( const QString & );
	void SetContainerYFromText( const QString & );
	void SetContainerZFromText( const QString & );
	void SetContainerAngleFromText( const QString & );

	void SetEggX( int );
	void SetEggY( int );
	void SetEggZ( int );
	void SetEggAngle( int );

	void SetEggMove();
	void SetEggRotate();
	void SetEggScale();

	void SetEggXFromText( const QString & );
	void SetEggYFromText( const QString & );
	void SetEggZFromText( const QString & );
	void SetEggAngleFromText( const QString & );

	// world
	void ReplaceContainer();
	void ReplaceContainerMaterial( int );
	void ReplaceSkybox( int );
	void ReplaceEgg();

	void SetRenderState( int );
	void SetCameraPosition( int );
	void SetDefaultCameraPostion();
	void GoCameraStartPosition();
	void SetCameraXPosition( int ); // input from lineEdit, slide
	void SetCameraYPosition( int ); // input from lineEdit, slide
	void SetCameraZPosition( int ); // input from lineEdit, slide

	void SetCameraXPositionFromText( const QString & );
	void SetCameraYPositionFromText( const QString & );
	void SetCameraZPositionFromText( const QString & );

	// light
	void SetAmbientLightRed( int ); // input from lineEdit, slide
	void SetAmbientLightGreen( int ); // input from lineEdit, slide
	void SetAmbientLightBlue( int ); // input from lineEdit, slide

	void SetAmbientLightRedFromText( const QString & );
	void SetAmbientLightGreenFromText( const QString & );
	void SetAmbientLightBlueFromText( const QString & );

	void SetLightX( int );
	void SetLightY( int );
	void SetLightZ( int );
	void SetLightPower( int );

	void SetLightXFromText( const QString & );
	void SetLightYFromText( const QString & );
	void SetLightZFromText( const QString & );
	void SetLightPowerFromText( const QString & );

	void SetLightDiffuse();
	void SetLightSpecular();
	void SetLightPosition();
	void SetLightDirection();

	// material
	void SetFogType( int );
	void SetFogDensity( int );
	void SetFogDensityFromText( const QString & );
	void SetFogLinearStart( const QString & );
	void SetFogLinearEnd( const QString & );
	void SetFogFadeColorRed( int );
	void SetFogFadeColorGreen( int );
	void SetFogFadeColorBlue( int );

	void UseMaterial(); // add to using list
	void UnuseMaterial(); // remove from using list	

	// widget
	void ClosePropertyEditor();
	void CloseMessage();
};