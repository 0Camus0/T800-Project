#version 100
precision highp float;

varying highp vec2 vTex;

uniform highp sampler2D texture0;//diffuse

void main(){
	highp vec4 color=texture2D(texture0,vTex);
	color.a=1.0;
    gl_FragColor=color;
}
