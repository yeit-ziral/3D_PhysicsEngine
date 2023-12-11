#pragma once
class Shader
{
protected:
	virtual ~Shader();

public:
	static class VertexShader* GetVS(wstring file);
	static class PixelShader* GetPS(wstring file);
	static class ComputeShader* GetCS(wstring file);

	static void Delete();

	virtual void SetShader() = 0;

	wstring GetPath() { return path; }

protected:
	ID3DBlob* blob;

	static map<wstring, Shader*> shaders; // 자기자신의 포인터를 가지는 변수는 static으로 만들어서 무한루프를 막음(동적할당 할 경우 만들어진 주소값을 담은 변수의 주소값을 또 만들고 이것이 반복됨)

	wstring path;
};
