/*
-----------------------------------------------------------------------------
CQ
Copyright 2006 Lee Ungju, Lee Junho

Also see acknowledgements in credit.txt

This program is free software; you can redistribute it and/or modify it under
uncommerical purpose.
-----------------------------------------------------------------------------
*/
#include "stdafx.h"
#include "Stage.h"

void Stage::clear()
{
	egg.eggVec.clear();
	egg.material.clear();

	lightMap.clear();
	factorData.factor.clear();
	minusFactorData.factor.clear();
	operatorData.usingOperator.clear();
}

void Stage::load( const std::string &f )
{
	clear();

	Ogre::ConfigFile cf;
	//const std::string path = ".\\stage\\";
	//cf.load( path + f );

	cf.load( f );

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	while(seci.hasMoreElements())
	{
		std::string secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap * settings = seci.getNext();
		/*
#ifdef _DEBUG
		_cprintf( "file content: \n" );
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		
		for(i=settings->begin(); i!=settings->end(); ++i)
		{
			//std::cout << i->first << ' ' << i->second << std::endl;
			_cprintf( "%s: %s\n", i->first.c_str(), i->second.c_str() );
		}

		_cprintf( "\n\n" );
#endif
		*/
		if( secName == "Container" )
		{
			setContainer( settings );
		} 
		else if( secName == "Egg" )
		{
			setEgg( settings );
		} 
		else if( secName == "Camera" )
		{
			setCamera( settings );
		}
		else if( secName == "Light" )
		{
			setLight( settings );
		}
		else if( secName == "AmbientLight" )
		{
			setAmbientLight( settings );
		}
		else if( secName == "Skybox" )
		{
			setSkybox( settings );
		}
		else if( secName == "Value" )
		{
			setValue( settings );
		}
		else if( secName == "Operator" )
		{
			setOperator( settings );
		}
		else if( secName == "Fog" )
		{
			setFog( settings );
		}
		else if( secName == "Factor" )
		{
			setFactor( settings );
		}
		else if( secName == "MinusFactor" )
		{
			setMinusFactor( settings );
		}
		else if( secName == "Music" )
		{
			setMusic( settings );
		}
		else
		{
#ifndef NDEBUG
			// not defined section. cannot process
			_cprintf( "not defined section inputed\n" );
#endif
		}
	}

	mixEgg();

#ifndef NDEBUG
	_cprintf( "file processing complete\n" );
#endif
}

void Stage::parseTextToNumeric( const std::string & text, std::vector<std::string> & vec)
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

void Stage::setContainer(Ogre::ConfigFile::SettingsMultiMap * s)
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "mesh" )
		{
			container.mesh = i->second;			
		}
		else if( i->first == "material" )
		{
			container.material = i->second;
		}
		else if( i->first == "position_x" )
		{
			container.position.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_y" )
		{
			container.position.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_z" )
		{
			container.position.z = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_x" )
		{
			container.rotate.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_y" )
		{
			container.rotate.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_z" )
		{
			container.rotate.z = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_angle" )
		{
			container.rotate.w = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_x" )
		{
			container.scale.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_y" )
		{
			container.scale.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_z" )
		{
			container.scale.z = ::atof( i->second.c_str() );
		}
	}
}

void Stage::setEgg(Ogre::ConfigFile::SettingsMultiMap * s)
{
	/*
	# format: name=material, radius, pos_x, pos_y, pos_z
	*/
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "mesh" )
		{
			egg.mesh = i->second;
		}
		else if( i->first == "standardRadius" )
		{
			egg.standardRadius = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_x" )
		{
			egg.position.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_y" )
		{
			egg.position.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_z" )
		{
			egg.position.z = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_x" )
		{
			egg.rotate.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_y" )
		{
			egg.rotate.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_axis_z" )
		{
			egg.rotate.z = ::atof( i->second.c_str() );
		}
		else if( i->first == "rotate_angle" )
		{
			egg.rotate.w = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_x" )
		{
			egg.scale.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_y" )
		{
			egg.scale.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "scale_z" )
		{
			egg.scale.z = ::atof( i->second.c_str() );
		}
		else
		{
			EggElement e;
			std::vector< std::string > v;

			this->parseTextToNumeric( i->second, v );

			e.name = i->first;
			e.material = v[ 0 ];
			e.position = Ogre::Vector3( ::atof( v[ 1 ].c_str() ), ::atof( v[ 2 ].c_str() ), ::atof( v[ 3 ].c_str() ) );
			e.radius = ::atof( v[ 4 ].c_str() );
			//e.value = v[ 5 ];
			
			egg.eggVec.push_back( e );
			egg.material.insert( e.material );
		}
	}
}

void Stage::setCamera(Ogre::ConfigFile::SettingsMultiMap *s)
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "position_x" )
		{
			camera.position.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_y" )
		{
			camera.position.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "position_z" )
		{
			camera.position.z = ::atof( i->second.c_str() );
		}
		else if( i->first == "lookAt_x" )
		{
			camera.lookAt.x = ::atof( i->second.c_str() );
		}
		else if( i->first == "lookAt_y" )
		{
			camera.lookAt.y = ::atof( i->second.c_str() );
		}
		else if( i->first == "lookAt_z" )
		{
			camera.lookAt.z = ::atof( i->second.c_str() );
		}
	}
}

void Stage::setLight(Ogre::ConfigFile::SettingsMultiMap * s)
{
/*
# format: name = diffuse r, diffuse g, diffuse b, specular_r, specular_g, specular_b, pos x, pos y, pos y, dir x, dir y, dir z
*/
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		std::vector< std::string > v;
		this->parseTextToNumeric( i->second, v );

		Light l;
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

void Stage::setAmbientLight(Ogre::ConfigFile::SettingsMultiMap * s)
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
#ifdef _DEBUG
	cprintf( "Stage::setAmbientLight()\n" );
	cprintf( "\t ambient: %f %f %f\n", ambientLight.r, ambientLight.g, ambientLight.b );
#endif
}

void Stage::setSkybox(Ogre::ConfigFile::SettingsMultiMap * s)
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "material" )
		{
			skybox.material = i->second;
		}
		else if( i->first == "range" )
		{
			skybox.range = ::atoi( i->second.c_str() );
		}
	}
}

void Stage::setValue(Ogre::ConfigFile::SettingsMultiMap * s)
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "EggNumericRangeFrom" )
		{
			value.eggNumericRange.first = ::atoi( i->second.c_str() );
		}
		else if( i->first == "EggNumericRangeTo" )
		{
			value.eggNumericRange.second = ::atoi( i->second.c_str() );
		}
		else if( i->first == "AnswerNumericRangeFrom" )
		{
			value.answerNumericRange.first = ::atoi( i->second.c_str() );
		}
		else if( i->first == "AnswerNumericRangeTo" )
		{
			value.answerNumericRange.second = ::atoi( i->second.c_str() );
		}
		else if( i->first == "NumericRate" )
		{
			value.numericRate = ::atof( i->second.c_str() );
		}
		else if( i->first == "LimitTime" )
		{
			value.limitTime = ::atoi( i->second.c_str() );
		}
		else if( i->first == "AnswerQuantity" )
		{
			value.answerQuantity = ::atoi( i->second.c_str() );
		}
		else if( i->first == "regenerationTime" )
		{
			value.regenerationTime = ::atoi( i->second.c_str() );
		}
	}
}

void Stage::setOperator(Ogre::ConfigFile::SettingsMultiMap * s)
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

void Stage::setFog( Ogre::ConfigFile::SettingsMultiMap * s )
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "mode" )
		{
			if( i->second == "FOG_LINEAR" )
			{
				fog.mode = Ogre::FOG_LINEAR;
			}
			else if( i->second == "FOG_NONE" )
			{
				fog.mode = Ogre::FOG_NONE;
			}
			else if( i->second == "FOG_EXP" )
			{
				fog.mode = Ogre::FOG_EXP;
			}
			else if( i->second == "FOG_EXP2" )
			{
				fog.mode = Ogre::FOG_EXP2;
			}
		}
		else if( i->first == "linear_start" )
		{
			fog.linear.first = ::atof( i->second.c_str() );
		}
		else if( i->first == "linear_end" )
		{
			fog.linear.second = ::atof( i->second.c_str() );
		}
		else if( i->first == "density" )
		{
			fog.density = ::atof( i->second.c_str() );
		}
		else if( i->first == "fade_color_r" )
		{
			fog.fade.r = ::atof( i->second.c_str() );
		}
		else if( i->first == "fade_color_g" )
		{
			fog.fade.g = ::atof( i->second.c_str() );
		}
		else if( i->first == "fade_color_b" )
		{
			fog.fade.b = ::atof( i->second.c_str() );
		}
	}
}

void Stage::setFactor( Ogre::ConfigFile::SettingsMultiMap * s )
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "quantity" )
		{
			factorData.quantity = ::atoi( i->second.c_str() );
		}
		else if( i->first == "factor" )
		{
			factorData.factor.push_back( i->second );
		}
	}
}

void Stage::setMinusFactor( Ogre::ConfigFile::SettingsMultiMap * s )
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "quantity" )
		{
			minusFactorData.quantity = ::atoi( i->second.c_str() );
		}
		else if( i->first == "factor" )
		{
			minusFactorData.factor.push_back( i->second );
		}
	}
}

void Stage::setMusic( Ogre::ConfigFile::SettingsMultiMap * s )
{
	Ogre::ConfigFile::SettingsMultiMap::iterator i;

	for( i = s->begin(); i != s->end(); ++i )
	{
		if( i->first == "file" )
		{
			music = i->second.c_str();
		}
	}
}

/*
function:
	1. all egg set with number
	2. some egg set with operator as numericRate
*/
void Stage::mixEgg()
{
	// assign numeric randomly
	std::vector< EggElement > & e = egg.eggVec;
	for( size_t i = 0; i < e.size(); ++ i )
	{
		e[ i ].value = GetRandomNumeric();
	}

	// assign operator randomly // in most case, operator quantity is smaller than numeric
	size_t opQty = static_cast< size_t >( e.size() * ( 1.0f - value.numericRate ) );
	const size_t step = static_cast< size_t >( e.size() / opQty );
	
	for( size_t i = 0; i < e.size(); i += step )
	{
		const size_t index = i + rand() % step;

		if( index < e.size() )
		{
             e[ index ].value = GetRandomOperator();
		}

		if( ! opQty -- )
			break;
	}
}


std::string Stage::GetRandomNumeric()
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
	const int & from = value.eggNumericRange.first;
	const int & to = value.eggNumericRange.second;

	// CAUTION: If you called srand() in little time, you will receive equal value
	const int index = rand() % abs( from - to - 1 );
	
	sprintf( buffer, "%d", from + index );
	return std::string( buffer );
}

std::string Stage::GetRandomOperator()
{
#ifndef NDEBUG
	_cprintf( "Document::GetRandomNumeric()\n" );
#endif
	static DWORD tick = GetTickCount();

	if( tick < GetTickCount() )
	{
		srand( tick = GetTickCount() );
	}

	const std::list< std::string > & l = operatorData.usingOperator;
	const size_t index = ( int )( rand() ) % l.size();

	std::list< std::string >::const_iterator it = l.begin();
	std::advance( it, index );

	return *it;
}
