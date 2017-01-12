#include "Core.h"

#if USING_COUT
#include <iostream>
using namespace std;
#else
#include <stdio.h>
#endif

void func() {
#if USING_COUT
	cout << "Funcion de mi libreria " << endl;
#else
	printf("Funcion de mi libreria\n");
#endif
}