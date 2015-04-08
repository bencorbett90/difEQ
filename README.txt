# difEQ v0.1
Numerically solves linear differential equations.

  difEQ numerically solves linear differential equations for given initial conditions. That is difEQ solves the following system of equations for the vector r(t) = <x(t), y(t), z(t)>:
                                      r'' + Q * r' + P * r = F;
                                      r(0) = <x(0), y(0), z(0)>;
                                      r'(0) = <x'(0), y'(0), z'(0)>;
  In the above notation ' denotes the total time derivative and * is the dot product of two vectors. In order to specify a particular initial value problem the user has to provide the three functions Q, P, and F which specify the equation as well as r(0) and r'(0) which are the initial conditions. 

  difEQ numerically aproximates the solution to the initial value problem (IVP) by using the provided differential equation (DEQ) and the initial conditions to compute r''. Once it has r'' it can then taylor expand r about the current time and use this to estimate r and r' at a deltaT in the future. Then it recalculates r'' based on the new values of r and r' and the process over again. This process of estimating r and r', then using those values to solve for r'' is called a frame. If we let r_n be the r vector in frame n, and deltaT the time between frames then we can write down a recursive formula for r_(n+1):
                                     r_(n+1) += .5 r''_n * deltaT^2 + r'_n * deltaT
                                     r'_(n+1) += r''_n * deltaT
  It is easy to see that the initial conditions r(0) and r'(0) are constants and only refrenced during the very first frame. In contrast Q, R, F are not constants and are called on every frame. Inorder to be more accurate the number of frames is usually quite large (easily on the order of 10^9) it is imperative that the modifiers Q, P, F be quite fast. For this reason in order to change the modifiers you must recompile the entire difEQ.c source code after changing the functions. Making this easier is a ongoing project.


Compiling and running:
  difEQ was written using GCC in c99 and Anaconda python on Windows 7, you will need to have these installed in order to run difEQ. If you install Anaconda in a location other than the default C:\Anaconda you will need to change the system call      at the bottom of difEQ to the correct path. While not tested, difEQ should work on Linux by simply changing all the path     arguments to their correct location.
    
  I have included a very primitive version of make by which to compile difEQ.c. It will take any number of command line arguments and simply copy those over to it's call to gcc. I HIGHLY reccoment compiling with the -O3 or -Ofast flags, as these decrease run time by a very significant factor (around 5x in my experience). To compile make.c itself you should use the command "gcc make.c -o make -std=c99".
  
  As mentioned above you will have to recompile difEQ.c each time you change any of the modifiers, however the initial conditions can be fed in as file using the command "difEQ filepath" where filepath is the location of the input file. Input files follow the following format:
                                       t_i, t_f, deltaT, storeFrac
                                       x_0 y_0 z_0
                                       x'_0 y'_0 z'_0
The following characters are ignored and may be added in to the input file without issue: "<>," as well as whitespace and tabs.


