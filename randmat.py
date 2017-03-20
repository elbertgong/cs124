import random
dim = input("Enter dim of matrices ")

f = open('random.txt','a')

for i in range(0,2*dim*dim):
	f.write('\n'.join('{0}'.format(random.randint(0,3)))+'\n')