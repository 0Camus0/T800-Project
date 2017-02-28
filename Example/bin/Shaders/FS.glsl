uniform highp sampler2D diffuse;

varying highp vec2 vecUVCoords;

void main(){
	highp vec3 vector = texture2D(diffuse,vecUVCoords).rgb;
	gl_FragColor = vec4(vector,1.0);
}