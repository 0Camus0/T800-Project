cbuffer ConstantBuffer{
    float4x4 WVP;
	float4x4 World;  
	float4x4 WorldView;
	float4x4 WVPInverse;
	float4	 LightPositions[16];
	float4	 LightColors[16];
	float4   CameraPosition;
	float4 	 CameraInfo;
}

struct VS_INPUT{
    float4 position : POSITION;
    float2 texture0 : TEXCOORD;
};

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float2 texture0  : TEXCOORD;
	float4 Pos		 : TEXCOORD1;
	float4 PosCorner : TEXCOORD2;
};

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.hposition = mul( WVP , input.position );
    OUT.texture0  = input.texture0;
	OUT.Pos	  	  = OUT.hposition;
    OUT.PosCorner = mul(WVPInverse, float4(input.position.xy,1.0,1.0));
	OUT.PosCorner.xyz /= OUT.PosCorner.w;
	OUT.PosCorner = OUT.PosCorner - CameraPosition;

	return OUT;
}
