#version 100
precision highp float;

attribute highp vec3 cVer;
attribute highp vec3 cNor;
attribute highp vec3 cTan;
attribute highp vec3 cBin;
attribute highp vec2 cTex;

varying highp vec2 vTex;
varying highp vec3 vNor;
varying highp vec3 vBin;
varying highp vec3 vPixelPos;
varying highp float varyingIntensity;
varying highp mat3 varyingTBN;

uniform highp mat4 uMat0;//Model
uniform highp mat4 uMat1;//Projection
uniform highp vec4 uVec0;

void main(){
	vTex=vec2(cTex.x,cTex.y);
	//vNor=(cNor+vec3(1.0))/vec3(2.0);
	highp mat3 normalMatrix=mat3(uMat0[0].xyz,uMat0[1].xyz,uMat0[2].xyz);
	vNor=normalMatrix*cNor;
	vBin=normalMatrix*cBin;

	varyingTBN=(mat3(
        normalize(normalMatrix*cTan),
        normalize(normalMatrix*-cBin),
        normalize(normalMatrix*-cNor))
    );

	//Position
	highp vec4 finalPos=vec4(cVer,1.0);
	finalPos=uMat0*finalPos;
	vPixelPos=finalPos.xyz;
	gl_Position=uMat1*finalPos;

    highp vec3 lightPosition=normalize(uVec0.xyz-finalPos.xyz);
	highp vec3 normalizedNormal=normalize(normalMatrix*cNor);
	varyingIntensity=dot(lightPosition,normalizedNormal);
}
