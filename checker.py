import numpy as np
f = open("random.txt", "r")
nums = f.read().split()
dim = input("Enter dim")
a = nums[:dim*dim]
b = nums[dim*dim:2*dim*dim]
a = np.reshape(a, (dim,dim))
b = np.reshape(b, (dim,dim))

#print(a)
#print(b)

#c = np.matmul(a,b)
#print(c.diagonal())
print a
print b
print np.matmul(a,b)
