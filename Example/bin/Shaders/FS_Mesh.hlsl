cbuffer ConstantBuffer{
    float4x4 WVP;
	float4x4 World;  
	float4	 LightPos;
	float4 	 LightColor;
	float4   CameraPosition;
	float4	 Ambient;
}

#define PHONG 1
#define BLINN 2

#define SPECULAR_MODEL BLINN

#if   SPECULAR_MODEL == PHONG
#define USING_PHONG
#elif SPECULAR_MODEL == BLINN
#define USING_BLINN
#endif


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
	float4 Ambiental = color*Ambient;
	
	float4  Lambert  = LightColor;
	float4	LightDir = normalize(LightPos-input.wPos);
	float4	normal   = normalize(input.hnormal);  
	float   att		 = dot(normal,LightDir);
	att				 = clamp( att , 0.0 , 1.0 );
	Lambert			*= color*att;
	
	float4   Specular = LightColor;
	float4   EyeDir = normalize(CameraPosition-input.wPos);
	
	float  specular  = 0.0;
	float specIntesivity = 5.0;
	float shinness = 7.0;

#ifdef USING_PHONG
	float3 	ReflectedLight = reflect(-LightDir,normal).xyz;
	specular = max ( dot(ReflectedLight,EyeDir.xyz), 0.0);	
	specular = pow( specular ,shinness);		
#elif defined(USING_BLINN)
	float3 ReflectedLight = normalize(EyeDir+LightDir).xyz; 
	specular = max ( dot(ReflectedLight,normal.xyz), 0.0);	
	specular = pow( specular ,shinness);	
#endif

	specular *= att;
	specular *= specIntesivity;
	Specular *= specular;
	
	float4  Final = Lambert+Specular;
	color = Final;
	#endif
#endif		
	
    return color;
}
