#include "stdafx.h"
#include "Data.h"

Data::Data()
{
	clear();
}

Data::~Data()
{
}

void Data::clear()
{
	fileName = "";

	eggData.egg.clear();
	eggData.material.clear();

	lightMap.clear();
	// eggData.clear(); // not yet
}

void Data::saveAs( const std::string & name)
{
	fileName = name;
	std::ofstream file;

	try
	{
		if( 0 == fileName.size() )
		{
			throw Exception( Exception::RequiredFileName );
		}

		file.open( fileName.c_str() );

		file << "# config using Ogre self parser" << std::endl;
		file << "# default setting for builder" << std::endl;
		file << "# wrote by Ungju" << std::endl << std::endl;

  		// save containerData
		file << "[Container]" << std::endl;
		file << "mesh=" << this->containerData.mesh << std::endl;
		file << "material=" << this->containerData.material << std::endl;
		file << "position_x=" << this->containerData.position.x << std::endl;
		file << "position_y=" << this->containerData.position.y << std::endl;
		file << "position_z=" << this->containerData.position.z << std::endl;
		file << "rotate_axis_x=" << this->containerData.rotate.x << std::endl;
		file << "rotate_axis_y=" << this->containerData.rotate.y << std::endl;
		file << "rotate_axis_z=" << this->containerData.rotate.z << std::endl;
		file << "rotate_axis_angle=" << this->containerData.rotate.w << std::endl;
		file << "scale_x=" << this->containerData.scale.x << std::endl;
		file << "scale_y=" << this->containerData.scale.y << std::endl;
		file << "scale_z=" << this->containerData.scale.z << std::endl;
		file << std::endl;

		// save eggData
		{
			file << "[Egg]" << std::endl;
			file << "mesh=" << this->eggData.mesh << std::endl;
			file << "standardRadius=" << this->eggData.standardRadius << std::endl;
			file << "#pos, rotate, scale apply at eggNode" << std::endl;
			file << "position_x=" << this->eggData.position.x << std::endl;
			file << "position_y=" << this->eggData.position.y << std::endl;
			file << "position_z=" << this->eggData.position.z << std::endl;
			file << "rotate_axis_x=" << this->eggData.rotate.x << std::endl;
			file << "rotate_axis_y=" << this->eggData.rotate.y << std::endl;
			file << "rotate_axis_z=" << this->eggData.rotate.z << std::endl;
			file << "rotate_angle=" << this->eggData.rotate.w << std::endl;
			file << "scale_x=" << this->eggData.scale.x << std::endl;
			file << "scale_y=" << this->eggData.scale.y << std::endl;
			file << "scale_z=" << this->eggData.scale.z << std::endl;

			file << "# format: name=material, pos_x, pos_y, pos_z, radius, value" << std::endl;
			std::vector< Egg >::const_iterator it;

			for( it = eggData.egg.begin(); it != eggData.egg.end(); ++ it )
			{
				file << it->name << '=' << it->material << ", ";
				file << it->position.x << ", " << it->position.y << ", " << it->position.z << ", ";
				file << it->radius << ", " << it->value << std::endl;
			}

			file << std::endl;
		}
				
		// save cameraData
		file << "[Camera]" << std::endl;
		file << "position_x=" << this->cameraData.position.x << std::endl;
		file << "position_y=" << this->cameraData.position.y << std::endl;
		file << "position_z=" << this->cameraData.position.z << std::endl;
		file << "lookAt_x=" << this->cameraData.lookAt.x << std::endl;
		file << "lookAt_y=" << this->cameraData.lookAt.y << std::endl;
		file << "lookAt_z=" << this->cameraData.lookAt.z << std::endl;
		file << std::endl;

		// save lightMap
		{
			file << "[Light]" << std::endl;
			file << "# format: name = diffuse r, diffuse g, diffuse b, specular_r, specular_g, specular_b, pos x, pos y, pos y, dir x, dir y, dir z, power, type, spotlightRangeDegreeX, spotlightRangeDegreeY" << std::endl;
			file << "# light type: spotlight, point, directional" << std::endl;
			file << "# caution: spotlight range use only when you select spotlight type" << std::endl;
			std::map< std::string, LightData >::const_iterator it;

			for( it = lightMap.begin(); it != lightMap.end(); ++ it )
			{
				const LightData & l = it->second;

				file << it->first << '=';
				file << l.diffuse.r << ", " << l.diffuse.g << ", " << l.diffuse.b << ", ";
				file << l.specular.r << ", " << l.specular.g << ", " << l.specular.b << ", ";
				file << l.position.x << ", " << l.position.y << ", " << l.position.z << ", ";
				file << l.direction.x << ", " <<  l.direction.y << ", " << l.direction.z << ", ";
				file << l.power << ", ";

				switch( l.type )
				{
				case Ogre::Light::LT_SPOTLIGHT:
					file << "spotlight, " << l.spotlightRangeDegree.first << ", " << l.spotlightRangeDegree.second;
					break;
				case Ogre::Light::LT_DIRECTIONAL:
					file << "directional";
					break;
				case Ogre::Light::LT_POINT:
					file << "point";
					break;
				}

				file << std::endl;
			}

			file << std::endl;
		}
		
		// save ambientLight
		file << "[AmbientLight]" << std::endl;
		file << "r=" << this->ambientLight.r << std::endl;
		file << "g=" << this->ambientLight.g << std::endl;
		file << "b=" << this->ambientLight.b << std::endl;
		file << std::endl;

		// save skyboxData
		file << "[Skybox]" << std::endl;
		file << "material=" << this->skyboxData.material << std::endl;
		file << "range=" << this->skyboxData.range << std::endl;
		file << std::endl;

		// save valueData
		file << "[Value]" << std::endl;
		file << "EggNumericRangeFrom=" << this->valueData.eggNumericRange.first << std::endl;
		file << "EggNumericRangeTo=" << this->valueData.eggNumericRange.second << std::endl;
		file << "AnswerNumericRangeFrom=" << this->valueData.answerNumericRange.first << std::endl;
		file << "AnswerNumericRangeTo=" << this->valueData.answerNumericRange.second << std::endl;
		file << "# total * NumericRate = numeric quantity" << std::endl;
		file << "NumericRate=" << this->valueData.numericRate << std::endl;
		file << "AnswerQuantity=" << this->valueData.answerQuantity << std::endl;
		file << "LimitTime=" << this->valueData.limitTime << std::endl;
		file << std::endl;

		// save operatorData
		file << "[Operator]" << std::endl;		
		file << "sum=" << ( ( this->operatorData.sum ) ? "true" : "false" ) << std::endl;
		file << "subtract=" << ( ( this->operatorData.subtract ) ? "true" : "false" ) << std::endl;
		file << "multiply=" << ( ( this->operatorData.multiply ) ? "true" : "false" ) << std::endl;
		file << "mod=" << ( ( this->operatorData.mod ) ? "true" : "false" ) << std::endl;
		file << "factorial=" << ( ( this->operatorData.factorial ) ? "true" : "false" ) << std::endl;
		file << "bracket=" << ( ( this->operatorData.bracket ) ? "true" : "false" ) << std::endl;
		file << "square=" << ( ( this->operatorData.square ) ? "true" : "false" ) << std::endl;
		file << "invert=" << ( ( this->operatorData.invert ) ? "true" : "false" ) << std::endl;
		file << std::endl;

		// save fog
		std::string mode;
		switch( fogData.mode )
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

		file << "[Fog]" << std::endl;
		file << "mode=" << mode << std::endl;
		file << "density=" << fogData.density << std::endl;
		file << "linear_start=" << fogData.linear.first << std::endl;
		file << "linear_end=" << fogData.linear.second << std::endl;
		file << "fade_color_r=" << fogData.fade.r << std::endl;
		file << "fade_color_g=" << fogData.fade.g << std::endl;
		file << "fade_color_b=" << fogData.fade.b << std::endl;
	}
	catch( Exception & e )
	{
#ifndef NDEBUG
		_cprintf( "* * exception: %s\n", e.getMessage().c_str() );
#endif
	}

	file.close();
}

void Data::save()
{
	if( fileName.size() )
	{
		saveAs( fileName );
	}
}

void Data::load( const std::string &f )
{
	clear();

	Ogre::ConfigFile cf;
	cf.load( f );

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	while(seci.hasMoreElements())
	{
		std::string secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap * settings = seci.getNext();

#ifndef NDEBUG
		_cprintf( "file content: \n" );
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		std::cout << '[' << secName << ']' << std::endl;

		for(i=settings->begin(); i!=settings->end(); ++i)
		{
			std::cout << i->first << ' ' << i->second << std::endl;
		}

		_cprintf( "\n\n" );
#endif

		if( secName == "Container" )
		{
			setContainerData( settings );
		} 
		else if( secName == "Egg" )
		{
			setEggData( settings );
		} 
		else if( secName == "Camera" )
		{
			setCameraData( settings );
		}
		else if( secName == "Light" )
		{
			setLightData( settings );
		}
		else if( secName == "AmbientLight" )
		{
			setAmbientLightData( settings );
		}
		else if( secName == "Skybox" )
		{
			setSkyboxData( settings );
		}
		else if( secName == "Value" )
		{
			setValueData( settings );
		}
		else if( secName == "Operator" )
		{
			setOperatorData( settings );
		}
		else if( secName == "Fog" )
		{
			setFogData( settings );
		}
		else
		{
#ifndef NDEBUG
			// not defined section. cannot process
			_cprintf( "not defined section inputed\n" );
#endif
		}
	}

#ifndef NDEBUG
	_cprintf( "file processing complete\n" );
#endif
}

void Data::parseTextToNumeric( const std::string & text, std::vector<std::string> & vec)
{
	std::string token;
    
    for( size_t i = 0; i < text.length(); ++ i )
    {
         if( ' ' == text[ i ] )
             continue;
             
         if( ',' == text[ i ] )
         {
             vec.push_back( token );
             token.clear();
         }
         else
         {
             token.push_back( text[ i ] );
         }
    }
    
    if( token.size() )
    {
        vec.push_back( token );
    }
}

void Data::setContainerData(Ogre::ConfigFile::SettingsMultiMap * s)
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "mesh" )
		{
			containerData.mesh = i->second;
			
		}
		else if( i->first == "material" )
		{
			containerData.material = i->second;
		}
		else if( i->first == "position_x" )
		{
			containerData.position.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_y" )
		{
			containerData.position.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_z" )
		{
			containerData.position.z = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_x" )
		{
			containerData.rotate.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_y" )
		{
			containerData.rotate.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_z" )
		{
			containerData.rotate.z = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_angle" )
		{
			containerData.rotate.w = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_x" )
		{
			containerData.scale.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_y" )
		{
			containerData.scale.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_z" )
		{
			containerData.scale.z = ::atof( i->second.c_str() );
		}
	}
}

void Data::setEggData(Ogre::ConfigFile::SettingsMultiMap * s)
{
	/*
	# format: name=material, radius, pos_x, pos_y, pos_z
	*/
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "mesh" )
		{
			eggData.mesh = i->second;
		}
		else if( i->first == "standardRadius" )
		{
			eggData.standardRadius = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_x" )
		{
			eggData.position.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_y" )
		{
			eggData.position.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_z" )
		{
			eggData.position.z = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_x" )
		{
			eggData.rotate.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_y" )
		{
			eggData.rotate.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_z" )
		{
			eggData.rotate.z = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_angle" )
		{
			eggData.rotate.w = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_x" )
		{
			eggData.scale.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_y" )
		{
			eggData.scale.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_z" )
		{
			eggData.scale.z = ::atof( i->second.c_str() );
		}
		else
		{
			Egg e;
			std::vector< std::string > v;

			this->parseTextToNumeric( i->second, v );

			e.name = i->first;
			e.material = v[ 0 ];
			e.position = Ogre::Vector3( ::atof( v[ 1 ].c_str() ), ::atof( v[ 2 ].c_str() ), ::atof( v[ 3 ].c_str() ) );
			e.radius = ::atof( v[ 4 ].c_str() );
			e.value = v[ 5 ];
			
			eggData.egg.push_back( e );
			eggData.material.insert( e.material );
		}
	}
}

void Data::setCameraData(Ogre::ConfigFile::SettingsMultiMap *s)
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "position_x" )
		{
			cameraData.position.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_y" )
		{
			cameraData.position.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_z" )
		{
			cameraData.position.z = ::atof( i->second.c_str() );
		}
		else if( i->first == "lookAt_x" )
		{
			cameraData.lookAt.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "lookAt_y" )
		{
			cameraData.lookAt.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "lookAt_z" )
		{
			cameraData.lookAt.z = ::atof( i->second.c_str() );
		}
	}
}

void Data::setLightData(Ogre::ConfigFile::SettingsMultiMap * s)
{
/*
# format: name = diffuse r, diffuse g, diffuse b, specular_r, specular_g, specular_b, pos x, pos y, pos y, dir x, dir y, dir z
*/
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		std::vector< std::string > v;
		this->parseTextToNumeric( i->second, v );

		LightData l;
		l.diffuse = Ogre::ColourValue( ::atof( v[ 0 ].c_str() ), ::atof( v[ 1 ].c_str() ), ::atof( v[ 2 ].c_str() )  );
		l.specular = Ogre::ColourValue( ::atof( v[ 3 ].c_str() ), ::atof( v[ 4 ].c_str() ), ::atof( v[ 5 ].c_str() ) );
		l.position = Ogre::Vector3( ::atof( v[ 6 ].c_str() ), ::atof( v[ 7 ].c_str() ), ::atof( v[ 8 ].c_str() ) );
		l.direction = Ogre::Vector3( ::atof( v[ 9 ].c_str() ), ::atof( v[ 10 ].c_str() ), ::atof( v[ 11 ].c_str() ) );
		l.power = ::atof( v[ 12 ].c_str() );
		
		if( "spotlight" == v[ 13 ] )
		{
			l.type = Ogre::Light::LT_SPOTLIGHT;
			l.spotlightRangeDegree.first = atof( v[ 14 ].c_str() );
			l.spotlightRangeDegree.second = atof( v[ 15 ].c_str() );
		}
		else if( "point" == v[ 13 ] )
		{
			l.type = Ogre::Light::LT_POINT;
			l.spotlightRangeDegree.first = 0;
			l.spotlightRangeDegree.second = 0;
		}
		else if( "directional" == v[ 13 ] )
		{
			l.type = Ogre::Light::LT_DIRECTIONAL;
			l.spotlightRangeDegree.first = 0;
			l.spotlightRangeDegree.second = 0;
		}

		lightMap[ i->first ] = l;
	}
}

void Data::setAmbientLightData(Ogre::ConfigFile::SettingsMultiMap * s)
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "r" )
		{
			ambientLight.r = ::atof( i->second.c_str() );
		}
		else if( i->first == "g" )
		{
			ambientLight.g = ::atof( i->second.c_str() );
		}
		else if( i->first == "b" )
		{
			ambientLight.b = ::atof( i->second.c_str() );
		}
	}
}

void Data::setSkyboxData(Ogre::ConfigFile::SettingsMultiMap * s)
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "material" )
		{
			skyboxData.material = i->second;
		}
		else if( i->first == "range" )
		{
			skyboxData.range = ::atoi( i->second.c_str() );
		}
	}
}

void Data::setValueData(Ogre::ConfigFile::SettingsMultiMap * s)
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "EggNumericRangeFrom" )
		{
			valueData.eggNumericRange.first = ::atoi( i->second.c_str() );
		}
		else if( i->first == "EggNumericRangeTo" )
		{
			valueData.eggNumericRange.second = ::atoi( i->second.c_str() );
		}
		else if( i->first == "AnswerNumericRangeFrom" )
		{
			valueData.answerNumericRange.first = ::atoi( i->second.c_str() );
		}
		else if( i->first == "AnswerNumericRangeTo" )
		{
			valueData.answerNumericRange.second = ::atoi( i->second.c_str() );
		}
		else if( i->first == "NumericRate" )
		{
			valueData.numericRate = ::atof( i->second.c_str() );
		}
		else if( i->first == "LimitTime" )
		{
			valueData.limitTime = ::atoi( i->second.c_str() );
		}
		else if( i->first == "AnswerQuantity" )
		{
			valueData.answerQuantity = ::atoi( i->second.c_str() );
		}
	}
}

void Data::setOperatorData(Ogre::ConfigFile::SettingsMultiMap * s)
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "sum" )
		{
			if( operatorData.sum = !( abs( i->second.compare( "true" ) ) ) )
			{
				operatorData.usingOperator.push_back( "+" );
			}
		}
		else if( i->first == "subtract" )
		{
			if( operatorData.subtract = !( abs( i->second.compare( "true" ) ) ) )
			{
				operatorData.usingOperator.push_back( "-" );
			}
		}
		else if( i->first == "multiply" )
		{
			if( operatorData.multiply = !( abs( i->second.compare( "true" ) ) ) )
			{
				operatorData.usingOperator.push_back( "*" );
			}
		}
		else if( i->first == "mod" )
		{
			if( operatorData.mod = !( abs( i->second.compare( "true" ) ) ) )
			{
				operatorData.usingOperator.push_back( "%" );
			}
		}
		else if( i->first == "factorial" )
		{
			if( operatorData.factorial = !( abs( i->second.compare( "true" ) ) ) )
			{
				operatorData.usingOperator.push_back( "!" );
			}
		}
		else if( i->first == "bracket" )
		{
			if( operatorData.bracket = !( abs( i->second.compare( "true" ) ) ) )
			{
				operatorData.usingOperator.push_back( "(" );
				operatorData.usingOperator.push_back( ")" );
			}
		}
		else if( i->first == "square" )
		{
			if( operatorData.square = !( abs( i->second.compare( "true" ) ) ) )
			{
				operatorData.usingOperator.push_back( "^" );
			}
		}
		else if( i->first == "invert" )
		{
			if( operatorData.invert = !( abs( i->second.compare( "true" ) ) ) )
			{
				operatorData.usingOperator.push_back( "~" );
			}
		}
	}
}

void Data::setFogData( Ogre::ConfigFile::SettingsMultiMap * s )
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "mode" )
		{
			if( i->second == "FOG_LINEAR" )
			{
				fogData.mode = Ogre::FOG_LINEAR;
			}
			else if( i->second == "FOG_NONE" )
			{
				fogData.mode = Ogre::FOG_NONE;
			}
			else if( i->second == "FOG_EXP" )
			{
				fogData.mode = Ogre::FOG_EXP;
			}
			else if( i->second == "FOG_EXP2" )
			{
				fogData.mode = Ogre::FOG_EXP2;
			}
		}
		else if( i->first == "linear_start" )
		{
			fogData.linear.first = ::atof( i->second.c_str() );
		}
		else if( i->first == "linear_end" )
		{
			fogData.linear.second = ::atof( i->second.c_str() );
		}
		else if( i->first == "density" )
		{
			fogData.density = ::atof( i->second.c_str() );
		}
		else if( i->first == "fade_color_r" )
		{
			fogData.fade.r = ::atof( i->second.c_str() );
		}
		else if( i->first == "fade_color_g" )
		{
			fogData.fade.g = ::atof( i->second.c_str() );
		}
		else if( i->first == "fade_color_b" )
		{
			fogData.fade.b = ::atof( i->second.c_str() );
		}
	}
}