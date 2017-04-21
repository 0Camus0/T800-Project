
uniform mediump sampler2D DiffuseTex;

#ifdef SPECULAR_MAP
uniform mediump sampler2D SpecularTex;
#endif

#ifdef GLOSS_MAP
uniform mediump sampler2D GlossTex;
#endif

#ifdef NORMAL_MAP
uniform mediump sampler2D NormalTex;
#endif

uniform highp vec4 LightPos;
uniform highp vec4 LightColor;
uniform highp vec4 CameraPosition;
uniform highp vec4 CameraInfo;
uniform highp vec4 Ambient;


#define PHONG 1
#define BLINN 2

#define SPECULAR_MODEL BLINN

#if   SPECULAR_MODEL == PHONG
#define USING_PHONG
#elif SPECULAR_MODEL == BLINN
#define USING_BLINN
#endif


#define AMBIENT
#define DIFFUSE
#define SPECULAR
#define FRESNEL 
/*

#ifdef DIFFUSE_MAP
#undef DIFFUSE_MAP
#endif

#ifdef SPECULAR_MAP
#undef SPECULAR_MAP
#endif

#ifdef GLOSS_MAP
#undef GLOSS_MAP
#endif 

#ifdef NORMAL_MAP
#undef NORMAL_MAP
#endif

*/

#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif


#ifdef USE_NORMALS
varying highp vec4 hnormal;
#endif

#ifdef USE_TANGENTS
varying highp vec4 htangent;
#endif

#ifdef USE_BINORMALS
varying highp vec4 hbinormal;
#endif

varying highp vec4 Pos;
varying highp vec4 WorldPos;

#ifdef SIMPLE_COLOR
void main(){
	gl_FragColor = vec4(0.5,0.5,0.5,1.0);
}
#elif defined(G_BUFFER_PASS)
void main(){
	lowp vec4 color    = vec4(0.5,0.5,0.5,1.0);
	lowp vec4 normal   = vec4(0.5,0.5,0.5,1.0);
	lowp vec4 specular = vec4(0.5,0.5,0.5,1.0);
	lowp vec4 reflect  = vec4(0.5,0.5,0.5,1.0);

	mediump float specIntesivity = 0.8;
	mediump float shinness = 2.0;
	
	#ifdef DIFFUSE_MAP
		color = texture2D(DiffuseTex,vecUVCoords);
	#endif
	
	normal.xyz   = normalize(hnormal).xyz;  
	#ifdef NORMAL_MAP	
		lowp vec3 normalTex  = texture2D(NormalTex,vecUVCoords).xyz;
		normalTex 		 	 = normalTex*vec3(2.0,2.0,2.0) - vec3(1.0,1.0,1.0);
		normalTex		 	 = normalize(normalTex);
		normalTex.g 	 	 = -normalTex.g;
		lowp vec3 tangent	 = normalize(htangent).xyz;
		lowp vec3 binormal	 = normalize(hbinormal).xyz;
		lowp mat3	TBN 	 = mat3(tangent,binormal,normal);
		normal.xyz		 	 = TBN*normalTex;
		normal.xyz		 	 = normalize(normal.xyz)*0.5 + 0.5;
	#else
		normal.xyz 			 = normal.xyz*0.5 + 0.5;
	#endif
	
	#ifdef SPECULAR_MAP
		specular = texture2D(SpecularTex,vecUVCoords);
	#endif
	
	#ifdef GLOSS_MAP
		shinness = texture2D(GlossTex,vecUVCoords).r + shinness;
	#endif
	
	gl_FragData[0].rgb  = color.rgb;
	gl_FragData[0].a 	= specIntesivity;
	
	gl_FragData[1].rgb  = normal.rgb;
	gl_FragData[1].a 	= shinness;
	
	gl_FragData[2].rgb  = specular.rgb;
	gl_FragData[2].a 	= shinness;
	
	#ifdef NO_LIGHT
		gl_FragData[3]	= vec4(1.0,0.0,1.0,1.0);
	#else
		#ifdef NORMAL_MAP	
			gl_FragData[3] 	= vec4(0.0,0.0,0.0,1.0);
		#else
			gl_FragData[3] 	= vec4(0.0,0.0,1.0,1.0);
		#endif
	#endif
	
	#ifdef NON_LINEAR_DEPTH
		// gl_FragDepth	= Pos.z / Pos.w;
	#else
		gl_FragDepth = Pos.z / CameraInfo.y;
	#endif
	
}
#else
void main(){

	lowp vec4 color = vec4(0.5,0.5,0.5,1.0);
	lowp vec4 Final = vec4(0.0,0.0,0.0,1.0);
	
#ifdef USE_TEXCOORD0
	#ifdef NO_LIGHT
		color = texture2D(DiffuseTex,vecUVCoords);
	#else
		#ifdef DIFFUSE_MAP
		color = texture2D(DiffuseTex,vecUVCoords);
		#endif
		
		#ifdef SPECULAR_MAP
		lowp vec4 specularmap = texture2D(SpecularTex,vecUVCoords);
		#endif
		
		#ifdef USE_NORMALS
			lowp vec4 Ambiental = color*Ambient;
		
			lowp vec4   Lambert  = LightColor;
			lowp vec4 	Specular = LightColor;
			lowp vec4	Fresnel  = LightColor;
			lowp vec3	LightDir = normalize(LightPos-WorldPos).xyz;
			lowp vec3   EyeDir   = normalize(CameraPosition-WorldPos).xyz;
			lowp vec3	normal   = normalize(hnormal).xyz;  
			mediump float att			 = 1.0;
			
			
		#ifdef NORMAL_MAP	
			lowp vec3 normalTex = texture2D(NormalTex,vecUVCoords).xyz;
			normalTex 		 = normalTex*vec3(2.0,2.0,2.0) - vec3(1.0,1.0,1.0);
			normalTex		 = normalize(normalTex);
			normalTex.g 	 = -normalTex.g;
			lowp vec3 tangent	 = normalize(htangent).xyz;
			lowp vec3 binormal	 = normalize(hbinormal).xyz;
			lowp mat3	TBN = mat3(tangent,binormal,normal);
			normal			 = TBN*normalTex;
			normal			 = normalize(normal);
		#endif
		
		#ifdef DIFFUSE
			att		 	     = dot(normal,LightDir)*0.5 + 0.5;
			att				 = pow( att , 2.0 );	
			att				 = clamp( att , 0.0 , 1.0 );
			Lambert			*= color*att;
		#endif
		
		#ifdef SPECULAR
			highp float specular  = 0.0;
			highp float specIntesivity = 0.8;
			highp float shinness = 8.0;
			
			#ifdef GLOSS_MAP
				shinness = texture2D(GlossTex,vecUVCoords).r + shinness;
			#endif

		#ifdef USING_PHONG
			lowp vec3 ReflectedLight = reflect(-LightDir,normal);
			//specular = max ( dot(ReflectedLight,EyeDir), 0.0);	
			specular =  dot(ReflectedLight,EyeDir)*0.5 + 0.5;	
			specular = pow( specular ,shinness);		
		#elif defined(USING_BLINN)
			lowp vec3 ReflectedLight = normalize(EyeDir+LightDir); 
			specular = max ( dot(ReflectedLight,normal)*0.5 + 0.5, 0.0);	
			specular = pow( specular ,shinness);	
		#endif
			
			specular *= att; 
			specular *= specIntesivity;
			Specular *= specular;
			
			#ifdef SPECULAR_MAP
				Specular.xyz *= specularmap.xyz;
			#endif
				
		#endif
		
	#ifdef FRESNEL
		mediump float  FresnelAtt	= dot(normal,EyeDir);
		lowp float  FresnelIntensity = 1.0;
		#ifdef SPECULAR_MAP
			lowp vec4 FresnelCol = vec4(specularmap.xyz,1.0);
		#else
			lowp vec4 FresnelCol = vec4(1.0,1.0,1.0,1.0);	
		#endif
		FresnelAtt		= abs(FresnelAtt);
		FresnelAtt 		= 1.0 - FresnelAtt;
		FresnelAtt 		= clamp( FresnelAtt , 0.0 , 1.0 );
		FresnelAtt		= pow( FresnelAtt , 4.0 );	
		FresnelAtt 		= clamp(FresnelAtt , 0.0 , 1.0 );
		Fresnel 		= FresnelCol*FresnelIntensity*FresnelAtt; 
	#endif
		
		#ifdef AMBIENT
			Final += Ambiental;
		#endif
		
		#ifdef DIFFUSE
			Final += Lambert;
		#endif
		
		#ifdef SPECULAR
			Final += Specular;
		#endif
		
		#ifdef SPECULAR
			Final += Specular;
		#endif
		
		#ifdef FRESNEL
			Final += Fresnel;
		#endif
		
		color = Final;
		#endif
	#endif
#endif

	gl_FragColor = color;
}

#endif