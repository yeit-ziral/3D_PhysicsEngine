#include "Framework.h"
#include "Circle.h"

Circle::Circle(Vector4 color, float radius)
{
    material = new Material(L"Diffuse");

    CreateMesh(color, radius);
    CreateNormal();

    //WVP
    mesh = new Mesh(vertices, indices);
}

Circle::~Circle()
{
    delete mesh;
    delete material;
}

void Circle::Update()
{
    Transform::Update();
}

void Circle::Render()
{
    material->SetMaterial();
    mesh->SetMesh(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    Transform::SetWorld();

    DC->DrawIndexed(indices.size(), 0, 0);
}

void Circle::CreateMesh(Vector4 color, float radius)
{
    VertexColorNormal* temp = new VertexColorNormal();
    for (int i = 0; i < 42; i++)
    {
        temp->pos = XMFLOAT3(radius * cos(Radian(i * 9)), radius * sin(Radian(i * 9)), 0.0f);
        temp->color = color;
        vertices.push_back(*temp);
    }

    for (int i = 0; i < 41; i++)
    {
        indices.push_back(i);
    }
}

void Circle::CreateNormal()
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
