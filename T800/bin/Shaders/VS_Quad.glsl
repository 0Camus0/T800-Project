attribute highp vec4 Vertex;
attribute highp vec2 UV;

varying highp vec2 vecUVCoords;
varying highp vec4 wPos;

uniform highp mat4 WVP;
uniform highp mat4 World;

void main(){
	vecUVCoords = UV;
	wPos = WVP*Vertex;
	gl_Position = wPos;
}