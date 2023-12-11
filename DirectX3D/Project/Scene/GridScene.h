#pragma once
class GridScene : public Scene
{
public:
	GridScene();
	~GridScene();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

private:
	void CreateMesh();

private:
	int width = 100, height = 100;

	Material* material;

	VertexBuffer* vertexBuffer;

	vector<VertexColor> vertices;

	WorldBuffer* worldBuffer;
};