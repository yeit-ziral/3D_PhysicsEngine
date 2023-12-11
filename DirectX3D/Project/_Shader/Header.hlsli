cbuffer World : register(b0)
{
    matrix world;
    int hasAnimation;
};

cbuffer View : register(b1)
{
    matrix view;
    matrix invView;
};

cbuffer Proj : register(b2)
{
    matrix proj;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    float4 mEmissive;
    
    int hasDiffuseMap;
    int hasSpecularMap;
    int hasNormalMap;
    int hasAlphaMap;
    
    float shininess;
    // 받아오는곳은 패딩 필요 없음
};

struct Frame
{
    int clip;
    uint curFrame;
    float time;
    float speed;
};

struct Motion
{
    float takeTime;
    float tweenTime;
    float runningTime;
    float padding2;
    
    Frame cur, next;
};

cbuffer FrameBuffer : register(b3)
{
    Motion motion;
}

#define MAX_INSTANCE 128

cbuffer FrameInstancingBuffer : register(b4) // 
{
    Motion motions[MAX_INSTANCE];
}

Texture2DArray transformMap : register(t0);

//struct VertexInput
//{
//    float4 pos : POSITION; // 동차좌표계 쓰려고 float3가 아닌 float4로 한 차수 올림
//    float4 color : COLOR;
//};

struct VertexColor
{
    float4 pos : POSITIONT;
    float4 color : COLOR;
};

struct VertexTexture
{
    float4 pos : POSITION;
    float2 uv : UV;
};


struct VertexTextureNormal
{
    float4 pos : POSITION; // 동차좌표계 쓰려고 float3가 아닌 float4로 한 차수 올림
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct VertexColorNormal
{
    float4 pos : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

struct VertexTextureNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexTextureNormalTangentAlpha
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 alpha : ALPHA;
};

struct VertexTextureNormalTangentBlend
{
    float4 pos      : POSITION;
    float2 uv       : UV;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float4 indices  : BLENDINDICES; // 정점에 영향 받을 bone의 index들
    float4 weight   : BLENDWEIGHTS; // 영향 받을 정도
};

struct VertexInstancing
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES; // 정점에 영향 받을 bone의 index들
    float4 weight : BLENDWEIGHTS; // 영향 받을 정도
    
    matrix transform : INSTANCE_TRANSFORM;
    uint index       : INSTANCE_INDEX;
};

Texture2D  diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);


SamplerState samp : register(s0); // Desc 같은 것, Sampling 할 때 세부적인 사항을 설명

///Light

struct LightData
{
    float4 color;
    
    float3 direction;
    int    type;
    
    float3 posiiton;
    float  range;
    
    float inner;
    float outer;
    float length;
    int   active;
};

struct LightMaterial
{
    float3 normal;
    float4 diffuseColor;
    float4 emissive;
    float4 specularIntecsity;
    
    float shininess;
    
    float3 viewPos;
    float3 worldPos;
};

struct LightVertexOutput
{
    float4 pos       : SV_POSITION;
    float2 uv        : UV;
    float3 normal    : NORMAL;
    float3 tangent   : TANGENT;
    float3 binormal  : BINORMAL;
    float3 viewPos   : POSITION0;
    float3 worldPos  : POSITION1;
};

#define MAX_LIGHT 10

cbuffer LightBuffer : register(b0) // pixelshader에 적용하는거라서 World에 할당된 b0와 다름
{
    LightData lights[MAX_LIGHT];
    
    int     lightCount;
    float3  ambientLight;
    float3  ambientCeil;
};

float3 GetNormal(float3 t, float3 b, float3 n, float2 uv)
{
    float3 T = normalize(t);
    float3 B = normalize(b);
    float3 N = normalize(n);
    
    float3 normal = N;
    
    if (hasNormalMap)
    {
        float4 normalSample = normalMap.Sample(samp, uv);
        
        normal = normalSample * 2.0f - 1.0f;
        
        float3x3 TBN = float3x3(T, B, N);
        
        normal = normalize(mul(normal, TBN));
    }
    
    return normal;
}

LightMaterial GetLightMaterial(LightVertexOutput input)
{
    LightMaterial material;
    
    material.normal             = GetNormal(input.tangent, input.binormal, input.normal, input.uv);
    material.emissive           = float4(0.1f, 0.1f, 0.1f, 0.1f);
    
    [branch]
    if(hasDiffuseMap)
        material.diffuseColor = diffuseMap.Sample(samp, input.uv);
    else
        material.diffuseColor = float4(1, 1, 1, 1);
    
    [branch]
    if(hasSpecularMap)
        material.specularIntecsity = specularMap.Sample(samp, input.uv);
    else
        material.specularIntecsity = float4(1, 1, 1, 1);
    
    //material.specularIntecsity  = specularMap.Sample(samp, input.uv);
    
    material.viewPos  = input.viewPos;
    material.worldPos = input.worldPos;
    
    return material;
}

float4 CalculateAmbient(LightMaterial material)
{
    float up = material.normal.y * 0.5f + 0.5f;
    
    float4 result = (float4(ambientLight + up * ambientCeil, 1.0f));
    
    return result * material.diffuseColor * mAmbient;
}

//float4 CalculateEmissive(LightMaterial material)
//{
    
//}

float4 CalculateDirectional(LightMaterial material, LightData data)
{
    float3 toLight = normalize(data.direction);
    
    float diffuseIntensity = saturate(dot(material.normal, -toLight));
    
    float4 finalColor = data.color * diffuseIntensity * mDiffuse;
    
    // Blinn Phong Shading
    
    float3 viewDir = normalize(material.worldPos - material.viewPos); // 카메라가 정점을 바라보는 방향
    float3 halfWay = normalize(viewDir + toLight);
    float specular = saturate(dot(material.normal, -halfWay));
    
    finalColor += data.color * pow(specular, shininess) * material.specularIntecsity * mSpecular;
    
    return finalColor * material.diffuseColor;
}

float4 CalculatePoint(LightMaterial material, LightData data)
{
    float3 toLight = material.worldPos - data.posiiton;
    float distanceToLight = length(toLight);
    
    toLight /= distanceToLight;
    
    float diffuseIntensity = saturate(dot(material.normal, -toLight));
    
    float4 finalColor = data.color * diffuseIntensity * mDiffuse;
    
    float3 viewDir = normalize(material.worldPos - material.viewPos); // 카메라가 정점을 바라보는 방향
    float3 halfWay = normalize(viewDir + toLight);
    float specular = saturate(dot(material.normal, -halfWay));
    
    finalColor += data.color * pow(specular, shininess) * material.specularIntecsity * mSpecular;
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / data.range);
    
    float attention = pow(distanceToLightNormal, 2);
    
    return finalColor * material.diffuseColor * attention;
}

float4 CalculateSpot(LightMaterial material, LightData data)
{
    float3 toLight = material.worldPos - data.posiiton;
    float distanceToLight = length(toLight);
    
    toLight /= distanceToLight;
    
    float diffuseIntensity = saturate(dot(material.normal, -toLight));
    
    float4 finalColor = data.color * diffuseIntensity * mDiffuse;
    
    float3 viewDir = normalize(material.worldPos - material.viewPos); // 카메라가 정점을 바라보는 방향
    float3 halfWay = normalize(viewDir + toLight);
    float specular = saturate(dot(material.normal, -halfWay));
    
    finalColor += data.color * pow(specular, shininess) * material.specularIntecsity * mSpecular;
    
    float3 dir = normalize(data.direction);
    float cosAngle = dot(dir, toLight);
    
    float outer = cos(radians(data.outer));
    float inner = 1.0f / cos(radians(data.inner));
    
    float4 conAttention = saturate((cosAngle - outer) * inner);
    
    conAttention = pow(conAttention, 2);
    
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / data.range);
    
    float attention = pow(distanceToLightNormal, 2);
    
    return finalColor * material.diffuseColor * attention * conAttention;
}

float4 CalculateCapsule(LightMaterial material, LightData data)
{
    float3 direction = normalize(data.direction);
    float3 start = material.worldPos - data.posiiton;
    float distanceOnLine = dot(start, direction) / data.length;
    distanceOnLine = saturate(distanceOnLine) * data.length;
    
    float3 pointOnLine = data.posiiton + direction * distanceOnLine;
    
    float3 toLight = material.worldPos - pointOnLine;
    float distanceToLight = length(toLight);
    
    toLight /= distanceToLight;
    
    float diffuseIntensity = saturate(dot(material.normal, -toLight));
    
    float4 finalColor = data.color * diffuseIntensity * mDiffuse;
    
    float3 viewDir = normalize(material.worldPos - material.viewPos); // 카메라가 정점을 바라보는 방향
    float3 halfWay = normalize(viewDir + toLight);
    float specular = saturate(dot(material.normal, -halfWay));
    
    finalColor += data.color * pow(specular, shininess) * material.specularIntecsity * mSpecular;
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / data.range);
    
    float attention = pow(distanceToLightNormal, 2);
    
    return finalColor * material.diffuseColor * attention;
}

float4 CalculateLights(LightMaterial material)
{
    float4 color = 0;
    
    [unroll(MAX_LIGHT)] // for문을 최적화 하는데 [MAX_LIGHT] 만큼만 하도록 제한함
    for (int i = 0; i < lightCount; i++)
    {
        if(!lights[i].active)
            continue;
        
        [branch]
        if(lights[i].type == 0)
            color += CalculateDirectional(material, lights[i]);
        else if(lights[i].type == 1)
            color += CalculatePoint(material, lights[i]);
        else if (lights[i].type == 2)
            color += CalculateSpot(material, lights[i]);
        else if (lights[i].type == 3)
            color += CalculateCapsule(material, lights[i]);
    }
    
    return color;
}

matrix SkinWorld(float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix curAnim, nextAnim;
    matrix cur, next;
    
    float4 c0, c1, c2, c3;
    
    float4 n0, n1, n2, n3;
    
    int clipIndex[2];
    int curFrame[2];
    float time[2];
    
    clipIndex[0] = motion.cur.clip;
    curFrame[0] = motion.cur.curFrame;
    time[0] = motion.cur.time;
        
    clipIndex[1] = motion.next.clip;
    curFrame[1] = motion.next.curFrame;
    time[1] = motion.next.time;
    
    
    [unroll] // shader에만 쓰이는 명령어, 반복문에 쓰는 명령어, 반복문을 펼쳐서 쓴것처럼 바꿔줌-> 속도가 빨라지지만 메모리를 더 많이 차지함(지역변수를 다 반복한 만큼 생성해서 쓰기 때문)
    for (int i = 0; i < 4; i++)
    {
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[0], clipIndex[0], 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[0], clipIndex[0], 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[0], clipIndex[0], 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[0], clipIndex[0], 0));
                                                                    
        cur = matrix(c0, c1, c2, c3);
        
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[0] + 1, clipIndex[0], 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[0] + 1, clipIndex[0], 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[0] + 1, clipIndex[0], 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[0] + 1, clipIndex[0], 0));
                                                                    
        next = matrix(n0, n1, n2, n3);
        
        
        curAnim = lerp(cur, next, time[0]);
        
        
        [flatten]
        if (clipIndex[1] > -1)
        {
            c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[1], clipIndex[1], 0));
            c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[1], clipIndex[1], 0));
            c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[1], clipIndex[1], 0));
            c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[1], clipIndex[1], 0));
                                                                    
            cur = matrix(c0, c1, c2, c3);
        
            n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[1] + 1, clipIndex[1], 0));
            n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[1] + 1, clipIndex[1], 0));
            n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[1] + 1, clipIndex[1], 0));
            n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[1] + 1, clipIndex[1], 0));
                                                                    
            next = matrix(n0, n1, n2, n3);
        
        
            nextAnim = lerp(cur, next, time[1]);
            
            curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
        }
        
        transform += mul(weights[i], curAnim);
    }
    
    return transform;
}

matrix SkinWorld(uint instancedIndex, float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix curAnim, nextAnim;
    matrix cur, next;
    
    float4 c0, c1, c2, c3;
    
    float4 n0, n1, n2, n3;
    
    int clipIndex[2];
    int curFrame[2];
    float time[2];
    
    clipIndex[0] = motions[instancedIndex].cur.clip;
    curFrame[0] = motions[instancedIndex].cur.curFrame;
    time[0] = motions[instancedIndex].cur.time;
        
    clipIndex[1] = motions[instancedIndex].next.clip;
    curFrame[1] = motions[instancedIndex].next.curFrame;
    time[1] = motions[instancedIndex].next.time;
    
    
    [unroll] // shader에만 쓰이는 명령어, 반복문에 쓰는 명령어, 반복문을 펼쳐서 쓴것처럼 바꿔줌-> 속도가 빨라지지만 메모리를 더 많이 차지함(지역변수를 다 반복한 만큼 생성해서 쓰기 때문)
    for (int i = 0; i < 4; i++)
    {
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[0], clipIndex[0], 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[0], clipIndex[0], 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[0], clipIndex[0], 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[0], clipIndex[0], 0));
                                                                    
        cur = matrix(c0, c1, c2, c3);
        
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[0] + 1, clipIndex[0], 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[0] + 1, clipIndex[0], 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[0] + 1, clipIndex[0], 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[0] + 1, clipIndex[0], 0));
                                                                    
        next = matrix(n0, n1, n2, n3);
        
        
        curAnim = lerp(cur, next, time[0]);
        
        
        [flatten]
        if (clipIndex[1] > -1)
        {
            c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[1], clipIndex[1], 0));
            c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[1], clipIndex[1], 0));
            c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[1], clipIndex[1], 0));
            c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[1], clipIndex[1], 0));
                                                                    
            cur = matrix(c0, c1, c2, c3);
        
            n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[1] + 1, clipIndex[1], 0));
            n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[1] + 1, clipIndex[1], 0));
            n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[1] + 1, clipIndex[1], 0));
            n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[1] + 1, clipIndex[1], 0));
                                                                    
            next = matrix(n0, n1, n2, n3);
        
        
            nextAnim = lerp(cur, next, time[1]);
            
            curAnim = lerp(curAnim, nextAnim, motions[instancedIndex].tweenTime);
        }
        
        transform += mul(weights[i], curAnim);
    }
    
    return transform;
}





///////////////////////// Deferred Render///////////////////////////////
#pragma region Deferred Render

#define MIN_SHININESS  0.1f
#define MAX_SHININESS 50.0f


struct GBufferOutput
{
    float4  diffuse : SV_Target0;
    float4 specular : SV_Target1;
    float4   normal : SV_Target2;
    float4  ambient : SV_Target3;
    float4 emissive : SV_Target4;
};

GBufferOutput PackGBuffer(float3 baseColor, float3 normal, float specularIntensity, float4 ambient)
{
    GBufferOutput output;
    
    float specPowNorm = (shininess - MIN_SHININESS) / MAX_SHININESS;
    
    output.diffuse = float4(baseColor, 1.0f);
    output.specular = float4(specPowNorm, specularIntensity, 0, 1);
    output.normal = float4(normal * 0.5f + 0.5f, 1);
    output.ambient = ambient;
    output.emissive = mEmissive;

    return output;
}

#pragma endregion Deferred Render