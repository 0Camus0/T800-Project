varying highp vec3 NormalTransformed;

void main(){
	highp vec3 normal = normalize(NormalTransformed*0.5 + 0.5);
	gl_FragColor = vec4(normal,1.0);
}