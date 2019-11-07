/*
-----------------------------------------------------------------------------
CQ
Copyright 2006 Lee Ungju, Lee Junho

Also see acknowledgements in credit.txt

This program is free software; you can redistribute it and/or modify it under
uncommerical purpose.
-----------------------------------------------------------------------------
*/
#include "StdAfx.h"
#include "Interface.h"
#include "SinglePlayState.h"
#include "PlayState.h"

Interface Interface::mInterface;

Interface::Interface()
{
	mCycleColor.push_back( ColourValue( 1, 0, 0 ) );
	mCycleColor.push_back( ColourValue( 1, 0.5, 0 ) );
	mCycleColor.push_back( ColourValue( 1, 1, 0 ) );
	mCycleColor.push_back( ColourValue( 0, 1, 0 ) );
	mCycleColor.push_back( ColourValue( 0, 0, 1 ) );
	mCycleColor.push_back( ColourValue( 1, 0, 1 ) );
}

void Interface::enter()
{
	mMinusFactor = mPlusFactor = false;
	mIsAnimate = false;

	mOverlay = OverlayManager::getSingleton().getByName( "Game/Interface" );	
	mOverlay->getChild( "Result/TimeBar" )->getChild( "Result/TimeBarHide" )->setWidth( 0 );

	OverlayManager::getSingleton().getOverlayElement( "Game/MessagePanelText" )->hide();

	OverlayContainer * operandContainer = static_cast< OverlayContainer * >(
		mOverlay->getChild( "Game/OperandPanel" )->getChild( "Game/InputedOperandPanel" )
	);

	OverlayContainer * resultContainer = static_cast< OverlayContainer * >(
		mOverlay->getChild( "Game/OperandPanel" )->getChild( "Game/ResultPanel" )
	);

	OverlayContainer * remainedAnswerContainer = mOverlay->getChild( "Game/RemainedAnswerPanel");
	OverlayContainer * clearededAnswerContainer = mOverlay->getChild( "Game/ClearedAnswerPanel");
	
	removeAllElement( operandContainer );
	removeAllElement( resultContainer );
	removeAllElement( remainedAnswerContainer );
	removeAllElement( clearededAnswerContainer );

	//Root::getSingleton().startRendering();
}

void Interface::exit()
{
	mOverlay->hide();
	mFadedMessage.clear();
}

void Interface::pause()
{
	show( false );
}

void Interface::resume()
{
	show( true );
}

void Interface::show( const bool & isShow )
{
	if( mOverlay )
	{
		if( isShow )
		{
			mOverlay->show();
		}
		else
		{
			mOverlay->hide();
		}
	}
#ifdef _DEBUG
	else
	{
		cprintf( "Interface::show()\n" );
		cprintf( "\t warning: overlay not defined. but you used this function.\n" );
	}
#endif
}

//void Interface::setMaxMilliSecond( const double & s )
void Interface::setMaxSecond( const float & s )
{
	mMaxSecond = s;
}

//void Interface::setMilliSecond( const double & s )
void Interface::setSecond( const float & s )
{
	char buffer[ 80 ];
	sprintf( buffer, "%0.1f", s );

	OverlayElement * time = OverlayManager::getSingleton().getOverlayElement( "Game/RemainedTimeText" );
	time->setCaption( buffer );

	OverlayElement * timeBar = mOverlay->getChild( "Result/TimeBar" )->getChild( "Result/TimeBarHide" );
	timeBar->setWidth( ( mMaxSecond - s ) / mMaxSecond );
}

void Interface::setInputedOperand( const std::string & s )
{
#ifdef _DEBUG
	cprintf( "Interface::setInputedOperand()\n" );
#endif

	OverlayContainer * container = static_cast< OverlayContainer * >(
		mOverlay->getChild( "Game/OperandPanel" )->getChild( "Game/InputedOperandPanel" )
	);

	removeAllElement( container );
	
	char buffer[ 80 ];
	float letter_unit;
	float letter_top, tex_top;
	float tex_height, tex_width;
	float left = 0.007;
	std::string char_height;
	size_t i = 0;
#ifdef _DEBUG
	cprintf( "\t operation size: %d\n", getOperationSize( s ) );
#endif
	if( 11 < getOperationSize( s ) )
	{
		letter_top = -0.01;
		tex_top = 0.01;
		char_height = "0.1";
		letter_unit = 0.03;
		tex_height = 0.06;
		tex_width = 0.05;
	}
	else
	{
		letter_top = 0;
		tex_top = 0.02;
		char_height = "0.15";
		letter_unit = 0.03;
		tex_height = 0.12;
		tex_width = 0.10;
	}
	
	// put text
	std::string str = s;
	bool lineFeed = false;

#ifdef _DEBUG
	cprintf( "\t statement: %s\n", s.c_str() );
#endif
	//size_t step = 0;

	while( 1 < str.size() )
	{
		const size_t index = str.find( "|" );
		const std::string input = str.substr( 0, index );
		bool isLetter;

		OverlayElement * element;
		ColourValue color;
		
		sprintf( buffer, "InputedUnit%d", i );
		if( '=' == input[ 0 ] ) // answer
		{
			element = OverlayManager::getSingleton().createOverlayElementFromTemplate( "OperandTemplate", "TextArea", buffer );

			sprintf( buffer, "%s", input.substr( 1, input.size() ).c_str() );
			element->setColour( ColourValue::White );
			element->setParameter( "char_height", char_height );
			element->setCaption( buffer );
			
			element->setLeft( left );
			element->setTop( letter_top );			

			left += letter_unit * static_cast< float >( strlen( buffer ) );
			isLetter = true;
		}
		else
		{
			const std::string material = SinglePlayState::getInstance()->mMaterialMap[ input.substr( 1, input.size() ) ] + "Panel";

			element = OverlayManager::getSingleton().createOverlayElement( "Panel", buffer	);
			element->setMaterialName( material );
			element->setWidth( tex_width );
			element->setHeight( tex_height );
			
			element->setLeft( left );
			element->setTop( tex_top );

			left += tex_width;
			isLetter = false;
		}

		if( ! lineFeed && left > 1 )
		{
			letter_top = 0.05;
			tex_top = 0.072;
			//top = 0.05;
			lineFeed = true;

			element->setLeft( left = 0.007 );

			if( isLetter )
			{
				element->setTop( letter_top );
				//left += letter_unit * static_cast< float >( strlen( buffer ) );
			}
			else
			{
				element->setTop( tex_top );
				left += tex_width;
			}
		}

		container->addChild( element );

		++ i;
		str = str.substr( index + 1, str.size() );		
	}
}

void Interface::removeAllElement( OverlayContainer * container )
{
	OverlayContainer::ChildIterator it = container->getChildIterator();

	// remove all text element
	while( it.hasMoreElements() )
	{
		OverlayElement * element = it.getNext();
		container->removeChild( element->getName() );
		OverlayManager::getSingleton().destroyOverlayElement( element );
	}
}

bool Interface::frameEnded( const FrameEvent & evt )
{
	if( ! mFadedMessage.empty() )
	{
		OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "Game/MessagePanelText" );
		FadedMessage & message = mFadedMessage.back();
		
		if( 0 > message.second )
		{
			element->hide();
			element->setTop( 0 );

			mFadedMessage.pop_back();
		}
		else
		{
			message.second -= evt.timeSinceLastFrame;
			
			float top;
			top = element->getTop();
			element->setTop( top - 0.001 );
			
			element->setCaption( message.first );
			element->show();
		}
	}

	{
		OverlayElement * tagElement = 0;
		OverlayElement * factorElement = 0;

		if( mPlusFactor )
		{
			tagElement = OverlayManager::getSingleton().getOverlayElement( "FactorPanelTitle/PlusFactorImage" );
			factorElement = OverlayManager::getSingleton().getOverlayElement( "Game/FactorPanel" );
		}
		else if( mMinusFactor )
		{
			tagElement = OverlayManager::getSingleton().getOverlayElement( "FactorPanelTitle/MinusFactorImage" );
			factorElement = OverlayManager::getSingleton().getOverlayElement( "Game/MinusFactorPanel" );
		}

		if( ! tagElement || ! factorElement )
		{
			return true;
		}

		static float time;
		static float prevSecond;
		static float curSecond;

		if( 0.1 < curSecond - prevSecond )
		{
			// animate
			tagElement->isVisible() ? tagElement->hide() : tagElement->show();
			factorElement->isVisible() ? factorElement->hide() : factorElement->show();

			prevSecond = curSecond;
			time += 0.1;

			if( 1.5 < time )
			{
				tagElement->show();
				factorElement->show();
				mPlusFactor = mMinusFactor = false;
				time = 0;
			}
		}
		else
		{
			curSecond += evt.timeSinceLastFrame;
		}
	}

	
	return true;

	/*
	if( 1 < curSecond - prevSecond )
	{
		prevSecond = curSecond;
#ifdef _DEBUG
		cprintf( "Interface::frameEnded()\n" );
		cprintf( "\t one seconed over\n" );
#endif
	}
	else
	{
		curSecond += evt.timeSinceLastFrame;
	}
	*/
}

void Interface::setPanelNumber( const int & i, const ColourValue & color )
{
	static char buffer[ 80 ];

	OverlayElement * element = mOverlay->getChild( "Game/RequiredAnswerPanel" )->getChild( "Game/RequiredAnswerText" );

	sprintf( buffer, "%d", i );
	element->setCaption( buffer );
	element->setColour( color );
}

void Interface::setPanelBanner( const std::string & str, const ColourValue & color )
{
	OverlayElement * element = mOverlay->getChild( "Game/RequiredAnswerPanel" )->getChild( "Game/RequiredAnswerBanner" );

	element->setCaption( str );
	element->setColour( color );
}

void Interface::decreaseAnswer()
{
	OverlayContainer * container = mOverlay->getChild( "Game/RemainedAnswerPanel" );
	OverlayContainer::ChildIterator it = container->getChildIterator();
	OverlayElement * element;

	while( it.hasMoreElements() )
	{
		element = it.getNext();
	}

	container->removeChild( element->getName() );
	OverlayManager::getSingleton().destroyOverlayElement( element );
}

void Interface::increaseCQPanel( const float & f )
{
	static int i = 0;
	static char buffer[ 80 ];

	float top = 0.38;
	const float unit = 0.02;

	OverlayContainer * container = mOverlay->getChild( "Game/ClearedAnswerPanel" );
	OverlayContainer::ChildIterator it = container->getChildIterator();
	//OverlayContainer::ChildContainerIterator it = container->getChildContainerIterator();

#ifdef _DEBUG
	cprintf( "Interface::increaseCQPanel()\n" );
	cprintf( "\t container: %d\n", container );
	cprintf( "\t it.hasMoreElements(): %d\n", it.hasMoreElements() );
#endif
	while( it.hasMoreElements() )
	{
		it.moveNext();
		top += unit;
	}

	sprintf( buffer, "ClearedAnswer%0.3f", top );

	OverlayElement * element = OverlayManager::getSingleton().createOverlayElement( "Panel", buffer );

	element->setTop( top + unit );
	element->setWidth( 0.1 );
	element->setHeight( 0.1 );
	element->setLeft( 0.01 );
	element->setMaterialName( "CalcKing/ClearedAnswerPanel" );

	container->addChild( element );
}

void Interface::setAnswer( const std::vector< int > & answer )
{
	static char buffer[ 80 ];
	float top = 0.01;
	const float unit = 0.02;
	
	OverlayContainer * container = mOverlay->getChild( "Game/RemainedAnswerPanel" );

#ifdef _DEBUG
	cprintf( "Interface::setAnswer()\n" );
	cprintf( "\t container: %d\n", container );
#endif
	//int i = 0

	for( std::vector< int >::const_iterator it = answer.begin();
		answer.end() != it; 
		++ it, top += unit )
	{
		sprintf( buffer, "RemainedAnswer%0.3f", top );
		OverlayElement * element = OverlayManager::getSingleton().createOverlayElement( "Panel", buffer );

		element->setTop( top );
		element->setWidth( 0.1 );
		element->setHeight( 0.1 );
		element->setLeft( 0.01 );
		element->setMaterialName( "CalcKing/RemainedAnswerPanel" );

		container->addChild( element );
	}	
}

size_t Interface::getOperationSize( const std::string & s ) const
{
	size_t count = 0;

	/*
	for( size_t i = 0; i < s.size(); ++ i )
	{
		if( '0' <= s[ i ] && '9' >= s[ i ] ||
			'+' == s[ i ] || '-' == s[ i ] ||
			'*' == s[ i ] || '!' == s[ i ] ||
			'(' == s[ i ] || ')' == s[ i ] ||
			'=' == s[ i ] || '?' == s[ i ] ||
			'%' == s[ i ] || '^' == s[ i ] )
		{
			++ count;
		}
	}
	*/
	for( size_t i = 0; i < s.size(); ++ i )
	{
		if( '|' == s[ i ] )
		{
			++ count;
		}
	}

	const size_t answerLength = s.size() - s.find( "==" ) - 2;

	return count - 1 + answerLength;
}

void Interface::putMessage( const std::string & s, const std::string & factor )
{
	if( ! mFadedMessage.empty() && s == mFadedMessage.back().first )
	{
		return;
	}

	putMessage( s );

	if( "Plus Factor" == factor )
	{
		mPlusFactor = true;
	}
	else if( "Minus Factor" == factor ) // true: plus CQ
	{
		mMinusFactor = true;
	}
}

void Interface::putMessage( const std::string & s )
{
	const float liveSecond = 2.0f;

	mFadedMessage.push_back( FadedMessage( s, liveSecond ) );
}

void Interface::setFactor( const std::map< std::string, size_t > & factorMap, 
						  std::map< std::string, size_t > & matchedFactor )
{
	OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "FactorPanelTitle/PlusFactorImage" );
	factorMap.size() ? element->show() : element->hide();	

#ifdef _DEBUG
	cprintf( "Interface::setFactor()\n" );
#endif
	static char buffer[ 80 ];
	OverlayContainer * container = mOverlay->getChild( "Game/FactorPanel" );
	removeAllElement( container );

	float top = 0;
	float left = 0.05;
	//float left = 0;
	float height = 0.05;
	float width = 0.04;
	const float width_unit = 0.05;
	size_t i = 0;


//	for( std::set< std::string >::const_iterator it = factorSet.begin();
//		factorSet.end() != it;
//		++it, ++i )

#ifdef _DEBUG
	cprintf( "\t factorMap:\n" );
	for( std::map< std::string, size_t >::const_iterator it = factorMap.begin();
		factorMap.end() != it; ++it )
	{
		cprintf( "\t\t key: %s, value: %d\n", it->first.c_str(), it->second );
	}

	cprintf( "\t matchedFactorMap:\n" );
	for( std::map< std::string, size_t >::const_iterator it = matchedFactor.begin();
		matchedFactor.end() != it;
		++it )
	{
		cprintf( "\t\t *it: %s ( %d )\n", it->first.c_str(), it->second );
	}
#endif

	for( std::map< std::string, size_t >::const_iterator it = factorMap.begin();
		factorMap.end() != it;
		++it )
	{
		size_t count = it->second;

		while( 0 < count-- )
		{
			sprintf( buffer, "FactorPanel%d", ++i );
			
			OverlayElement * element = OverlayManager::getSingleton().createOverlayElement( "Panel", buffer );
			element->setTop( top );
			element->setLeft( left );
			element->setHeight( height );
			element->setWidth( width );
			element->setMaterialName( SinglePlayState::getInstance()->mMaterialMap[ it->first ] + "Panel" );

			container->addChild( element );

			if( 0 < matchedFactor[ it->first ] )
			//if( matchedFactor.end() != matchedFactor.find( *it ) )
			{
				sprintf( buffer, "CheckedFactorPanel%d", ++i );
				element = OverlayManager::getSingleton().createOverlayElement( "Panel", buffer );
				element->setTop( top );
				element->setLeft( left );
				element->setHeight( height );
				element->setWidth( width );
				element->setMaterialName( "CalcKing/LockedPanel" );

				container->addChild( element );

				--matchedFactor[ it->first ];
			}

			left += width_unit;
		}
	}
}

void Interface::setMinusFactor( const std::set< std::string > & s )
{
#ifdef _DEBUG
	cprintf( "Interface::setMinusFactor()\n" );
#endif
	OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "FactorPanelTitle/MinusFactorImage" );
	s.size() ? element->show() : element->hide();

	OverlayContainer * container = mOverlay->getChild( "Game/MinusFactorPanel" );
	removeAllElement( container );

	float top = 0;
	float left = 0.05;
	float height = 0.05;
	float width = 0.04;
	const float width_unit = 0.05;
	size_t i = 0;

	for( std::set< std::string >::const_iterator it = s.begin();
		s.end() != it;
		++it, ++i )
	{
		static char buffer[ 80 ];
		sprintf( buffer, "MinusFactorPanel%d", i );

#ifdef _DEBUG
		cprintf( "\t value: %s\n", ( *it ).c_str() );
#endif
		
		OverlayElement * element = OverlayManager::getSingleton().createOverlayElement( "Panel", buffer );
		element->setTop( top );
		element->setLeft( left );
		element->setHeight( height );
		element->setWidth( width );
		element->setMaterialName( SinglePlayState::getInstance()->mMaterialMap[ *it ] + "Panel" );

		container->addChild( element );
		left += width_unit;
	}
}