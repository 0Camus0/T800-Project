attribute highp vec4 Vertex;
attribute highp vec2 UV;

varying highp vec2 vecUVCoords;
varying highp vec4 Pos;
varying highp vec4 PosCorner;

uniform highp mat4 WVP;
uniform highp mat4 World;
uniform highp mat4 WorldView;
uniform highp mat4 WVPInverse;
uniform highp vec4 LightPositions[128];
uniform highp vec4 LightColors[128];
uniform highp vec4 CameraPosition;
uniform highp vec4 CameraInfo;

void main(){
	vecUVCoords = UV;	
	Pos = Vertex;
	PosCorner = WVPInverse*vec4(Vertex.xy,1.0,1.0);
	PosCorner.xyz /= PosCorner.w;
	PosCorner = PosCorner - CameraPosition;
	gl_Position = WVP*Vertex;
}