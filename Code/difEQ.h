char* description = "difEQ version 0.1\n";
int LINESIZE = 256; 
double R_0[] = {0.0, 0.0, 0.0}; //The initial position
double V_0[] = {1.0, 10.0, 10.0}; //The initial velocity
double T_INIT = 0.0; //The start time
double T_FIN = 11.0; //The end time
double DELTAT = 1E-6; //The time step for approximation.
double STOREFRAC = 1E4; //The fraction of points that are saved.
int useTime = 1; //If 1 return the time taken for various processes.
int showProgress = 0; //If 1 show the progress bar when computing. Slows down by a factor of 2.
char* output = "data\\points.txt"; //The name of the file to write to.


double r[] = {0.0, 0.0, 0.0}; //The position vector
double v[] = {0.0, 0.0, 0.0}; //The velocity vector
double a[] = {0.0, 0.0, 0.0}; //The acceleration vector
double t_i; //The time the simulation starts at.
double t_f; //The time the simulations ends at.
int storeFrac; //The fraction of points that are saved.
double deltaT; //The time step for approximation.
double t; //The current time
double* points; //The collection of points written into.
long long numSamples; //The number of samples stored in POINTS.
long long iteration; //The current iteration depth.
int index; //The current index in POINTS.
