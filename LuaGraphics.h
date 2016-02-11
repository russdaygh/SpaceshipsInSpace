#pragma once

#include "RustyLib.h"

namespace LuaGraphics
{
	extern "C"
	{
		std::string DrawRectString = "DrawRect";
		static int DrawRect(lua_State* lua_state)
		{
			using RustyLib::Graphics::Renderer;

			int minX,minY,maxX,maxY;

			minX = LuaUtils::GetInt(1);
			minY = LuaUtils::GetInt(2);
			maxX = LuaUtils::GetInt(3);
			maxY = LuaUtils::GetInt(4);
			if(LuaUtils::GetErrorCode()) return 0;

			Renderer* r = &Renderer::Instance();

			r->RenderRect(minX,minY,maxX,maxY);

			return 0;
		}

		std::string DrawTextString = "DrawText";
		static int DrawText(lua_State* lua_state)
		{
			using RustyLib::Graphics::Renderer;

			std::string text;
			int minX,minY;

			text = LuaUtils::GetString(1);
			minX = LuaUtils::GetInt(2);
			minY = LuaUtils::GetInt(3);

			Renderer* r = &Renderer::Instance();

			r->RenderText(text, minX, minY);

			return 0;
		}
	}
}