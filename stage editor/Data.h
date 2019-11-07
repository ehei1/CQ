#pragma once
#pragma message("Compiling Data.h - this should happen just once per project.\n")


class Data
{
public:
	Data( void );
	~Data( void );

	void clear();
	void load( const std::string & );
	void saveAs( const std::string & );
	void save();

public:
	std::string fileName;

	struct ContainerData
	{
		std::string mesh;
		std::string material;
		Ogre::Vector3 position;
		Ogre::Quaternion rotate;
		Ogre::Vector3 scale;
	} containerData;

	struct Egg
	{
		std::string name;
		std::string material;
		Ogre::Vector3 position;
		float radius;
		std::string value; // on ball
	};
	
	struct EggData
	{
		float standardRadius;
		Ogre::Vector3 position;
		Ogre::Quaternion rotate;
		Ogre::Vector3 scale;

		std::string mesh;
		std::vector< Egg > egg;
		std::set< std::string > material;
	} eggData;

	struct CameraData
	{
		Ogre::Vector3 position;
		Ogre::Vector3 lookAt;
	} cameraData;

	struct LightData
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
	std::map< LightName, LightData > lightMap;
	Ogre::ColourValue ambientLight;
	
	struct SkyboxData
	{
		std::string material;
		size_t range;
	} skyboxData;

	struct ValueData
	{
		typedef int from, to;

		std::pair< from, to > eggNumericRange;
		std::pair< from, to > answerNumericRange;
		float numericRate; // total * NumericRate = numeric quantity
		int limitTime;
		int answerQuantity; // player need correct answer to clear stage
	} valueData;

	struct FogData
	{
		Ogre::FogMode mode;
		Ogre::ColourValue fade;
		std::pair< float, float > linear; // start, end
		float density;
	} fogData;

	struct OperatorData
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

private:
	void setContainerData( Ogre::ConfigFile::SettingsMultiMap * );
	void setEggData( Ogre::ConfigFile::SettingsMultiMap * );
	void setCameraData( Ogre::ConfigFile::SettingsMultiMap * );
	void setLightData( Ogre::ConfigFile::SettingsMultiMap * );
	void setAmbientLightData( Ogre::ConfigFile::SettingsMultiMap * );
	void setSkyboxData( Ogre::ConfigFile::SettingsMultiMap * );
	void setValueData( Ogre::ConfigFile::SettingsMultiMap * );
	void setOperatorData( Ogre::ConfigFile::SettingsMultiMap * );
	void setFogData( Ogre::ConfigFile::SettingsMultiMap * );

	void parseTextToNumeric( const std::string & text, std::vector< std::string > & result );

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
				message = "file data is invaild";
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