#include "RBInput.h"
using namespace RebornEngine;
bool RBInput::Initialize()
{
	for (int i = 0; i < MAX_KEY_NUM; i++)
	{
		m_KeyState[i] = RBInput_KeyState::KEY_IDLE;
	}

	m_bCursorLocked = false;

	::GetPhysicalCursorPos(&m_CursorPosLastFrame);
	::GetPhysicalCursorPos(&m_CursorPos);

	return true;
}

void RBInput::LockCursor()
{
	if (!m_bCursorLocked)
	{
		::GetPhysicalCursorPos(&m_CursorLockingPos);

		m_bCursorLocked = true;
	}
}

void RBInput::UnlockCursor()
{
	if (m_bCursorLocked)
	{
		m_bCursorLocked = false;;
	}
}

void RBInput::ShowCursor()
{
	::ShowCursor(TRUE);
}

void RBInput::HideCursor()
{
	::ShowCursor(FALSE);
}

void RBInput::GetCursorPos(int & x, int & y)
{
	x = m_CursorPos.x;
	y = m_CursorPos.y;
}

void RBInput::GetCursorRelPos(int & dx, int & dy)
{
	dx = m_CursorPos.x - m_CursorPosLastFrame.x;
	dy = m_CursorPos.y - m_CursorPosLastFrame.y;
}

RBInput_KeyState RBInput::GetKeyState(int keycode)
{
	return m_KeyState[keycode];
}

bool RBInput::IsKeyDown(int keycode)
{
	return m_KeyState[keycode] == RBInput_KeyState::KEY_PRESS;
}


RBInput::RBInput()
{
}

RBInput::~RBInput()
{
}

void RBInput::Update()
{
	// Update system key states
	for (int i = VK_LBUTTON; i <= VK_RMENU; i++)
	{
		if ((GetKeyState(i) & 0x80) != 0)
		{
			if (!m_KeyState[i])
			{
				m_KeyState[i] = RBInput_KeyState::KEY_DOWN;
			}
			else if(m_KeyState[i] == RBInput_KeyState::KEY_DOWN)
			{
				m_KeyState[i] = RBInput_KeyState::KEY_PRESS;
			}
		}
		else if(m_KeyState[i] == RBInput_KeyState::KEY_PRESS ||
				m_KeyState[i] == RBInput_KeyState::KEY_DOWN)
		{
			m_KeyState[i] = RBInput_KeyState::KEY_UP;
		}
		else
		{
			m_KeyState[i] = RBInput_KeyState::KEY_IDLE;
		}
	}

	m_CursorPosLastFrame = m_CursorPos;
	::GetPhysicalCursorPos(&m_CursorPos);

	if (m_bCursorLocked)
	{
		::SetPhysicalCursorPos(m_CursorLockingPos.x, m_CursorLockingPos.y);
		m_CursorPosLastFrame = m_CursorLockingPos;
	}
}
