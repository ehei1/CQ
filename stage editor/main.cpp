#include "stdafx.h"

/*
#ifndef NDEBUG
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
//#include <iostream>
#endif
*/

#include "MainWindow.h"
#include "main.h"
//#include <QWindowsXPStyle>
//#include <QCDEStyle>
//#include <QPlastiqueStyle>
//#include <QMotifStyle>
//#include <ArthurStyle.h>


int main( int argc, char * argv[] )
{
#ifndef NDEBUG
	/*
		get debug console info from:
		http://www.ogre3d.org/wiki/index.php/HowTo_(part_II)#How_to_have_a_Debug_Console_.28Easy_and_powerful_Alternative.29
	*/
	::AllocConsole();

	_cprintf( "Hello, debug console\n" );
#endif

    QApplication app(argc, argv);
    QMainWindow window;

	QStyle * style = new ::QPlastiqueStyle;
	app.setStyle( style );

	MainWindow ui;
	ui.setupUi( & window );
	
	const int result = app.exec();

#ifndef NDEBUG
	// Ogre::LogManager & lm = Ogre::LogManager::getSingleton();
	// lm.logMessage( "Hi, I'm log" );

	::FreeConsole();
#endif

	return result;
}

/*
#ifndef NDEBUG
void showWin32Console()
{
	static const WORD MAX_CONSOLE_LINES = 500;
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;
	// allocate a console for this app
	AllocConsole();
	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
	coninfo.dwSize);
	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );
	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );
	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();
}
#endif
*/