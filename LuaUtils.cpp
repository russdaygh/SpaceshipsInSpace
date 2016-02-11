#include "stdafx.h"
#include "LuaUtilities.h"
#include "LuaGraphics.h"

lua_State* LuaUtils::lua_state;

Lua_Error LuaUtils::error_code = Lua_Error::None;

void LuaUtils::Initialise(void )
{
	if(!lua_state)
	{
		lua_state = lua_open();
		luaL_openlibs(lua_state);
		error_code = Lua_Error::None;

		RegisterMethod(LuaGraphics::DrawRect, LuaGraphics::DrawRectString);
		RegisterMethod(LuaGraphics::DrawText, LuaGraphics::DrawTextString);
	}
}

void LuaUtils::SetErrorCode(Lua_Error errCode)
{
	error_code = errCode;
}

void LuaUtils::LoadFile(std::string file)
{
	if(!lua_state)
	{
		Initialise();
	}

	if(luaL_dofile(lua_state, file.c_str()))
	{
		//We have an error
		SetErrorCode(Lua_Error::Invalid_File);
	}
}

Lua_Error LuaUtils::GetErrorCode(void )
{
	Lua_Error currErr = error_code;
	//reset the erro code
	error_code = Lua_Error::None;
	//Return the original error
	return currErr;
}

void LuaUtils::DumpLuaStack(void )
{
	int stackSize = lua_gettop(lua_state);
	int type;

	for(int i = stackSize; i > 0; --i)
	{
		type = lua_type(lua_state, i);

		switch(type)
		{
			case LUA_TNIL:
				std::cout << "Element "<<i<<" is a NIL" << std::endl;
			break;
			case LUA_TNUMBER:
				std::cout << "Element "<<i<<" is a NUMBER" << std::endl;
			break;
			case LUA_TBOOLEAN:
				std::cout << "Element "<<i<<" is a BOOLEAN" << std::endl;
			break;
			case LUA_TSTRING:
				std::cout << "Element "<<i<<" is a STRING" << std::endl;
			break;
			case LUA_TTABLE:
				std::cout << "Element "<<i<<" is a TABLE" << std::endl;
			break;
			case LUA_TFUNCTION:
				std::cout << "Element "<<i<<" is a FUNCTION" << std::endl;
			break;
			case LUA_TUSERDATA:
				std::cout << "Element "<<i<<" is a USER DATA" << std::endl;
			break;
			case LUA_TTHREAD:
				std::cout << "Element "<<i<<" is a THREAD" << std::endl;
			break;
			case LUA_TLIGHTUSERDATA:
				std::cout << "Element "<<i<<" is a LIGHT USER DATA" << std::endl;
			break;
			default:
				std::cout << "Element "<<i<<" is of unknown type" << std::endl;
			break;
		}
	}
}

void LuaUtils::ClearLuaStack(void )
{
	if(lua_state)
	{
		lua_pop(lua_state, -1);
	}
}

void LuaUtils::CallMethod(std::string name, int numArgs, int numRets)
{
	if(!lua_state)
	{
		Initialise();
	}

	lua_getglobal(lua_state, name.c_str());

	if(!lua_isfunction(lua_state, stackTop))
	{
		//Error
		SetErrorCode(Lua_Error::Method_Not_Registered);
		return;
	}
	else
	{
		int err = lua_pcall(lua_state, numArgs, numRets, 0);
		if(err)
		{
			if(err = LUA_ERRRUN)
			{
				SetErrorCode(Lua_Error::Run_Time_Error);
			}
			if(err == LUA_ERRERR)
			{
				SetErrorCode(Lua_Error::Error_Handle_Error);
			}
		}
	}
}

void LuaUtils::RegisterMethod(int (*pMethod)(lua_State*), std::string name)
{
	if(!lua_state)
	{
		Initialise();
	}

	//Check if it is already registered
	lua_getglobal(lua_state, name.c_str());

	if(lua_iscfunction(lua_state, stackTop))
	{
		//Methods is already registered
		return;
	}
	else
	{
		//Register the method
		lua_register(lua_state, name.c_str(), pMethod);

		lua_getglobal(lua_state, name.c_str());
		if(lua_iscfunction(lua_state, stackTop))
		{
			//No error
			SetErrorCode(Lua_Error::None);
			return;
		}
		else
		{
			//We have an error
			SetErrorCode(Lua_Error::Method_Registration_Failed);
			return;
		}
	}
}

void LuaUtils::GetTable(std::string name)
{
	if(!lua_state)
	{
		Initialise();
	}

	lua_getglobal(lua_state, name.c_str());

	if(!lua_istable(lua_state, stackTop))
	{
		//Error!
		SetErrorCode(Lua_Error::Incorrect_Type);
	}
}

char LuaUtils::GetChar(std::string name)
{
	if(!lua_state)
	{
		Initialise();
	}

	lua_getglobal(lua_state, name.c_str());

	if(!lua_isstring(lua_state, stackTop))
	{
		//Error!
		SetErrorCode(Lua_Error::Incorrect_Type);
		DumpLuaStack();
		return 0;
	}
	else
	{
		char* str;
		str = (char*)lua_tostring(lua_state, stackTop);
		return str[0];
	}
}

int LuaUtils::GetInt(int stackIndex)
{
	if(!lua_state)
	{
		Initialise();
	}

	if(!lua_isnumber(lua_state, stackIndex))
	{
		//Error!
		SetErrorCode(Lua_Error::Incorrect_Type);
		return 0;
	}
	else
	{
		return (int)lua_tonumber(lua_state, stackIndex);
	}
}

int LuaUtils::GetInt(std::string name)
{
	if(!lua_state)
	{
		Initialise();
	}

	lua_getglobal(lua_state, name.c_str());

	if(!lua_isnumber(lua_state, stackTop))
	{
		//Error!
		SetErrorCode(Lua_Error::Incorrect_Type);
		return 0;
	}
	else
	{
		return (int)lua_tonumber(lua_state, stackTop);
	}
}

float LuaUtils::GetFloat(std::string name)
{
	if(!lua_state)
	{
		Initialise();
	}

	lua_getglobal(lua_state, name.c_str());

	if(!lua_isnumber(lua_state, stackTop))
	{
		//Error!
		SetErrorCode(Lua_Error::Incorrect_Type);
		return 0.0f;
	}
	else
	{
		double num = lua_tonumber(lua_state, stackTop);
		return (float)num;
	}
}

std::string LuaUtils::GetString(int stackIndex)
{
	if(!lua_state)
	{
		Initialise();
	}

	if(!lua_isstring(lua_state, stackIndex))
	{
		//Error
		SetErrorCode(Lua_Error::Incorrect_Type);
		return 0;
	}
	else
	{
		std::string str = lua_tostring(lua_state, stackIndex);
		return str;
	}
}

std::string LuaUtils::GetString(std::string name)
{
	if(!lua_state)
	{
		Initialise();
	}

	lua_getglobal(lua_state, name.c_str());

	if(!lua_isstring(lua_state, stackTop))
	{
		//Error!
		SetErrorCode(Lua_Error::Incorrect_Type);
		return 0;
	}
	else
	{
		std::string str;
		str = lua_tostring(lua_state, stackTop);
		return str;
	}
}

Vector3* LuaUtils::GetVector3(std::string name)
{
	if(!lua_state)
	{
		Initialise();
	}

	lua_getglobal(lua_state, name.c_str());

	if(!lua_istable(lua_state, stackTop))
	{
		//We don't have a table
		//the format by which we store vectors in Lua
		SetErrorCode(Lua_Error::Incorrect_Type);
		return 0;
	}
	else
	{
		float x,y,z;

		lua_pushnumber(lua_state, 1);
		lua_gettable(lua_state, stackTop-1);

		if(!lua_isnumber(lua_state, stackTop))
		{
			//Error
			SetErrorCode(Lua_Error::Incorrect_Type);
			return 0;
		}
		else
		{
			x = (float)lua_tonumber(lua_state,stackTop);
		}

		lua_pushnumber(lua_state, 2);
		lua_gettable(lua_state, stackTop-2);
		if(!lua_isnumber(lua_state, stackTop))
		{
			//Error
			SetErrorCode(Lua_Error::Incorrect_Type);
			return 0;
		}
		else
		{
			y = (float)lua_tonumber(lua_state,stackTop);
		}

		lua_pushnumber(lua_state, 3);
		lua_gettable(lua_state, stackTop-3);
		if(!lua_isnumber(lua_state, stackTop))
		{
			//Error
			SetErrorCode(Lua_Error::Incorrect_Type);
			return 0;
		}
		else
		{
			z = (float)lua_tonumber(lua_state,stackTop);
		}

		Vector3 *vec = new Vector3(x,y,z);
		return vec;
	}
}