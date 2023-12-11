#pragma once
class PixelShader : public Shader
{
	friend class Shader;
private:
	PixelShader(wstring file);
	~PixelShader();

	ID3D11PixelShader* pixelShader;

public:
	// Shader을(를) 통해 상속됨
	virtual void SetShader() override;
};

