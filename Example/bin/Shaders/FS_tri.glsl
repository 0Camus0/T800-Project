varying highp vec3 VertexColor;

void main(){
	gl_FragColor = vec4(VertexColor,1.0);	
}