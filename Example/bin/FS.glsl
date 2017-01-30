varying highp vec3 NormalTransformed;
varying highp vec2 UVInterpolated;
varying highp vec4 ClipSpace;

void main(){
	highp vec3 normal = normalize( NormalTransformed*0.5 + 0.5 );
	highp vec2 uv = UVInterpolated;
	gl_FragColor = vec4(normal,1.0);	
	//highp float depth = ClipSpace.z/ClipSpace.w;
	//gl_FragColor = vec4(depth,depth,depth,1.0);	
}