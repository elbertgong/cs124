import numpy as np
f = open("random.txt", "r")
nums = f.read().split()
dim = input("Enter dim ")
dim = int(dim)
a = nums[:dim*dim]
b = nums[dim*dim:2*dim*dim]
a = np.reshape(a, (dim,dim))
b = np.reshape(b, (dim,dim))
a = a.astype(int)
b = b.astype(int)

#print(a)
#print(b)
print np.matmul(a,b)
#print(c.diagonal())
