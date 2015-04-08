#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "difEQ.h"



/** Sets up the initial conditions, including solving for the initial acceleration. */
void initialize(double* r_0, double* v_0, double t_initial, double t_final, 
		double dt, int sf) {
	for (int i = 0; i < 3; i++) {
		r[i] = r_0[i];
		v[i] = v_0[i];
	}
	t_i = t_initial;
	t_f = t_final;
	t = t_initial;
	deltaT = dt;
	storeFrac = sf;
	numSamples = ceil(ceil((t_f - t_i)/deltaT + 1) / storeFrac);
	iteration = 0;
	index = 0;
	double srcR[3];
	double srcV[3];
	double srcF[3];
	rMod(srcR);
	vMod(srcV);
	drivingF(srcF);
	for (int j = 0; j < 3; j++) {
		a[j] = srcF[j] - srcV[j]*v[j] - srcR[j]*r[j];
	}
}

/** Performs a step. Writes the new R, V, A and updates t to t + deltaT. */
void step() {
	for (int i = 0; i < 3; i++) {
		r[i] = .5*a[i]*deltaT*deltaT + v[i]*deltaT + r[i];
		v[i] = v[i] + a[i]*deltaT; 
	}
	double srcR[3];
	double srcV[3];
	double srcF[3];
	rMod(srcR);
	vMod(srcV);
	drivingF(srcF);
	for (int j = 0; j < 3; j++) {
		a[j] = srcF[j] - srcV[j]*v[j] - srcR[j]*r[j];
	}
	iteration++;
	t += deltaT;
}


/** Read the position at time nearest to T into POS
	and return the actual time of the nearest point. */
double querry(double t, double* pos) {
	if (t > t_f || t < t_i) {
		return -1;
	}
	int i = 3*round(t / (deltaT * storeFrac));
	pos[0] = points[i];
	pos[1] = points[i+1];
	pos[2] = points[i+2];
	return i/3 * deltaT * storeFrac;
}


void store() {
	points[index++] = r[0];
	points[index++] = r[1];
	points[index++] = r[2];
}

void write_data() {
	clock_t curTime;
	if (useTime) {
		curTime = clock(); 
	} if (showProgress) {
		printf("Writing data to disk:  0%%");
		FILE* data = fopen(output, "w");
		double pos[3];
		int i = 0;
		for (double T = t_i; T < t_f; T += deltaT * storeFrac) {
			if (i % 100 == 0) {
				int percent = round((T - t_i) * 100 / (deltaT * (t_f - t_i)));
				printf("\rWriting data to disk: %02d", percent);
			}
			double t = querry(T, pos);
			fprintf(data, "%f %f %f %f\n", pos[0], pos[1], pos[2], t);
			i++;
		}
		fclose(data);
	} else {
		printf("Writing data to disk...");
		FILE* data = fopen(output, "w");
		double pos[3];
		for (double T = t_i; T < t_f; T += deltaT * storeFrac) {
			double t = querry(T, pos);
			fprintf(data, "%f %f %f %f\n", pos[0], pos[1], pos[2], t);
		}
		fclose(data);
	}
	printf("\rWriting data to disk: Done\n");
	
	if (useTime) {
		curTime = clock() - curTime;
		printf("%f seconds writing data to disk\n", (float) curTime / CLOCKS_PER_SEC);
	}
}

/* Reads STR as line LINE of the input file and does the appropriate
	operations with the values. Return 1 if any error and 0 if none. */
int readLine(char* str, int line) {
	char* ignorechars = "<>, /\t";
	
	char* item = strtok(str, ignorechars);
	int err = 0;
	double value;
	if (line == 1) {
		for (int i = 0; i < 4; i++) {
			if (item == NULL) {
				printf("Line 1: not enough arguments.\n");
				return 1;
			} 
			value = atof(item);
			if (i == 0) t_i = value;
			if (i == 1) t_f = value;
			if (i == 2) deltaT = value;
			if (i == 3) storeFrac = value;
			item = strtok(NULL, ignorechars);
		}
		if (t_i >= t_f) {
			printf("Line 1: t_i > t_f\n");
			return 1;
		} if (deltaT == 0) {
			printf("Line 1: deltaT is 0");
			return 1;
		} 
	} if (line == 2 || line == 3) {
		for (int i = 0; i < 3; i++) {
			if (item == NULL) {
				printf("Line %d: not enough arguments.\n", line);
				return 1;
			} 
			value = atof(item);
			if (line == 2) {
				r[i] = value;
			} if (line == 3) {
				v[i] = value;
			} 
			item = strtok(NULL, ignorechars);
		}
	}
	return 0;
}

/** Loads the initial condions from INPUT. */
int loadConditions(FILE* input) {
	char line[LINESIZE];
	int lineNum = 0;
	int err = 0;
	while(fgets(line, sizeof(line), input)) {
		lineNum++;
		err += readLine(line, lineNum);
	}
	return err == 0 ? 0 : 1;

}

/** Prints the standard info for this simulation. */
void printInfo() {
	printf(description);
	printf("Start time: %f\tStop time: %f\tDeltaT: %e\n", t_i, t_f, deltaT);
	printf("Initial position = <%f, %f, %f>\n", r[0], r[1], r[2]);
	printf("Initial velocity = <%f, %f, %f>\n", v[0], v[1], v[2]);
	printf("Show Time: %s\tShow Progress: %s\n", useTime == 1? "yes" : "no", showProgress == 1? "yes" : "no");
	printf("Number of sample points = %d\n", numSamples);
}


int main(int argc, char **argv) {
	if (argc == 2) {
		FILE* input = fopen(argv[1], "r");
		if (input == NULL) {
			printf("The file doesn't exist.\nUsing preset conditions.\n");
			initialize(R_0, V_0, T_INIT, T_FIN, DELTAT, STOREFRAC);
		} else if (loadConditions(input) == 1) {
			printf("The file wasn't formatted correctly.\nUsing preset conditions.\n");
			initialize(R_0, V_0, T_INIT, T_FIN, DELTAT, STOREFRAC);
		}
	fclose(input);
	} else {
		initialize(R_0, V_0, T_INIT, T_FIN, DELTAT, STOREFRAC);
	}


	initialize(r, v, t_i, t_f, deltaT, storeFrac);
	points = malloc(sizeof(double) * 3 * numSamples);
	printInfo();
	if (points == NULL) {
		printf("Not enough memory to store all %d points.", numSamples);
		return 1;
	}


	
	clock_t curTime;
	if (useTime) {
		curTime = clock(); 
	}
	if (showProgress == 1) {
		double numFrames = (t_f - t_i) / deltaT;
		printf("\nCalculating:  0%%");
		while (t <= t_f) {
			if (iteration % storeFrac == 0) {
				store();
			} if (iteration % (storeFrac * 10) == 0) {		
				int percent = round(iteration * 100 / numFrames); 
				printf("\rCalculating: %02d", percent);
			}
			step();
		}
	} else {
		printf("\nCalculating...");
		while (t <= t_f) {
			if (iteration % storeFrac == 0) {
				store();
			}	
			step();
		}
	}

	printf("\rCalculating: Done\n");

	if (useTime) {
		curTime = clock() - curTime;
		printf("%f seconds calculating\n", (float) curTime / CLOCKS_PER_SEC);
		printf("%e seconds per frame\n", (float) curTime / (CLOCKS_PER_SEC * iteration));
	}

	write_data();
	free(points);
	char cmd[LINESIZE];
	sprintf(cmd, "C:\\Anaconda\\python -i graphUtility.py %s", output); //Apend Start if running in new window.
	system(cmd);
	return 0;
}