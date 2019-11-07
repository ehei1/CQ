#include "StdAfx.h"
#include "ConfigData.h"

ConfigData::ConfigData(void)
{
}

ConfigData::~ConfigData(void)
{
}

void ConfigData::load( const std::string & file )
{
	Ogre::ConfigFile cf;
	cf.load( file );

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	while( seci.hasMoreElements() )
	{
		const std::string & secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap * settings = seci.getNext();

		if( "Material" == secName )
		{
			setMaterialMap( settings );
		}
	}
}

void ConfigData::setMaterialMap( Ogre::ConfigFile::SettingsMultiMap * s )
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		materialMap[ i->first ] = i->second;
	}
}