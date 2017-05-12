#version 100
precision highp float;

varying highp vec2 vTex;
varying highp vec3 vNor;
varying highp vec3 vBin;
varying highp vec3 vPixelPos;
varying highp float varyingIntensity;
varying highp mat3 varyingTBN;

uniform highp sampler2D texture0;//diffuse
uniform highp sampler2D texture1;//normal
uniform highp samplerCube texture2;//cube
uniform highp vec4 uVec0;

void main(){
    highp float intensityNormal=uVec0.w;
	highp vec3 lightPosition=normalize(uVec0.xyz-vPixelPos);
	highp vec3 normalizedNormal=normalize(vNor);

	highp float intensity=dot(lightPosition,normalizedNormal);

	highp vec3 colorDiffuse=texture2D(texture0,vTex).xyz;
    highp vec3 normalTex=texture2D(texture1,vTex).xyz;
    normalTex.x=1.0-normalTex.x;

    highp vec3 normalTexVector=normalTex*vec3(2.0)-vec3(1.0);
    highp vec3 normalTexVectorTBN=varyingTBN*normalTexVector;
    highp vec3 colorCube=textureCube(texture2,mix(normalizedNormal,normalTexVectorTBN,intensityNormal)).xyz;

    intensity=dot(lightPosition,normalTexVectorTBN);
    intensity=clamp(intensity,0.0,1.0);
    //gl_FragColor=vec4(mix(colorDiffuse,vec3(colorCube),0.5),1.0);
    gl_FragColor=vec4(vec3(colorCube),1.0);
	//gl_FragColor=vec4(mix(colorDiffuse,vec3(1.0),intensity),1.0);//vec3(intensity*varyingIntensity)*normalTexVector.rgb
}
