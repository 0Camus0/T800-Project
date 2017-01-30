attribute highp vec4 Vertex;
attribute highp vec4 Normal;
attribute highp vec2 UV;

varying highp vec4 NormalTransformed;

uniform highp mat4 WVP;
uniform highp mat4 World;

void main(){
	NormalTransformed	= World*Normal;
	gl_Position = WVP*Vertex;
}