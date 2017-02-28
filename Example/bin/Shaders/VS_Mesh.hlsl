cbuffer ConstantBuffer{
    float4x4 WVP;
	float4x4 World;    
}

struct VS_INPUT{
    float4 position : POSITION;
	
#ifdef USE_NORMALS
	float4 normal   : NORMAL;
#endif

#ifdef USE_TANGENTS
	float4 tangent  : TANGENT;
#endif

#ifdef USE_BINORMALS
	float4 binormal  : BINORMAL;
#endif

#ifdef USE_TEXCOORD0
    float2 texture0 : TEXCOORD;
#endif
};

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

VS_OUTPUT VS( VS_INPUT input ){
    VS_OUTPUT OUT;
    OUT.hposition = mul( WVP , input.position );
	
#ifdef USE_NORMALS	
	OUT.hnormal = normalize( mul( World , input.normal ) );
#endif

#ifdef USE_TANGENTS	
	OUT.htangent = normalize( mul( World , input.tangent ) );
#endif

#ifdef USE_BINORMALS	
	OUT.hbinormal = normalize( mul( World , input.binormal ) );
#endif
	
#ifdef USE_TEXCOORD0
    OUT.texture0 = input.texture0;
#endif

    return OUT;
}
