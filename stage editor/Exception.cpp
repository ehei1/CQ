#include "stdafx.h"
#include "Exception.h"

Exception::Exception( Exception::Type t )
: type( t )
{
}


Exception::~Exception(void)
{
}


std::string Exception::getMessage()
{
	std::string message;

	switch( type )
	{
	case SceneManagerIsNull:
		message = "Scene manager is null. It must allocated before using";
		break;
	case UnknownException:
		message = "Unknown exception occured";
		break;
	default:
		message = "pre-defined exception is not";
		break;
	}

	return message;
}