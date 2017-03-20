import random
dim = input("Enter dim of matrices ")
m1 = input("Enter min matrix value ")
m2 = input("Enter max matrix value ")

f = open('random.txt','a')

for i in range(0,2*dim*dim):
	f.write('\n'.join('{0}'.format(random.randint(m1,m2)))+'\n')