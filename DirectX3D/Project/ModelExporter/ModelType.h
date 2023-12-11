#pragma once

typedef VertexTextureNormalTangentBlend ModelVertex;

struct MeshData
{
	string name;

	UINT materialIndex;

	vector<ModelVertex> vertices;
	vector<UINT>		 indices;
};

struct NodeData
{
	int index;

	string name;

	int parent;

	Matrix tranform;
};

struct BoneData
{
	int index;

	string name;

	Matrix offset;
};

struct VertexWeights // 정점마다 들어가는 가중치 -> 애니메이션이 더 자연스러워 보이도록
{
	 UINT indices[4];
	float weights[4];

	void Add(const UINT& index, const float& weight)
	{
		for (UINT i = 0; i < 4; i++)
		{
			if (weights[i] == 0.0f)
			{
				indices[i] = index;
				weights[i] = weight;
				return;
			}
		}
	}

	void Normalize()
	{
		float sum = 0.0f;

		for  (UINT i = 0; i < 4;  i++)
		{
			sum += weights[i];
		}
		for (UINT i = 0; i < 4; i++)
		{
			weights[i] /= sum;
		}
	}
};

struct KeyTransform
{
	float time;

	Vector3 scale;
	Vector4 rotation; // 쿼터니언 각 사용해서 vector4 사용(<-> 오일러각) : global rotation을 표현하기 더 적합하기 때문
	Vector3 position;
};

struct KeyFrame
{
	string boneName;

	vector<KeyTransform> transforms;
};

struct ClipNode
{
	vector<KeyTransform> keyFrame;
	aiString name;
};

struct Clip
{
	string name;

	UINT frameCount;

	float ticksPerSecond;

	float duration;

	vector<KeyFrame*> keyFrame;
};

struct ClipTransform
{
	Matrix transform[MAX_FRAME_KEY][MAX_BONE];
};