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

	lowp vec4 normal = texture2D(tex1,vecUVCoords);
//	normal = normal * 2.0 - 1.0;
	normal	= normalize(normal);
	
	lowp vec2 distor = vec2(normal.xy);
	if(normal.x == 0 && normal.y == 0)
		distor = vec2(1.0,1.0);
		
	gl_FragColor = texture2D(tex0,vecUVCoords*distor);

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




