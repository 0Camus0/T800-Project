attribute highp vec3 Vertex;
attribute highp vec3 Normal;
attribute highp vec2 UV;

varying highp vec3 NormalTransformed;
varying highp vec2 UVInterpolated;
varying highp vec4 ClipSpace;

uniform highp mat4 WVP;
uniform highp mat4 World;

void main(){
	NormalTransformed	= Normal; //(World*vec4(Normal,1.0)).xyz;
	UVInterpolated		= UV;
	ClipSpace			= WVP*vec4(Vertex,1.0);
	gl_Position = ClipSpace;
}