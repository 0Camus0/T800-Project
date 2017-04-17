varying highp vec2 vecUVCoords;
varying highp vec4 wPos;

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
	
	lowp vec4 normal = texture2D(tex1,coords);
	normal	= normalize(normal);
	
	if(normal.x == 0 && normal.y == 0){
		gl_FragColor = texture2D(tex0,coords);
	}else{		
		lowp vec2 distor = vec2(normal.xy);
		lowp vec2 coord2 = vecUVCoords*distor;
		coord2.y = 1.0 - coord2.y;	
		gl_FragColor = texture2D(tex0,coord2);
	}

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




