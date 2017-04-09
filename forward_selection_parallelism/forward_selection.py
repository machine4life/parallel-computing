import numpy as np
from sklearn import datasets,linear_model

def forward_selection(train,ini_target,cols):
	epoches = len(cols)
	coefs = []
	target  = ini_target
	for i in range(epoches):
		cosin = -20
		sel_col = 10000
		for j in cols:
			den = np.sqrt(np.sum(np.square(train[:,j]))) * np.sqrt(np.sum(np.square(target)))                
			tmp = np.dot(train[:,j],target) / den
			if tmp > cosin:
				cosin = tmp
				sel_col = j
		cols.remove(sel_col)
		proj_vec= np.dot(target,train[:,sel_col])/np.sum(np.square(train[:,sel_col])) * train[:,sel_col]
		proj_len= np.dot(target,train[:,sel_col])/np.sqrt(np.sum(np.square(train[:,sel_col])))
		target = target - proj_vec
		coefs.append( [sel_col,proj_len] ) 
	return coefs

if __name__ == '__main__':
	
	diabetes = datasets.load_diabetes()
	train = diabetes.data
	target = diabetes.target
	
	# coefs without grouped method
	coefs_01 = forward_selection(train,target,cols = [i for i in range(0,10)])
	print 'coefs_01:\n',coefs_01,'\n','-'*40
	
	
	# coefs with grouped method
	feature_num = 10
	group_mem_num = 2
	for i in range(0,feature_num,group_mem_num):
		if i + group_mem_num < 10:
			coefs = forward_selection(train,target,cols = [ i for i in range(i,i + group_mem_num) ])
		else:
			coefs = forward_selection(train,target,cols = [ i for i in range(i,feature_num) ])

		print 'coefs:\n',coefs,'\n','-'*40
