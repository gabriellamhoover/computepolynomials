# computepolynomials
#//	Compile with:
//	$ g++ polynomialComputer.cpp -o polynomialComputer -lpthread
//
//	To test this program in a stand-alone fashion, do:
//	$ ./polynomialComputer < floatList.txt 
//
//	The output (to both stdout and stderr) should be:
//	x = 0.5
//	1 = 1
//	2*0.5 + 1 = 2
//	3*0.5^2 + 2*0.5 + 1 = 2.75
//	0.5 2.75
//	x = 1
//	1 = 1
//	2*1 + 1 = 3
//	3*1^2 + 2*1 + 1 = 6
//	1 6
//	x = 1.5
//	1 = 1
//	2*1.5 + 1 = 4
//	3*1.5^2 + 2*1.5 + 1 = 10.75
//	1.5 10.75
//	x = 2
//	1 = 1
//	2*2 + 1 = 5
//	3*2^2 + 2*2 + 1 = 17
//	2 17
//	Stopping readX()
//	Stopping constantTerm()
//	Stopping linearTerm()
//	Stopping squaredTerm()
