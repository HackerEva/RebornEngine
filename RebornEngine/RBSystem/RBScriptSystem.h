#ifndef RBSCRIPTSYSTEM_H
#define RBSCRIPTSYSTEM_H
#include<map>
#include"RBSingleton.h"
#include"../../lua5.3/lua.hpp"
#include"../RBScene/RBSceneObject.h"



namespace RebornEngine
{
	class RBSceneObject;

	enum ScriptParamType
	{
		SPT_None,
		SPT_Float,
	};

	struct ScriptParams
	{
		ScriptParamType type[10];
	};

	class RBScriptSystem
		: public RBSingleton<RBScriptSystem>
	{
		friend class RBSingleton<RBScriptSystem>;
		RBScriptSystem();
	public:
		bool Initialize();
		void Shutdown();

		void RegisterFunction(const char* func_name, lua_CFunction func, ScriptParams paramTypes);
		bool Start();

		void RegisterScriptableObject(RBSceneObject* obj);
		void UnregisterScriptableObject(RBSceneObject* obj);
		void UpdateScriptableObjects();
	private:
		lua_State * m_LuaState;
		vector<RBSceneObject*>		m_ScriptableObjects;
		std::map<string, ScriptParams>	m_ScriptParams;
	};
}
#define RBSCRIPT RBScriptSystem::Instance()

#endif
