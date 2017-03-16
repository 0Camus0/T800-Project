cbuffer ConstantBuffer{
    float4x4 WVP;
	float4x4 World;  
	float4	 LightPos;
	float4 	 LightColor;
	float4   CameraPosition;
	float4	 Ambient;
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
	
	float4 wPos		: TEXCOORD1;
};

float4 FS( VS_OUTPUT input ) : SV_TARGET  {
    float4 color = float4(0.0,0.0,0.0,1.0);
	
#ifdef USE_TEXCOORD0
	color = TextureRGB.Sample( SS, input.texture0 );	
	
	#ifdef USE_NORMALS
	float4	LightDir = normalize(LightPos-input.wPos);
	float4	normal   = normalize(input.hnormal);  
	float   att		 = dot(normal,LightDir);
	att				 = clamp( att , 0.0 , 1.0 );
	float4  Lambert  = Ambient*color + LightColor*color*att;
	color			 = Lambert;
	#endif
#endif		
	
    return color;
}
