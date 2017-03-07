//----------------------	GB-EFM Analysis (Graph-Based Elementary Flux Mode Analysis)
//----------------------	Mona Arabzadeh, Amirkabir University of Technology
//----------------------	Fall 2015 -- Winter 2017 
//----------------------	PhD -- partOne


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
#include <vector>
#include <algorithm>
#include <bitset>
#include <iterator>

#include "structs.h"
#include "funcs.cpp"

using namespace std;  //introduces namespace std

//-------------------------------------------------------------- PreDefined Functions -Begin
vector <vector <float>> readFileS (istream & fin);
vector<NODE> CreateNetwork(vector<vector<float>> S, vector<NODE>& network);				//Step_0

PASSONEOUTPUT passOne(vector<NODE>& network, istream &fin);								//Step_1
PASSONEOUTPUT eliminateSamePaths(PASSONEOUTPUT PassOne);					

PASSONEOUTPUT eliminateMultiWayPaths(PASSONEOUTPUT PassOne);							//Step_2

void printPassOne (vector<vector<NODE>> pathsPassOne, ofstream &fout);
void printPassOneRec (vector<bitset<bitSetNumOfReac>> pathsPassOne, ofstream &fout);	

void passTwo (PASSONEOUTPUT passOne);													//Step_3 and Step_4
void printEFMs(ofstream &fout);
//-------------------------------------------------------------- PreDefined Functions -END
//---------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------Main Part-------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
	//----------------------------------------------  variables -BEGIN
	char timeStr [9];

	string inFile1,inFile2, outFile1, outFile2;
	ifstream *fin, *fin2;
	ofstream *fout, *fout2;

	vector<vector<float>> S;
	vector<NODE> network;

	PASSONEOUTPUT PassOne;
	//----------------------------------------------  variables -END


	_strtime_s(timeStr);
	printf( "\nThe current time is %s \n\n", timeStr);

	//---------------------------------------------- 
	//should be filled by user before run 
	//----------------------------------------------
	inFile1 = "inputs\\Sample.txt";					// input stoichiometric matrix in "input" folder
	outFile1 = "outputs\\Sample_report.txt";		// output :: pathways topology (after Step1 and Step2) and EFMs (after Step3 and Step4) at the end of the file
	
	//outFile2 = "outputs\\Sample_test.txt"; 

	fin = new ifstream(inFile1.c_str());
	fin2 = new ifstream(inFile2.c_str());

	fout = new ofstream(outFile1.c_str());
	fout2 = new ofstream(outFile2.c_str());

	//-----------------------------------------------------------------------------	Read the input stoichiometric matrix
	S = readFileS(*fin);
	//-----------------------------------------------------------------------------	Convert the S matrix to network
	network = CreateNetwork(S, network);
	//-----------------------------------------------------------------------------	passOne :: is Step1
	PassOne = passOne(network, *fin2);
	//----------------------------------------------
	
	_strtime_s(timeStr);
	printf( "\nThe current time is %s \n\n", timeStr);
	//------------------------------------------------------------------------------ Eliminate same paths 
	PassOne = eliminateSamePaths(PassOne);
	//----------------------------------------------

	_strtime_s(timeStr);
	printf( "\nThe current time is %s \n\n", timeStr);
	printf("number of PassOne after eliminating same paths = %d", PassOne.paths.size());
	//----------------------------------------------

	PassOne = eliminateMultiWayPaths(PassOne);	
	//------------------------------------------------------------------------------- eliminateExtraPaths :: is Step 2 

	_strtime_s(timeStr);
	printf( "\nThe current time is %s \n\n", timeStr);
	printf("number of PassOne after eliminating MultiWay paths = %d", PassOne.paths.size());
	//----------------------------------------------

	printPassOne (PassOne.paths, *fout);
	//----------------------------------------------
	printPassOneRec (PassOne.ReactionFlag, *fout2);
	//----------------------------------------------

	_strtime_s(timeStr);
	printf( "\nThe current time is %s \n\n", timeStr);
	//-------------------------------------------------------------------------------	passTwo :: is Step 3 and Step 4 
	passTwo(PassOne);

	//----------------------------------------------
	_strtime_s(timeStr);
	printf( "\nThe current time is %s \n\n", timeStr);
	//----------------------------------------------
	printEFMs(*fout); 

	//----------------------------------------------
	_strtime_s(timeStr);
	printf( "\nThe current time is %s \n\n", timeStr);

	//----------------------------------------------
	printf("Press any key to continue...");
	_getch();

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------Main FUNCTIONS -------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------
vector <vector <float>> readFileS (istream & fin)
{
	vector <vector <float>> S;
	vector<float> temp;
	float n = 0;
	char c;

	fin>>NumbersOfMetabolits;
	fin>>NumbersOfReactions;

	for (int l=0 ; l<NumbersOfReactions; l++){
		fin >> c;
		IfBeingReversible.push_back(c);
	}

	for (int k=0 ; k<NumbersOfMetabolits; k++){
		temp.clear();
		for (int h=0 ; h<NumbersOfReactions; h++){
			fin >> n;
			temp.push_back(n);
		}
		S.push_back(temp);
	}
	return S;
}
//*************************************************************************************************
vector<NODE> CreateNetwork(vector<vector<float>> S, vector<NODE>& network){

	vector<vector<int>> source;

	for (int i=0; i < NumbersOfMetabolits; i++)
	{
		NODE node;
		node.NodeName = i;
		for (int j=0; j < NumbersOfReactions; j++)
		{
			vector<float> temp = S.at(i);
			source = FindSource(S, j, temp.at(j));
			// reversibility check 
			if (IfBeingReversible.at(j) == '0') //if (IfBeingReversible.at(j) == 'i')			<<<<<<< irreversible == 0 >>>>>
			{
				if (temp.at(j)>0)
				{
					INPUT input;
					input.setParams(source.at(0), source.at(1), j, temp.at(j), false, 0);
					node.inputs.push_back(input);
				}
				else if (temp.at(j)<0)
				{
					OUTPUT output;
					output.setParams(source.at(1), source.at(0), j, temp.at(j), false, 0);
					node.outputs.push_back(output);
				}
			} //end if not reversible
			else  // if (IfBeingReversible.at(j) == 'r')										 <<<<< reversible == 1 >>>>>
			{
				if (temp.at(j)>0)
				{
					INPUT input;
					input.setParams(source.at(0), source.at(1), j, temp.at(j), false, 0);
					node.inputs.push_back(input);

					OUTPUT output;
					output.setParams(source.at(1), source.at(0), j, /*-*/temp.at(j), true, 0);		// if reaction reversible ::: if coefficient > (like input) ::::: but if it's output
					node.outputs.push_back(output);
				}
				else if (temp.at(j)<0)
				{
					INPUT input;
					input.setParams(source.at(0), source.at(1), j, /*-*/temp.at(j), true, 0);		// if reaction reversible ::: if coefficient < (like output) ::::: but if it's input 
					node.inputs.push_back(input);
					OUTPUT output;
					output.setParams(source.at(1), source.at(0), j, temp.at(j), false, 0);
					node.outputs.push_back(output);
				}

			}// end if reversible

		}// for j

		network.push_back(node);
	}// for i
	return network;
}
//*************************************************************************************************
//************************************************************************************************* PASSOne :: STEP_1
//*************************************************************************************************
PASSONEOUTPUT passOne(vector<NODE>& network, istream &fin)
{

	PASSONEOUTPUT			passOneOutput;
	int CandidateNodeName	=	-1;
	int heyYou				=	-1;


	vector<INNODE> parentsOfEachNode;

	vector<FLUX>			CandidateNodePath;
	vector<NODE>			CandidateNodeNodesPassOne;
	vector<int>				Boundary_Metabolits;
	vector<PACKAGEONE>		packages;						// the whole packages we have till now 
	vector<PACKAGEONE>		tempPackages;					// the packages we just got from this node and we have to add to the other packages we have  
	PACKAGEONE				tempPack;						// it's a temp variable just for passing the information we got from packages.at(0)  


	//Boundary bound = PreFunction(network, fin);			// Read the file ;;; the external metabolites are pointing to boundary metabolites, push back them and you have the reaction names  

	Boundary_Metabolits.push_back(firstMet); 
	THEREACTIONNAME = firstRec;								// first reaction number 

	for (int i=0; i<(int)/*bound.Metabolits*/Boundary_Metabolits.size(); i++) //ooooooooooooooooooooooooooooooooooooooooooooooooooooooooo This "for" Can be executed in parallel devices 
	{
		//printf( "Boundry node = %d \n\n", i);

		//to comment
		//CandidateNodeName = bound.Metabolits.at(i); //network.at(CandidateMatabol);		
		//THEREACTIONNAME = bound.Reactions.at(i);
		//to comment

		CandidateNodeName = Boundary_Metabolits.at(i);
		THEMETABOLITE = CandidateNodeName; 
		THEREVERSE = false; 
		THEPARENT = -1;

		//clearrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr alllllllllllllllllllllll

		printf( "Clearing Flags and temporary paths ... \n\n");
		packages.clear();	
		tempPack.ClearPackage();

		bool inWhile = true;
		bool notDone = true;

		///5555555555555555555555555555555555555555555555555555555555555555555555555555555555
		//beforehandFunction();// set other outputs and "related stuff" and outputtobechecked 
		/*int g = 0;
		while(bound.Reactions.at(g) == THEREACTIONNAME)
		{
		CandidateOutput.push_back(bound.Metabolits.at(g));
		OUTPUTtoBeCHECKED.set(bound.Metabolits.at(g));
		ReverseVector.push_back(false);
		CandidateNodeName_parent.push_back(-1);
		FLUX f; f.reactionFlux = 1; f.reationName = THEREACTIONNAME; 
		CandidateLastReaction.push_back(f);

		bound.Reactions.erase(bound.Reactions.begin() + g);
		bound.Metabolits.erase(bound.Metabolits.begin() + g);
		//g++;
		}
		delete &g;*/ 
		///2222222222222222222222222222222222222222222222222222222222222222222222222222222222

		while(inWhile)	//we have unfinished packages .... 
		{

			if (notDone){
				//printf( "\n We have unfinished packages ...\n");
				printf( "\n N = %d\n", THEMETABOLITE);

				//------------------------------------------------------------------------------
				//------------------------------------------------------------------------------
				//------------------------------------------------------------------------------
				tempPackages = InNodeFunctionPassOne(network.at(THEMETABOLITE), tempPack);
				//------------------------------------------------------------------------------
				//------------------------------------------------------------------------------
				//------------------------------------------------------------------------------

				//printf( "\n Number of produced packages in this node = %d \n", tempPackages.size());
				packages = Packages_Add(packages, tempPackages, notDone);
				tempPackages.clear();
			}

			if (packages.size()==0)										// if package.size()==0
			{
				printf( "\n There is no packages, we are done! \n\n");
				inWhile = false;
				break;
			} //................................................................................................
			else if (AllOut(packages.at(0).outputNode))					// if the package is complete ==>> outputNode "empty or -1" 
			{
				//............
				if(packages.at(0).ifExternalOutReactSeen) // means it reached to an external reaction --> Path ends and its not a cycle path
				{	
					passOneOutput.paths.push_back(packages.at(0).nodesPassOne);
					passOneOutput.ReactionFlagDir.push_back(packages.at(0).ReactionFlagDir);
					passOneOutput.ReactionFlag.push_back(packages.at(0).ReactionFlag);
					printf("NofP = %d", passOneOutput.paths.size());

					/*if ( passOneOutput.paths.size() >= 20000)
						return passOneOutput;*/

				}
				//.............
				// what to do :: break the while and do not come in
				packages.erase(packages.begin());
				notDone = false;

			}//..................................................................................................
			else 													// if we have unseen nodes in package
			{
				// finding not -1 output
				for (int g=0; g<(int)packages.at(0).outputNode.size(); g++)
				{
					if (packages.at(0).outputNode.at(g)!=-1)
					{
						heyYou = g;
						break;
					}
				}
				// finding not -1 output

				// ......... assigning
				THEMETABOLITE	=	packages.at(0).outputNode.at(heyYou);
				THEREVERSE		=	packages.at(0).ifReverseFlag.at(heyYou);
				THEREACTIONNAME = packages.at(0).lastReaction.at(heyYou).reacNum; 
				THEPARENT = packages.at(0).parentNode.at(heyYou);

				// ........  modifying 
				packages.at(0).outputNode.erase(packages.at(0).outputNode.begin()		+ heyYou);	
				packages.at(0).ifReverseFlag.erase(packages.at(0).ifReverseFlag.begin() + heyYou);
				packages.at(0).lastReaction.erase(packages.at(0).lastReaction.begin()	+ heyYou);
				packages.at(0).parentNode.erase(packages.at(0).parentNode.begin()		+ heyYou);

				//.........  setting 
				tempPack.SetPackage (
					tempPack.nodesPassOne = packages.at(0).nodesPassOne,//usage pass1
					tempPack.reactDependSetPoint = packages.at(0).reactDependSetPoint,
					tempPack.ReactionFlag = packages.at(0).ReactionFlag,
					tempPack.ReactionFlagDir = packages.at(0).ReactionFlagDir, 
					tempPack.MetabolitFlag = packages.at(0).MetabolitFlag,
					tempPack.OutputsToBeChecked = packages.at(0).OutputsToBeChecked,
					tempPack.InputsToBeChecked = packages.at(0).InputsToBeChecked,
					tempPack.outputNode = packages.at(0).outputNode,
					tempPack.ifReverseFlag = packages.at(0).ifReverseFlag,
					tempPack.lastReaction = packages.at(0).lastReaction,
					tempPack.parentNode = packages.at(0).parentNode,
					tempPack.ParentsInfoForEachNode = packages.at(0).ParentsInfoForEachNode,
					tempPack.ifExternalOutReactSeen = packages.at(0).ifExternalOutReactSeen,
					tempPack.itPassedRevIf = packages.at(0).itPassedRevIf
					);

				notDone = true; // should go insdie the if(notDone)

				// ......... cleaning 
				packages.erase(packages.begin());
			}//...................................................................................................

		}//--------------------end while(1)
	}//end for											 //ooooooooooooooooooooooooooooooooooooooooooooooooooooooooo This for Can execute in parallel devices
	return passOneOutput; 
}				
//*************************************************************************************************
PASSONEOUTPUT eliminateSamePaths(PASSONEOUTPUT PassOne)
{

	for (int i = 0; i < (int)PassOne.ReactionFlag.size()-1; i++)
	{
		for (int j = i+1; j < (int)PassOne.ReactionFlag.size(); j++)
		{
			bitset<bitSetNumOfReac> R1 = PassOne.ReactionFlag.at(i); 
			bitset<bitSetNumOfReac> R2 = PassOne.ReactionFlag.at(j);

			if (R1 == R2)
			{
				PassOne.ReactionFlag.erase(PassOne.ReactionFlag.begin()			+ j);
				PassOne.paths.erase(PassOne.paths.begin()						+ j);
				PassOne.ReactionFlagDir.erase(PassOne.ReactionFlagDir.begin()	+ j);
			}
		}
	}
	return PassOne;
}
//************************************************************************************************* STEP_2
PASSONEOUTPUT eliminateMultiWayPaths(PASSONEOUTPUT PassOne)
{
	for (int i=0; i<(int)PassOne.paths.size(); i++)
	{
		ArrayRR RR; 
		memset(RR.nodeinfo, false, sizeof RR.nodeinfo ); 
		memset(RR.reacinfo, false, sizeof RR.reacinfo );

		//----------

		vector<int> parents;			// size of all nodes in one path 		
		for (int u=0; u<(int)PassOne.paths.at(i).size(); u++)		{parents.push_back(-1);}	// initialization

		vector<int> visited;			// size of all nodes in one path 		
		for (int u=0; u<(int)PassOne.paths.at(i).size(); u++)		{visited.push_back(false);}	// initialization

		vector<int> FWorBW;				// size of all nodes in one path 		
		for (int u=0; u<(int)PassOne.paths.at(i).size(); u++)		{FWorBW.push_back(true);}	// initialization

		bitset<bitSetNumOfReac> RF = false;

		//----------
		bool goIn = false;
		for (int j=0; j<(int)PassOne.paths.at(i).size(); j++)
		{
			//......
			if ((PassOne.paths.at(i).at(j).inputs.size() > 1) && (PassOne.paths.at(i).at(j).outputs.size() > 1))
			{
				goIn = true;
				visited.at(j) = true;
			}
			//...... 
			if (FWorBW.at(j)){

				if (PassOne.paths.at(i).at(j).outputs.size() > 1)
				{
					// save the parents for the next loop
					for (int g = 0; g < (int)PassOne.paths.at(i).at(j).outputs.size(); g++)
					{
						for (int f = 0; f < (int)PassOne.paths.at(i).at(j).outputs.at(g).metabolitNamesOut.size(); f++)
						{
							int ii = findIndeces(PassOne.paths.at(i), PassOne.paths.at(i).at(j).outputs.at(g).metabolitNamesOut.at(f));
							parents.at(ii) = PassOne.paths.at(i).at(j).outputs.at(g).reacNum;
						}

						for (int f = 0; f < (int)PassOne.paths.at(i).at(j).outputs.at(g).metabolitNamesIn.size(); f++)
						{
							if (PassOne.paths.at(i).at(j).outputs.at(g).metabolitNamesIn.at(f) != PassOne.paths.at(i).at(j).NodeName) {
								int ii = findIndeces(PassOne.paths.at(i), PassOne.paths.at(i).at(j).outputs.at(g).metabolitNamesIn.at(f));
								parents.at(ii) = PassOne.paths.at(i).at(j).outputs.at(g).reacNum;
								FWorBW.at(ii) = false;
							}
						}
					}
					int p  = parents.at(j); // parent reaction for that node

					int count = 0; 
					for(int k=0; k<(int)PassOne.paths.at(i).at(j).outputs.size(); k++)
					{
						RF.set(PassOne.paths.at(i).at(j).outputs.at(k).reacNum);
						RR.nodeinfo[PassOne.paths.at(i).at(j).outputs.at(k).reacNum][PassOne.paths.at(i).at(j).NodeName] = true;
						RR.reacinfo[PassOne.paths.at(i).at(j).outputs.at(k).reacNum][(PassOne.paths.at(i).at(j).NodeName*5) + count] = true;
						count ++; 
						if (p != -1){

							for (int x=0; x<bitSetNumOfMeta; x++)
								RR.nodeinfo[PassOne.paths.at(i).at(j).outputs.at(k).reacNum][x] |= RR.nodeinfo[p][x];

							for (int x=0; x<bitSetNumOfMeta*5; x++)
								RR.reacinfo[PassOne.paths.at(i).at(j).outputs.at(k).reacNum][x] |= RR.reacinfo[p][x]; 
						}
					}
				} //end if

				else // else  !> 1		""just transfer the info of last reaction""  and "update parent for output metabolites"
				{
					int p  = parents.at(j); // parent reaction for that node
					
					for (int f=0; f<(int)PassOne.paths.at(i).at(j).outputs.at(0).metabolitNamesOut.size(); f++)
					{
						int ii = findIndeces(PassOne.paths.at(i), PassOne.paths.at(i).at(j).outputs.at(0).metabolitNamesOut.at(f));
						parents.at(ii) = PassOne.paths.at(i).at(j).outputs.at(0).reacNum;
					}

					for (int f=0; f<(int)PassOne.paths.at(i).at(j).outputs.at(0).metabolitNamesIn.size(); f++)
					{
						if ( PassOne.paths.at(i).at(j).outputs.at(0).metabolitNamesIn.at(f) !=  PassOne.paths.at(i).at(j).NodeName){
							int ii = findIndeces(PassOne.paths.at(i), PassOne.paths.at(i).at(j).outputs.at(0).metabolitNamesIn.at(f));
							parents.at(ii) = PassOne.paths.at(i).at(j).outputs.at(0).reacNum;
							FWorBW.at(ii) = false;
						}
					}

					if (p != -1){
						RF.set(PassOne.paths.at(i).at(j).outputs.at(0).reacNum);
						for (int x=0; x<bitSetNumOfMeta; x++)
							RR.nodeinfo[PassOne.paths.at(i).at(j).outputs.at(0).reacNum][x] |= RR.nodeinfo[p][x];
						for (int x=0; x<bitSetNumOfMeta*5; x++)
							RR.reacinfo[PassOne.paths.at(i).at(j).outputs.at(0).reacNum][x] |= RR.reacinfo[p][x]; 	
					}
				} //end else 

			} //------------------------------------------------------- end if FW
			else if (!FWorBW.at(j))// else if backward 
			{
				if (PassOne.paths.at(i).at(j).inputs.size() > 1)
				{
					// save the parents for next loop
					for (int g = 0; g < (int)PassOne.paths.at(i).at(j).inputs.size(); g++)
					{
						for (int f = 0; f < (int)PassOne.paths.at(i).at(j).inputs.at(g).metabolitNamesIn.size(); f++)
						{
							int ii = findIndeces(PassOne.paths.at(i), PassOne.paths.at(i).at(j).inputs.at(g).metabolitNamesIn.at(f));
							parents.at(ii) = PassOne.paths.at(i).at(j).inputs.at(g).reacNum;
							FWorBW.at(ii) = false;
						}

					for (int f = 0; f < (int)PassOne.paths.at(i).at(j).inputs.at(g).metabolitNamesOut.size(); f++)
						{
							if (PassOne.paths.at(i).at(j).inputs.at(g).metabolitNamesOut.at(f) != PassOne.paths.at(i).at(j).NodeName) {
								int ii = findIndeces(PassOne.paths.at(i), PassOne.paths.at(i).at(j).inputs.at(g).metabolitNamesOut.at(f));
								parents.at(ii) = PassOne.paths.at(i).at(j).inputs.at(g).reacNum;
							}
						}

					}

					int p  = parents.at(j); // parent reaction for that node

					int count = 0; 
					for(int k=0; k<(int)PassOne.paths.at(i).at(j).inputs.size(); k++)
					{
						RF.set(PassOne.paths.at(i).at(j).inputs.at(k).reacNum);
						RR.nodeinfo[PassOne.paths.at(i).at(j).inputs.at(k).reacNum][PassOne.paths.at(i).at(j).NodeName] = true;
						RR.reacinfo[PassOne.paths.at(i).at(j).inputs.at(k).reacNum][(PassOne.paths.at(i).at(j).NodeName*5) + count] = true;	
						count ++; 

						if (p != -1){

							for (int che=0; che<bitSetNumOfMeta; che++)
							{RR.nodeinfo[PassOne.paths.at(i).at(j).inputs.at(k).reacNum][che] |=	RR.nodeinfo[p][che];}

							for (int che=0; che<bitSetNumOfMeta*5; che++)
							{RR.reacinfo[PassOne.paths.at(i).at(j).inputs.at(k).reacNum][che] |=	RR.reacinfo[p][che]; }
						}
					}
				} //end if

				else // else  !> 1		""just transfer the info of last reaction"" 
				{
					int p  = parents.at(j); // parent reaction for that node

					for (int f=0; f<(int)PassOne.paths.at(i).at(j).inputs.at(0).metabolitNamesIn.size(); f++)
					{
						int ii = findIndeces(PassOne.paths.at(i), PassOne.paths.at(i).at(j).inputs.at(0).metabolitNamesIn.at(f));
						parents.at(ii) = PassOne.paths.at(i).at(j).inputs.at(0).reacNum;
						FWorBW.at(ii) = false;
					}
					
					for (int f=0; f<(int)PassOne.paths.at(i).at(j).inputs.at(0).metabolitNamesOut.size(); f++)
					{
						if ( PassOne.paths.at(i).at(j).inputs.at(0).metabolitNamesOut.at(f) !=  PassOne.paths.at(i).at(j).NodeName){
							int ii = findIndeces(PassOne.paths.at(i), PassOne.paths.at(i).at(j).inputs.at(0).metabolitNamesOut.at(f));
							parents.at(ii) = PassOne.paths.at(i).at(j).inputs.at(0).reacNum;
						}
					}


					if (p != -1){
						RF.set(PassOne.paths.at(i).at(j).inputs.at(0).reacNum);
						for (int x=0; x<bitSetNumOfMeta; x++)
							RR.nodeinfo[PassOne.paths.at(i).at(j).inputs.at(0).reacNum][x] |= RR.nodeinfo[p][x];
						for (int x=0; x<bitSetNumOfMeta*5; x++)
							RR.reacinfo[PassOne.paths.at(i).at(j).inputs.at(0).reacNum][x] |= RR.reacinfo[p][x]; 	
					}
				} //end else 

			}//------------------------------------- end else BK
		} //end for each node j  :: 1

		//--------- for those node with more than two inputs and outputs do 
		/////////////////////////////////////////////////////////////////////////......CrossCheck.......\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

		if (goIn) {
			bool contin = true;
			for (int j=0; j<(int)PassOne.paths.at(i).size(); j++)
			{
				if (!contin)
					break;	//suppose to break j 


				if (visited.at(j) && FWorBW.at(j))
				{
					bool crossCheck =  false;

					//phaze 1: check each input with others (AND nodeinfo)
					// for each common node make a cover (OR reacinfo)
					// go to outputs with that cover :: check each output with others for just the common node 
					// if found the common node check => (sudoko check :: between reacinfo)
					// if true :: crossCheck = true, break :: break which loop?? go to -> if(crossCheck)

					for (int p = 0; p < (int)PassOne.paths.at(i).at(j).inputs.size(); p++)
					{
						for (int q = p+1; q < (int)PassOne.paths.at(i).at(j).inputs.size(); q++)
						{
							for (int pq = 0; pq < bitSetNumOfMeta; pq++)
							{
								if (RR.nodeinfo[PassOne.paths.at(i).at(j).inputs.at(p).reacNum][pq] && RR.nodeinfo[PassOne.paths.at(i).at(j).inputs.at(q).reacNum][pq])
								{
									//.................
									//make the cover 
									bitset <5> cover, covertmp;
									for (int c=0; c<5; c++)
										cover[c] = RR.reacinfo[PassOne.paths.at(i).at(j).inputs.at(p).reacNum][pq*5+c] | RR.reacinfo[PassOne.paths.at(i).at(j).inputs.at(q).reacNum][pq*5+c]; 

									vector< bitset <5>> coverVec;
									for (int pp = 0; pp < (int)PassOne.paths.at(i).at(j).outputs.size(); pp++)
									{
										if (RR.nodeinfo[PassOne.paths.at(i).at(j).outputs.at(pp).reacNum][pq])
										{
											for (int c=0; c<5; c++)
											{
												covertmp[c] = RR.reacinfo[PassOne.paths.at(i).at(j).outputs.at(pp).reacNum][pq*5+c];
											}
											coverVec.push_back(covertmp);
										}
									}
									//crosscheck
									if (coverVec.size() > 1)
										crossCheck = crossCheckFunc(cover, coverVec); 	
									if (crossCheck)
										break;

									
									//.................
								}// end if
							}// end for pq

						}//end for q
					}//end for p
					//............................................
					if(crossCheck)
					{
						PassOne.paths.erase(PassOne.paths.begin() + i); 
						PassOne.ReactionFlag.erase(PassOne.ReactionFlag.begin() + i);
						PassOne.ReactionFlagDir.erase(PassOne.ReactionFlagDir.begin() + i); 
						contin = false;
						i--;
						break; // which loop ??
					}

				}// end if visited
				else if (visited.at(j) && (!FWorBW.at(j))) // switch input and outputs ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
				{
					bool crossCheck =  false;

					//phaze 1: check each input with others (AND nodeinfo)
					// for each common node make a cover (OR reacinfo)
					// go to outputs with that cover :: check each output with others for just the common node 
					// if found the common node check => (sudoko check :: between reacinfo)
					// if true :: crossCheck = true, break :: break which loop?? go to -> if(crossCheck)

					for (int p = 0; p < (int)PassOne.paths.at(i).at(j).outputs.size(); p++)
					{
						for (int q = p+1; q < (int)PassOne.paths.at(i).at(j).outputs.size(); q++)
						{
							for (int pq = 0; pq < bitSetNumOfMeta; pq++)
							{
								if (RR.nodeinfo[PassOne.paths.at(i).at(j).outputs.at(p).reacNum][pq] && RR.nodeinfo[PassOne.paths.at(i).at(j).outputs.at(q).reacNum][pq])
								{
									//.................
									//make the cover 
									bitset <5> cover, covertmp;
									for (int c=0; c<5; c++)
										cover[c] = RR.reacinfo[PassOne.paths.at(i).at(j).outputs.at(p).reacNum][pq*5+c] | RR.reacinfo[PassOne.paths.at(i).at(j).outputs.at(q).reacNum][pq*5+c]; 

									vector< bitset <5>> coverVec;
									for (int pp = 0; pp < (int)PassOne.paths.at(i).at(j).inputs.size(); pp++)
									{
										if (RR.nodeinfo[PassOne.paths.at(i).at(j).inputs.at(pp).reacNum][pq])
										{
											for (int c=0; c<5; c++)
											{
												covertmp[c] = RR.reacinfo[PassOne.paths.at(i).at(j).inputs.at(pp).reacNum][pq*5+c];
											}
											coverVec.push_back(covertmp);
										}
									}
									//crosscheck
									if (coverVec.size() > 1)
										crossCheck = crossCheckFunc(cover, coverVec);

									if (crossCheck)
										break;
									//.................
								}// end if
							}// end for pq

						}//end for q
					}//end for p
					//............................................
					if(crossCheck)
					{
						PassOne.paths.erase(PassOne.paths.begin() + i); 
						PassOne.ReactionFlag.erase(PassOne.ReactionFlag.begin() + i);
						PassOne.ReactionFlagDir.erase(PassOne.ReactionFlagDir.begin() + i); 
						contin = false;
						i--;
						break; // which loop ??
					}

				}// end else if visited

			} //end for each node j  :: 2

		}// end if goIn
		//---------
	}//end for each path i
	return PassOne; 
}
//*************************************************************************************************
void printPassOne (vector<vector<NODE>> pathsPassOne, ofstream &fout) {

	for (int i=0; i<(int)pathsPassOne.size(); i++)	// each path
	{
		vector<NODE> p = pathsPassOne.at(i);
		//printf("*************************************************\n\n");
		fout << "*************************************************\n\n";
		fout << i << "\n"; 

		for (int j=0; j<(int)p.size(); j++)			//each node in a path
		{
			NODE n = p.at(j);

			//print rx ty ::: NODE x :: rx ty ::: \n 
			for (int k=0; k<(int)n.inputs.size(); k++)	//each input
			{
				//printf("r%d t%d ::: ",n.inputs.at(k).reacNum,n.inputs.at(k).type);
				fout << "r"<<n.inputs.at(k).reacNum<< "t" << n.inputs.at(k).type <<"::: ";
			}

			//printf( "NODE <==> %d ::: ", n.NodeName);
			fout << "NODE <==>" <<n.NodeName << "\t" << n.getFWorBK() << "::: ";

			for (int k=0; k<(int)n.outputs.size(); k++)	//each output
			{
				//printf("r%d t%d ::: ",n.outputs.at(k).reacNum,n.outputs.at(k).type);
				fout << "r" << n.outputs.at(k).reacNum << "t" << n.outputs.at(k).type << ":::";
			}

			//printf("\n-------------------------\n");
			fout << "\n-------------------------\n";
		}// end for each node in a path

	}// end for each path
	fout << "\n\n"; 
}
//*************************************************************************************************
void printPassOneRec (vector<bitset<bitSetNumOfReac>> pathsPassOne, ofstream &fout) {

	for (int i=0; i<(int)pathsPassOne.size(); i++)	// each path
	{
		bitset<bitSetNumOfReac> p = pathsPassOne.at(i);

		for (int j=0; j<(int)p.size(); j++)			//each node in a path
		{
			fout << p.at(j) <<"\t";
		}
		fout << "\n"; 
	}// end for each path
}
//*************************************************************************************************
//************************************************************************************************* PASSTwo :: STEP_3 and STEP_4
//*************************************************************************************************
void passTwo (PASSONEOUTPUT passOne)	//EFMs ==> vector<vector<FLUX>> 
{
	for (int i=0; i<(int)passOne.paths.size(); i++)	//for each passOne <<<..................Parallel execution.....................>>
	{
		vector<FLUX> path; 
		vector<NODE> NodeS = passOne.paths.at(i);


		vector<bool> visitedFlag;			// size of all nodes in one path 		
		for (int u=0; u<(int)NodeS.size(); u++)		{visitedFlag.push_back(false);}	// initialization

		vector<bool> visitAgain;			// size of all nodes in one path 		
		for (int u=0; u<(int)NodeS.size(); u++)		{visitAgain.push_back(false);}	// initialization

		vector<int> nodeCaseVec;			// size of all nodes in one path
		for (int u=0; u<(int)NodeS.size(); u++)		{nodeCaseVec.push_back(-1);}	// initialization

		vector<int> waitList;				// saving array of [waiting [node,j] pairs 
		ArrayR R;							// size of the number of all Rs (or more as I set)
		memset(R.flux, 0, bitSetNumOfReac * sizeof(R.flux[0]));		// improve :: 0 is not a good initial for flux 
		memset(R.num, -1, bitSetNumOfReac * sizeof(R.num[0]));

		//------initializeFluxToStart

		notEFM = R.setFlux(firstRec, 1) ;	

		//------initializeFluxToStart

		// step 3	==> go for all nodes; sign nodes with type 2,3,4,5, more than two 9s (you will go through them again to findWayAndSync)  
		// step 3'  ==> wait in nodes with more than two 8s ==> how: put a flag for the r when got a value continue back the node  :: done in step1 
		// step 4   ==> go for all signed nodes   
		// Deghat: check for non-EFM pasths => reactions which gets two flux (how? i dont know) 

		// DataBase ======>>>	Array for Rs: index: r number, flux, flag(node num)	  

		//---------------------------------
		//--------------------------- step3
		//---------------------------------

		for (int j=0; j<(int)NodeS.size(); j++)
		{
			//=================================================
			bool CR = checkReady(NodeS.at(j), R);				//if the node input/output (FW/BK) reaction flux is available :: one is enough?    i think yes 

			if (CR)
			{
				//visit the node =>> 0- check if visited before (when calc set the visitedflag) 1- which case 2- do the case stuff 
				if (!visitedFlag.at(j))
				{						
					int nodeCase = caseRecog(NodeS.at(j));		
					nodeCaseVec.at(j) = nodeCase;			//::::::: improve ::::: save nodecase and use it for step3

					//caseStuff(); 
					if (((nodeCase & 13)!= 0) && ((nodeCase & 2)== 0))		// 1101 & n == 0 means visitAgain happened
					{
						visitAgain.at(j) = true; 
						visitedFlag.at(j) = true;
						R = fluxCalc(NodeS.at(j), R, passOne.ReactionFlagDir.at(i));	

						if (notEFM)
						{break;	/*scape step 3 	break the for and go the the next;*/ }

						// change global readyNodes
						// check if the R_num of the R_flux is full ==> push back R_num
						// if yes: 
						if (readyNodes != -1)
						{
							// d=find node_num in waitlist for odd numbers push_back(d+1) 
							// j = waitList.at()	// new j is always less than current j => we dont need to keep the current j, the order of seen not seen nodes are not know 
							j = FindJ(waitList, readyNodes); 
						}

					}//end if nodecases 
					else if (nodeCase == 0)
					{	
						visitedFlag.at(j) = true;
						R = fluxCalc(NodeS.at(j), R, passOne.ReactionFlagDir.at(i));

						if (notEFM)
						{break;		/*delete path 	break the for and go the the next;*/ }

						// check if the R_num of the R_flux is full ==> push back R_num
						// if yes: 
						if (readyNodes != -1)
						{
							// d=find node_num in waitlist for odd numbers push_back(d+1) 
							// j = waitList.at()	// new j is always less than current j => we dont need to keep the current j, the order of seen not seen nodes are not know 
							j = FindJ(waitList, readyNodes); 
						}

					} //end else nodecase -1
					else if ((nodeCase & 2)!= 0)		// 0010 & n == 0 means 8s happened
					{
						//save the pointer [nodenumber, j; .., ..;  ]  
						//save dependent reactions in R :: reactions we are waiting for the nodes :::   non-fluxed 8type input reactions 

						// what to do with more than two reactions????????????? ==> solved ::: ready budan ra check mikonad agar ready nabud dore wait mishavad ..

						vector<int> NotReadyRs = checkReadyCase3(NodeS.at(j), R); // if hameye 8-type nodes have flux
						bool check = NotReadyRs.empty();
						if (check)
						{
							if ((nodeCase & 13)!= 0)		// 1101 & n == 0 means visitgain happened)
								visitAgain.at(j) = true; 

							visitedFlag.at(j) = true;
							R = fluxCalc(NodeS.at(j), R, passOne.ReactionFlagDir.at(i));

							if (notEFM)
							{break;		/*delete path 	break the for and go the the next;*/ }

							// check if the R_num of the R_flux is full ==> push back R_num
							// if yes: 
							if (readyNodes != -1)
							{
								// d=find node_num in waitlist for odd numbers push_back(d+1) 
								// j = waitList.at()	// new j is always less than current j => we dont need to keep the current j, the order of seen not seen nodes are not know 
								j = FindJ(waitList, readyNodes); 
							}

						}//if check
						else //(!check)
						{
							waitList.push_back(NodeS.at(j).NodeName);	waitList.push_back(j); 
							R = updateReactionArrayNum(R, NodeS.at(j).NodeName, NotReadyRs);		// az bare dovom be ba'd overwrite mishavad.. yani ghablan dar R_num num sabt shode bude
						}// else check

					} //end else nodecase3


				} //if visited

			}// if CR 
			//==================================================
		} // end for nodes 

		//---------------------------------
		//--------------------------- step4
		//---------------------------------

		if (!notEFM){

			vector	<int>	indices;
			vector	<int>	reacsLocal;  

			for (int k=0; k<(int)NodeS.size(); k++)
			{

				if (visitAgain.at(k) && !notEFM)
				{

					NODE node = NodeS.at(k);

					vector<int> avoidLoop_CheckVisitVec;			// har node dar nodes dide shod jaye an ra set kon 
					for (int u=0; u<(int)NodeS.size(); u++)		{avoidLoop_CheckVisitVec.push_back(0);}	// initialization

					bool stayIn = true;
					int kkk = k;
					while (stayIn)		//which is empty at first .. then it will loop while it's not empty .. if empty: we have arrived to input/output 
					{

						//-------------------------------------------------------------------------------------------------
						//--------------------------------------------------------------------------------------------

						// find extra_flux in the node that we are in ==> 

						float PorC = findExtraFlux(node, R, nodeCaseVec.at(kkk));												// update_xVec 

						if (PorC == 0)// if PorC == 0 we are Done!
						{
							if (indices.size()==0)	// we have reached to input break the loop
							{
								stayIn = false;
								break;
							}

							kkk = indices.at(0);					indices.erase(indices.begin());	 		// go to the next node 
							xVec.push_back(reacsLocal.at(0));		reacsLocal.erase(reacsLocal.begin());		

							node = NodeS.at(kkk);

							if (visitAgain.at(kkk))
								visitAgain.at(kkk) = false;		// check as unvisit
						}

						else if (PorC != 0) 
						{

							R = update_flux(PorC,  node, R, nodeCaseVec.at(kkk)/*, x*/, avoidLoop_CheckVisitVec.at(kkk));				// use_xVec , update_nodenames, update_reacs 
							avoidLoop_CheckVisitVec.at(kkk)++;

							vector<vector<int>> tmp = update_indices(NodeS/*, nodenames, reacs*/, indices, reacsLocal);		// find and push back next nodes in indices, clear_nodenames, clear_reacs
							indices = tmp.at(0);	reacsLocal = tmp.at(0);

							xVec.clear(); // clear after use in update_flux 

							if (indices.size()==0)	// we have reached to input break the loop
							{
								stayIn = false;
								break;
							}

							kkk = indices.at(0);					indices.erase(indices.begin());	 		// go to the next node 
							xVec.push_back(reacsLocal.at(0));		reacsLocal.erase(reacsLocal.begin());		

							node = NodeS.at(kkk);

							if (visitAgain.at(kkk))
								visitAgain.at(kkk) = false;		// check as unvisit
						}

						// ----------------
						if (notEFM)
						{
							//notEFM = false;
							break; // break the while if notEFM got ture inside the while loop
						}
						// ----------------
					} //end while indices
					//---------------------------------------------------------------------------------------------
					//-------------------------------------------------------------------------------------------------

				}// end if visied 
			}// end for nodes
			//--------------------
			if (!notEFM)
			{
				path = MakeFluxVectorFromArray(R); //improve :: we have used reactions from passOne ==> keep and use that  
				EFMs.push_back(path);
				printf (" \t i = %d \t", i);
			}
		}// end notEFM
		else notEFM = false;
	}//.....................................  end for each passOne <<<..................Parallel execution.....................>>

	return;
}
//*************************************************************************************************
void printEFMs(ofstream &fout) // check also for EFMs with "the same subset of reactions" ==> Just for test
{

	fout << EFMs.size() <<"\n";
	fout << NumbersOfReactions <<"\n";

	for (int i=0; i< (int)EFMs.size(); i++)
	{
		//printf( "\n \nEFM Number %d \n\n", i);
		float arrayEFM [bitSetNumOfReac] = {0}; 

		for (int j=0; j< (int)EFMs.at(i).size(); j++)
		{
			arrayEFM [EFMs.at(i).at(j).reationName] = EFMs.at(i).at(j).reactionFlux;
			//printf( "R%d = %f \t", EFMs.at(i).at(j).reationName, EFMs.at(i).at(j).reactionFlux);
		}
		//-------------
		for (int i = 0; i < NumbersOfReactions; i++)
		{
			fout << arrayEFM[i] << "\t";
		}
		fout << "\n";
		//------------- 

	}// end for number of EFMs
}
//*************************************************************************************************


