#/usr/bin/python3
# PLOTS SPEED UP VS RETRIEVED ELEMENTS FOR BOTH

import matplotlib.pyplot as plt
import numpy as np

test1 = np.loadtxt("uniform.txt")
test2 = np.loadtxt("tulips.txt")

dim=7
temp1=test1[(test1[:,0]==dim)]
temp2=test2[(test2[:,0]==dim)]
plt.loglog(temp1[::5,1]/1e7, temp1[::5,3]/temp1[::5,2], '--*k')
plt.loglog(temp2[::5,1]/7990272, temp2[::5,3]/temp2[::5,2], '-ok')
plt.legend(("Uniform Database","Non Uniform Database"))
plt.xlim((1e-5,1))
plt.ylim((1e-2,10))
plt.xlabel("Proportion of retreived elements")
plt.ylabel("Relative Speed")

plt.savefig("fig04.pdf")
# plt.show()