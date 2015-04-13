# difEQ v2.0.1-beta
Numerically solves linear differential equations.
Note: See documentation.pdf for a full description.

difEQ will numerically solve any second order linear differential equation in three dimensions.
You will need GCC and Anaconda python in order to run difEQ.

Once in the proper directory simply compile make using "gcc make.c -o make -std=c99" then once make is successfully compiled enter "make -Ofast". Now you are all set, simply enter "difEQ filepath" where filepath is the directory of the input file. A input file consists of 3 lines specifying the following conditions in the following format:
start time, end time, deltaT, store fraction
x_0, y_0, z_0
x'_0, y'_0, z'_0
which can be followed by an optional 4th line containing a string specifying the name of the equation package to be solved. The current only available packages are called "default" and "test".

However it is very easy to add a package. Simply add a new set of three equations to "equations.c", increment the numEQ variable, add pointers to your equations to the eqns[] variable, and add your equations package name to the eqnNames[] variable. Recompile difEQ using "make -Ofast" and then you can simply call your package from the input file as normal.  

Once the equation has been solved an interactive python terminal will open, this is where you can interact with the data. Enter showGraph() and a graph of the solution will appear. Enter animate() to animate the function. You can also find the specific position at any time by entering r(t) where t is the time in question. You can also use print_cnd() to find points that satisfy a condition that you specify. Feel free to add new functionality be editing the graphUtility.py file, and unless you want to consistently define conditions in the terminal, this is how you will define new conditions.

Tips
* If the calculation phase is taking a long time, decrease the number of simulation frames. This can be done by increasing deltaT or by decreasing the range of the simulation time.
* If writing the data to disk is taking a long time, decrease the total number of stored points.This can be done by increasing deltaT, increasing the store fraction, or by decreasing the range of the simulation time.
* If the animation is going to slow or too fast try using animate(x), which adds x points per frame instead of the default 50.
