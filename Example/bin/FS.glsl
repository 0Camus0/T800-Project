uniform mediump sampler2D diffuse;

varying highp vec2 vecUVCoords;

void main(){
	lowp vec3 vector = texture2D(diffuse,vecUVCoords).rgb;
	gl_FragColor = vec4(vector,1.0);
}