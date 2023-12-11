#pragma once
class Cube : public Transform // 부모클래스여서 간접접근하지 않아도 됨
{
    typedef VertexColorNormal VertexType;

public:
	Cube(Vector4 color);
	~Cube();

    void Update();
    void Render();

    void CreateMesh(Vector4 color);
    void CreateNormal();

    bool _pendulumMovement = false;

    // 6면 정보 보내기
#pragma region 6_sides

    vector<Vector3> GetTopVertices() 
    {
        vector<Vector3> a = { vertices[0].pos, vertices[4].pos, vertices[1].pos, vertices[5].pos };

        return a; 
    }
    vector<Vector3> GetBottomVertices()
    {
        vector<Vector3> a = { vertices[2].pos, vertices[3].pos, vertices[6].pos, vertices[7].pos };

        return a;
    }
    vector<Vector3> GetFrontVertices()
    {
        vector<Vector3> a = { vertices[0].pos, vertices[1].pos, vertices[2].pos, vertices[3].pos };

        return a;
    }
    vector<Vector3> GetBackVertices()
    {
        vector<Vector3> a = { vertices[5].pos, vertices[4].pos, vertices[7].pos, vertices[6].pos };

        return a;
    }
    vector<Vector3> GetRightVertices()
    {
        vector<Vector3> a = { vertices[1].pos, vertices[5].pos, vertices[3].pos, vertices[7].pos };

        return a;
    }
    vector<Vector3> GetLeftVertices()
    {
        vector<Vector3> a = { vertices[4].pos, vertices[0].pos, vertices[6].pos, vertices[2].pos };

        return a;
    }

#pragma endregion

private:
    Material* material;

    Mesh* mesh;

    WorldBuffer* worldBuffer;

    vector<VertexType> vertices;
    vector<UINT> indices;

    static int count;

};

