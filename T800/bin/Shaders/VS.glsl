attribute highp vec4 Vertex;
attribute highp vec4 Normal;
attribute highp vec2 UV;

varying highp vec2 vecUVCoords;

uniform highp mat4 WVP;
uniform highp mat4 World;

void main(){
	vecUVCoords	= UV;
	vecUVCoords.y = 1.0 - vecUVCoords.y;
	gl_Position = WVP*Vertex;
}