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
#include "RankState.h"

#include "SinglePlayState.h"
#include "MainMenuState.h"
#include "HighScoreState.h"

RankState RankState::mRankState;

void RankState::enter()
{
	mOverlay = OverlayManager::getSingleton().getByName( "Game/RankPanel" );
	
	OverlayElement * scoreElement = OverlayManager::getSingleton().getOverlayElement( "RankPanel/Score" );
	OverlayElement * inputElement = OverlayManager::getSingleton().getOverlayElement( "RankPanel/Input" );
	OverlayElement * messageElement = OverlayManager::getSingleton().getOverlayElement( "RankPanel/Message" );

	OverlayElement * scoreTextElement = OverlayManager::getSingleton().getOverlayElement( "RankPanel/ScoreText" );

	const float & cq = SinglePlayState::getInstance()->mCQ;
	static char buffer[ 80 ];
	sprintf( buffer, "%0.1f", cq );
	std::string score = ( 0 > cq ) ? "-" : "+";
	
	scoreTextElement->setCaption( score + buffer );
	
	if( isRank() )
	{	
		messageElement->setCaption( "Press ENTER to finish" );
		messageElement->setTop( 0.84 );
		scoreElement->setTop( 0.2 );
		

		inputElement->setTop( 0.5 );
		inputElement->setCaption( "_" );
		inputElement->show();

		mInputMode = true;
		mName = "_";
	}
	else
	{
		messageElement->setCaption( "Press any key" );
		messageElement->setTop( 0.72 );
		scoreElement->setTop( 0.4 );
		inputElement->hide();

		mInputMode = false;
	}

	mOverlay->show();

	{
		SoundManager::getSingleton().StopAllSounds();

		int index = SoundManager::getSingleton().CreateLoopedStream( StateManager::getInstance()->mAudio.rankingMusic );
		int channel = INVALID_SOUND_CHANNEL;

		SceneNode * sceneNode = SinglePlayState::getInstance()->mSceneMgr->getRootSceneNode();
		SoundManager::getSingleton().PlaySound( index, sceneNode, &channel);
	}
}

void RankState::exit()
{
	addRank();
	SoundManager::getSingleton().StopAllSounds();

	mOverlay->hide();
}

void RankState::pause()
{
}

void RankState::resume()
{
}

void RankState::keyPressed(KeyEvent* e)
{
	const size_t size = 8;
	const int key = e->getKey();
	const char keyChar = e->getKeyChar();

	e->consume();

	if( ! mInputMode || ::KC_RETURN == key )
	{
		StateManager::getInstance()->popState();
		//StateManager::getInstance()->pushState( HighScoreState::getInstance() );
		return;
	}

#ifdef _DEBUG
	cprintf( "void RankState::keyPressed(KeyEvent* e)\n" );
	cprintf( "\t char: %c\n", mName[ mName.size() - 1 ] );
	cprintf( "\t name: %s\n", mName.c_str() );
#endif
	if( '_' == mName[ mName.size() - 1 ] )
	{
		mName.resize( mName.size() - 1 ); // eliminate cursor
	}

	if( ::KC_BACK == key && ! mName.empty() )
	{
		mName.resize( mName.size() - 1 );
	}

	if( mName.size() > size - 1 )
	{
		return;
	}

	switch( keyChar )
	{
	case 'A':	mName.push_back( 'A' );	break;
	case 'a':	mName.push_back( 'a' );	break;
	case 'B':	mName.push_back( 'B' ); break;
	case 'b':	mName.push_back( 'b' ); break;
	case 'C':	mName.push_back( 'C' ); break;
	case 'c':	mName.push_back( 'c' ); break;
	case 'D':	mName.push_back( 'D' ); break;
	case 'd':	mName.push_back( 'd' ); break;
	case 'E':	mName.push_back( 'E' ); break;
	case 'e':	mName.push_back( 'e' ); break;
	case 'F':	mName.push_back( 'F' ); break;
	case 'f':	mName.push_back( 'f' ); break;
	case 'G':	mName.push_back( 'G' ); break;
	case 'g':	mName.push_back( 'g' ); break;
	case 'H':	mName.push_back( 'H' ); break;
	case 'h':	mName.push_back( 'h' ); break;
	case 'I':	mName.push_back( 'I' ); break;
	case 'i':	mName.push_back( 'i' ); break;
	case 'J':	mName.push_back( 'J' ); break;
	case 'j':	mName.push_back( 'j' ); break;
	case 'K':	mName.push_back( 'K' ); break;
	case 'k':	mName.push_back( 'k' ); break;
	case 'L':	mName.push_back( 'L' ); break;
	case 'l':	mName.push_back( 'l' ); break;
	case 'M':	mName.push_back( 'M' ); break;
	case 'm':	mName.push_back( 'm' ); break;
	case 'N':	mName.push_back( 'N' ); break;
	case 'n':	mName.push_back( 'n' ); break;
	case 'O':	mName.push_back( 'O' ); break;
	case 'o':	mName.push_back( 'o' ); break;
	case 'P':	mName.push_back( 'P' ); break;
	case 'p':	mName.push_back( 'p' ); break;
	case 'Q':	mName.push_back( 'Q' ); break;
	case 'q':	mName.push_back( 'q' ); break;
	case 'R':	mName.push_back( 'R' ); break;
	case 'r':	mName.push_back( 'r' ); break;
	case 'S':	mName.push_back( 'S' ); break;
	case 's':	mName.push_back( 's' ); break;
	case 'T':	mName.push_back( 'T' ); break;
	case 't':	mName.push_back( 't' ); break;
	case 'U':	mName.push_back( 'U' ); break;
	case 'u':	mName.push_back( 'u' ); break;
	case 'V':	mName.push_back( 'V' ); break;
	case 'v':	mName.push_back( 'v' ); break;
	case 'W':	mName.push_back( 'W' ); break;
	case 'w':	mName.push_back( 'w' ); break;
	case 'X':	mName.push_back( 'X' ); break;
	case 'x':	mName.push_back( 'x' ); break;
	case 'Y':	mName.push_back( 'Y' ); break;
	case 'y':	mName.push_back( 'y' ); break;
	case 'Z':	mName.push_back( 'Z' ); break;
	case 'z':	mName.push_back( 'z' ); break;
	case '0':	mName.push_back( '0' ); break;
	case '1':	mName.push_back( '1' ); break;
	case '2':	mName.push_back( '2' ); break;
	case '3':	mName.push_back( '3' ); break;
	case '4':	mName.push_back( '4' ); break;
	case '5':	mName.push_back( '5' ); break;
	case '6':	mName.push_back( '6' ); break;
	case '7':	mName.push_back( '7' ); break;
	case '8':	mName.push_back( '8' ); break;
	case '9':	mName.push_back( '9' ); break;
	}

	if( mName.size() < size )
	{
		mName.push_back( '_' ); // add cursor
	}

	OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "RankPanel/InputEdit" );
	element->setCaption( mName );
}

bool RankState::frameStarted(const FrameEvent& evt)
{
	static float prevSecond;
	static float curSecond;
	static OverlayElement * element = OverlayManager::getSingleton().getOverlayElement( "RankPanel/Message" );
	static bool isShow;

	if( 0.5f < curSecond - prevSecond )
	{
		isShow = ! isShow;
		prevSecond = curSecond;
	}
	else
	{
		isShow ? element->show() : element->hide();

		curSecond += evt.timeSinceLastFrame;
	}

	return true;
}

bool RankState::frameEnded(const FrameEvent& evt)
{
	return true;
}

void RankState::mousePressed( Ogre::MouseEvent * e )
{
	e->consume();
}

void RankState::mouseMoved( Ogre::MouseEvent * e )
{
	e->consume();
}

void RankState::addRank()
{
	const float & cq = SinglePlayState::getInstance()->mCQ;
	std::list< MainMenuState::Result > & rank = MainMenuState::getInstance()->mRank;

	for( std::list< MainMenuState::Result >::iterator it = rank.begin();
		rank.end() != it; ++ it )
	{
		if( it->second < cq )
		{
			if( 1 == mName.size() ) // mName = "_"
			{
				mName = "CalcKing";
			}

			if( '_' == mName[ mName.size() - 1 ] )
			{
				mName.resize( mName.size() - 1 );
			}

			rank.insert( it, MainMenuState::Result( mName, cq ) );
			rank.pop_back();
			return;
		}
	}
}

bool RankState::isRank()
{
	const float & cq = SinglePlayState::getInstance()->mCQ;
	const std::list< MainMenuState::Result > & rank = MainMenuState::getInstance()->mRank;

	for( std::list< MainMenuState::Result >::const_iterator it = rank.begin();
		rank.end() != it; ++ it )
	{
		if( it->second < cq )
		{
			return true;
			//rank.insert( it, score );
			//rank.pop_back();
		}
	}
	
	return false;
}