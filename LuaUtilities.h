#pragma once

#include "StdAfx.h"
#include "RustyLib.h"

extern "C"
{
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
}

//typedef int Lua_Error;

enum Lua_Error
{
	None,
	Incorrect_Type,
	Invalid_File,
	Run_Time_Error,
	Error_Handle_Error,
	Method_Not_Registered,
	Method_Registration_Failed
};

class LuaUtils
{
private:
	static const int stackTop = -1;
	static void Initialise(void );
	static lua_State* lua_state;

	static void SetErrorCode(Lua_Error code);
	static Lua_Error error_code;

public:
	static void LoadFile(std::string file);

	static Lua_Error GetErrorCode(void );
	static void DumpLuaStack(void );
	static void ClearLuaStack(void );

	static void CallMethod(std::string name, int numArgs, int numRets);
	//This is rather dodgy :S
	static void RegisterMethod(int (*pMethod)(lua_State*), std::string name);

	static void GetTable(std::string name);

	static char GetChar(std::string name);
	static int GetInt(int stackIndex);
	static int GetInt(std::string name);
	static float GetFloat(std::string name);
	static std::string GetString(int stackIndex);
	static std::string GetString(std::string name);	
	static Vector3* GetVector3(std::string name);
};