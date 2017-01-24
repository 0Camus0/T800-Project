attribute highp vec3 MyVertex;

uniform highp mat4 MyMatrix;

void main(){
	gl_Position = MyMatrix*vec4(MyVertex,1.0);
}