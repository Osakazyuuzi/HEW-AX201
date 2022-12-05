#ifndef ___TIMER_H___
#define ___TIMER_H___

#include <Windows.h>
#pragma comment(lib, "winmm.lib")

//--- エイリアス定義
using Frame = unsigned short;
using Time = float;

class Timer {
public:

	static void Init(Frame fps);
	static void Uninit();
	static bool Update();

	//--- セット関数
	static void SetFPS(Frame fps) { m_fps = fps; }

	//--- ゲット関数
	static Frame GetFPS() { return m_fps; }
	static Time GetDeltaTime() { return m_deltaTime; }

	// 変換関数
	static Time ConvertFrameToTime(Frame flame);
	static Frame ConvertTimeToFrame(Time time);

private:
	static Frame m_fps;
	static Time  m_deltaTime;
	static DWORD m_preExec;
};
#endif //!___TIMER_H___