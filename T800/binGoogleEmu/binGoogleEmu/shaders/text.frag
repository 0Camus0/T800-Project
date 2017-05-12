#version 100
precision highp float;

varying highp vec2 vTex;

uniform highp sampler2D texture0;//diffuse

void main(){
	highp float alpha=texture2D(texture0,vTex).a;
	highp vec4 color=vec4(vec3(1.0),alpha);
    gl_FragColor=color;
}
