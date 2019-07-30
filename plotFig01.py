#/usr/bin/python3
# PLOTS SPEED UP VS DIMENSION FOR BOTH DATABASE
import matplotlib.pyplot as plt
import numpy as np

test1 = np.loadtxt("fig01-uniform.txt")
test2 = np.loadtxt("fig01-tulips.txt")

plt.semilogy(test1[:,0],test1[:,3]/test1[:,2],'--*k')
plt.semilogy(test2[:,0],test2[:,3]/test2[:,2],'-ok')
plt.legend(("Uniform Database","Non Uniform Database"))
plt.xlabel("Number of dimensions")
plt.ylabel("Relative Speed")

plt.show()