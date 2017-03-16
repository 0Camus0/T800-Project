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
		lowp vec4	LightDir = normalize(LightPos-wPos);
		lowp vec4	normal   = normalize(hnormal);  
		lowp float  att		 = dot(normal,LightDir);
		att				 = clamp( att , 0.0 , 1.0 );
		lowp vec4  Lambert  = Ambient*color + LightColor*color*att;
		color			 = Lambert;
	#endif
#endif
	gl_FragColor = color;
}