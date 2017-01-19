attribute highp vec3 MyVertex;

void main(){
	gl_Position = vec4(MyVertex,1.0);
}