from pylab import *
from mpl_toolkits.mplot3d import Axes3D
from numpy import *
import matplotlib.animation as animation
import sys

t_vals = []
x_vals = []
y_vals = []
z_vals = []
data = []
t_min = 0
t_max = 0
x_min = float("inf")
x_max = float("-inf")
y_min = float("inf")
y_max = float("-inf")
z_min = float("inf")
z_max = float("-inf")

# Loads the file specified by the command line argument if it exits.
def main():
	if len(sys.argv) > 1:
		loadFile(sys.argv[1])

#Loads values in the file specified by FILEPATH into t_vals, x_vals, y_vals, z_vals. 
def loadFile(filePath):
	global x_vals, y_vals, z_vals, t_vals, data, t_min, t_max, x_min, x_max, y_min, y_max, z_min, z_max
	try:
		data = open(filePath, 'r')
	except IOError, e:
		print("The requested file doesn't exist.")
		return;
		
	data = open(filePath, 'r')
	for line in data:
		items = line.split(" ")
		if len(items) != 4:
			print("Error in data stream: " + line)
		else:
			x_vals += [float(items[0])]
			y_vals += [float(items[1])]
			z_vals += [float(items[2])]
			t_vals += [float(items[3])]
	t_min = t_vals[0]
	t_max = t_vals[-1]
	for item in x_vals:
		if item > x_max:
			x_max = item
		if item < x_min:
			x_min = item
	for item in y_vals:
		if item > y_max:
			y_max = item
		if item < y_min:
			y_min = item
	for item in z_vals:
		if item > z_max:
			z_max = item
		if item < z_min:
			z_min = item


#Displays the graph of x_vals, y_vals, z_vals. 
def showGraph():
	graph = plt.figure()
	ax = graph.add_subplot(111, projection='3d')
	ax.plot(x_vals, y_vals, zs = z_vals);
	ax.set_xlabel('X axis')
	ax.set_ylabel('Y axis')
	ax.set_zlabel('Z axis')
	ax.autoscale_view()
	show()


#	Finds the point nearest to time and print the value, and returns the index.
#	Or -1 if there is an error.
def find_index(t):
	if (t_max < t or t < t_min):
		print("The requested time is out of the simulation range.")
		return -1

	length = len(t_vals)
	found = False
	high_b, low_b = length-1, 0

	while (found == False):
		index = (high_b + low_b) / 2
		guess = t_vals[index]
		
		if (high_b - low_b < 2):
			if (abs(t - t_vals[high_b]) < abs(t - t_vals[low_b])):
				return high_b
			else:
				return low_b
		if (t > guess):
			low_b = index
		if (t < guess):
			high_b = index
		if (t == guess):
			found = True
			return index

# Prints the position at time T.
def r(t):
	index = find_index(t)
	if (index != -1):
		print_vector(index)

# Returnes the indices of the set of points that satisfy COND from T_MIN to T_MAX
def find_cnd(cond, t_start = t_min, t_stop = t_max):
	indices = []
	min_t, max_t = find_index(t_min), find_index(t_max)
	for frame in range(min_t, max_t):
		if (cond(frame) == 0):
			indices += [frame]
			print("exact match!")
		elif (cond(frame - 1) * cond(frame) < 0):
			print("approx")
			if (abs(cond(frame -1)) < abs(cond(frame))):
				indices += [frame -1]
			else:
				indices += [frame]
	return indices

# Prints out the list of positinos that satisfy CND from T_MIN to T_MAX.
def print_cnd(cnd, t_start = t_min, t_stop = t_max):
	frames = find_cnd(cnd, t_start, t_stop)
	if (len(frames) == 0):
		print("There were no points that matched the condition.")
	for frame in frames:
		print_vector(frame)


# Prints out a representation of the position vector at frame FRAME.
def print_vector(frame):
	if (frame >= len(t_vals) or frame < 0):
		printf("Frame out of scope of simulation: " + frame)
	else:
		t = t_vals[frame]
		x = x_vals[frame]
		y = y_vals[frame]
		z = z_vals[frame]
		print("r({:e}) = <{:e}, {:e}, {:e}>".format(t, x, y, z))

# An example condition that finds when y == 0.
def cnd(frame):
	if (frame < 0):
		return float("NaN")
	else:
		return y_vals[frame]

# Animates the position as a function of time. LENGTH specifies the number of points 
# to add at each frame.
def animate(length = 50):
	graph = plt.figure()
	ax = graph.add_subplot(111, projection='3d')
	ax.set_xlim3d([x_min, x_max])
	ax.set_ylim3d([y_min, y_max])
	ax.set_zlim3d([z_min, z_max])
	ax.set_xlabel('X axis')
	ax.set_ylabel('Y axis')
	ax.set_zlabel('Z axis')

	deltaT = t_vals[1] - t_vals[0]
	
	def update_points(index):
		i = index * length
		ax.plot(x_vals[i:i+length], y_vals[i:i+length], zs = z_vals[i:i+length], color = 'g')

	line_ani = animation.FuncAnimation(graph, update_points, len(x_vals) / length, repeat = False, interval = 1)
	show()



main()

