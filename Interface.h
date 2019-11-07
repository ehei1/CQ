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

class Interface
{
public:
	void enter();
	void exit();
	void pause();
	void resume();

	//void frameStarted(); // for update overlay
	bool frameEnded( const FrameEvent & ); // for update overlay

	static Interface * getInstance() { return & mInterface; }

	void show( const bool & );

	// time managing
	void setMaxSecond( const float & );
	void setSecond( const float & );
	
	// answer managing
	void setAnswer( const std::vector< int > & );
	void setFactor( const std::map< std::string, size_t > &, std::map< std::string, size_t > & );
	void setMinusFactor( const std::set< std::string > & );
	//void revealAnswer() {}
	void decreaseAnswer();
	void increaseCQPanel( const float & f );
	
	// operand managng
	void setInputedOperand( const std::string & );
	
	// score managing

	// set RequiredAnswerPanel
	// TODO: default color should put to CalcKingConfig.txt
	void setPanelNumber( const int & i, const ColourValue & color = ColourValue::White );
	void setPanelBanner( const std::string & str, const ColourValue & color = ColourValue( 0, 1, 1 ) );
	size_t getOperationSize( const std::string & ) const;
	void putMessage( const std::string & );
	void putMessage( const std::string &, const std::string & ); // true: plus CQ, false: minus CQ

protected:
	Interface();

	Overlay * mOverlay;

	//OverlayContainer * mTimeBarHide;
	//OverlayContainer * mInputedOperand;
	//OverlayContainer * mResult;

	//OverlayElement * mTimeOverlayElement;
	//OverlayElement * mInputedOverlayElement;
	//OverlayElement * mResultOverlayElement;

	double mMaxSecond;

	// answer managing
	int getResult() {} // binding DLL or script to get result of inputed operand
	void removeAllElement( OverlayContainer * );

private:
	static Interface mInterface;
	std::vector< ColourValue > mCycleColor;

	bool mIsAnimate;
	
	typedef std::pair< std::string, float > FadedMessage; // first: message, second: live time
	std::vector< FadedMessage > mFadedMessage;

	bool mMinusFactor;
	bool mPlusFactor;
};