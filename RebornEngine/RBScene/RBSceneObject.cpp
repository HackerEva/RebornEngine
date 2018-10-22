#include "RBSceneObject.h"
#include <iterator>
#include"../../lua5.3/lua.hpp"
#include"../RBSystem/RBScriptSystem.h"
#include <sstream>
#include"../RBEngine.h"

using namespace RebornEngine;


static int ScriptFunc_MoveTo(lua_State* state)
{
	int n = lua_gettop(state);

	if (n < 4)
		return 0;

	RBSceneObject* obj = static_cast<RBSceneObject*>(lua_touserdata(state, 1));
	RBVector3 target;
	target.x = (float)lua_tonumber(state, 2);
	target.y = (float)lua_tonumber(state, 3);
	target.z = (float)lua_tonumber(state, 4);

	float sqrDist = (target - obj->GetPosition()).SquaredMagitude();
	if (sqrDist <= 1.0f)
		obj->SetPosition(target);
	else
	{
		RBVector3 rel = (target - obj->GetPosition()).GetNormalizedVec3();
		if (rel.SquaredMagitude() > 0.0f)
			obj->Translate(rel);
	}

	return 0;
}

static int ScriptFunc_Swing(lua_State* state)
{
	int n = lua_gettop(state);

	if (n < 7)
		return 0;

	RBSceneObject* obj = static_cast<RBSceneObject*>(lua_touserdata(state, 1));
	RBVector3 origin, target;
	origin.x = (float)lua_tonumber(state, 2);
	origin.y = (float)lua_tonumber(state, 3);
	origin.z = (float)lua_tonumber(state, 4);
	target.x = (float)lua_tonumber(state, 5);
	target.y = (float)lua_tonumber(state, 6);
	target.z = (float)lua_tonumber(state, 7);

	float amplitude = (float)lua_tonumber(state, 8);
	RBVector3 pos = RBVector3::Lerp(origin, target, sinf(RBEngine::GetTimer().TotalTime()) * 0.5f + 0.5f);

	obj->SetPosition(pos);

	return 0;
}

static int ScriptFunc_Rotate(lua_State* state)
{
	int n = lua_gettop(state);

	if (n < 4)
		return 0;

	RBSceneObject* obj = static_cast<RBSceneObject*>(lua_touserdata(state, 1));
	RBVector3 axis;
	axis.x = (float)lua_tonumber(state, 2);
	axis.y = (float)lua_tonumber(state, 3);
	axis.z = (float)lua_tonumber(state, 4);

	float scale = 1.0f;
	if (n >= 5)
		scale = (float)lua_tonumber(state, 5);
	axis *= RBEngine().GetTimer().TotalTime() * scale;

	RBMatrix4x4 transform = obj->GetNodeTransform();
	obj->SetRotation(RBMatrix4x4::CreateZAxisRotation(axis.z) *
		RBMatrix4x4::CreateYAxisRotation(axis.y) *
		RBMatrix4x4::CreateXAxisRotation(axis.x));

	return 0;
}



void RBSceneObject::RegisterScriptFunctions()
{
	RBSCRIPT.RegisterFunction("MoveTo", ScriptFunc_MoveTo, { { SPT_Float, SPT_Float, SPT_Float } });
	RBSCRIPT.RegisterFunction("Swing", ScriptFunc_Swing, { { SPT_Float, SPT_Float, SPT_Float, SPT_Float, SPT_Float, SPT_Float, SPT_Float } });
	RBSCRIPT.RegisterFunction("Rotate", ScriptFunc_Rotate, { { SPT_Float, SPT_Float, SPT_Float, SPT_Float } });

}

RBSceneObject::RBSceneObject()
{
	m_NodeTransform = RBMatrix4x4::IDENTITY;
	RBSCRIPT.RegisterScriptableObject(this);

}

RBSceneObject::~RBSceneObject()
{
	RBSCRIPT.UnregisterScriptableObject(this);
}

const RBMatrix4x4 & RBSceneObject::GetNodeTransform() const
{
	return m_NodeTransform;
}

void RBSceneObject::SetTransform(const RBMatrix4x4 & transform)
{
	m_NodeTransform = transform;
}

void RBSceneObject::SetRotation(const RBMatrix4x4 & rot)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m_NodeTransform.m[i][j] = rot.m[i][j];
}

void RBSceneObject::SetPosition(const RBVector3 & pos)
{
	m_NodeTransform.SetTranslation(pos);
}

RBVector3 RBSceneObject::GetPosition() const
{
	return m_NodeTransform.GetRow(3).ToVector3();
}

void RBSceneObject::LookAt(const RBVector3 & target, const RBVector3 & world_up)
{
	RBVector3 pos = m_NodeTransform.GetTranslation();
	RBVector3 forward = target - pos;
	forward.Normalize();
	RBVector3 right = world_up.Cross(forward);
	right.Normalize();
	RBVector3 up = forward.Cross(right);

	m_NodeTransform.SetRow(0, RBVector4(right, 0));
	m_NodeTransform.SetRow(1, RBVector4(up, 0));
	m_NodeTransform.SetRow(2, RBVector4(forward, 0));
	m_NodeTransform.SetRow(3, RBVector4(pos, 1));

}

void RBSceneObject::Translate(const RBVector3 & v)
{
	m_NodeTransform.Translate(v);
}

void RBSceneObject::TranslateLocal(const RBVector3 & v)
{
	m_NodeTransform.TranslateLocal(v);
}

const vector<string>& RBSceneObject::GetParsedScript()
{
	if (!m_Script.empty() && m_ParsedScript.empty())
	{
		size_t pos = 0;
		string script = m_Script;

		while ((pos = script.find(',')) != string::npos)
		{
			script = script.replace(pos, 1, " ");
		}

		std::istringstream iss(script);
		copy(std::istream_iterator<string>(iss),
			std::istream_iterator<string>(),
			back_inserter(m_ParsedScript));
	}

	return m_ParsedScript;

}
