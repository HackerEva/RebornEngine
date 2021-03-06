#include "RBScriptSystem.h"
#include <windows.h>
#include<WinBase.h>
namespace RebornEngine
{
	void print_error(lua_State* state) {
		// The error message is on top of the stack.
		// Fetch it, print it and then pop it off the stack.
		const char* message = lua_tostring(state, -1);
		OutputDebugStringA(message);
		OutputDebugStringA("\n");
		lua_pop(state, 1);
	}

	RBScriptSystem::RBScriptSystem()
		: m_LuaState(nullptr)
	{

	}

	bool RBScriptSystem::Initialize()
	{
		m_LuaState = luaL_newstate();

		// Use standard library in lua scripts
		luaL_openlibs(m_LuaState);

		int result = luaL_loadfile(m_LuaState, "Assets/Scripts/GameMain.lua");

		if (result != LUA_OK) {
			print_error(m_LuaState);
			return false;
		}

		RBSceneObject::RegisterScriptFunctions();
		return true;
	}

	void RBScriptSystem::Shutdown()
	{
		lua_close(m_LuaState);
	}

	void RBScriptSystem::RegisterFunction(const char * func_name, lua_CFunction func, ScriptParams paramTypes)
	{
		lua_register(m_LuaState, func_name, func);
		m_ScriptParams[func_name] = paramTypes;

	}

	bool RBScriptSystem::Start()
	{
		// Run script once for function calling from c++
		int result = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);

		if (result != LUA_OK) {
			print_error(m_LuaState);
			return false;
		}

		return true;
	}

	void RBScriptSystem::RegisterScriptableObject(RBSceneObject * obj)
	{
		if (find(m_ScriptableObjects.begin(), m_ScriptableObjects.end(), obj) == m_ScriptableObjects.end())
		{
			m_ScriptableObjects.push_back(obj);
		}


	}

	void RBScriptSystem::UnregisterScriptableObject(RBSceneObject * obj)
	{
		vector<RBSceneObject*>::iterator iter = find(m_ScriptableObjects.begin(), m_ScriptableObjects.end(), obj);
		if (iter != m_ScriptableObjects.end())
			m_ScriptableObjects.erase(iter);

	}

	void RBScriptSystem::UpdateScriptableObjects()
	{
		for (vector<RBSceneObject*>::iterator iter = m_ScriptableObjects.begin(); iter != m_ScriptableObjects.end(); iter++)
		{
			if ((*iter)->GetScript() == "")
				continue;

			const vector<string>& parsedCmds = (*iter)->GetParsedScript();
			if (parsedCmds.empty())
				continue;

			//const ScriptParams& params = m_ScriptParams[parsedCmds[0]];

			lua_getglobal(m_LuaState, parsedCmds[0].c_str());
			if (lua_isfunction(m_LuaState, -1))
			{
				lua_pushlightuserdata(m_LuaState, *iter);

				int paramCount = 0;
				for (UINT i = 0; i < parsedCmds.size() - 1; i++)
				{
					//switch (params.type[i])
					//{
					//case SPT_Float:
					lua_pushnumber(m_LuaState, atof(parsedCmds[i + 1].c_str()));
					paramCount++;
					//	break;
					//default:
					//	break;
					//}
				}

				int result = lua_pcall(m_LuaState, paramCount + 1, 0, 0);
				if (result != LUA_OK)
				{
					print_error(m_LuaState);
				}
			}
		}

	}

}
