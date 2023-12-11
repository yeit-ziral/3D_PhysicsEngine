#pragma once

class Scene
{
public:
	virtual ~Scene() = default; // 업캐스팅 했을 때 잘 소멸시키기 위해서 가상함수로 만듬

	virtual void Update() = 0; // 프레임 단위로 정보를 갱신, = 0을 abstract 대신 사용해서 순수 가상함수로 만듬

	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

	virtual void Initialize() {};
	virtual void Release() {};
};