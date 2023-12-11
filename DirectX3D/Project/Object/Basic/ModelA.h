#pragma once
class ModelA : public Transform
{
public:
	ModelA(string name);
	~ModelA();

	void Update();
	void Render();

private:
	ModelReader* reader;

	MatrixBuffer* worldBuffer;
};