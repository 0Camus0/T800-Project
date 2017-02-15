attribute highp vec4 Vertex;


#ifdef USE_NORMALS
attribute highp vec4 Normal;
#endif

#ifdef USE_TANGENTS
attribute highp vec4 Tangent;
#endif

#ifdef USE_BINORMALS
attribute highp vec4 Binormal;
#endif

#ifdef USE_TEXCOORD0
attribute highp vec2 UV;
#endif

varying highp vec3 vecTransformed;

#ifdef USE_TEXCOORD0
varying highp vec2 vecUVCoords;
#endif

uniform highp mat4 WVP;
uniform highp mat4 World;

void main(){
#ifdef USE_NORMALS
	vecTransformed	= normalize(mat3(World)*vec3(Normal));
#else
	vecTransformed	= normalize(mat3(World)*Vertex.xyz);
#endif

#ifdef USE_TEXCOORD0
	vecUVCoords = UV;
	vecUVCoords.y = vecUVCoords.y;
#endif

	gl_Position = WVP*Vertex;
}