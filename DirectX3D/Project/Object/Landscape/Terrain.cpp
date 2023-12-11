#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(wstring diffuseFile, wstring heightFile) // Specular 적용시키기
{
	material = new Material();
	material->SetShader(L"09Light");
	material->SetDiffuseMap(diffuseFile);

	heightMap = Texture::Get(heightFile);

	CreateMesh();

	CreateNormal();

	CreateTangent();

	mesh = new Mesh(vertices, indices);
}

Terrain::~Terrain()
{
	delete mesh;
	delete material;
}

void Terrain::Render()
{
	Transform::SetWorld(0);

	mesh->SetMesh();
	material->SetMaterial();

	DC->DrawIndexed(indices.size(), 0, 0);
}

bool Terrain::Picking(OUT Vector3* position)
{
	Ray ray = CAMERA->ScreenPointToRay(mousePos);

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = (x + 0) + (width * (z + 0));
			index[1] = (x + 1) + (width * (z + 0));
			index[2] = (x + 0) + (width * (z + 1));
			index[3] = (x + 1) + (width * (z + 1));

			Vector3 pos[4];
			for (UINT i = 0; i < 4; i++)
			{
				pos[i] = vertices[index[i]].pos;
			}

			float distance = 0.0f;

			// ray의 origin부터 poligon까지의 거리를 반환하는 함수
			if (TriangleTests::Intersects(ray.origin, ray.direction, pos[0], pos[1], pos[2], distance)) // distance에 거리가 기록 됨 // 이 함수도  OUT형식임(연산이 느림)
			{
				*position = ray.origin + ray.direction * distance;

				return true;
			}
			if (TriangleTests::Intersects(ray.origin, ray.direction, pos[2], pos[1], pos[3], distance))
			{
				*position = ray.origin + ray.direction * distance;

				return true;
			}
		}
	}

	return false;
}

bool Terrain::OnTheGround(Vector3* position)
{
	float dis = 1000.0f;

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = (x + 0) + (width * (z + 0));
			index[1] = (x + 1) + (width * (z + 0));
			index[2] = (x + 0) + (width * (z + 1));
			index[3] = (x + 1) + (width * (z + 1));

			Vector3 pos[4];
			for (UINT i = 0; i < 4; i++)
			{
				pos[i] = vertices[index[i]].pos;
			}

			float disAll = 0;

			for (int i = 0; i < 3; i++)
			{
				Vector3 temp = { abs(position->x - pos[i].x), 0, abs(position->z - pos[i].z) };
				disAll += temp.Length();
			}

			float disAll2 = 0;

			for (int i = 1; i < 4; i++)
			{
				Vector3 temp = { abs(position->x - pos[i].x), 0, abs(position->z - pos[i].z) };
				disAll2 += temp.Length();
			}

			if (disAll <= disAll2)
			{
				if (disAll < dis)
				{
					dis = disAll;

					float temp = 0;
					for (int i = 0; i < 3; i++)
					{
						temp += pos[i].y;
					}

					position->y = temp / 3;
				}
			}
			else
			{
				if (disAll2 < dis)
				{
					dis = disAll;

					float temp = 0;
					for (int i = 1; i < 4; i++)
					{
						temp += pos[i].y;
					}

					position->y = temp / 3;
				}
			}
		}
	}


	return false;
}

float Terrain::GetHeight(Vector3 position)
{
	position.x /= scale.x;
	position.z /= scale.z;

	int x = (int)position.x;
	int z = (int)position.z;

	if (x < 0 || x > width - 1)
		return 0.0f;

	if (z < 0 || z > height - 1)
		return 0.0f;

	UINT index[4];
	index[0] = x + 0 + width * (z + 1);
	index[1] = x + 1 + width * (z + 1);
	index[2] = x + 0 + width * (z + 0);
	index[3] = x + 1 + width * (z + 0);

	Vector3 vertex[4];

	for (UINT i = 0; i < 4; i++)
	{
		vertex[i] = vertices[index[i]].pos;
	}

	float u = abs(position.x - vertex[0].x);
	float v = abs(position.z - vertex[0].z);

	Vector3 result;

	if (u + v <= 1.0f)
	{
		result = vertex[0] + (vertex[2] - vertex[0]) * u + (vertex[1] - vertex[0]) * v;
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		result = vertex[3] + (vertex[2] - vertex[3]) * u + (vertex[1] - vertex[3]) * v;
	}

	return result.y * scale.y;
}

void Terrain::CreateMesh()
{
	width = heightMap->GetSize().x;
	height = heightMap->GetSize().y;

	vector<Vector4>colors = heightMap->ReadPixels();

	//height map을 찍기 위해 나눔, 실제 이미지와는 상관 없이 임의적으로 나눈것임
	// Vertices
	for (float z = 0; z < height; z++)
	{
		for (float x = 0; x < width; x++)
		{
			VertexType vertex;
			vertex.pos = Vector3(x, 0, z);

			vertex.uv.x = x / (width - 1);
			vertex.uv.y = 1 - z / (height - 1);

			//HeightMap
			UINT index = x + z * width;
			vertex.pos.y = colors[index].x * MAP_HEIGHT;

			vertex.normal = vertex.pos / vertex.pos.y;

			vertices.push_back(vertex); // normal값 넣어주기
		}
	}

	//Indices
	for (float z = 0; z < height - 1; z++)
	{
		for (float x = 0; x < width - 1; x++)
		{
			indices.push_back(x + 0 + width * (z + 1)); // 7
			indices.push_back(x + 1 + width * (z + 1)); // 8
			indices.push_back(x + 0 + width * (z + 0)); // 0

			indices.push_back(x + 0 + width * (z + 0)); // 0
			indices.push_back(x + 1 + width * (z + 1)); // 8
			indices.push_back(x + 1 + width * (z + 0)); // 1
		}
	}

}

void Terrain::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];
		
		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Vector3 v01 = p1 - p0;
		Vector3 v02 = p2 - p0;

		Vector3 normal = Vector3::Cross(v01, v02).GetNormalized();

		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}
}

void Terrain::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 p0 = vertices[index0].pos;
		Vector3 p1 = vertices[index1].pos;
		Vector3 p2 = vertices[index2].pos;

		Vector2 uv0 = vertices[index0].uv;
		Vector2 uv1 = vertices[index1].uv;
		Vector2 uv2 = vertices[index2].uv;

		Vector3 e01 = p1 - p0;
		Vector3 e02 = p2 - p0;

		float u1 = uv1.x - uv0.x;
		float v1 = uv1.y - uv0.y;

		float u2 = uv2.x - uv0.x;
		float v2 = uv2.y - uv0.y;

		float D = 1.0f / (u1 * v2 - v1 * u2); // Determiant (판별식) : 여기서는 값이 몇개인지 확인함

		Vector3 tangent = D * (e01 * v2 - e02 * v1);

		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}

	for (VertexType& vertex : vertices)
	{
		Vector3 T = vertex.tangent;
		Vector3 N = vertex.normal;

		vertex.tangent = (T - N * Vector3::Dot(N, T)).GetNormalized();
	}
}
