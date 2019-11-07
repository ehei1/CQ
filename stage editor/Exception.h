#pragma once
#pragma message("Compiling Exception.h - this should happen just once per project.\n")

class Exception
{
public:
	enum Type
	{
		SceneManagerIsNull, UnknownException
	};

	Exception( Type t = Exception::UnknownException );
	~Exception(void);
	std::string getMessage();
	
private:
	Exception::Type type;
};
