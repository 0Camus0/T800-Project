attribute highp vec4 Vertex;


#ifdef USE_NORMALS
attribute highp vec4 Normal;
varying highp vec4 hnormal;
#endif

#ifdef USE_TANGENTS
attribute highp vec4 Tangent;
varying highp vec4 htangent;
#endif

#ifdef USE_BINORMALS
attribute highp vec4 Binormal;
varying highp vec4 hbinormal;
#endif

#ifdef USE_TEXCOORD0
attribute highp vec2 UV;
#endif


#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif

varying highp vec4 Pos;
varying highp vec4 WorldPos;

uniform highp mat4 WVP;
uniform highp mat4 World;
uniform highp mat4 WorldView;
uniform highp vec4 LightPos;
uniform highp vec4 LightColor;
uniform highp vec4 CameraPosition;
uniform highp vec4 CameraInfo;
uniform highp vec4 Ambient;

void main(){
	mat3 RotWorld = mat3(World);
#ifdef USE_NORMALS
	hnormal	= vec4(normalize(RotWorld*vec3(Normal)),1.0);
#endif

#ifdef USE_TANGENTS
	htangent	= vec4(normalize(RotWorld*vec3(Tangent)),1.0);
#endif

#ifdef USE_BINORMALS
	hbinormal	= vec4(normalize(RotWorld*vec3(Binormal)),1.0);
#endif

	Pos 	 = WorldView*Vertex;
	WorldPos = World*Vertex;
	
#ifdef USE_TEXCOORD0
	vecUVCoords = UV;
	vecUVCoords.y = vecUVCoords.y;
#endif

	gl_Position = WVP*Vertex;
}