#pragma once

class ConfigData
{
public:
	ConfigData(void);
	~ConfigData(void);
	void load( const std::string & );

	// property
	std::map< std::string, std::string > materialMap; // match texture on egg with value

private:
	void setMaterialMap( Ogre::ConfigFile::SettingsMultiMap * );
};
