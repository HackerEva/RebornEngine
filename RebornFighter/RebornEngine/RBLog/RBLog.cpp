#include "RBLog.h"
#include <Windows.h>
using namespace RebornEngine;
bool RebornEngine::RBLog::Initialize()
{
	//=============== Initialize Console =============================
	AllocConsole();
	
	
	FILE* new_std_in_out;
	freopen_s(&new_std_in_out, "CONOUT$", "w", stdout);
	freopen_s(&new_std_in_out, "CONIN$", "r", stdin);
	std::cout << "\t\t Direction Key WSAD \n\n";
	std::cout << "\t\t Lift Mouse Button to Use Mouse Look \n\n";
	//==============================================================
	return false;
}

void RebornEngine::RBLog::print(string info)
{
	cout << info;
}

void RebornEngine::RBLog::printLine(string info)
{
	cout << info <<endl;
}

RebornEngine::RBLog::RBLog()
{
}

RebornEngine::RBLog::~RBLog()
{
}
