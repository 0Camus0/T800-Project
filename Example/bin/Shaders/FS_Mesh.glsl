uniform mediump sampler2D diffuse;
uniform highp vec4 LightPos;
uniform highp vec4 LightColor;
uniform highp vec4 CameraPosition;
uniform highp vec4 Ambient;

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
	
		lowp vec4   Diffuse = LightColor;
		lowp vec4	LightDir = normalize(LightPos-wPos);
		lowp vec4	normal   = normalize(hnormal);  
		lowp float  att		 = dot(normal,LightDir);
		att				 = clamp( att , 0.0 , 1.0 );
		Diffuse	*= att;
		Diffuse *= color;
		
		lowp vec4   Specular = LightColor;
		lowp vec4   EyeDir = normalize(CameraPosition-wPos);
		
		highp float  specular  = 0.0;
	
		
		lowp vec4 	ReflectedLight = reflect(-LightDir,normal);
		specular = dot(EyeDir,ReflectedLight);
	
/*
		highp vec4 floatvector = normalize(EyeDir+LightDir); 
		specular = dot(floatvector,normal);
	*/
		specular = max( specular , 0.0 );
		specular = pow( specular , 2.0);
		

		Specular *= specular;
		
		lowp vec4  Final = /*Ambiental + Diffuse + */Specular;
		color = Final;
		
		
	#endif
#endif
	gl_FragColor = color;
}