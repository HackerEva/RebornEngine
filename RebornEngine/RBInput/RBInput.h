#ifndef RBINPUT_H
#define RBINPUT_H

#include "../RBSystem/RBSingleton.h"
#include <Windows.h>

namespace RebornEngine {
	enum RBInput_KeyState
	{
		KEY_IDLE = 0,
		KEY_DOWN,
		KEY_PRESS,
		KEY_UP,
		KEY_MAX
		//KEY_IDLE = 0,
		//KEY_DOWN,
		//KEY_REPEAT,
		//KEY_UP

	};

#define MAX_KEY_NUM 0xFF

	class RBInput : public RBSingleton<RBInput>
	{
		friend class RBSingleton<RBInput>;
		friend class RBEngine;
	public:
		bool Initialize();

		void LockCursor();

		void UnlockCursor();

		void ShowCursor();

		void HideCursor();

		void GetCursorPos(int& x, int& y);

		void GetCursorRelPos(int& dx, int& dy);

		RBInput_KeyState GetKeyState(int keycode);

		bool IsKeyDown(int keycode);

	protected:
		RBInput();
		~RBInput();

		void Update();

	private:
		RBInput_KeyState	m_KeyState[MAX_KEY_NUM];

		POINT	m_CursorPos;
		POINT	m_CursorPosLastFrame;

		bool	m_bCursorLocked;
		POINT	m_CursorLockingPos;
	};

}
#define RBINPUT RBInput::Instance()

#endif
