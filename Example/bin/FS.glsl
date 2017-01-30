varying highp vec4 NormalTransformed;

void main(){
	highp vec4 normal = normalize(NormalTransformed*0.5 + 0.5);
	normal.w = 1.0;
	gl_FragColor = normal;
}