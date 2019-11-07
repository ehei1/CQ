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
#include "HighScoreState.h"

#include "MainMenuState.h"

HighScoreState HighScoreState::mHighScoreState;

void HighScoreState::enter()
{
#ifdef _DEBUG
	cprintf( "HighScoreState::enter()\n" );
#endif
	//mOverlay = OverlayManager::getSingleton().getByName( "Game/HighScorePanel" );
	mOverlay = OverlayManager::getSingleton().getByName( "Menu/HighScoreMenu" );

	OverlayContainer * container = static_cast< OverlayContainer * >(
		OverlayManager::getSingleton().getOverlayElement( "HighScorePanel/Rank" )
	);
	/*
	OverlayContainer * container = static_cast< OverlayContainer * >( 
		mOverlay->getChild( "HighScorePanel/MainPanel" )->getChild( "HighScorePanel/Rank" )
	);
	*/

	// add element
	int row = 0;
	const float top = 0.2;
	const float height_unit = 0.05;
	const float left = 0.25;
	static char buffer[ 80 ];
	const std::string char_height = "0.05";

	std::list< MainMenuState::Result > & rank = MainMenuState::getInstance()->mRank;
	std::list< MainMenuState::Result >::const_iterator it = rank.begin();
	
#ifdef _DEBUG
	cprintf( "\t rank size: %d\n", rank.size() );
#endif

	for( ; rank.end() != it; ++ it, row ++ )
	{
#ifdef _DEBUG
		cprintf( "\t %s \t %0.1f\n", it->first.c_str(), it->second );
#endif
		{
			sprintf( buffer, "HighScorePanel//RankPlayer%d", row );
			OverlayElement * element = OverlayManager::getSingleton().createOverlayElementFromTemplate( 
				"OperandTemplate", "TextArea", buffer
			);

			sprintf( buffer, "%s", it->first.c_str() );
			element->setCaption( buffer );
			
			/*
			mCycleColor.push_back( ColourValue( 1, 0, 0 ) );
			mCycleColor.push_back( ColourValue( 1, 0.5, 0 ) );
			mCycleColor.push_back( ColourValue( 1, 1, 0 ) );
			mCycleColor.push_back( ColourValue( 0, 1, 0 ) );
			mCycleColor.push_back( ColourValue( 0, 0, 1 ) );
			mCycleColor.push_back( ColourValue( 1, 0, 1 ) );
			*/
			
			ColourValue color;

			if( 0 == row )
			{
				color = ColourValue( 1, 0, 0 );
			}
			else if( 0 < row && 3 > row )
			{
				color = ColourValue( 1, 0.5, 0 );
			}
			else if( 3 <= row && 6 > row )
			{
				color = ColourValue( 1, 1, 0 );
			}
			else if( 6 <= row )
			{
				color = ColourValue( 0, 1, 0 );
			}

			element->setColour( color );
			element->setTop( top + row * height_unit );
			element->setParameter( "char_height", char_height );
			element->setLeft( left );

			container->addChild( element );
		}

		{
			sprintf( buffer, "HighScorePanel//RankScore%d", row );
			OverlayElement * element = OverlayManager::getSingleton().createOverlayElementFromTemplate( 
				"OperandTemplate", "TextArea", buffer
			);

			sprintf( buffer, "%0.1f", it->second );
			element->setCaption( buffer );

			ColourValue color;

			if( 0 == row )
			{
				color = ColourValue( 1, 0, 0 );
			}
			else if( 0 < row && 3 > row )
			{
				color = ColourValue( 1, 0.5, 0 );
			}
			else if( 3 <= row && 6 > row )
			{
				color = ColourValue( 1, 1, 0 );
			}
			else if( 6 <= row )
			{
				color = ColourValue( 0, 1, 0 );
			}

			element->setColour( color );
			element->setTop( top + row * height_unit );
			element->setParameter( "char_height", char_height );
			element->setParameter( "alignment", "right" );
			element->setLeft( left + 0.3 );

			container->addChild( element );
		}
	}

	MenuState::enter();
}

void HighScoreState::keyPressed(Ogre::KeyEvent *e)
{
	MenuState::exit();

	StateManager::getInstance()->popState();
}

void HighScoreState::exit()
{
	/*
	OverlayContainer * container = static_cast< OverlayContainer * >( 
		mOverlay->getChild( "HighScorePanel/MainPanel" )->getChild( "HighScorePanel/Rank" )
	);
	*/
	OverlayContainer * container = static_cast< OverlayContainer * >(
		OverlayManager::getSingleton().getOverlayElement( "HighScorePanel/Rank" )
	);
	
	OverlayContainer::ChildIterator it = container->getChildIterator();

	while( it.hasMoreElements() )
	{
		OverlayElement * element = it.getNext();
		container->removeChild( element->getName() );
		OverlayManager::getSingleton().destroyOverlayElement( element );
	}
}

bool HighScoreState::frameStarted( const Ogre::FrameEvent & evt )
{
	static float prevSecond;
	static float curSecond;
	static OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "HighScorePanel/Message" );

	if( 0.5f < curSecond - prevSecond )
	{
		element->isVisible() ? element->hide() : element->show();
		prevSecond = curSecond;
	}
	else
	{
		curSecond += evt.timeSinceLastFrame;
	}

	return true;
}