cbuffer ConstantBuffer{
    float4x4 WVP;
	float4x4 World;  
}

struct VS_INPUT{
    float4 position : POSITION;
    float2 texture0 : TEXCOORD;
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float2 texture0  : TEXCOORD;
	float4 wPos		 : TEXCOORD1;
};

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.hposition = mul( WVP , input.position );
    OUT.texture0  = input.texture0;
	OUT.wPos	  = OUT.hposition;
    return OUT;
}
