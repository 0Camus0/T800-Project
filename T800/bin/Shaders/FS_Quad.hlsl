cbuffer ConstantBuffer{
    float4x4 WVP;
	float4x4 World;  
	float4x4 WorldView;
	float4x4 WVPInverse;
	float4x4 WVPLight;
	float4	 LightPositions[128];
	float4	 LightColors[128];
	float4   CameraPosition;
	float4 	 CameraInfo;
	float4	 LightCameraPosition;
	float4 	 LightCameraInfo;
}

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float2 texture0  : TEXCOORD;
	float4 Pos		: TEXCOORD1;
	float4 PosCorner : VPOS;
};

SamplerState SS;

#ifdef DEFERRED_PASS
Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
Texture2D tex3 : register(t3);
Texture2D tex4 : register(t4);
Texture2D tex5 : register(t5);
TextureCube texEnv : register(t6);
float4 FS( VS_OUTPUT input ) : SV_TARGET {
	float4 Final = float4(0.0,0.0,0.0,1.0);
	float4 color  =  tex0.Sample( SS, input.texture0);
	float4 matId  =	tex3.Sample( SS, input.texture0);
	float depth = tex4.Sample( SS, input.texture0 ).r;
	
	#ifdef NON_LINEAR_DEPTH
		float4 position = mul(WVPInverse,float4( input.PosCorner.xy ,depth,1.0));
		position.xyz /= position.w;
	#else		
		float4 position = CameraPosition + input.PosCorner*depth;
	#endif
	 
	 float3  EyeDir   = normalize(CameraPosition-position).xyz;
	 
	if(matId.r == 1.0 && matId.g == 0.0){		
		float3 RefCol = texEnv.Sample( SS, -EyeDir ).zyx;
		Final.xyz = RefCol.xyz;
	}else{
		float Rad = 15.0;
		float cutoff = 0.8;
		float4 Lambert = float4(1.0,1.0,1.0,1.0);
		float4 Specular = float4(1.0,1.0,1.0,1.0);
		float4 Fresnel	 =  float4(1.0,1.0,1.0,1.0);
		
	#ifdef USING_16BIT_NORMALS		
		float2 normalmap = tex1.Sample( SS, input.texture0 ).rg;
		float4 nn = float4(normalmap,0,0)*float4(2,2,0,0) + float4(-1,-1,1,-1);
		float l = dot(nn.xyz,-nn.xyw);
		nn.z = l;
		nn.xy *= sqrt(l);
		float3 normal = nn.xyz * 2 + float3(0,0,-1);
	#else
		float4 normalmap = tex1.Sample( SS, input.texture0 );
		float3 normal = normalmap.xyz*2 - 1;
		normal = normalize(normal);
	#endif
		float4 specularmap = tex2.Sample( SS, input.texture0);
		
		float3 ReflectedVec = normalize(reflect(-EyeDir,normal.xyz));		
		float3 RefCol = texEnv.Sample( SS, ReflectedVec ).zyx;
		
		int NumLights = (int)CameraInfo.w;
			for(int i=0;i<NumLights;i++){
				float dist = distance(LightPositions[i],position);
				if(dist < Rad*2.0){
					Lambert  = LightColors[i];
					Specular = LightColors[i];
					Fresnel	 =  LightColors[i];			
					
					float3	LightDir = normalize(LightPositions[i]-position).xyz;
					float   att		 = 1.0;
					att		 	     = dot(normal.xyz,LightDir)*0.5 + 0.5;;
					att				 = pow( att , 2.0 );	
					att				 = clamp( att , 0.0 , 1.0 );
					Lambert			*= color*att;
					
					float  specular  = 0.0;
					float specIntesivity = 1.5;
					float shinness = 4.0;	
					
					#ifndef USING_16BIT_NORMALS
						shinness = normalmap.a + shinness;
					#endif
					
					float3 ReflectedLight = normalize(EyeDir+LightDir); 
					specular = max ( dot(ReflectedLight,normal.xyz)*0.5 + 0.5, 0.0);	
					specular = pow( specular ,shinness);	

					specular *= att;
					specular *= specIntesivity;
					Specular *= specular;
					Specular.xyz *= specularmap.xyz;
									
					float d = max(dist - Rad, 0.0);
					float denom = d/Rad + 1.0;
					
					float attenuation = 1.0 / (denom*denom);
					 
					attenuation = (attenuation - cutoff) / (1.0 - cutoff);
					attenuation = max(attenuation, 0.0);
						
					Final += Lambert*attenuation;
					Final += Specular*attenuation;
				}
			}
		if(matId.b == 0.0){
			float  FresnelAtt	= dot(normal.xyz,EyeDir);
			float  FresnelIntensity = 6.0f;
			float4 FresnelCol = float4(RefCol.xyz,1.0);

			FresnelAtt		= abs(FresnelAtt);
			FresnelAtt 		= 1.0 - FresnelAtt;
			FresnelAtt 		= clamp( FresnelAtt , 0.0 , 1.0 );
			FresnelAtt		= pow( FresnelAtt , 4.0 );	
			FresnelAtt 		= clamp(FresnelAtt , 0.0 , 1.0 );
			Fresnel 		= FresnelCol*FresnelIntensity*FresnelAtt;
		
			Final += Fresnel;		
		}

		Final.xyz *= tex5.Sample( SS, input.texture0).xyz;
		
	}
	return Final;

}
#elif defined(SHADOW_COMP_PASS)
Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
float4 FS( VS_OUTPUT input ) : SV_TARGET {
	float4 Fcolor = float4(1.0,1.0,1.0,1.0);
	float depth = tex0.Sample( SS, input.texture0 );
	
	#ifdef NON_LINEAR_DEPTH
		float4 position = mul(WVPInverse,float4( input.PosCorner.xy ,depth,1.0));
		position.xyz /= position.w;
		position.w = 1.0;
	#else		
		float4 position = CameraPosition + input.PosCorner*depth;
	#endif
	
	float4 LightPos = mul(WVPLight , position);
#ifdef NON_LINEAR_DEPTH
	LightPos.xyz /= LightPos.w;
#else
	LightPos.xy /= LightPos.w;
	LightPos.z /= LightCameraInfo.y;
#endif
	float2 SHTC = LightPos.xy*0.5 + 0.5;
	
	if(SHTC.x < 1.0 && SHTC.y < 1.0 && SHTC.x  > 0.0 && SHTC.y > 0.0 && LightPos.w > 0.0 && LightPos.z < 1.0 ){
		SHTC.y = 1.0 - SHTC.y;
		float depthSM = tex1.Sample( SS, SHTC );
		float depthPos = LightPos.z;
		depthSM += 0.000005;
		if( depthPos > depthSM)
			Fcolor = 0.25*float4(1.0,1.0,1.0,1.0);	   
	}else{
		Fcolor = float4(1.0,1.0,1.0,1.0);
	}
	
	  return Fcolor;
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
