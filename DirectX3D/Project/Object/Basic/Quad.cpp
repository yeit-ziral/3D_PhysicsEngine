#include "Framework.h"
#include "Quad.h"

Quad::Quad(Vector2 size)
{
	float L = -size.x * 0.5f;
	float R = +size.x * 0.5f;
	float B = -size.y * 0.5f;
	float T = +size.y * 0.5f;

	vertices =
	{
		{Vector3(L, T, 0.0f), Vector2(0,0), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)},
		{Vector3(R, T, 0.0f), Vector2(1,0), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)},
		{Vector3(L, B, 0.0f), Vector2(0,1), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)},
		{Vector3(R, B, 0.0f), Vector2(1,1), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)}
	};

	indices =
	{
		0, 1, 2,
		2, 1, 3
	};

	mesh = new Mesh(vertices, indices);

	material = new Material();
	//material->SetShader(L"Diffuse");
	material->SetShader(L"09Light");
	//material->SetDiffuseMap(L"Landscape/Box.png");


}

Quad::Quad(vector<Vector3> points)
{
	vertices =
	{
		{Vector3(points[0].x, points[0].y, points[0].z), Vector2(0,0), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)},
		{Vector3(points[1].x, points[1].y, points[1].z), Vector2(1,0), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)},
		{Vector3(points[2].x, points[2].y, points[2].z), Vector2(0,1), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)},
		{Vector3(points[3].x, points[3].y, points[3].z), Vector2(1,1), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)}
	};

	indices =
	{
		0, 1, 2,
		2, 1, 3
	};

	mesh = new Mesh(vertices, indices);

	material = new Material();
	material->SetShader(L"09Light");
	material->SetDiffuseMap(L"Landscape/Box.png");

}

Quad::Quad(wstring file)
{
	material = new Material();
	material->SetShader(L"09Light");
	material->SetDiffuseMap(file);

	Vector2 size = material->GetDiffuseMap()->GetSize();

	float L = -size.x * 0.5f;
	float R = +size.x * 0.5f;
	float B = -size.y * 0.5f;
	float T = +size.y * 0.5f;

	vertices =
	{
		{Vector3(L, T, 0.0f), Vector2(0,0), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)},
		{Vector3(R, T, 0.0f), Vector2(1,0), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)},
		{Vector3(L, B, 0.0f), Vector2(0,1), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)},
		{Vector3(R, B, 0.0f), Vector2(1,1), Vector3(0, 0, -1), Vector3(1.0f, 0.0f, 0.0f)}
	};

	indices =
	{
		0, 1, 2,
		2, 1, 3
	};

	mesh = new Mesh(vertices, indices);
}

Quad::~Quad()
{
	delete mesh;

}

void Quad::Render()
{
	Transform::SetWorld();

	material->SetMaterial();
	mesh->SetMesh();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Quad::RenderInstanced(UINT instacnceCount)
{

	material->SetMaterial();
	mesh->SetMesh();



	DC->DrawIndexedInstanced(indices.size(), instacnceCount, 0, 0, 0);
}
