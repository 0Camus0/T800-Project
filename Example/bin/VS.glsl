attribute highp vec3 MyVertex;
attribute lowp vec3 MyColor;

varying highp vec3 VertexColor;

uniform highp mat4 MyMatrix;

void main(){
	VertexColor	= MyColor;
	gl_Position = MyMatrix*vec4(MyVertex,1.0);
}