#ifndef STRUCTS_H
#define STRUCTS_H

//----------------------------------------
#include <stdio.h>
#include <math.h>
#include<sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <string>
#include "assert.h"
#include <conio.h>
#include <time.h>
#include "vector"
#include <algorithm>

using namespace std;  //introduces namespace std

//------------------------------------------
//should be filled by user before run 
//------------------------------------------
#define bitSetNumOfMeta 16
#define bitSetNumOfReac 24

int firstMet = 15;
int firstRec = 15;

//------------------------------------------
//------------------------------------------


struct FLUX;

vector<char> IfBeingReversible;
int NumbersOfMetabolits=0, NumbersOfReactions=0;


//vector<int> outputNodes;
vector<vector<FLUX>> EFMs; 
vector<bitset<bitSetNumOfReac>> EFMsReactions;

float THEFLUX			= 0;
int THEREACTIONNAME		= -1;
int THEPARENT			= -1;
int THEMETABOLITE		= -1;
bool FINDLOOP			= false;
bool THEDETOUR			= false;
bool THEREVERSE			= false;

float TEMPFACTOR		= 0;

int THEREACTION			= -1;// the name of the reaction we do want to ADD flux to 

int THEINDEX			= -1; // the index of the metabolite which its parents wants to be checked in pack.parentsOfEachNode 

int PINDEX				= -1; // the index of the parentnode which we want to delete after updating the parent info 

//..............................
struct ArrayR{

	float	flux	[bitSetNumOfReac];		// the flux of the reaction 
	int		num		[bitSetNumOfReac];		// number of the node we want to get back to 

	bool setFlux (int m, float f){
		bool haveFlux = false;
		if (flux[m] == 0)					// improve :: 0 is not a good initial value
			flux[m] = f;
		else 
		{
			if (abs(flux[m]) != abs(f))
				haveFlux = true;
		}
		return haveFlux;
	}

	void setNum(int m, int n){
		num[m] = n;
	}

	float getFlux(int m){
		return flux[m]; 
	}

	int getNum(int m){
		return num[m]; 
	}
};
//..............................
struct ArrayRR{
	bool		nodeinfo [bitSetNumOfReac][bitSetNumOfMeta]; 
	bool		reacinfo [bitSetNumOfReac][bitSetNumOfMeta*5]; 
};
//..............................
struct SIBLINGS;
//...............................
struct RG {

	bitset<bitSetNumOfReac> reacts;
	float cof;
};
//...............................
struct reactionGroupNode{
	vector<RG> reactionGroup; 
};
//...............................
struct Boundary {
	vector<int>		Metabolits;
	vector<int>		Reactions;
};
//...............................
//...............................
struct SIBLINGS {
	vector<int>		inputSiblings;
	vector<int>		outputSiblings;
};
//...............................
struct OUTPUT;  
struct INPUT;
//...............................
struct INPUT {
	vector<int> metabolitNamesIn;
	vector<int>	metabolitNamesOut;			
	int		reacNum;
	float	Coef;
	bool	direction;

	int		type;								// added for passOne --> which type of in/out is in a path 

	bitset<bitSetNumOfMeta> 	SetPoints;		// to use for node parents	step 1'	::: just used in passOne'  ::: lastshot
	bitset<bitSetNumOfMeta*3>	SetPoints3; 

	void setParams(vector<int> m, vector<int> s, int r, float c, bool d, int t/*, bitset<bitSetNumOfMeta> sp, bitset<bitSetNumOfMeta*3> sp3*/)
	{	
		metabolitNamesIn	= m;
		metabolitNamesOut	= s; 
		reacNum = r; 
		Coef = c;
		direction = d; 
		type = t;
		/*SetPoints = sp;
		SetPoints3 = sp3; */
	}

	void setType(int t)
	{
		type = t; 
	}

};
//...............................
struct OUTPUT {
	vector<int> metabolitNamesIn;
	vector<int>	metabolitNamesOut;
	int		reacNum;
	float	Coef;
	bool	direction;

	int				type; // added for passOne --> which type of in/out is in a path 

	bitset<bitSetNumOfMeta> 	SetPoints;		// to use for node parents	step 1'	::: just used in passOne'  ::: lastshot
	bitset<bitSetNumOfMeta*5>	SetPoints5; 

	void setParams(vector<int> m, vector<int> s, int r, float c, bool d, int t/*, bitset<bitSetNumOfMeta> sp, bitset<bitSetNumOfMeta*3> sp3*/)
	{
		metabolitNamesIn	= m;
		metabolitNamesOut	= s; 
		reacNum		= r; 
		Coef		= c;
		direction	= d; 
		type		= t;
		/*SetPoints = sp;
		SetPoints3 = sp3;*/
	}

	void setType(int t)
	{
		type = t; 
	};

};
//...............................
//...............................
struct INOUT {
	vector<int> metabolitNamesIn;
	vector<int>	 metabolitNamesOut;
	int		reacNum;
	float	Coef;
	bool	direction;

	int				type; // added for passOne --> which type of in/out is in a path 

	bitset<bitSetNumOfReac> 	reactSetPoints;		// to use for reaction parents		::: just used in passOne  ::: lastshot


	void setParams(vector<int> m, vector<int> s, int r, float c, bool d, int t/*, bitset<bitSetNumOfReac> sp*/)
	{
		metabolitNamesIn	= m;
		metabolitNamesOut	= s; 
		reacNum = r; 
		Coef = c;
		direction = d; 
		type = t;
		//reactSetPoints = sp; 

	}

	void setType(int t)
	{
		type = t; 
	};

};
//...............................
struct FLUX {
	int		reationName;
	float	reactionFlux;
};
//............................... for Pass2
/*struct PATH {
vector<FLUX>	fluxes;
//	int				PathNum;
};*/

//............................... for Pass1
/*struct PATHPassOne {

vector<NODE>	nodes;
};*/
//...............................
struct NODEInform {

	// positive number shows the production/ negative number shows the consumtion / 0 already no usage 
	float			ConsumeProduct;
	int				ConsProdFromWhReac;
	int				ConsProdtFromWhMetabInReac; 

	float			ConsumeProduct2;
	int				ConsProdFromWhReac2;
	int				ConsProdtFromWhMetabInReac2; 

	int				NodeNumberInNode; 

	//int				packNumberInNode;
};
//...............................
struct NODE {
	vector<INPUT>	inputs;
	vector<OUTPUT>	outputs;
	//	vector<PATH>	paths;
	int				NodeName;

	bool			FWorBK; // for passTwo ==> TRUE: when we want to go through this node again for finding flux values 

	void Node(vector<INPUT> i, vector<OUTPUT> o, int nn, bool FB)
	{
		inputs		= i;
		outputs		= o; 
		NodeName	= nn; 
		FWorBK		= FB;
	}

	void setFWorBK(bool flag)
	{
		FWorBK = flag; 
	}

	bool getFWorBK(void)
	{
		return FWorBK; 
	}

};
//...............................
struct INNODE{
	int		NodeName;	//last shot change

	//NODE N; 

	bitset<bitSetNumOfMeta>	parentMetasTillNow;
	//	bitset<bitSetNumOfReac>	parentReacsTillNow; 

	//bitset<bitSetNumOfMeta>	parentMetasTillNowR;
	//	bitset<bitSetNumOfReac>	parentReacsTillNowR; 

	bitset<bitSetNumOfMeta>	parentMetasTillNowR; // if parentMetasTillNow && ifreverse == true 

	//bitset<bitSetNumOfMeta> ifDetour; 

	//int		lastInputReactNumber;
	//bool	ifNOTCOFACTORnode;		// remove this 


	//vector<bool>	numberOfUsedOuts; //number of used outputs for each metabolites // for each metabolite push back "true" if u used one of the outputs  

};
//...............................
struct PACKAGE {

	vector<FLUX> path;			//usage pass2
	vector <NODE> nodesPassOne;	//usage pass1

	bitset<bitSetNumOfReac> ReactionFlag;
	bitset<bitSetNumOfReac> ReactionFlagDir;

	bitset<bitSetNumOfMeta> MetabolitFlag;
	bitset<bitSetNumOfMeta> OutputsToBeChecked;
	bitset<bitSetNumOfMeta> InputsToBeChecked;
	//bitset<bitSetNumOfMeta>	loopDoubt;	// for each metabolite in the package if the flag is 1 ==> that metabolite visited more than one time from the inputs (not in parent list)
	// loop detected ==> if we saw this metbolite from the output ==> reset flag 
	// at the end if you find a path with there is at least one set flag in loopDoub ==> Discard EFM

	vector<int>		outputNode;
	vector<bool>	ifReverseFlag;
	vector<FLUX>	lastReaction;
	vector<int>		parentNode;

	//vector<bool>	ifJustInputsOutputs;
	//	int				packNum;	

	//vector<NODEInform>		NodeInform;

	//bool				ifDetourHappen;			//kolan aya dar path in etefagh oftade ya na, rabty be metabolit nadarad
	vector<INNODE>		ParentsInfoForEachNode;


	//vector<int>			orderOfFactorNodes;		//push back factor nodes ;;; if you needed to update previous recations: 
	//		1- start from the last factor node,  
	//		2- calculate the coFactor  ??? 
	//		2-1 if that node is a cofactor more than one time  
	//		3- update the parent reactions of that node,  
	bool			ifExternalOutReactSeen;


	void package();

	void SetPackage(vector<FLUX> path,		//usage pass2
		vector <NODE> nodesPassOne,	//usage pass1

		bitset<bitSetNumOfReac> ReactionFlag,
		bitset<bitSetNumOfReac> ReactionFlagDir,

		bitset<bitSetNumOfMeta> MetabolitFlag,
		bitset<bitSetNumOfMeta> OutputsToBeChecked,
		bitset<bitSetNumOfMeta> InputsToBeChecked,

		vector<int>		outputNode,
		vector<bool>	ifReverseFlag,
		vector<FLUX>	lastReaction,
		vector<int>		parentNode,

		vector<INNODE>		ParentsInfoForEachNode,

		bool				ifExternalOutReactSeen)

	{	path = path;	//usage pass2
	nodesPassOne = nodesPassOne;//usage pass1

	ReactionFlag = ReactionFlag;
	ReactionFlagDir = ReactionFlagDir; 

	MetabolitFlag = MetabolitFlag;
	OutputsToBeChecked = OutputsToBeChecked;
	InputsToBeChecked  = InputsToBeChecked;

	outputNode  = outputNode;
	ifReverseFlag = ifReverseFlag;
	lastReaction = lastReaction;
	parentNode = parentNode;

	ParentsInfoForEachNode = ParentsInfoForEachNode;

	ifExternalOutReactSeen = ifExternalOutReactSeen;
	}

	void ClearPackage()

	{	path.clear();	//usage pass2
	nodesPassOne.clear();//usage pass1

	ReactionFlag.reset();
	ReactionFlagDir.reset(); 

	MetabolitFlag.reset();
	OutputsToBeChecked.reset();
	InputsToBeChecked.reset();

	outputNode.clear();
	ifReverseFlag.clear();
	lastReaction.clear();
	parentNode.clear();

	ParentsInfoForEachNode.clear();

	ifExternalOutReactSeen = false;
	}

};

//...............................
struct PACKAGEONE {

	//vector<FLUX> path;			//usage pass2
	vector <NODE> nodesPassOne;		//usage pass1

	vector <NODE> reactDependSetPoint;	// dependency check

	bitset<bitSetNumOfReac> ReactionFlag;
	bitset<bitSetNumOfReac> ReactionFlagDir;

	bitset<bitSetNumOfMeta> MetabolitFlag;
	bitset<bitSetNumOfMeta> OutputsToBeChecked;
	bitset<bitSetNumOfMeta> InputsToBeChecked;
	//bitset<bitSetNumOfMeta>	loopDoubt;	// for each metabolite in the package if the flag is 1 ==> that metabolite visited more than one time from the inputs (not in parent list)
	// loop detected ==> if we saw this metbolite from the output ==> reset flag 
	// at the end if you find a path with there is at least one set flag in loopDoub ==> Discard EFM

	vector<int>		outputNode;
	vector<bool>	ifReverseFlag;
	vector<INOUT>	lastReaction;
	vector<int>		parentNode;

	//vector<bool>	ifJustInputsOutputs;
	//	int				packNum;	

	//vector<NODEInform>		NodeInform;

	//bool				ifDetourHappen;			//kolan aya dar path in etefagh oftade ya na, rabty be metabolit nadarad
	vector<INNODE>		ParentsInfoForEachNode;


	//vector<int>			orderOfFactorNodes;		//push back factor nodes ;;; if you needed to update previous recations: 
	//		1- start from the last factor node,  
	//		2- calculate the coFactor  ??? 
	//		2-1 if that node is a cofactor more than one time  
	//		3- update the parent reactions of that node,  
	bool			ifExternalOutReactSeen;


	bool 	itPassedRevIf;				// to use for priorities		== > if the parent goes to "reversible else" one time keep that ::  just used in passOne	:: lastshot


	void package();

	void SetPackage(//vector<FLUX> path,		//usage pass2
		vector <NODE> nodesPassOne,	//usage pass1
		vector <NODE> reactDependSetPoint,

		bitset<bitSetNumOfReac> ReactionFlag,
		bitset<bitSetNumOfReac> ReactionFlagDir,

		bitset<bitSetNumOfMeta> MetabolitFlag,
		bitset<bitSetNumOfMeta> OutputsToBeChecked,
		bitset<bitSetNumOfMeta> InputsToBeChecked,

		vector<int>		outputNode,
		vector<bool>	ifReverseFlag,
		vector<INOUT>	lastReaction,
		vector<int>		parentNode,

		vector<INNODE>		ParentsInfoForEachNode,

		bool				ifExternalOutReactSeen, 
		bool 	itPassedRevIf)

	{	//	p.path = path;	//usage pass2
		nodesPassOne = nodesPassOne;//usage pass1
		reactDependSetPoint = reactDependSetPoint;

		ReactionFlag = ReactionFlag;
		ReactionFlagDir = ReactionFlagDir; 

		MetabolitFlag = MetabolitFlag;
		OutputsToBeChecked = OutputsToBeChecked;
		InputsToBeChecked  = InputsToBeChecked;

		outputNode  = outputNode;
		ifReverseFlag = ifReverseFlag;
		lastReaction = lastReaction;
		parentNode = parentNode;

		ParentsInfoForEachNode = ParentsInfoForEachNode;

		ifExternalOutReactSeen = ifExternalOutReactSeen;
		itPassedRevIf  = itPassedRevIf;
	}

	void ClearPackage()

	{	//p.path.clear();	//usage pass2
		nodesPassOne.clear();//usage pass1
		reactDependSetPoint.clear();

		ReactionFlag.reset();
		ReactionFlagDir.reset(); 

		MetabolitFlag.reset();
		OutputsToBeChecked.reset();
		InputsToBeChecked.reset();

		outputNode.clear();
		ifReverseFlag.clear();
		lastReaction.clear();
		parentNode.clear();

		ParentsInfoForEachNode.clear();

		ifExternalOutReactSeen = false;
		itPassedRevIf = false;
	}

};
//*************************************************************************************************
struct PASSONEOUTPUT
{
	vector<vector<NODE>>	paths;
	vector<bitset<bitSetNumOfReac>> ReactionFlagDir;
	vector<bitset<bitSetNumOfReac>> ReactionFlag;

};

#endif /*STRUCTS_H*/