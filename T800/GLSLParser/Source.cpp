#include <iostream>

#include <fstream>

using namespace std;

#include "GLSLParser.h"

int main()
{

	GLSL_Parser parser_("Anim.vert","Anim.frag");
	parser_.Parse();

	cout << "Attributes:" << endl;
	for (int i = 0; i < parser_.attributes.size() ; i++){
		cout << parser_.attributes[i].name << endl;
	}
	cout << endl;

	cout << "Varyings:" << endl;
	for (int i = 0; i < parser_.varying.size(); i++) {
		cout << parser_.varying[i].name << endl;
	}
	cout << endl;

	cout << "Uniforms:" << endl;
	for (int i = 0; i < parser_.uniforms.size(); i++) {
		cout << parser_.uniforms[i].name << endl;
	}
	cout << endl;

	return 0;
}