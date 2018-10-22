#ifndef RBIAPPLICATION_H
#define RBIAPPLICATION_H
#include"RBTimer.h"
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include<iostream>  
#include <fstream>  
#include <locale>  


namespace RebornEngine
{
	class RBIApplication
	{
	public:
		virtual ~RBIApplication() {}

		virtual bool Initialize() = 0;
		virtual void UpdateScene(const RBTimer& timer) = 0;
		virtual void RenderScene() = 0;

		virtual void OnResize(int width, int height) {}
		virtual const TCHAR* WindowTitle() { return L"Reborn Engine"; }
	};
}

#endif
