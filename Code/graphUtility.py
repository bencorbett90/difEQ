from pylab import *
from mpl_toolkits.mplot3d import Axes3D
from numpy import *

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

def loadFile(filePath):
	global x_vals, y_vals, z_vals, t_vals, data, t_min, t_max
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


def showGraph():
	graph = plt.figure()
	ax = graph.add_subplot(111, projection='3d')
	ax.plot(x_vals, y_vals, zs = z_vals);
	ax.set_xlabel('X axis')
	ax.set_ylabel('Y axis')
	ax.set_zlabel('Z axis')
	ax.autoscale_view()
	show()

def main():
	loadFile(sys.argv[1])

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

def print_r(t):
	index = find_index(t)
	if (index != -1):
		print_r(index)

#	Find the 
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

def print_cnd(cnd, t_start = t_min, t_stop = t_max):
	frames = find_cnd(cnd, t_start, t_stop)
	if (len(frames) == 0):
		print("There were no points that matched the condition.")
	for frame in frames:
		print_r(frame)

def print_r(frame):
	if (frame >= len(t_vals) or frame < 0):
		printf("Frame out of scope of simulation: " + frame)
	else:
		t = t_vals[frame]
		x = x_vals[frame]
		y = y_vals[frame]
		z = z_vals[frame]
		print("r({:e}) = <{:e}, {:e}, {:e}>".format(t, x, y, z))

def cnd(frame):
	if (frame < 0):
		return float("NaN")
	else:
		return y_vals[frame]



main()