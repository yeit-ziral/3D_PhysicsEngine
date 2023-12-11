#pragma once

class MatrixBuffer : public ConstBuffer
{
public:
	MatrixBuffer()
		: ConstBuffer(&data, sizeof(data)) // 부모의 생서자 호출해서 데이터 생성을 같이 해줌
	{
		data.matrix = XMMatrixIdentity();
	}

	void SetData(XMMATRIX value)
	{
		data.matrix = XMMatrixTranspose(value);
	}

private:
	struct Data
	{
		XMMATRIX matrix;
	} data;
};

class LightBuffer : public ConstBuffer
{

public:
	struct LightData
	{
		Vector4 color = Vector4(1, 1, 1, 1);

		Vector3 direction = Vector3(0, -1, 1);
		int		type = 0;

		// Point Light
		Vector3 position = Vector3();
		float	range = 100.0f;

		// Spot Light
		float inner = 55.0f;
		float outer = 70.0f;

		//Capsule Light
		float length = 50.0f;
		int	  active = true;
	};

public:
	LightBuffer()
		: ConstBuffer(&data, sizeof(data)) // 부모의 생서자 호출해서 데이터 생성을 같이 해줌
	{

	}

	struct Data
	{
		LightData lights[MAX_LIGHT];

		int lightCount = 1;
		Vector3 ambientLight = Vector3(0.1f, 0.1f, 0.1f);
		Vector3 ambientCeil = Vector3(0.1f, 0.1f, 0.1f);
		float padding;
	} data;
};

class WorldBuffer : public ConstBuffer
{
public:
	WorldBuffer()
		: ConstBuffer(&data, sizeof(data)) // 부모의 생서자 호출해서 데이터 생성을 같이 해줌
	{
		data.view = XMMatrixIdentity();
		data.hasAnimation = 0;
	}

	void SetData(Matrix view, int hasAnimation = 0)
	{
		data.view = XMMatrixTranspose(view);
		data.hasAnimation = hasAnimation;
	}

private:
	struct Data
	{
		Matrix view;
		int hasAnimation;
		Vector3 padding;
	} data;
};

class ViewBuffer : public ConstBuffer
{
public:
	ViewBuffer()
		: ConstBuffer(&data, sizeof(data)) // 부모의 생서자 호출해서 데이터 생성을 같이 해줌
	{
		data.view = XMMatrixIdentity();
		data.inverseView = XMMatrixIdentity();
	}

	void SetData(Matrix view, Matrix inverseView)
	{
		data.view = XMMatrixTranspose(view);
		data.inverseView	= XMMatrixTranspose(inverseView);
	}

private:
	struct Data
	{
		Matrix view;
		Matrix inverseView;
	} data;
};

class MaterialBuffer : public ConstBuffer
{
public:
	MaterialBuffer()
		: ConstBuffer(&data, sizeof(Data))
	{

	}

	struct Data
	{
		Vector4 diffuse  = Vector4(1, 1, 1, 1);
		Vector4 specular = Vector4(1, 1, 1, 1);
		Vector4 ambient  = Vector4(1, 1, 1, 1);
		Vector4 emissive = Vector4(0, 0, 0, 0);

		int hasDiffuseMap  = 0;
		int hasSpecularMap = 0;
		int hasNormalMap   = 0;
		int hasAlphaMap	   = 0;

		float shininess = 24.0f;

		Vector3 padding = {};
	} data;
};

class RayBuffer : public ConstBuffer
{
public:
	RayBuffer()
		: ConstBuffer(&data, sizeof(data)) 
	{
		
	}

	struct Data
	{
		Vector3 origin		= {};
		float	outputSize	= 0;

		Vector3 direction	= {};
		float	padding		= 0;
	} data;
};

class BrushBuffer : public ConstBuffer
{
public:
	BrushBuffer()
		: ConstBuffer(&data, sizeof(data))
	{

	}

	struct Data
	{
		//int	type	= 1;
		int	type	= 0;
		Vector3 location = {};

		float	range = 10;
		Vector3 color = {0.0f, 0.5f, 0.0f};
	} data;
};

class FrameBuffer : public ConstBuffer
{
public:
	FrameBuffer()
		: ConstBuffer(&data, sizeof(data)) 
	{
		data.next.clip = -1; // 다음 동작이 없다고 생각
	}

	struct Frame
	{
		int clip		= 0;
		UINT curFrame	= 0;
		float time		= 0.0f;
		float speed		= 1.0f;
	};

	struct Data
	{
		float takeTime		= 0.0f;
		float tweenTime		= 0.0f;
		float runningTime	= 0.0f;
		float padding		= 0.0f;

		Frame cur, next;
	} data;
};

class FrameInstancingBuffer : public ConstBuffer
{
public:
	FrameInstancingBuffer()
		: ConstBuffer(&data, sizeof(data))
	{
	}

	struct Frame
	{
		int clip = 0;
		UINT curFrame = 0;
		float time = 0.0f;
		float speed = 1.0f;
	};

	struct Motion
	{
		Motion()
		{
			next.clip = -1;// 다음 동작이 없다고 생각
		}

		float takeTime = 0.0f;
		float tweenTime = 0.0f;
		float runningTime = 0.0f;
		float padding = 0.0f;

		Frame cur, next;
	};

	struct Data
	{
		Motion motion[MAX_INSTANCE];
	} data;
};

class FloatValueBuffer : public ConstBuffer
{
public:
	FloatValueBuffer()
		: ConstBuffer(&data, sizeof(data))
	{

	}

	struct Data
	{
		float values[4] = {};
	} data;
};

class IntValueBuffer : public ConstBuffer
{
public:
	IntValueBuffer()
		: ConstBuffer(&data, sizeof(data))
	{

	}

	struct Data
	{
		int values[4] = {};
	} data;
};

class SkyBuffer : public ConstBuffer
{
public:
	SkyBuffer()
		: ConstBuffer(&data, sizeof(data))
	{

	}

	struct Data
	{
		Vector3 centerColor = { 1.0f, 0.6f, 0.2f };
		float	height		= 5.0f;

		Vector3 apexColor	= { 0.3f, 0.5f, 1.0f };
		float	padding;
	} data;
};
