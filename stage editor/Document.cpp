#include "stdafx.h"
#include "ConfigData.h"
#include "Document.h"

#define INITFILENAME "first.calc"

char buffer[ 80 ];

Document::Document( Ogre::SceneManager * mSceneMgr )
: _isModified( false )
{
	try
	{
		if( NULL == mSceneMgr )
			throw Exception( Exception::SceneManagerIsNull );

		this->documentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "documentNode" );

		// load config data
		configData.load( "CalcKingConfig.txt" );

		//Load( INITFILENAME );
		BuildWorld( INITFILENAME );
	}
	catch( ::Exception & e )
	{
#ifndef NDEBUG
		// put to console e.getMessage();
		_cprintf( "* * exception: %s\n", e.getMessage().c_str() );
#endif
	}
}

Document::~Document(void)
{
	Clear();
}

void Document::New()
{
#ifndef NDEBUG
	//std::cout << "* * Document::New()" << std::endl;
	
#endif
	/* check in Qt
	// check modify
	//		yes: verify and clear
	//		no: new document

	if( this->isModified )
	{
		// request save
		// if answer is yes
		//		save
		// if answer is cancel
		//		stop New()
	}
	*/

	Clear();
	BuildWorld( INITFILENAME );
}

void Document::Clear()
{
	// detach all light, container, egg from SceneManager
	// this->documentNode->removeAndDestroyAllChildren(); // ?? functional???

	// delete container
	std::string entityName = "defaultContainer";
	std::string nodeName = entityName + "Node";
	Ogre::SceneManager * m = documentNode->getCreator();

	m->getSceneNode( nodeName )->detachObject( entityName );
	m->destroyEntity( entityName );
	m->destroySceneNode( nodeName );	

	// delete egg
	deleteAllEgg();

	// delete light
	std::map< std::string, Data::LightData >::const_iterator it = data.lightMap.begin();

	nodeName = "lightNode";

	for( ; it != data.lightMap.end(); ++ it )
	{
		m->getSceneNode( nodeName )->detachObject( it->first );
		m->destroyLight( it->first );
	}

	m->destroySceneNode( nodeName );

	this->_isModified = false;
}

void Document::BuildWorld( const std::string & name )
{
	data.load( name );
	
	// common local var for initializing
	Ogre::Entity * entity = 0;
	Ogre::SceneManager * m = this->documentNode->getCreator();

	//mWindow->getViewport(0)->setBackgroundColour( fadeColour );

	// set default camera location
	Ogre::Camera * c = m->getCamera( "defaultCamera" );
	c->setPosition( data.cameraData.position );
	c->lookAt( data.cameraData.lookAt );
	c->setAutoTracking( true, this->documentNode );

	// set default skybox	
	m->setSkyBox( true, data.skyboxData.material, data.skyboxData.range );

	// set egg
	SetEgg();

	// set default container
	{
		entity = m->createEntity( "defaultContainer", data.containerData.mesh ); // it's only one. thus, unique name don't need
		entity->setMaterialName( data.containerData.material );
		Ogre::SceneNode * cNode = this->documentNode->createChildSceneNode( "defaultContainerNode" );

		cNode->attachObject( entity );
		cNode->setPosition( this->data.containerData.position );
		cNode->scale( this->data.containerData.scale );
		cNode->rotate( this->data.containerData.rotate );
	}

	// set light
	{
		Ogre::SceneNode * lightNode = this->documentNode->createChildSceneNode( "lightNode" );
		std::map< std::string, Data::LightData >::const_iterator it;
		for( it = data.lightMap.begin(); it != data.lightMap.end(); ++ it )
		{
			Ogre::Light * l = m->createLight( it->first );
			const Data::LightData & data = it->second;
			
			l->setPosition( data.position );
			l->setType( data.type );
			l->setPowerScale( data.power ); // power is used only in High Dynamic Range(HDR) rendering

			if( Ogre::Light::LT_SPOTLIGHT == data.type )
			{
				l->setSpotlightRange( 
					Ogre::Radian( data.spotlightRangeDegree.first ), 
					Ogre::Radian( data.spotlightRangeDegree.second )
				); // TODO: constants are controlled by first.calc. fix Data class
			}

			Ogre::Vector3 dir = data.direction;
			dir.normalise();
			l->setDirection(dir);

			l->setDiffuseColour( data.diffuse );
			l->setSpecularColour( data.specular );
			l->setCastShadows( true );

			lightNode->attachObject( l );
		}
	}

	// set default ambient light
	m->setAmbientLight( data.ambientLight );

	// set default shadow
	m->setShadowTechnique( Ogre::SHADOWDETAILTYPE_TEXTURE ); // TODO: constants are controlled by first.calc. fix Data class

	// set fog
	setFog();	
}


void Document::Save()
{
	// export to Ogre config file
	// extract data from mSceneMgr and save to Data class
	// write to file. use serialize (ex: cout << str)
	data.save();
	_isModified = false;
}

void Document::SaveAs( const std::string & file )
{
	data.saveAs( file );
	_isModified = false;
}

void Document::Load( const std::string & name )
{
	// TODO: check save or cancel or ignore
	this->Clear();
	this->BuildWorld( name );
	this->data.fileName = name;
}

// TODO: SaveAs

void Document::Undo()
{
	// not yet
}

void Document::Redo()
{
	// not yet
}

void Document::GetValue( const Ogre::SubMesh * pSubMesh, Ogre::Vector3 & position, float & radius )
{
	struct MeshInfo
	{
		Ogre::Vector3 Size;
		Ogre::Vector3 Min;
		Ogre::Vector3 Max;
		Ogre::Vector3 Centre;
	}; 

	MeshInfo meshInfo ;

	Ogre::Vector3 vmin(FLT_MAX,FLT_MAX,FLT_MAX);
	Ogre::Vector3 vmax(FLT_MIN,FLT_MIN,FLT_MIN); 

	const Ogre::VertexElement* elem = pSubMesh->vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION); 
	Ogre::HardwareVertexBufferSharedPtr vbuf = pSubMesh->vertexData->vertexBufferBinding->getBuffer(elem->getSource()); 
	
	Ogre::Real * vertexData = new Ogre::Real[pSubMesh->vertexData->vertexCount * 3];
	Ogre::Real * pDest = vertexData;
	unsigned char * pData = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY)); 
	
	for ( size_t i = 0; i < pSubMesh->vertexData->vertexCount; ++ i ) 
	{ 
		float* pFloat; 
		elem->baseVertexPointerToElement(pData, &pFloat); 

		*pDest++ = *pFloat++; 
		*pDest++ = *pFloat++; 
		*pDest++ = *pFloat++;

		pData += vbuf->getVertexSize(); 
	} 

	vbuf->unlock();

	Ogre::HardwareIndexBufferSharedPtr ibuf = pSubMesh->indexData->indexBuffer;
	unsigned short* indexData = new unsigned short[pSubMesh->indexData->indexCount];  

	ibuf->readData(0, ibuf->getSizeInBytes(), indexData);

	//float tri[3][3];

	for ( size_t j = 0; j < pSubMesh->indexData->indexCount; ++ j )
	{
		const size_t index = indexData[ j ] * 3 ;
		const Ogre::Vector3 location ( vertexData + index ) ;
		
		vmin.x = std::min ( vmin.x , location.x ) ;
		vmin.y = std::min ( vmin.y , location.y ) ;
		vmin.z = std::min ( vmin.z , location.z ) ;

		vmax.x = std::max ( vmax.x , location.x ) ;
		vmax.y = std::max ( vmax.y , location.y ) ;
		vmax.z = std::max ( vmax.z , location.z ) ;
	}
    
	delete[] indexData;
	delete[] vertexData;   

	meshInfo.Max = vmax ;
	meshInfo.Min = vmin ;
	meshInfo.Size.x = abs( vmax.x - vmin.x );
	meshInfo.Size.y = abs( vmax.y - vmin.y );
	meshInfo.Size.z = abs( vmax.z - vmin.z ); 
	meshInfo.Centre = meshInfo.Size / 2 + meshInfo.Min ;

	position = meshInfo.Centre;
	radius = std::min( std::min( meshInfo.Size.x, meshInfo.Size.y ), meshInfo.Size.z );
}


void Document::replaceContainer( const std::string & fileName )
{
#ifndef NDEBUG
	_cprintf( "new container file: %s", fileName.c_str() );
#endif
	const std::string entityName = "defaultContainer";
	const std::string nodeName = "defaultContainerNode";

	// detach & destroy current container mesh
	Ogre::SceneManager * m = this->documentNode->getCreator();

	m->getSceneNode( nodeName )->detachObject( entityName );
	m->destroyEntity( entityName );

	// create new entity
	Ogre::SceneNode * n = m->getSceneNode( nodeName );
	Ogre::Entity * e = m->createEntity( entityName, data.containerData.mesh = fileName );	

#ifndef NDEBUG
	_cprintf( "new container name: %s\n", data.containerData.mesh.c_str() );
#endif
	// attach new container
	n->attachObject( e );
}

void Document::replaceSkybox( const std::string & materialName )
{
	// set skybox
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->setSkyBox( true, data.skyboxData.material = materialName, 1000 );
}

void Document::replaceEgg( const std::string & meshName )
{
	deleteAllEgg();

	//ExtractEgg( data.eggData.mesh = meshName );
	ExtractEgg( meshName );
}

void Document::deleteAllEgg()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();

	for( size_t i = 0; i < data.eggData.egg.size(); ++ i )
	{
		const Data::Egg & e = data.eggData.egg[ i ];

		{	
			Ogre::Entity * entity = m->getEntity( e.name );
			Ogre::SceneNode * eNode = entity->getParentSceneNode();

			eNode->detachObject( entity );
			m->destroyEntity( entity );
			m->destroySceneNode( eNode->getName() );
		}
	}

	m->destroySceneNode( "eggNode" );

}

// only extract radius, position
void Document::ExtractEgg( const std::string & mesh )
{
	Ogre::SceneManager * m = this->documentNode->getCreator();

	// create mesh
	Ogre::Entity * e = m->createEntity( "eggBunch", mesh );

	data.eggData.egg.clear();
	char buffer[ 80 ];

	// extract data
#ifndef NDEBUG
	cprintf( "Document::ExtractEgg()\n" );
	cprintf( "\t mesh: %s\n", mesh.c_str() );
	cprintf( "\t sub entity: %d\n", e->getNumSubEntities() );
#endif
	for( size_t i = 0; i < e->getNumSubEntities(); ++ i )
	{
		Data::Egg egg;
		Ogre::SubEntity * subEntity= e->getSubEntity( i );
		
		sprintf( buffer, "egg%d", i );
		egg.name = buffer;

		GetValue( subEntity->getSubMesh(), egg.position, egg.radius );

		egg.material = "defaultContainer";
		egg.value = "?";
		data.eggData.egg.push_back( egg );
	}

	// destroy mesh
	m->destroyEntity( "eggBunch" );

	SetEgg();
}

void Document::SetEgg()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	Ogre::SceneNode * eggNode = this->documentNode->createChildSceneNode( "eggNode" );
	Ogre::Entity * entity = m->createEntity( "standardEgg", data.eggData.mesh );

	Ogre::Vector3 position;
	float radius;
	GetValue( entity->getSubEntity( 0 )->getSubMesh(), position, radius );
	const float standardRadius = this->data.eggData.standardRadius = radius;

	std::vector< Data::Egg >::const_iterator it;

	for( it = data.eggData.egg.begin(); it != data.eggData.egg.end(); ++ it )
	{
		entity->setMaterialName( configData.materialMap[ it->value ] );

		Ogre::SceneNode * n = eggNode->createChildSceneNode( it->name + "Node" );
		n->attachObject( entity->clone( it->name ) );
		n->setPosition( it->position );
		
		// scales setting
		n->setScale( n->getScale() * it->radius / standardRadius * 0.7 );
	}

	m->destroyEntity( "standardEgg" );

	eggNode->setPosition( this->data.eggData.position );
	eggNode->scale( this->data.eggData.scale );
	eggNode->rotate( this->data.eggData.rotate );
}

void Document::mixEgg()
{
	// TODO: check GUI input. if these is not changed, you need not mix

	// assign numeric randomly
	std::vector< Data::Egg > & egg = data.eggData.egg;
	for( size_t i = 0; i < egg.size(); ++ i )
	{
		egg[ i ].value = GetRandomNumeric();
	}

	// assign operator randomly // in most case, operator quantity is smaller than numeric
	size_t opQty = static_cast< size_t >( egg.size() * ( 1.0f - data.valueData.numericRate ) );
	const size_t step = static_cast< size_t >( egg.size() / opQty );

#ifndef NDEBUG
	_cprintf( "operation quantity: %d, step: %d\n",opQty, step );
#endif

	//::srand( ::GetTickCount() );
	
	for( size_t i = 0; i < egg.size(); i += step )
	{
		const size_t index = i + rand() % step;

		if( index < egg.size() )
		{
             egg[ index ].value = GetRandomOperator();
		}

		if( ! opQty -- )
			break;
	}

	Ogre::SceneManager * m = this->documentNode->getCreator();

	// force to change egg billboard material
	for( size_t i = 0; i < egg.size(); ++ i )
	{
		//Ogre::BillboardSet * bbs = m->getBillboardSet( egg[ i ].name + "bbs" );
		//bbs->setMaterialName( this->configData.materialMap[ egg[ i ].value ] );
		Ogre::Entity * e = m->getEntity( egg[ i ].name );
		e->setMaterialName( this->configData.materialMap[ egg[ i ].value ] );

#ifndef NDEBUG
		_cprintf( "new assigend data: egg[ %d ]: %s\n", i, egg[ i ].value.c_str() );
#endif
	}
}

std::string Document::GetRandomNumeric()
{
#ifndef NDEBUG
	_cprintf( "Document::GetRandomNumeric()\n" );
#endif
	static DWORD tick = GetTickCount();

	if( tick < GetTickCount() )
	{
		srand( tick = GetTickCount() );
	}

	char buffer[ 80 ];
	const int & from = data.valueData.eggNumericRange.first;
	const int & to = data.valueData.eggNumericRange.second;

	// CAUTION: If you called srand() in little time, you will receive equal value
	const int index = rand() % abs( from - to - 1 );
	
	sprintf( buffer, "%d", from + index );
	return std::string( buffer );
}

std::string Document::GetRandomOperator()
{
#ifndef NDEBUG
	_cprintf( "Document::GetRandomNumeric()\n" );
#endif
	static DWORD tick = GetTickCount();

	if( tick < GetTickCount() )
	{
		srand( tick = GetTickCount() );
	}

	const std::list< std::string > & l = data.operatorData.usingOperator;
	const size_t index = ( int )( rand() ) % l.size();

	std::list< std::string >::const_iterator it = l.begin();
	std::advance( it, index );

	return *it;
}

void Document::moveCamera()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getCamera( "defaultCamera" )->setPosition( data.cameraData.position );

	_isModified = true;
}

// object
void Document::moveContainer()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getSceneNode( "defaultContainerNode" )->setPosition( data.containerData.position );

	_isModified = true;
}

void Document::rotateContainer()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getSceneNode( "defaultContainerNode" )->rotate( data.containerData.rotate );

	_isModified = true;
}

void Document::scaleContainer()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getSceneNode( "defaultContainerNode" )->setScale( data.containerData.scale );

	_isModified = true;
}

void Document::moveEgg()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getSceneNode( "eggNode" )->setPosition( data.eggData.position );

	_isModified = true;
}

void Document::rotateEgg()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getSceneNode( "eggNode" )->rotate( data.eggData.rotate );

	_isModified = true;
}

void Document::scaleEgg()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getSceneNode( "eggNode" )->setScale( data.eggData.scale );

	_isModified = true;
}

// light
void Document::setAmbientLight()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->setAmbientLight( data.ambientLight );

	_isModified = true;
}

void Document::setLightDiffuse( const std::string & name )
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getLight( name )->setDiffuseColour( data.lightMap[ name ].diffuse );

	_isModified = true;
}

void Document::setLightSpecular( const std::string & name )
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getLight( name )->setSpecularColour( data.lightMap[ name ].specular );

	_isModified = true;
}

void Document::setLightDirection( const std::string & name )
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getLight( name )->setDirection( data.lightMap[ name ].direction );

	_isModified = true;
}

void Document::setLightPosition( const std::string & name )
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getLight( name )->setPosition( data.lightMap[ name ].position );

	_isModified = true;
}

void Document::setLightPowerScale( const std::string & name )
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getLight( name )->setPowerScale( data.lightMap[ name ].power );

	_isModified = true;
}

void Document::setLightType( const std::string & name )
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->getLight( name )->setType( data.lightMap[ name ].type );

	_isModified = true;
}

void Document::setFog()
{
	Ogre::SceneManager * m = this->documentNode->getCreator();
	m->setFog( data.fogData.mode, data.fogData.fade, data.fogData.density, data.fogData.linear.first, data.fogData.linear.second );
}