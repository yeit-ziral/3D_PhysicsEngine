#pragma once
class StateManager : public Singleton<StateManager>
{
	friend class Singletion;
public:
	StateManager();
	~StateManager();

	void CreateSamplerState();
	void CreateRasterizerState();
	void CreateBlendState();
	void CreateDepthStencilState();

public:
	RasterizerState* GetRS() { return rasterizerState; }

	void AlphaBegin();
	void AlphaEnd();
	void AlphaToCoverageEnable();

	void DepthEnable();
	void DepthDisable();
	void DepthWriteMaskZero();

	void Set();
	void PostSet();

private:
	SamplerState* samplerState = nullptr;
	RasterizerState* rasterizerState = nullptr;

	vector<BlendState*> blendStates;

	vector<DepthStencilState*> depthStencilStates;
};