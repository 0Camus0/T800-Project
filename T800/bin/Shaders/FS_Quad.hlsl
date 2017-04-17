cbuffer ConstantBuffer{
    float4x4 WVP;
	float4x4 World;  
}

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float2 texture0  : TEXCOORD;
	float4 wPos		: TEXCOORD1;
};

SamplerState SS;

#ifdef DEFERRED_PASS
Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
Texture2D tex3 : register(t3);
Texture2D tex4 : register(t4);
float4 FS( VS_OUTPUT input ) : SV_TARGET {
	float4 color = float4(0.5,0.5,0.5,1.0);
	float4 normal =  tex1.Sample( SS, input.texture0);
	normal	= normalize(normal);

	if(normal.x == 0 && normal.y == 0){
		color = tex0.Sample( SS, input.texture0);
	}else{
		float2 distor = float2(normal.xy);
		color = tex0.Sample( SS, input.texture0*distor);
	}

	return color;
}

#elif defined(FSQUAD_1_TEX)
Texture2D tex0 : register(t0);
float4 FS( VS_OUTPUT input ) : SV_TARGET {
	return tex0.Sample( SS, input.texture0);
}
#elif defined(FSQUAD_2_TEX)
Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
float4 FS( VS_OUTPUT input ) : SV_TARGET {
	return tex0.Sample( SS, input.texture0);
}
#elif defined(FSQUAD_3_TEX)
Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
float4 FS( VS_OUTPUT input ) : SV_TARGET {
	return tex0.Sample( SS, input.texture0);
}
#else
Texture2D tex0 : register(t0);
float4 FS( VS_OUTPUT input ) : SV_TARGET {
	return tex0.Sample( SS, input.texture0);
}
#endif
