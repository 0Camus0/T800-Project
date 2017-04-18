uniform highp mat4 WVP;
uniform highp mat4 World;
uniform highp mat4 WorldView;
uniform highp mat4 WVPInverse;
uniform highp vec4 LightPositions[128];
uniform highp vec4 LightColors[128];
uniform highp vec4 CameraPosition;
uniform highp vec4 CameraInfo;

varying highp vec2 vecUVCoords;
varying highp vec4 Pos;
varying highp vec4 PosCorner;

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
	lowp vec4 color  =  texture2D(tex0,coords);
	lowp vec4 matId  =	texture2D(tex3,coords);

	if(matId.r == 1.0 && matId.g == 0.0){
		Final = color;
	}else{
		highp float Rad = 25.0;
		highp float cutoff = 0.8;
		lowp vec4 Lambert = vec4(1.0,1.0,1.0,1.0);
		lowp vec4 Specular = vec4(1.0,1.0,1.0,1.0);
		lowp vec4 Fresnel	 =  vec4(1.0,1.0,1.0,1.0);
		lowp vec4 normalmap = texture2D(tex1,coords);
		lowp vec4 normal = normalmap*2.0 - 1.0;
		
		vec2 distor = vec2(normalmap.xy);	
		if(matId.b == 1.0){
			distor = vec2(1.0,1.0);
		}	
		lowp vec4 ReflectCol = texture2D(tex0,coords);
		
		lowp vec4 specularmap = texture2D(tex2,coords);
		normal	= normalize(normal);
		
		highp float depth = texture2D(tex4,coords).r;
		highp vec4 position = CameraPosition + PosCorner*depth;
		
		
		lowp vec3  EyeDir   = normalize(CameraPosition-position).xyz;
		
		highp int NumLights =  int(CameraInfo.w);
			for(highp int i=0;i<NumLights;i++){
				Lambert  = LightColors[i];
				Specular = LightColors[i];
				Fresnel	 = LightColors[i];			
				
				lowp  vec3  LightDir = normalize(LightPositions[i]-position).xyz;
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
				
				highp float dist = distance(LightPositions[i],position);
				highp float d = max(dist - Rad, 0.0);
				highp float denom = d/Rad + 1.0;
				
				highp float attenuation = 1.0 / (denom*denom);
				 
				attenuation = (attenuation - cutoff) / (1.0 - cutoff);
				attenuation = max(attenuation, 0.0);
     				
				Final += Lambert*attenuation;
				Final += Specular*attenuation;
			}
		if(matId.b == 0.0){
			highp float  FresnelAtt	= dot(normal.xyz,EyeDir);
			highp float  FresnelIntensity = 1.0f;
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

	gl_FragColor = Final;
}
#elif defined(FSQUAD_1_TEX)
uniform mediump sampler2D tex0;
void main(){
	gl_FragColor = texture2D(tex0,vecUVCoords);
}
#elif defined(FSQUAD_2_TEX)
uniform mediump sampler2D tex0;
uniform mediump sampler2D tex1;
void main(){
	gl_FragColor = texture2D(tex0,vecUVCoords);
}
#elif defined(FSQUAD_3_TEX)
uniform mediump sampler2D tex0;
uniform mediump sampler2D tex1;
uniform mediump sampler2D tex2;
void main(){
	gl_FragColor = texture2D(tex0,vecUVCoords);
}
#else
void main(){
	gl_FragColor = vec4(1.0,0.0,1.0,1.0);
}
#endif




