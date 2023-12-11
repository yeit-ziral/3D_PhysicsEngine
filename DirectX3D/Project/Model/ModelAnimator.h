#pragma once
class ModelAnimator : public Transform
{
public:
	ModelAnimator(string name, wstring shaderFile = L"02ModelAnimation");
	~ModelAnimator();

	void Update();
	void Render();

	void RenderInstanced(UINT instanceCount);

	void ReadClip(string file, UINT clipIndex = 0); // 하나짜리가 많아서 clipIndex 기본값을 0으로 함

	void PlayClip(UINT clipIndex, float speed = 1.0f, float takeTime = 0.2f);

	void StopClip() { isPlay = false; }

	void CreateTexture();

	void UpdateFrame();

	void Debug();

	Matrix GetTransformByBone(UINT boneIndex);

	Matrix GetTransformByNode(UINT nodeIndex);

	void SetEndEvent(function<void()> EndEvent, float ratio);

	ModelReader* GetReader() { return reader; }

	vector<ModelClip*> GetClip() { return clips; }

private:
	void CreateClipTransform(UINT index);

protected:
	ModelReader* reader;

	vector<ModelClip*> clips;

	string name;
	
	FrameBuffer* frameBuffer;

	ClipTransform* clipTransform;
	ClipTransform* nodeTransform; // 노드의 데이터를 임시 저장하기 위한 곳

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	bool isPlay = true;

	function<void()> EndEvent;

	float animRatio;
};