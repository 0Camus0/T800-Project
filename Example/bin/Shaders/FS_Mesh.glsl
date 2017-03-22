

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
uniform highp vec4 Ambient;


#define PHONG 1
#define BLINN 2

#define SPECULAR_MODEL BLINN

#if   SPECULAR_MODEL == PHONG
#define USING_PHONG
#elif SPECULAR_MODEL == BLINN
#define USING_BLINN
#endif


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



varying highp vec4 wPos;

void main(){

	lowp vec4 color = vec4(0.0,0.0,0.0,1.0);
	
#ifdef USE_TEXCOORD0
	color = texture2D(DiffuseTex,vecUVCoords);
	
	#ifdef SPECULAR_MAP
	lowp vec4 specularmap = texture2D(SpecularTex,vecUVCoords);
	#endif
	
	#ifdef USE_NORMALS
		lowp vec4 Ambiental = color*Ambient;
	
		lowp vec4   Lambert  = LightColor;
		lowp vec3	LightDir = normalize(LightPos-wPos).xyz;
		lowp vec3	normal   = normalize(hnormal).xyz;  
		
		
	#ifdef NORMAL_MAP	
		lowp vec3 normalTex = texture2D(NormalTex,vecUVCoords).xyz;
		normalTex 		 = normalTex*vec3(2.0,2.0,2.0) - vec3(1.0,1.0,1.0);
		normalTex		 = normalize(normalTex);
		normalTex.g 	-= normalTex.g;
		lowp vec3 tangent	 = normalize(htangent).xyz;
		lowp vec3 binormal	 = normalize(hbinormal).xyz;
		lowp mat3	TBN = mat3(tangent,binormal,normal);
		normal			 = TBN*normalTex;
		normal			 = normalize(normal);
	#endif
	
	
		lowp float  att		 = dot(normal,LightDir);
		att				 = clamp( att , 0.0 , 1.0 );
		Lambert			*= color*att;
		
		lowp vec4   Specular = LightColor;
		lowp vec3   EyeDir = normalize(CameraPosition-wPos).xyz;
		
		highp float specular  = 0.0;
		highp float specIntesivity = 2.0;
		highp float shinness = 4.0;
		
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
		specular = max ( dot(ReflectedLight,normal), 0.0);	
		specular = pow( specular ,shinness);	
	#endif
		
		specular *= att; 
		specular *= specIntesivity;
		Specular *= specular;
		
		#ifdef SPECULAR_MAP
		Specular.xyz *= specularmap.xyz;
		#endif
		
		lowp vec4  Final =  Lambert + Specular;
		color = Final;
		
	#endif
#endif
	gl_FragColor = color;
}