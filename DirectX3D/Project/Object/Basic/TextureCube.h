#pragma once
class TextureCube : public Transform
{
	enum Side
	{
		Left,
		Right,
		Bottom,
		Top,
		Front,
		Back
	};

public:
	TextureCube();
	~TextureCube();

	void Update();

	void Render();

	void CreateQuads();

private:
	vector<Quad*> quads;
};