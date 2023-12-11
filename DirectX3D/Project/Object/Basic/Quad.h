#pragma once
class Quad : public Transform
{
	typedef VertexTextureNormalTangent VertexType;
public:
	Quad(Vector2 size = {1,1});
	Quad(vector<Vector3> points);
	Quad(wstring file);
	~Quad();

	void Render();
	void RenderInstanced(UINT instacnceCount);

	Material* GetMaterial() { return material; }

protected:
	Material* material = nullptr;
	Mesh*	  mesh	   = nullptr;

	vector<VertexType>	vertices;
	vector<UINT>		indices;
};