#version 100
precision highp float;

attribute highp vec2 cVer;
attribute highp vec2 cTex;

uniform highp mat4 uMat0;//Model
uniform highp mat4 uMat1;//Projection

varying highp vec2 vTex;

void main(){
	vTex=vec2(cTex.x,cTex.y);
	//Position
	highp vec4 finalPos=vec4(cVer,0.0,1.0);
	finalPos=uMat0*finalPos;
	gl_Position=uMat1*finalPos;
}
