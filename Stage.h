/*
-----------------------------------------------------------------------------
CQ
Copyright 2006 Lee Ungju, Lee Junho

Also see acknowledgements in credit.txt

This program is free software; you can redistribute it and/or modify it under
uncommerical purpose.
-----------------------------------------------------------------------------
*/
#pragma once

class Stage
{
public:
	void clear();
	void load( const std::string & );

public:
	struct Container
	{
		std::string mesh;
		std::string material;
		Ogre::Vector3 position;
		Ogre::Quaternion rotate;
		Ogre::Vector3 scale;
	} container;

	struct EggElement
	{
		std::string name;
		std::string material;
		Ogre::Vector3 position;
		float radius;
		std::string value; // on ball
	};
	
	struct Egg
	{
		Ogre::Vector3 position;
		Ogre::Quaternion rotate;
		Ogre::Vector3 scale;
		float standardRadius;

		std::string mesh;
		std::vector< EggElement > eggVec;
		std::set< std::string > material;
	} egg;

	struct Camera
	{
		Ogre::Vector3 position;
		Ogre::Vector3 lookAt;
	} camera;

	struct Light
	{
		Ogre::Vector3 position;
		Ogre::Vector3 direction;
		Ogre::ColourValue diffuse;
		Ogre::ColourValue specular;

		float power;
		Ogre::Light::LightTypes type;
		std::pair< float, float > spotlightRangeDegree;
	};

	typedef std::string LightName;
	std::map< LightName, Light > lightMap;
	Ogre::ColourValue ambientLight;
	
	struct Skybox
	{
		std::string material;
		size_t range;
	} skybox;

	struct Value
	{
		typedef int from, to;

		std::pair< from, to > eggNumericRange;
		std::pair< from, to > answerNumericRange;
		float numericRate; // total * NumericRate = numeric quantity
		int limitTime;
		int answerQuantity; // player need correct answer to clear 
		float regenerationTime;
	} value;

	struct Fog
	{
		Ogre::FogMode mode;
		Ogre::ColourValue fade;
		std::pair< float, float > linear; // start, end
		float density;
	} fog;

	struct Operator
	{
		bool sum;
		bool subtract;
		bool multiply;
		bool mod;
		bool factorial;
		bool bracket;
		bool square;
		bool invert;
		std::list< std::string > usingOperator;
	} operatorData;

	struct Factor
	{
		int quantity;
		std::vector< std::string > factor;
	} factorData, minusFactorData;

	std::string music;

private:
	void setContainer( Ogre::ConfigFile::SettingsMultiMap * );
	void setEgg( Ogre::ConfigFile::SettingsMultiMap * );
	void setCamera( Ogre::ConfigFile::SettingsMultiMap * );
	void setLight( Ogre::ConfigFile::SettingsMultiMap * );
	void setAmbientLight( Ogre::ConfigFile::SettingsMultiMap * );
	void setSkybox( Ogre::ConfigFile::SettingsMultiMap * );
	void setValue( Ogre::ConfigFile::SettingsMultiMap * );
	void setOperator( Ogre::ConfigFile::SettingsMultiMap * );
	void setFog( Ogre::ConfigFile::SettingsMultiMap * );
	void setFactor( Ogre::ConfigFile::SettingsMultiMap * );
	void setMinusFactor( Ogre::ConfigFile::SettingsMultiMap * );
	void setMusic( Ogre::ConfigFile::SettingsMultiMap * );

	void parseTextToNumeric( const std::string & text, std::vector< std::string > & result );
	void mixEgg();
	std::string GetRandomOperator();
	std::string GetRandomNumeric();

	class Exception
	{
	public:
		enum Type 
		{
			InvaildFormat, RequiredFileName
		} type;

		Exception( const Type & t )
		: type( t )
		{}

		std::string getMessage() const
		{
			std::string message;

			switch( type )
			{
			case InvaildFormat:
				message = "file Stage is invaild";
				break;

			case RequiredFileName:
				message = "file name is not defined";
				break;
				
			default:
				message = "not defined exception";
				break;
			}

			return message;
		}
	};
};