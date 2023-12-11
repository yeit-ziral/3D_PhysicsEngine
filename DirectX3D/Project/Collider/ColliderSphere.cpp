#include "Framework.h"
#include "ColliderSphere.h"

ColliderSphere::ColliderSphere(float radius, UINT stackCount, UINT sliceCount)
	: radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
    type = SPHERE;
    CreateMesh();
}

ColliderSphere::~ColliderSphere()
{
}

bool ColliderSphere::Collision(IN Ray& ray, OUT Contact* contact)
{
    Transform::UpdateWorld();

    Vector3 O = ray.origin;
    Vector3 D = ray.direction;

    Vector3 P = this->globalPosition;
    Vector3 X = O - P;

    // »ï°¢ÇÔ¼ö ÀÌ¿ëÇÑ Á¢ÃËÆÇ´Ü
    //float theta = acos(Vector3::Dot(D, X) / (D.Length() * X.Length()));

    //if (X.Length() * sin(theta) <= Radius())
    //{
    //    return true;
    //}

    // °­»ç´Ô Á¢ÃËÆÇ´Ü
    float a = Vector3::Dot(D, D);
    float b = 2 * Vector3::Dot(D, X);
    float c = Vector3::Dot(X, X) - Radius() * Radius();

    if (b * b - 4 * a * c >= 0)
    {
        if (contact != nullptr)
        {
            float t = (-b - sqrt(b * b - 4 * a * c))/ (2 * a);

            contact->distance = t;
            contact->hitPoint = O + D * t;
        }

        return true;
    }

	return false;
}

bool ColliderSphere::Collision(ColliderBox* other)
{
    return other->Collision(this);
}

bool ColliderSphere::Collision(ColliderSphere* other)
{
    float distance = Distance(this->globalPosition, other->globalPosition);

	return distance <= this->Radius() + other->Radius();
}

bool ColliderSphere::Collision(ColliderCapsule* other)
{
	return false;
}

bool ColliderSphere::Block(ColliderBox* other)
{
    if (!Collision(other))
        return false;

    Vector3 virtualHalfSize = other->GetGlobalScale() * 0.5f;
    Vector3 dir = other->GetGlobalPosition() - this->GetGlobalPosition();
    Vector3 sum = virtualHalfSize + GetGlobalScale() * 0.5f;
    Vector3 overlap = Vector3(sum.x - abs(dir.x), sum.y - abs(dir.y), sum.z - abs(dir.z));

    Vector3 fixedPos = other->GetGlobalPosition();

    dir.Normalize();
    if (overlap.y > overlap.x && overlap.z > overlap.x)
    {
        if (dir.x < 0.0f)
            dir.x = -1.0f;
        else if (dir.x > 0.0f)
            dir.x = 1.0f;

        dir.y = 0;
        dir.z = 0;

        fixedPos.x += dir.x * overlap.x;
    }
    else if (overlap.x > overlap.y && overlap.z > overlap.y)
    {
        if (dir.y < 0.0f)
            dir.y = -1.0f;
        else if (dir.y > 0.0f)
            dir.y = 1.0f;

        dir.x = 0;
        dir.z = 0;

        fixedPos.y += dir.y * overlap.y;
    }
    else if (overlap.x > overlap.z && overlap.y > overlap.z)
    {
        if (dir.z < 0.0f)
            dir.z = -1.0f;
        else if (dir.z > 0.0f)
            dir.z = 1.0f;

        dir.x = 0;
        dir.y = 0;

        fixedPos.z += dir.z * overlap.z;
    }

    //other->GetParent()->translation = dir * Time::Delta();
    other->GetParent()->translation += dir * 0.01f;

    return true;
}

bool ColliderSphere::Block(ColliderSphere* other)
{
    if (!Collision(other))
        return false;

    Vector3 moveableCenter = other->GetGlobalPosition();
    Vector3 blockCenter = this->GetGlobalPosition();
    Vector3 dir = moveableCenter - blockCenter;
    float scalar = abs((other->GetGlobalScale().x + GetGlobalScale().x) - dir.Length());
    dir.Normalize();

    other->GetParent()->translation += dir * 0.01f;
    //other->translation += (dir * scalar * Time::Delta());

}

void ColliderSphere::CreateMesh()
{
    for (int i = 0; i < stackCount + 1; i++)
    {
        float theta = i * XM_PI / stackCount;
        for (int j = 0; j < sliceCount + 1; j++)
        {
            float phi = j * XM_2PI / sliceCount;

            VertexType vertex;

            vertex.pos.x = radius * sin(theta) * cos(phi);
            vertex.pos.y = radius * cos(theta);
            vertex.pos.z = radius * sin(theta) * sin(phi);

            vertices.push_back(vertex);
        }
    }

    for (int j = 0; j < stackCount; j++)
    {
        for (int i = 0; i < sliceCount; i++)
        {
            indices.push_back(i + 0 + (j + 0) * (sliceCount + 1));
            indices.push_back(i + 1 + (j + 0) * (sliceCount + 1));

            indices.push_back(i + 0 + (j + 0) * (sliceCount + 1));
            indices.push_back(i + 0 + (j + 1) * (sliceCount + 1));
        }
    }

    mesh = new Mesh(vertices, indices);
}
