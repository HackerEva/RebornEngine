#include"RBEngine.h"
#include "RebornFighterApp.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	RBEngine Engine;
	RebornFighterApp app;

	Engine.BindApp(&app);

	if (Engine.Initialize())
	{
		Engine.Run();
		Engine.Shutdown();
	}
	else
	{
		MessageBox(0, L"Failed to initialize REngine.", 0, 0);
	}

	return 0;
}