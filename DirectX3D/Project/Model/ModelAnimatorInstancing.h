#pragma once
class ModelAnimatorInstancing : public ModelAnimator
{
public:
	ModelAnimatorInstancing(string name);
	~ModelAnimatorInstancing();

	virtual void Update();
	virtual void Render();
	virtual void Debug();

	Transform* Add();

	void PlayClip(UINT instanceIndex, int clip, float speed = 1.0f, float takeTime = 0.1f);

	Matrix GetTransformByNode(UINT instanceIndex, int nodeIndex);

private:
	void UpdateFrame(UINT instanceIndex);
	void UpdateTransforms();

private:

	vector<Transform*> transforms;

	vector<InstanceData> instanceDatas;

	VertexBuffer* instanceBuffer;

	UINT drawCount = 0;

	string name;

	FrameInstancingBuffer* frameInstancingBuffer;
};