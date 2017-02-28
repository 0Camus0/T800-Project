cbuffer ConstantBuffer{
    float4x4 World;
    float4x4 WVP;
}

Texture2D TextureRGB : register(t0);

SamplerState SS;

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
	
#ifdef USE_NORMALS
	float4 hnormal   : NORMAL;
#endif

#ifdef USE_TANGENTS
	float4 htangent   : TANGENT;
#endif

#ifdef USE_BINORMALS
	float4 hbinormal : BINORMAL;
#endif

#ifdef USE_TEXCOORD0
    float2 texture0  : TEXCOORD;
#endif
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
    float4 color;
	
#ifdef USE_TEXCOORD0
	color = TextureRGB.Sample( SS, input.texture0 );	
#else
	#ifdef USE_NORMALS
		color = normalize( input.hnormal );
	#else
		color = float4(0.5,0.5,0.5,1.0);
	#endif
#endif

    return color;
}
