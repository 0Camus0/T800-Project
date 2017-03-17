uniform mediump sampler2D diffuse;
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
	color = texture2D(diffuse,vecUVCoords);
	
	#ifdef USE_NORMALS
		lowp vec4 Ambiental = color*Ambient;
	
		lowp vec4   Lambert  = LightColor;
		lowp vec4	LightDir = normalize(LightPos-wPos);
		lowp vec4	normal   = normalize(hnormal);  
		lowp float  att		 = dot(normal,LightDir);
		att				 = clamp( att , 0.0 , 1.0 );
		Lambert			*= color*att;
		
		lowp vec4   Specular = LightColor;
		lowp vec4   EyeDir = normalize(CameraPosition-wPos);
		
		highp float specular  = 0.0;
		highp float specIntesivity = 5.0;
		highp float shinness = 4.0;

	#ifdef USING_PHONG
		lowp vec3 ReflectedLight = reflect(-LightDir,normal).xyz;
		//specular = max ( dot(ReflectedLight,EyeDir.xyz), 0.0);	
		specular =  dot(ReflectedLight,EyeDir.xyz)*0.5 + 0.5;	
		specular = pow( specular ,shinness);		
	#elif defined(USING_BLINN)
		lowp vec3 ReflectedLight = normalize(EyeDir+LightDir).xyz; 
		specular = max ( dot(ReflectedLight,normal.xyz), 0.0);	
		specular = pow( specular ,shinness);	
	#endif
		
		specular *= att; 
		specular *= specIntesivity;
		Specular *= specular;
		
		lowp vec4  Final = Ambiental + Lambert + Specular;
		color = Final;
		
		
	#endif
#endif
	gl_FragColor = color;
}