#ifndef _RBENGINE_H
#define _RBENGINE_H
#include <Windows.h>
#include"RBMath/RBMath.h"
#include"RBPhysics/RBPhysics.h"
#include"RBSystem/RBTimer.h"
#include"RBSystem/RBIApplication.h"
#include "RBLog/RBLog.h"
#include "RBSystem/RBTimer.h"
#include "RBInput/RBInput.h"
#include "RBRender/RBRenderSystem.h"
#include "RBAudio/RBAudio.h"
#include "RBScene/RBScene.h"
#include "RBShader/RBShader.h"
namespace RebornEngine
{
	class RBEngine
	{
	public:
		RBEngine();
		~RBEngine();

		// Initialize all engine components
		bool Initialize();

		bool Initialize(HWND hWnd, int width, int height);

		// Specify the application for the engine to run
		inline void BindApp(RBIApplication* app) { m_Application = app; }

		// Shutdown the engine and destroy all engine components
		void Shutdown();

		// Engine main loop
		void Run();

		void RunOneFrame(bool update_input = false);

		void ResizeClientWindow(int width, int height);
		RECT GetWindowRectInfo() const;

		static RBTimer& GetTimer() { return m_Timer; }
		static RBEngine* Instance() { return m_EngineInstance; }

		void SetEditorMode(bool editor) { m_bIsEditor = editor; }
		bool IsEditor() const { return m_bIsEditor; }

	private:
		bool CreateRenderWindow(int width, int height, bool fullscreen = false, int bpp = 32);
		void DestroyRenderWindow();
		void CalculateFrameStats();

		static RBEngine*	m_EngineInstance;
		bool				m_bIsEditor;

		HINSTANCE			m_hInst;
		HWND				m_hWnd;
		bool				m_bFullScreen;
		bool				m_UseEngineRenderWindow;
		RBIApplication*				m_Application;
		static RBTimer		m_Timer;
	};
}
#endif