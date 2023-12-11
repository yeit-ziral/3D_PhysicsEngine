#include "Framework.h"
#include "Time.h"

float Time::timeElapsed = 0.0;

Time::Time()
	: curTick(0), frameCount(0), frameRate(0), oneSecCount(0), runningTime(0), scanningRate(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerSecond);

	QueryPerformanceCounter((LARGE_INTEGER*)&lastTick);

	timeScale = 1.0 / ticksPerSecond;
}

Time::~Time()
{
}

void Time::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&curTick);

	timeElapsed = (curTick - lastTick) * timeScale;

	if (scanningRate != 0) // 수직동기화 작업
	{
		while (timeElapsed < (1.0 / scanningRate))
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&curTick);
			timeElapsed = (curTick - lastTick) * timeScale;
		}
	}

	lastTick = curTick;

	frameCount++;

	oneSecCount += timeElapsed;

	runningTime += timeElapsed;

	if (oneSecCount >= 1.0)
	{
		frameRate  = frameCount;
		frameCount = 0;

		oneSecCount = 0;
	}
}

void Time::Render()
{
	ImGui::Text("FPS : %d", frameRate);

	wstring str;
	str = L"RunTime : " + to_wstring((int)runningTime);
	FONT->RenderText(str, "D2Coding", Vector2(0, 100), Vector2(100,100));
}
