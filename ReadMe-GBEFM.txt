This is a code which has been Written for 
"Graph-Based Elementary Flux Mode Analysis ==> GB-EFM"

**For simplicity there are 3 files:
	
	1- EFMFinder.cpp ==> contains the "main" function and the basic functions used in "main"
	2- structs.h ==> contains all used structures 
	3- funcs.cpp ==> contains all inside functions of basic functions 
	
**The following variables should be set by user in the file before compiling and running the program 
(This will be improved in an upcoming versions by a better user interface)	

in EFMFinder.cpp :: 

inFile1 = "inputs\\Sample.txt";				// input stoichiometric matrix in "input" folder
outFile1 = "outputs\\Sample_report.txt";		// output :: pathways topology (after Step1 and Step2) and EFMs (after Step3 and Step4) at the end of the file

in structs.h :: 

#define bitSetNumOfMeta 16				// number of metabolites
#define bitSetNumOfReac 25				// number of reactions

int firstMet = 15;					// the index of the particular metabolite we want to find EFMs from
int firstRec = 15;					// the index of the boundary reaction of the metabolite above