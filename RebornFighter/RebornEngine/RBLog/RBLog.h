#ifndef RBLOG_H
#define RBLOG_H
#include <iostream>
#include "../RBSystem/RBSingleton.h"
#include <string>
using namespace std;

namespace RebornEngine 
{
	class RBLog : public RBSingleton<RBLog>
	{
		friend class RBSingleton<RBLog>;
	public:
		bool Initialize();
		void print(string info);
		void printLine(string info);
	protected:
		RBLog();
		~RBLog();
	};
#define RBLOG RBLog::Instance()
}

#endif