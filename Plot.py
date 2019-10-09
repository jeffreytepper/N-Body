from mpl_toolkits.mplot3d import Axes3D  
import matplotlib.pyplot as plt

filepath = 'out.txt'
with open(filepath) as fp:
   line = fp.readline()
   
   x = []
   y = []
   z = []
   while line:
       line = fp.readline()
       if len(line) > 9:
           pos = " ".join(line.split(":")[1].replace(" | ","|").split("|")[0].strip().split()).split(" ")
           x.append(float(pos[0]))
           y.append(float(pos[1]))
           z.append(float(pos[2]))
        
x1= x[0:len(x):3]
y1= y[0:len(y):3]
z1= z[0:len(z):3]

x2= x[1:len(x):3]
y2= y[1:len(y):3]
z2= z[1:len(z):3]

x3= x[2:len(x):3]
y3= y[2:len(y):3]
z3= z[2:len(z):3]

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x1, y1, z1)
plt.title("Body 1")
plt.savefig("body1.pdf")

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x2, y2, z2)
plt.title("Body 2")
plt.savefig("body2.pdf")

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x3, y3, z3)
plt.title("Body 3")
plt.savefig("body3.pdf")
