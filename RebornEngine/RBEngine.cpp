#include "RBEngine.h"
#include <iosfwd>
#include <sstream>
using namespace RebornEngine;

RBEngine* RBEngine::m_EngineInstance;

static TCHAR szWindowClass[] = _T("reborn_app");
RBTimer RBEngine::m_Timer;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


RebornEngine::RBEngine::RBEngine()
{ 
	m_bIsEditor = false;
	m_hInst = NULL;
	m_hWnd = NULL;
	m_bFullScreen = false;
	m_UseEngineRenderWindow = false;
	m_Application = NULL;

	SetProcessDPIAware();


	RBLOG.Initialize();
	RBLOG.printLine("RBLog Initial Complete:");
}

RebornEngine::RBEngine::~RBEngine()
{
}

bool RebornEngine::RBEngine::Initialize()
{
	m_EngineInstance = this;

	int width = 1024,
		height = 768;

	if (!CreateRenderWindow(width, height))
		return false;

	return Initialize(m_hWnd, width, height);

}

bool RebornEngine::RBEngine::Initialize(HWND hWnd, int width, int height)
{
	m_EngineInstance = this;

	if (!m_hWnd)
		m_hWnd = hWnd;

	if (!RBINPUT.Initialize())
		return false;

	if (!RBRENDERER.Initialize(m_hWnd, width, height, true))
		return false;

	if (!m_Application->Initialize())
		return false;

	return true;
}

void RebornEngine::RBEngine::Shutdown()
{	
	//delete m_Application;
	RBRENDERER.Shutdown();

	if (m_UseEngineRenderWindow)
		DestroyRenderWindow();

	m_EngineInstance = nullptr;

}

void RebornEngine::RBEngine::Run()
{
	bool bRunLoop = true;
	m_Timer.Reset();

	// Main game loop
	while (bRunLoop)
	{
		MSG msg;

		RBINPUT.Update();

		// Handle win32 window messages
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bRunLoop = false;
				break;
			}
			else
			{
				// Translate the message and dispatch it to WndProc()
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		RunOneFrame();

		CalculateFrameStats();

		// Press ESC to quit loop
		if (RBINPUT.IsKeyDown(VK_ESCAPE))
			bRunLoop = false;
	}
}

void RebornEngine::RBEngine::RunOneFrame(bool update_input)
{
	if (update_input)
	{
		RBINPUT.Update();
	}

	m_Timer.Tick();

	m_Application->UpdateScene(m_Timer);
	m_Application->RenderScene();

}

void RebornEngine::RBEngine::ResizeClientWindow(int width, int height)
{
	RBRENDERER.ResizeClient(width, height);
	m_Application->OnResize(width, height);

}

RECT RebornEngine::RBEngine::GetWindowRectInfo() const
{
	RECT rect;
	GetWindowRect(m_hWnd, &rect);

	return rect;
}

bool RebornEngine::RBEngine::CreateRenderWindow(int width, int height, bool fullscreen, int bpp)
{
	m_hInst = GetModuleHandle(NULL);
	m_bFullScreen = fullscreen;

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Rhino Engine"),
			NULL);

		return false;
	}

	// Window style: WS_POPUP				- No caption, no maximize/minumize buttons
	//				 WS_OVERLAPPEDWINDOW	- Normal window
	DWORD dwStyle = m_bFullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;

	// Adjust window size according to window style.
	// This will make sure correct client area.
	RECT win_rect = { 0, 0, width, height };
	AdjustWindowRect(&win_rect, dwStyle, false);

	int pos_x, pos_y;

	if (m_bFullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = bpp;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		pos_x = pos_y = 0;
	}
	else
	{
		pos_x = (GetSystemMetrics(SM_CXSCREEN) - (win_rect.right - win_rect.left)) / 2;
		pos_y = (GetSystemMetrics(SM_CYSCREEN) - (win_rect.bottom - win_rect.top)) / 2;
	}

	// Create window and validate
	m_hWnd = CreateWindow(
		szWindowClass,
		m_Application->WindowTitle(),
		dwStyle,
		pos_x, pos_y,
		win_rect.right - win_rect.left,
		win_rect.bottom - win_rect.top,
		NULL,
		NULL,
		m_hInst,
		NULL);

	if (!m_hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Rhino Engine"),
			NULL);

		return false;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	m_UseEngineRenderWindow = true;
	return true;
}

void RebornEngine::RBEngine::DestroyRenderWindow()
{
	if (m_bFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hWnd);
	m_hWnd = NULL;
	UnregisterClass(szWindowClass, m_hInst);
	m_hInst = NULL;

}

void RebornEngine::RBEngine::CalculateFrameStats()
{	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((m_Timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << m_Application->WindowTitle()
			<< L" - " << RBRENDERER.GetAdapterName() << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(m_hWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
