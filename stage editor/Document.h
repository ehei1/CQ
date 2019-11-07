#pragma once
#pragma message("Compiling Document.h - this should happen just once per project.\n")

#include "ConfigData.h"
#include "Data.h"
#include "Exception.h"

class Document
{
public:
	Document( Ogre::SceneManager * sceneManager );
	~Document(void);

	void New();
	void Save();
	void SaveAs ( const std::string & );
	void Load( const std::string & );

	void Undo();
	void Redo();

	void replaceContainer( const std::string & );
	void replaceSkybox( const std::string & ); // input: material name
	void replaceEgg( const std::string & ); // input: mesh
	void mixEgg();

	// world	
	void moveCamera();

	// object
	void moveContainer();
	void rotateContainer();
	void scaleContainer();

	void moveEgg();
	void rotateEgg();
	void scaleEgg();

	// light
	void setAmbientLight();
	void setLightDiffuse( const std::string & );
	void setLightSpecular( const std::string & );
	void setLightDirection( const std::string & );
	void setLightPosition( const std::string & );
	void setLightPowerScale( const std::string & );
	void setLightType( const std::string & );
	void setFog();

	bool isModified() const { return _isModified; }

	// control light
	//virtual bool Add() = 0;
	//virtual bool Delete() = 0;
	//virtual bool Modify() = 0;
	//virtual bool Select() = 0;

public:
	Data data;

private:
	// method
	void BuildWorld( const std::string & );
	void Clear();
	void GetValue( const Ogre::SubMesh *, Ogre::Vector3 & position, float & radius);

	typedef std::vector< std::pair< size_t, Ogre::Vector3 > > SphereInfo;
	void GetRadiusFromSubEntity( SphereInfo & info, std::string & meshName );
	void SetEgg();
	void ExtractEgg( const std::string & );
	void deleteAllEgg();
	
	std::string GetRandomNumeric();
	std::string GetRandomOperator();

	// property	
	Ogre::SceneNode * documentNode; // generated entity from document should attach only this node
	::Exception exception; // check file error and etc...
	ConfigData configData;

	bool _isModified;
};