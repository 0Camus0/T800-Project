cbuffer ConstantBuffer{
    float4x4 WVP;
	float4x4 World;  
	float4x4 WorldView;
	float4x4 WVPInverse;
	float4	 LightPositions[128];
	float4	 LightColors[128];
	float4   CameraPosition;
	float4 	 CameraInfo;
}

struct VS_OUTPUT{
    float4 hposition : SV_POSITION;
    float2 texture0  : TEXCOORD;
	float4 Pos		: TEXCOORD1;
	float4 PosCorner : TEXCOORD2;
};

SamplerState SS;

#ifdef DEFERRED_PASS
Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
Texture2D tex3 : register(t3);
Texture2D tex4 : register(t4);
float4 FS( VS_OUTPUT input ) : SV_TARGET {
	float4 Final = float4(0.0,0.0,0.0,1.0);
	float4 color  =  tex0.Sample( SS, input.texture0);
	float4 matId  =	tex3.Sample( SS, input.texture0);
	 
	if(matId.r == 1.0 && matId.g == 0.0){
		Final = color;
	}else{
		float Rad = 25.0;
		float cutoff = 0.8;
		float4 Lambert = float4(1.0,1.0,1.0,1.0);
		float4 Specular = float4(1.0,1.0,1.0,1.0);
		float4 Fresnel	 =  float4(1.0,1.0,1.0,1.0);
		float4 normalmap = tex1.Sample( SS, input.texture0);
		float4 normal = normalmap*2.0 - 1.0;
		
		float2 distor = float2(normalmap.xy);	
		if(matId.b == 1.0){
			distor = float2(1.0,1.0);
		}	
		float4 ReflectCol = tex0.Sample( SS, input.texture0*distor);
		
		float4 specularmap = tex2.Sample( SS, input.texture0);
		normal	= normalize(normal);

		float depth = tex4.Sample( SS, input.texture0 );
		float4 position = CameraPosition + input.PosCorner*depth;
		
		
		float3  EyeDir   = normalize(CameraPosition-position).xyz;
		
		int NumLights = (int)CameraInfo.w;
			for(int i=0;i<NumLights;i++){
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
				shinness = normal.a + shinness;
				
				float3 ReflectedLight = normalize(EyeDir+LightDir); 
				specular = max ( dot(ReflectedLight,normal.xyz)*0.5 + 0.5, 0.0);	
				specular = pow( specular ,shinness);	

				specular *= att;
				specular *= specIntesivity;
				Specular *= specular;
				Specular.xyz *= specularmap.xyz;
				
				float dist = distance(LightPositions[i],position);
				float d = max(dist - Rad, 0.0);
				float denom = d/Rad + 1.0;
				
				float attenuation = 1.0 / (denom*denom);
				 
				attenuation = (attenuation - cutoff) / (1.0 - cutoff);
				attenuation = max(attenuation, 0.0);
     				
				Final += Lambert*attenuation;
				Final += Specular*attenuation;
			}
		if(matId.b == 0.0){
			float  FresnelAtt	= dot(normal.xyz,EyeDir);
			float  FresnelIntensity = 1.0f;
			float4 FresnelCol = float4(ReflectCol.xyz,1.0);

			FresnelAtt		= abs(FresnelAtt);
			FresnelAtt 		= 1.0 - FresnelAtt;
			FresnelAtt 		= clamp( FresnelAtt , 0.0 , 1.0 );
			FresnelAtt		= pow( FresnelAtt , 4.0 );	
			FresnelAtt 		= clamp(FresnelAtt , 0.0 , 1.0 );
			Fresnel 		= FresnelCol*FresnelIntensity*FresnelAtt;
		
			Final += Fresnel;
		}
	}

	return Final;

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
