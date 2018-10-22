#ifndef RBSCENEOBJECT_H
#define RBSCENEOBJECT_H
//#include"RBScene.h"
#include"../RBMath/RBMath.h"
#include"../RBPhysics/RBPhysics.h"
#include<vector>
#include<string>
using std::string;
using std::vector;
namespace RebornEngine
{
	class RBScene;

	enum RBSceneObjectType
	{
		RB_SO_None,
		RB_SO_MeshObject,
		RB_SO_Camera,
	};

	class RBSceneObject
	{
	public:
		static void RegisterScriptFunctions();

		RBSceneObject();
		virtual ~RBSceneObject();

		void SetName(const string& name) { m_Name = name; }
		string GetName() const { return m_Name; }

		void SetScene(RBScene* scene) { m_Scene = scene; }
		RBScene* GetScene() const { return m_Scene; }

		virtual RBSceneObjectType GetType() const { return RB_SO_None; }
		virtual RBSceneObject* Clone() const { return nullptr; }

		const RBMatrix4x4& GetNodeTransform() const;

		void SetTransform(const RBMatrix4x4& transform);
		void SetRotation(const RBMatrix4x4& rot);
		void SetPosition(const RBVector3& pos);
		RBVector3 GetPosition() const;

		void LookAt(const RBVector3& target, const RBVector3& world_up = RBVector3(0, 1, 0));

		void Translate(const RBVector3& v);
		void TranslateLocal(const RBVector3& v);

		virtual RBAABB GetAabb() const { return RBAABB::Default; }
		virtual void Draw() {}
		virtual void DrawDepthPass() {}

		void SetScript(const string& script) { m_Script = script; }
		string GetScript() const { return m_Script; }
		const vector<string>& GetParsedScript();
	protected:
		string			m_Name;
		RBMatrix4x4		m_NodeTransform;
		RBScene*		m_Scene;
		string			m_Script;
		vector<string>	m_ParsedScript;
	};
}

#endif