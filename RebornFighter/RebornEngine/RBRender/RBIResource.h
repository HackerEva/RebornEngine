#ifndef RBIRESOURCE_H
#define RBIRESOURCE_H
#include<string>

using std::string;

namespace RebornEngine
{	
	

	enum RBResourceState
	{
		RB_RS_Empty,
		RB_RS_EnqueuedForLoading,
		RB_RS_Loaded,
	};

	enum RBResourceType
	{
		RB_RT_Mesh,
		RB_RT_Texture,
	};

	class RBIResource
	{
		friend class RBResourceManager;
	public:
		RBIResource(RBResourceType type, string path) : m_State(RB_RS_Empty), m_Type(type), m_ResourcePath(path) {}
		virtual ~RBIResource() = 0 {}

		RBResourceState GetResourceState() const { return m_State; }
		RBResourceType GetResourceType() const { return m_Type; }
		string GetPath() const { return m_ResourcePath; }
		bool IsResourceReady() const { return m_State == RB_RS_Loaded; }

		// Multi-threading loading complete callback
		//virtual void OnLoaded() = 0;
	protected:
		RBResourceState		m_State;
		RBResourceType		m_Type;
		string			m_ResourcePath;
	};

}

#endif // !RBIRESOURCE

