#include <math.h>


/** default: The defult equation consisting of all zeroes. */
///////////////////////////////////////////
void defaultV(double* src) {
	src[0] = -.4;
	src[1] = .8; 
	src[2] = .01; 
}


void defaultR(double* src) {
	src[0] = .3;
	src[1] = -.5;
	src[2] = -.1;
}

void defaultF(double* src) {
	src[0] = cos(t);
	src[1] = cos(t);
	src[2] = cos(t);
}
//////////////////////////////////////////////////////


/** test: An equation for testing purposes. */
//////////////////////////////////////////////////////
void testV(double* src) {
	src[0] = 1;
	src[1] = .6;
	src[2] = -4;
}

void testR(double* src) {
	src[0] = sin(r[1]);
	src[1] = cos(r[2]);
	src[2] = sin(r[0]);
}

void testF(double* src) {
	src[0] = tan(t/t_f);
	src[1] = cos(t);
	src[2] = sin(t);
}
//////////////////////////////////////////////////////
int numEqn = 2;
void (*eqns[])( double* ) = {defaultV, defaultR, defaultF, testV, testR, testF};
char* eqnNames[] = {"default", "test"};









/** DON'T MODIFY BELOW THIS LINE. */

void (*vMod)( double* );
void (*rMod)( double* );
void (*drivingF)( double* );
char name[256];

/** Retruns the index of the package NAME. */
int findFunc(char* name) {
	for (int i = 0; i < numEqn; i++) {
		char* eqnNm = eqnNames[i];
		if (strcmp(eqnNm, name) == 0) {
			return i;
		}
	}
	return -1;
}

/**Loads the proper functions from the package NAME. */
void getFunc(char* name) {
	int index = findFunc(name);
	
	if (*name == '\0') {
		printf("Running default equation\n");
		vMod = defaultV;
		rMod = defaultR;
		drivingF = defaultF;
	} else if (index == -1) {
		printf("Error couldn't load Equation: %s\n", name);
		printf("Running default equation\n");
		vMod = defaultV;
		rMod = defaultR;
		drivingF = defaultF;
	} else {
		vMod = eqns[3*index];
		rMod = eqns[3*index + 1];
		drivingF = eqns[3*index +2];
		printf("Loaded %s\n", name);
	}
}