uniform highp mat4 WVP;
uniform highp mat4 World;
uniform highp mat4 WorldView;
uniform highp mat4 WVPInverse;
uniform highp vec4 LightPositions[128];
uniform highp vec4 LightColors[128];
uniform highp vec4 CameraPosition;
uniform highp vec4 CameraInfo;

#ifdef ES_30
	in highp vec2 vecUVCoords;
	in highp vec4 Pos;
	in highp vec4 PosCorner;
#else
	varying highp vec2 vecUVCoords;
	varying highp vec4 Pos;
	varying highp vec4 PosCorner;
#endif

#ifdef ES_30
	layout(location = 0) out highp vec4 colorOut;
#endif

#ifdef DEFERRED_PASS
uniform mediump sampler2D tex0;
uniform mediump sampler2D tex1;
uniform mediump sampler2D tex2;
uniform mediump sampler2D tex3;
uniform mediump sampler2D tex4;
uniform mediump sampler2D tex5;
uniform mediump sampler2D tex6;
uniform mediump sampler2D tex7;
uniform mediump sampler2D tex8;

void main(){
	lowp vec2 coords = vecUVCoords;
	coords.y = 1.0 - coords.y;

	lowp vec4 Final  =  vec4(0.0,0.0,0.0,1.0);
	
	#ifdef ES_30
		lowp vec4 color  =  texture(tex0,coords);
		lowp vec4 matId  =	texture(tex3,coords);
	#else
		lowp vec4 color  =  texture2D(tex0,coords);
		lowp vec4 matId  =	texture2D(tex3,coords);
	#endif
	
	if(matId.r == 1.0 && matId.g == 0.0){
		Final = color;
	}else{
		highp float Rad = 15.0;
		highp float cutoff = 0.8;
		lowp vec4 Lambert = vec4(1.0,1.0,1.0,1.0);
		lowp vec4 Specular = vec4(1.0,1.0,1.0,1.0);
		lowp vec4 Fresnel	 =  vec4(1.0,1.0,1.0,1.0);
		#ifdef ES_30
			lowp vec4 normalmap = texture(tex1,coords);
		#else
			lowp vec4 normalmap = texture2D(tex1,coords);
		#endif			
		lowp vec4 normal = normalmap*2.0 - 1.0;
		normal	= normalize(normal);

		lowp vec2 distor = vec2(normalmap.xy);	
		if(matId.b == 1.0){
			distor = vec2(1.0,1.0);
		}	
		lowp vec2 coord2 = vecUVCoords*distor;
		coord2.y = 1.0 - coord2.y;
		#ifdef ES_30
			lowp vec4 ReflectCol = texture(tex0,coord2);
		#else
			lowp vec4 ReflectCol = texture2D(tex0,coord2);
		#endif
		
		#ifdef ES_30
			lowp vec4 specularmap = texture(tex2,coords);
		#else
			lowp vec4 specularmap = texture2D(tex2,coords);
		#endif
		
		#ifdef ES_30
			highp float depth = texture(tex4,coords).r;
		#else
			highp float depth = texture2D(tex4,coords).r;
		#endif
		
#ifdef NON_LINEAR_DEPTH
		highp vec2 vcoord = coords *2.0 - 1.0;
		highp vec4 position = WVPInverse*vec4(vcoord ,depth,1.0);
		position.xyz /= position.w;
		position = CameraPosition + position; 
#else	
		highp vec4 position = CameraPosition + PosCorner*depth;
#endif
		
		
		highp vec3  EyeDir   = normalize(CameraPosition-position).xyz;
		
		highp int NumLights =  int(CameraInfo.w);
			for(highp int i=0;i<NumLights;i++){
				highp float dist = distance(LightPositions[i],position);
				if(dist < (Rad*2.0)){
					Lambert  = LightColors[i];
					Specular = LightColors[i];
					Fresnel	 = LightColors[i];			
					
					highp  vec3  LightDir = normalize(LightPositions[i]-position).xyz;
					highp float   att		 = 1.0;
					att		 	     = dot(normal.xyz,LightDir)*0.5 + 0.5;
					att				 = pow( att , 2.0 );	
					att				 = clamp( att , 0.0 , 1.0 );
					Lambert			*= color*att;
				
					highp float  specular  = 0.0;
					highp float specIntesivity = 1.5;
					highp float shinness = 4.0;	
					shinness = normal.a + shinness;
					
					lowp vec3 ReflectedLight = normalize(EyeDir+LightDir); 
					specular = max ( dot(ReflectedLight,normal.xyz)*0.5 + 0.5, 0.0);	
					specular = pow( specular ,shinness);	

					specular *= att;
					specular *= specIntesivity;
					Specular *= specular;
					Specular.xyz *= specularmap.xyz;
					
					
					highp float d = max(dist - Rad, 0.0);
					highp float denom = d/Rad + 1.0;
					
					highp float attenuation = 1.0 / (denom*denom);
					 
					attenuation = (attenuation - cutoff) / (1.0 - cutoff);
					attenuation = max(attenuation, 0.0);
						
					Final += Lambert*attenuation;
					Final += Specular*attenuation;
				}
			}
		if(matId.b == 0.0){
			highp float  FresnelAtt	= dot(normal.xyz,EyeDir);
			highp float  FresnelIntensity = 1.0;
			lowp vec4 FresnelCol = vec4(ReflectCol.xyz,1.0);

			FresnelAtt		= abs(FresnelAtt);
			FresnelAtt 		= 1.0 - FresnelAtt;
			FresnelAtt 		= clamp( FresnelAtt , 0.0 , 1.0 );
			FresnelAtt		= pow( FresnelAtt , 4.0 );	
			FresnelAtt 		= clamp(FresnelAtt , 0.0 , 1.0 );
			Fresnel 		= FresnelCol*FresnelIntensity*FresnelAtt;
		
			Final += Fresnel;
		}		
	}
#ifdef ES_30
	colorOut = Final;
#else
	gl_FragColor = Final;
#endif
	
}
#elif defined(FSQUAD_1_TEX)
uniform mediump sampler2D tex0;
void main(){
	lowp vec2 coords = vecUVCoords;
	coords.y = 1.0 - coords.y;
	#ifdef ES_30
		colorOut = texture(tex0,coords);
	#else
		gl_FragColor = texture2D(tex0,coords);
	#endif
}
#elif defined(FSQUAD_2_TEX)
uniform mediump sampler2D tex0;
uniform mediump sampler2D tex1;
void main(){
	#ifdef ES_30
		colorOut = texture(tex0,vecUVCoords);
	#else
		gl_FragColor = texture2D(tex0,vecUVCoords);
	#endif
}
#elif defined(FSQUAD_3_TEX)
uniform mediump sampler2D tex0;
uniform mediump sampler2D tex1;
uniform mediump sampler2D tex2;
void main(){
	#ifdef ES_30
		colorOut = texture(tex0,vecUVCoords);
	#else
		gl_FragColor = texture2D(tex0,vecUVCoords);
	#endif
}
#else
void main(){
	#ifdef ES_30
		colorOut = vec4(1.0,0.0,1.0,1.0);
	#else
		gl_FragColor = vec4(1.0,0.0,1.0,1.0);
	#endif
}
#endif




