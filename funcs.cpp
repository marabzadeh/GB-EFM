#include "structs.h"

// ................................... General.....................................................
//*************************************************************************************************
INPUT INOUTconvertToInput(INOUT inou) //first input second output 
{
	INPUT in; 
	in.setParams(inou.metabolitNamesIn, inou. metabolitNamesOut, inou.reacNum, inou.Coef, inou.direction, inou.type);
	return in;
}
//*************************************************************************************************
OUTPUT INOUTconvertToOutput(INOUT inou) //first input second output 
{
	OUTPUT out; 
	out.setParams(inou.metabolitNamesIn, inou.metabolitNamesOut, inou.reacNum, inou.Coef, inou.direction, inou.type);
	return out;
}
//*************************************************************************************************
INPUT OUTconvertToInput(OUTPUT out)
{
	INPUT in; 

	in.setParams(out.metabolitNamesIn, out.metabolitNamesOut, out.reacNum, out.Coef, out.direction, out.type);
	return in;
}
//*************************************************************************************************
OUTPUT INconvertToOutput(INPUT in)
{
	OUTPUT out; 
	out.setParams(in.metabolitNamesIn, in.metabolitNamesOut, in.reacNum, in.Coef, in.direction, in.type);
	return out;
}
//************************************************************************************************
int FindTHEPARENTInList(vector<INNODE> parentsOfEachNode, int parent)
{
	int index = -1;

	for (int i=0; i < (int)parentsOfEachNode.size(); i++)
	{
		if(parentsOfEachNode.at(i).NodeName == parent) 
		{
			index = i;
			return index; 
		}
	}

	return index;
}
//************************************************************************************************
//
//
//
//
//
//
//
//
//
//
//

// ................................... CreateNetwork...............................................
//*************************************************************************************************
vector<vector<int>> FindSource(vector<vector<float>>& S, int j, float temp)
{
	vector<vector<int>> back; 
	vector<int> source;
	vector<int> sibling;

	if (temp > 0)
	{
		for (int h=0; h < NumbersOfMetabolits; h++)
		{
			if (S.at(h).at(j) < 0)
			{
				source.push_back(h);
			}//end if
			else if ((S.at(h).at(j) > 0))
			{
				sibling.push_back(h);
			}

		}

	}// end if temp

	//-------------
	else if (temp < 0)
	{
		for (int h=0; h < NumbersOfMetabolits; h++)
		{
			if (S.at(h).at(j) > 0)
			{
				source.push_back(h);
			}//end if
			else if ((S.at(h).at(j) < 0))
			{
				sibling.push_back(h);
			}

		}

	}// end else temp

	back.push_back(source);
	back.push_back(sibling);

	return back;
}
//*************************************************************************************************
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// ................................... passOne.....................................................
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
bool AllOut(vector<int> outputNode)
{
	bool ifYEs = true;

	for (int i=0; i<(int)outputNode.size(); i++)
	{
		if (outputNode.at(i)!= -1)
		{
			ifYEs = false;
			return  ifYEs;
		}
	}
	return ifYEs;
}
//*************************************************************************************************
vector<PACKAGEONE> Packages_Add(vector<PACKAGEONE> packages, vector<PACKAGEONE> tempPackages, bool notDone)
{
	if (!notDone)
	{
		for (int i=0; i<(int)tempPackages.size();i++)
		{
			packages.push_back(tempPackages.at(i));
		}
	}
	//-------------------
	else
	{
		//packages.erase(packages.begin());
		for (int i=0; i<(int)tempPackages.size(); i++)
		{
			packages.insert(packages.begin(), tempPackages.at(i));
		}
	}
	//-------------------
	return packages;
}
//*************************************************************************************************

// ................................... passOne :: InNodeFunctionPassOne :: FindInput ..........................................
//************************************************************************************************* 
INPUT FindInput(vector<INPUT> inputs/*, int CandidateNodeName_parent*/)
{
	INPUT in;

	bool test = false; //**

	for (int j=0; j<(int)inputs.size(); j++)
	{
		if (inputs.at(j).reacNum == THEREACTIONNAME)
		{
			in = inputs.at(j);
			test = true; //**
			break;
		}
	}

	if (!test)//**
		printf("FindInput function: no input with this reaction name, the metabolit = %d , the reaction = %d", THEMETABOLITE, THEREACTIONNAME); //**

	return in;
}
//*************************************************************************************************

// ................................... passOne :: InNodeFunctionPassOne :: FindInputR..........................................
//************************************************************************************************* 
INPUT FindInputR(vector<OUTPUT> outputs/*, int ReacName*/)
{
	INPUT in;
	//OUTPUT temp;

	bool test = false; //**

	for (int i=0; i<(int)outputs.size(); i++)
	{
		if (outputs.at(i).reacNum == THEREACTIONNAME)
		{
			//temp = outputs.at(i);
			//in = createInput(temp.metabolitNames, temp.reacNum, temp.Coef, temp.direction);		//changed 
			in = OUTconvertToInput(outputs.at(i));
			test = true; //**
			break;
		}	
	}
	if (!test)//**
		printf("FindInputR function: no input with this reaction name, the metabolit = %d , the reaction = %d", THEMETABOLITE, THEREACTIONNAME); //**


	return in;
}
//*************************************************************************************************

// ................................... passOne :: InNodeFunctionPassOne :: TheNodeIsInList.....................................
//*************************************************************************************************
int TheNodeIsInList(vector<INNODE> parentsOfEachNode, int name)
{
	int index = -1; // index = -2, -3, -4, -5 
	//	bool condition;

	for(int i=0; i<(int)parentsOfEachNode.size(); i++)
	{
		if (parentsOfEachNode.at(i).NodeName == name)
		{
			/*if(THEREVERSE)
			condition = parentsOfEachNode.at(i).ifReverse.test(name);
			else
			condition = !parentsOfEachNode.at(i).ifReverse.test(name); */

			if(parentsOfEachNode.at(i).parentMetasTillNow.test(name) && parentsOfEachNode.at(i).parentMetasTillNowR.test(name) && !THEREVERSE) // !1
			{
				//THEINDEX = i;
				index = -2;
				break; 
			}
			else if(parentsOfEachNode.at(i).parentMetasTillNow.test(name) && !parentsOfEachNode.at(i).parentMetasTillNowR.test(name) && !THEREVERSE)// 1
			{
				//THEINDEX = i;
				index = -3;
				break;
			}
			else if(parentsOfEachNode.at(i).parentMetasTillNow.test(name) && parentsOfEachNode.at(i).parentMetasTillNowR.test(name) && THEREVERSE)//  !2
			{
				//THEINDEX = i;
				index = -4;
				break;
			}
			else if(parentsOfEachNode.at(i).parentMetasTillNow.test(name) && !parentsOfEachNode.at(i).parentMetasTillNowR.test(name) && THEREVERSE)//  2
			{
				//THEINDEX = i;
				index = -5;
				break;
			}

		}
	}
	return index;
}
//*************************************************************************************************

// ................................... passOne :: InNodeFunctionPassOne :: FindIndex...........................................
//*************************************************************************************************
int FindIndex(vector<int> outputNodes, int name)
{
	for (int i=0; i< (int)outputNodes.size(); i++)
	{
		if (outputNodes.at(i) == name)
			return i;
	}

	printf("Could not find Index! We have trouble!! the metabolite we want to takeAway = %d, we are in metabolite = %d", name, THEMETABOLITE);
	return -1;
}
//*************************************************************************************************

// ................................... passOne :: InNodeFunctionPassOne :: UpdateParent........................................
INNODE UpdateParent(vector<INNODE> parentsOfEachNode, int metabolite, int parent/*, bool FW*/)
{

	bool FW = !THEREVERSE; 

	int nodeSeenBefore	=	FindTHEPARENTInList(parentsOfEachNode, metabolite);		// with the help of THEMETABOLITE we find the index :: we had seen this node before 
	int parentIndex		=	FindTHEPARENTInList(parentsOfEachNode, parent);			// with the help of THEPARENT we find the index of the parent to add to the previous list  

	INNODE tteemmpp;

	if(parentIndex == -1)
	{
		tteemmpp.NodeName = metabolite;
		tteemmpp.parentMetasTillNow.set(metabolite);

		if(!FW)		//THEREVERSE
			tteemmpp.parentMetasTillNowR.set(metabolite);				

	}//---------------------------
	else 
	{
		if (nodeSeenBefore == -1)		//---------------------------------------  we didn't see this node before and the parent is not -1
		{
			tteemmpp.NodeName = metabolite;

			tteemmpp.parentMetasTillNow = parentsOfEachNode.at(parentIndex).parentMetasTillNow;
			tteemmpp.parentMetasTillNow.set(metabolite);

			tteemmpp.parentMetasTillNowR = parentsOfEachNode.at(parentIndex).parentMetasTillNowR;
			if(!FW)		//THEREVERSE
				tteemmpp.parentMetasTillNowR.set(metabolite);				

		}
		else if(nodeSeenBefore != -1)	//----------------------------------------   if we have seen that node before
		{

			tteemmpp.parentMetasTillNow = parentsOfEachNode.at(nodeSeenBefore).parentMetasTillNow | parentsOfEachNode.at(parentIndex).parentMetasTillNow;
			tteemmpp.parentMetasTillNow.set(metabolite);

			tteemmpp.parentMetasTillNowR = parentsOfEachNode.at(nodeSeenBefore).parentMetasTillNowR | parentsOfEachNode.at(parentIndex).parentMetasTillNowR;
			if(!FW)		//THEREVERSE
				tteemmpp.parentMetasTillNowR.set(metabolite);				

		} //end else nodeSeenbefore
		//--------------------------
	} // end else parentIndex == -1


	PINDEX = nodeSeenBefore;
	return tteemmpp;
}
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
// ................................... passOne :: InNodeFunctionPassOne .......................................................
vector<PACKAGEONE> InNodeFunctionPassOne(NODE& candidateNOde, PACKAGEONE tmpPACK) 
{
	vector<PACKAGEONE> tempPackages;

	int tempoary = -1;
	bool a,b,c = false;

	PACKAGEONE tmpPACK_old = tmpPACK;


	//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	if (!THEREVERSE){

		INPUT in = FindInput(candidateNOde.inputs/*, THEPARENT*/); //with the help of THEREACTIONNAME

		//debug
		if (in.reacNum < 0)
		{
			printf("hello"); 
		}
		//debug 

		//===================================
		int index = 0; // if it remains 0 means that we didnt see this reaction before 

		if (tmpPACK.MetabolitFlag.test(THEMETABOLITE)){

			index = TheNodeIsInList(tmpPACK.ParentsInfoForEachNode, THEMETABOLITE);

			// if the metabolite has been seen before dependently ==>	-1 means not seen metbolite ::::::: -2,-3, -4,-5 means seen metabolite 

			// -- dont check the outputs
			// -- assign type for the dependent reaction 


			//tmpPACK.nodesPassOne = ModifySeenNodeInNodesPassOne(CandidateNodeNodesPassOne, index);
			for (int i = 0; i < (int)tmpPACK.nodesPassOne.size() ; i++)
			{
				if(tmpPACK.nodesPassOne.at(i).NodeName == THEMETABOLITE)
				{
					if (index == -1){
						in.setType(8);
						tmpPACK.nodesPassOne.at(i).inputs.push_back(in);
					}
					else if (index == -3){
						in.setType(-3);
						tmpPACK.nodesPassOne.at(i).inputs.push_back(in);
					}
					else if (index == -2){ //???????????????????????????????????????????????????????????????????????????????????/
						in.setType(-2);
						tmpPACK.nodesPassOne.at(i).inputs.push_back(in);
					}
				}
			}

			// do not continue 
			//tmpPACK.InputsToBeChecked.reset(THEMETABOLITE);

			//bool DCheck = crossCheck(tmpPACK.);
			//if (!DCheck)
			//{
			tmpPACK.OutputsToBeChecked.reset(THEMETABOLITE);
			tempPackages.push_back(tmpPACK);
			//}
			return tempPackages;

		} //end if seen metabolite 
		//===================================


		//else {  // [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
		// -- go for outputs seperatly (inja che konam, sabr konam baraye output ha ke javab biad?)			
		// -- in path ra ta tah boro --> agar output unseen dasht ==> path jadid tolid kon 

		tmpPACK_old.ReactionFlag.set(THEREACTIONNAME);
		if (in.direction)
			tmpPACK_old.ReactionFlagDir.set(THEREACTIONNAME);

		for (int j=0 ; j< (int)candidateNOde.outputs.size(); j++) // for each reaction
		{	
			tmpPACK = tmpPACK_old;
			//if(candidateNOde.outputs.at(j).reacNum != THEREACTIONNAME){ // for checking reversibility ------------ az tarighe vakoneshi ke amade barnagardad

			a = (!tmpPACK.ReactionFlagDir.test(candidateNOde.outputs.at(j).reacNum)) & candidateNOde.outputs.at(j).direction;
			b = tmpPACK.ReactionFlagDir.test(candidateNOde.outputs.at(j).reacNum) & (!candidateNOde.outputs.at(j).direction);
			c = tmpPACK.ReactionFlag.test(candidateNOde.outputs.at(j).reacNum) & (a|b);  // agar reaction dide shode va jahat hashun yeki nist

			if(!c){

				//------------ NUM1:::: add and change needed parameters 
				NODE node;

				vector<INPUT>	ii;		in.setType(18);									ii.push_back(in);
				vector<OUTPUT>	oo;		candidateNOde.outputs.at(j).setType(19);		oo.push_back(candidateNOde.outputs.at(j));

				node.Node(ii, oo, THEMETABOLITE, true); 
				tmpPACK.nodesPassOne.push_back(node);


				tmpPACK.ReactionFlag.set(ii.at(ii.size()-1).reacNum);
				tmpPACK.ReactionFlag.set(oo.at(oo.size()-1).reacNum);
				if (ii.at(ii.size()-1).direction)
					tmpPACK.ReactionFlagDir.set(ii.at(ii.size()-1).reacNum);
				if (oo.at(oo.size()-1).direction)
					tmpPACK.ReactionFlagDir.set(oo.at(oo.size()-1).reacNum);

				INNODE tteemmpp = UpdateParent(tmpPACK.ParentsInfoForEachNode, /*ii.at(ii.size()-1),*/ THEMETABOLITE, THEPARENT/*, node.FWorBK*/); 
				if (index != 0)
					tmpPACK.ParentsInfoForEachNode.erase(tmpPACK.ParentsInfoForEachNode.begin()+PINDEX);
				tmpPACK.ParentsInfoForEachNode.push_back(tteemmpp);

				tmpPACK.MetabolitFlag.set(THEMETABOLITE);
				tmpPACK.OutputsToBeChecked.reset(THEMETABOLITE);
				//tmpPACK.ifExternalOutReactSeen = false; 

				//-----------------------------------------------------------------------------------------------------------------------
				//------------------------------------------------- Pushing back input siblings and outputs  ---- create a node if needed 
				//-----------------------------------------------------------------------------------------------------------------------
				for (int k=0; k<(int)candidateNOde.outputs.at(j).metabolitNamesOut.size(); k++)
				{
					//if (candidateNOde.outputs.at(j).metabolitNames.at(k) != THEPARENT) {
					//--------------------------------------				
					if (tmpPACK.InputsToBeChecked.test(candidateNOde.outputs.at(j).metabolitNamesOut.at(k)) && !tmpPACK.MetabolitFlag.test(candidateNOde.outputs.at(j).metabolitNamesOut.at(k))) //added
					{
						tempoary = FindIndex(tmpPACK.outputNode, candidateNOde.outputs.at(j).metabolitNamesOut.at(k));
						//add the needed information th the existence database .. what to add ==> 
						// -- add the node to nodesPassOne ==> input, output, reaction types
						// -- set the required flags
						// -- delete the outputnode information from the stack 
						//------------------------------------------------------------------------------ NUM2:::: add and change needed parameters 
						NODE node;

						vector<INPUT>	ii;		candidateNOde.outputs.at(j).setType(18);			ii.push_back(OUTconvertToInput(candidateNOde.outputs.at(j)));
						vector<OUTPUT>	oo;		tmpPACK.lastReaction.at(tempoary).setType(19);		oo.push_back(INOUTconvertToOutput(tmpPACK.lastReaction.at(tempoary)));

						node.Node(ii, oo, candidateNOde.outputs.at(j).metabolitNamesOut.at(k), true); 
						tmpPACK.nodesPassOne.push_back(node);


						tmpPACK.ReactionFlag.set(ii.at(ii.size()-1).reacNum);
						tmpPACK.ReactionFlag.set(oo.at(oo.size()-1).reacNum);
						if (ii.at(ii.size()-1).direction)
							tmpPACK.ReactionFlagDir.set(ii.at(ii.size()-1).reacNum);
						if (oo.at(oo.size()-1).direction)
							tmpPACK.ReactionFlagDir.set(oo.at(oo.size()-1).reacNum);

						INNODE tteemmpp = UpdateParent(tmpPACK.ParentsInfoForEachNode,/* OUTconvertToInput(oo.at(oo.size()-1)),*/ candidateNOde.outputs.at(j).metabolitNamesOut.at(k), THEMETABOLITE/*, node.FWorBK*/);

						if (index != 0)
							tmpPACK.ParentsInfoForEachNode.erase(tmpPACK.ParentsInfoForEachNode.begin()+PINDEX);
						tmpPACK.ParentsInfoForEachNode.push_back(tteemmpp);

						tmpPACK.InputsToBeChecked.reset(candidateNOde.outputs.at(j).metabolitNamesOut.at(k));
						tmpPACK.MetabolitFlag.set(candidateNOde.outputs.at(j).metabolitNamesOut.at(k));
						//tmpPACK.ifExternalOutReactSeen = false;

						//---------------------------------------------------------------------------------

						tmpPACK.outputNode.erase(tmpPACK.outputNode.begin()			+	tempoary);
						tmpPACK.ifReverseFlag.erase(tmpPACK.ifReverseFlag.begin()	+	tempoary);
						tmpPACK.parentNode.erase(tmpPACK.parentNode.begin()			+	tempoary);
						tmpPACK.lastReaction.erase(tmpPACK.lastReaction.begin()		+	tempoary);
					}
					else //if (!tmpPACK.InputsToBeChecked.test(candidateNOde.outputs.at(j).metabolitNamesOut.at(k))) // If did not consume beforehand put it in outputs otherwise don't
					{

						INOUT inout; inout.setParams(candidateNOde.outputs.at(j).metabolitNamesIn, candidateNOde.outputs.at(j).metabolitNamesOut, candidateNOde.outputs.at(j).reacNum, candidateNOde.outputs.at(j).Coef,
							candidateNOde.outputs.at(j).direction, candidateNOde.outputs.at(j).type);	

						//hey1
						tmpPACK.outputNode.push_back(candidateNOde.outputs.at(j).metabolitNamesOut.at(k));
						tmpPACK.lastReaction.push_back(inout);		
						tmpPACK.ifReverseFlag.push_back(false);
						tmpPACK.parentNode.push_back(candidateNOde.NodeName);

						tmpPACK.OutputsToBeChecked.set(candidateNOde.outputs.at(j).metabolitNamesOut.at(k));
					}

					//---------------------------------------
					//} // end if THEPARENT (for reversibility test) 
				} // enf for 


				//------
				vector<int> alaki = candidateNOde.outputs.at(j).metabolitNamesIn; 
				/*if (candidateNOde.outputs.at(j).Coef<0)
				alaki = siblingsVec.at(candidateNOde.outputs.at(j).reacNum).inputSiblings; // alaki == in.metabolitNames
				if (candidateNOde.outputs.at(j).Coef>0)
				alaki = siblingsVec.at(candidateNOde.outputs.at(j).reacNum).outputSiblings; // alaki == in.metabolitNames*/


				for (int k=0; k<(int)alaki.size();k++)
				{
					if (alaki.at(k) != candidateNOde.NodeName)
					{

						if (tmpPACK.OutputsToBeChecked.test(alaki.at(k)) && !tmpPACK.MetabolitFlag.test(alaki.at(k)))		//added
						{
							tempoary = FindIndex(tmpPACK.outputNode, alaki.at(k));
							//------------------------------------------------------------------------------ NUM3:::: add and change needed parameters 
							NODE node;

							vector<INPUT>	ii;		tmpPACK.lastReaction.at(tempoary).setType(18);		ii.push_back(INOUTconvertToInput(tmpPACK.lastReaction.at(tempoary)));
							vector<OUTPUT>	oo;		candidateNOde.outputs.at(j).setType(19);			oo.push_back(candidateNOde.outputs.at(j));

							node.Node(ii, oo, alaki.at(k), false); 
							tmpPACK.nodesPassOne.push_back(node);


							tmpPACK.ReactionFlag.set(ii.at(ii.size()-1).reacNum);
							tmpPACK.ReactionFlag.set(oo.at(oo.size()-1).reacNum);
							if (ii.at(ii.size()-1).direction)
								tmpPACK.ReactionFlagDir.set(ii.at(ii.size()-1).reacNum);
							if (oo.at(oo.size()-1).direction)
								tmpPACK.ReactionFlagDir.set(oo.at(oo.size()-1).reacNum);

							INNODE tteemmpp = UpdateParent(tmpPACK.ParentsInfoForEachNode, /*ii.at(ii.size()-1),*/ alaki.at(k), THEMETABOLITE/*, node.FWorBK*/);	//ii.at(ii.size()-1)

							if (index != 0)
								tmpPACK.ParentsInfoForEachNode.erase(tmpPACK.ParentsInfoForEachNode.begin()+PINDEX);
							tmpPACK.ParentsInfoForEachNode.push_back(tteemmpp);

							tmpPACK.OutputsToBeChecked.reset(alaki.at(k));
							tmpPACK.MetabolitFlag.set(alaki.at(k));
							//tmpPACK.ifExternalOutReactSeen = false;
							//--------------------------------------------------------------------------------

							tmpPACK.outputNode.erase(tmpPACK.outputNode.begin()			+	tempoary);
							tmpPACK.ifReverseFlag.erase(tmpPACK.ifReverseFlag.begin()	+	tempoary);
							tmpPACK.parentNode.erase(tmpPACK.parentNode.begin()			+	tempoary);
							tmpPACK.lastReaction.erase(tmpPACK.lastReaction.begin()		+	tempoary);			
						}
						else //if (!tmpPACK.OutputsToBeChecked.test(alaki.at(k))) // If did not produce beforehand put it in outputs otherwise don't
						{	

							INOUT inout; inout.setParams(candidateNOde.outputs.at(j).metabolitNamesIn, candidateNOde.outputs.at(j).metabolitNamesOut, candidateNOde.outputs.at(j).reacNum, 
								candidateNOde.outputs.at(j).Coef, candidateNOde.outputs.at(j).direction, candidateNOde.outputs.at(j).type);																			

							//hey2
							/*tmpPACK.outputNode.push_back(alaki.at(k));
							tmpPACK.lastReaction.push_back(inout);	
							tmpPACK.ifReverseFlag.push_back(true);
							tmpPACK.parentNode.push_back(candidateNOde.NodeName);*/

							tmpPACK.outputNode.insert(tmpPACK.outputNode.begin(), alaki.at(k));
							tmpPACK.lastReaction.insert(tmpPACK.lastReaction.begin(), inout);	
							tmpPACK.ifReverseFlag.insert(tmpPACK.ifReverseFlag.begin(), true);
							tmpPACK.parentNode.insert(tmpPACK.parentNode.begin(), candidateNOde.NodeName);

							tmpPACK.InputsToBeChecked.set(alaki.at(k));
						}
						//--------------------------------------
					} // end if nodename  
				}	// end if alaki(k)

				//------

				// ((((---------------------------------------------------------------------------------------------------------((((( return parameters 
				// ------ check if we see at least an external output
				//------ If no output put it as -1 for further processing 
				if(candidateNOde.outputs.at(j).metabolitNamesOut.size()==0){
					tmpPACK.outputNode.push_back(-1);
					//
					INOUT io; 
					tmpPACK.lastReaction.push_back(io);
					tmpPACK.ifReverseFlag.push_back(false);
					tmpPACK.parentNode.push_back(candidateNOde.NodeName);

					tmpPACK.ifExternalOutReactSeen = true;
				}				
				//))))----------------------------------------------------------------------------------------------------------))))))) return parameters	

				tempPackages.push_back(tmpPACK);
			} // end if THE REACTION NAME
		}//for j


	}//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

	else{ // ReverseFlag

		//OUTPUT out;

		INPUT in = FindInputR(candidateNOde.outputs/*, THEREACTIONNAME*/);
		//================================================
		if (in.reacNum < 0)
		{
			printf("hello"); 
		}
		int index = 0; // if it remains 0 means that we didnt see this reaction before 
		if (tmpPACK.MetabolitFlag.test(THEMETABOLITE)){

			index = TheNodeIsInList(tmpPACK.ParentsInfoForEachNode, THEMETABOLITE);

			// if the metabolite has been seen before dependently ==>	-1 means not seen metbolite ::::::: -2,-3, -4,-5 means seen metabolite 
			// -- dont check the outputs
			// -- assign type for the dependent reaction 

			//tmpPACK.nodesPassOne = ModifySeenNodeInNodesPassOne(CandidateNodeNodesPassOne, index);
			for (int i = 0; i < (int)tmpPACK.nodesPassOne.size() ; i++)
			{
				if(tmpPACK.nodesPassOne.at(i).NodeName == THEMETABOLITE)
				{
					if (index == -1){	
						in.setType(9);
						tmpPACK.nodesPassOne.at(i).outputs.push_back(INconvertToOutput(in));
					}
					else if (index == -5){
						in.setType(-5);
						tmpPACK.nodesPassOne.at(i).outputs.push_back(INconvertToOutput(in));
					}
					else if (index == -4){ //???????????????????????????????????????????????????????????????????????????????????/
						in.setType(-4);
						tmpPACK.nodesPassOne.at(i).outputs.push_back(INconvertToOutput(in));
					}
				}
			}

			// do not continue 

			//bool DCheck = crossCheck(tmpPACK.);
			//if (!DCheck)
			//{
			tmpPACK.InputsToBeChecked.reset(THEMETABOLITE);
			//tmpPACK.OutputsToBeChecked.reset(THEMETABOLITE);
			tempPackages.push_back(tmpPACK);
			//}

			return tempPackages;
		} //end if seen metabolite 
		// [[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[

		//================================================

		//else {  
		// -- go for outputs seperatly (inja che konam, sabr konam baraye output ha ke javab biad?)			
		// -- in path ra ta tah boro --> agar output unseen dasht ==> path jadid tolid kon 

		tmpPACK_old.ReactionFlag.set(THEREACTIONNAME);
		if (in.direction)
			tmpPACK_old.ReactionFlagDir.set(THEREACTIONNAME);
		//...........................

		for (int j=0; j< (int)candidateNOde.inputs.size(); j++)
		{
			tmpPACK = tmpPACK_old;

			//if(candidateNOde.inputs.at(j).reacNum != THEREACTIONNAME){
			a = (!tmpPACK.ReactionFlagDir.test(candidateNOde.inputs.at(j).reacNum)) & candidateNOde.inputs.at(j).direction;
			b = tmpPACK.ReactionFlagDir.test(candidateNOde.inputs.at(j).reacNum) & (!candidateNOde.inputs.at(j).direction);
			c = tmpPACK.ReactionFlag.test(candidateNOde.inputs.at(j).reacNum) & (a|b);  

			if(!c){
				//.......................................

				//------------ NUM4:::: add and change needed parameters 
				NODE node;

				vector<INPUT>	ii;		candidateNOde.inputs.at(j).setType(18);			ii.push_back(candidateNOde.inputs.at(j));
				vector<OUTPUT>	oo;		in.setType(19);									oo.push_back(INconvertToOutput(in));


				node.Node(ii, oo, THEMETABOLITE, false); 
				tmpPACK.nodesPassOne.push_back(node);

				//here..........

				tmpPACK.ReactionFlag.set(ii.at(ii.size()-1).reacNum);
				tmpPACK.ReactionFlag.set(oo.at(oo.size()-1).reacNum);
				if (ii.at(ii.size()-1).direction)
					tmpPACK.ReactionFlagDir.set(ii.at(ii.size()-1).reacNum);
				if (oo.at(oo.size()-1).direction)
					tmpPACK.ReactionFlagDir.set(oo.at(oo.size()-1).reacNum);

				INNODE tteemmpp = UpdateParent(tmpPACK.ParentsInfoForEachNode, /*OUTconvertToInput(oo.at(oo.size()-1)),*/ THEMETABOLITE, THEPARENT/*, node.FWorBK*/);
				if (index != 0)
					tmpPACK.ParentsInfoForEachNode.erase(tmpPACK.ParentsInfoForEachNode.begin()+PINDEX);
				tmpPACK.ParentsInfoForEachNode.push_back(tteemmpp);

				tmpPACK.InputsToBeChecked.reset(THEMETABOLITE);
				tmpPACK.MetabolitFlag.set(THEMETABOLITE);
				//tmpPACK.ifExternalOutReactSeen = false;

				//-----------------------------------------------------------------------------------------------------------------------
				//------------------------------------------------- Pushing back input siblings and outputs  ---- create a node if needed 
				//-----------------------------------------------------------------------------------------------------------------------


				vector<int> alaki = candidateNOde.inputs.at(j).metabolitNamesOut; 
				/*if (candidateNOde.inputs.at(j).Coef>0)
				alaki = siblingsVec.at(candidateNOde.inputs.at(j).reacNum).outputSiblings; // alaki == in.metabolitNames
				if (candidateNOde.inputs.at(j).Coef<0)
				alaki = siblingsVec.at(candidateNOde.inputs.at(j).reacNum).inputSiblings; // alaki == in.metabolitNames*/

				for (int k=0; k<(int)alaki.size(); k++)
				{
					if ( alaki.at(k) != candidateNOde.NodeName)
					{
						//-------------------------------------
						if (tmpPACK.InputsToBeChecked.test(alaki.at(k)) && !tmpPACK.MetabolitFlag.test(alaki.at(k))) //added
						{
							tempoary = FindIndex(tmpPACK.outputNode, alaki.at(k));

							//------------------------------------------------------------------------------ NUM5:::: add and change needed parameters 
							NODE node;

							vector<INPUT>	ii;		candidateNOde.inputs.at(j).setType(18);				ii.push_back(candidateNOde.inputs.at(j));
							vector<OUTPUT>	oo;		tmpPACK.lastReaction.at(tempoary).setType(19);		oo.push_back(INOUTconvertToOutput(tmpPACK.lastReaction.at(tempoary)));

							node.Node(ii, oo, alaki.at(k), true); //candidateNOde.inputs.at(j).metabolitNames.at(k) 
							tmpPACK.nodesPassOne.push_back(node);

							tmpPACK.ReactionFlag.set(ii.at(ii.size()-1).reacNum);
							tmpPACK.ReactionFlag.set(oo.at(oo.size()-1).reacNum);
							if (ii.at(ii.size()-1).direction)
								tmpPACK.ReactionFlagDir.set(ii.at(ii.size()-1).reacNum);
							if (oo.at(oo.size()-1).direction)
								tmpPACK.ReactionFlagDir.set(oo.at(oo.size()-1).reacNum);

							INNODE tteemmpp = UpdateParent(tmpPACK.ParentsInfoForEachNode, /*OUTconvertToInput(oo.at(oo.size()-1)),*/ alaki.at(k), THEMETABOLITE/*, node.FWorBK*/);
							if (index != 0)
								tmpPACK.ParentsInfoForEachNode.erase(tmpPACK.ParentsInfoForEachNode.begin()+PINDEX);
							tmpPACK.ParentsInfoForEachNode.push_back(tteemmpp);

							tmpPACK.InputsToBeChecked.reset(alaki.at(k));
							tmpPACK.MetabolitFlag.set(alaki.at(k));
							//tmpPACK.ifExternalOutReactSeen = false;
							//---------------------------------------------------------------------------------


							tmpPACK.outputNode.erase(tmpPACK.outputNode.begin()			+	tempoary);
							tmpPACK.ifReverseFlag.erase(tmpPACK.ifReverseFlag.begin()	+	tempoary);
							tmpPACK.parentNode.erase(tmpPACK.parentNode.begin()			+	tempoary);
							tmpPACK.lastReaction.erase(tmpPACK.lastReaction.begin()		+	tempoary);
						}
						//-------------------------------------

						else //if (!tmpPACK.InputsToBeChecked.test(alaki.at(k))) // If did not produce beforehand put it in outputs otherwise don't
						{ 

							INOUT inout; inout.setParams(candidateNOde.inputs.at(j).metabolitNamesIn, candidateNOde.inputs.at(j).metabolitNamesOut, candidateNOde.inputs.at(j).reacNum, 
								candidateNOde.inputs.at(j).Coef, candidateNOde.inputs.at(j).direction, candidateNOde.inputs.at(j).type);																		//candidateNOde.inputs.at(j)

							//hey3
							/*tmpPACK.outputNode.push_back(alaki.at(k));
							tmpPACK.lastReaction.push_back(inout);		
							tmpPACK.ifReverseFlag.push_back(false);                                                                                          
							tmpPACK.parentNode.push_back(candidateNOde.NodeName);*/

							tmpPACK.outputNode.insert(tmpPACK.outputNode.begin(), alaki.at(k));
							tmpPACK.lastReaction.insert(tmpPACK.lastReaction.begin(), inout);	
							tmpPACK.ifReverseFlag.insert(tmpPACK.ifReverseFlag.begin(), false);
							tmpPACK.parentNode.insert(tmpPACK.parentNode.begin(), candidateNOde.NodeName);

							tmpPACK.OutputsToBeChecked.set(alaki.at(k));
						}
						//-------------------------------------
					}
				}

				//---------
				for (int k=0; k<(int)candidateNOde.inputs.at(j).metabolitNamesIn.size(); k++)
				{
					//-------------------------------------
					if (tmpPACK.OutputsToBeChecked.test(candidateNOde.inputs.at(j).metabolitNamesIn.at(k)) && !tmpPACK.MetabolitFlag.test(candidateNOde.inputs.at(j).metabolitNamesIn.at(k)))	//added
					{
						tempoary = FindIndex(tmpPACK.outputNode, candidateNOde.inputs.at(j).metabolitNamesIn.at(k));

						//------------------------------------------------------------------------------ NUM6:::: add and change needed parameters 
						NODE node;


						vector<INPUT>	ii;		tmpPACK.lastReaction.at(tempoary).setType(18);		ii.push_back(INOUTconvertToInput(tmpPACK.lastReaction.at(tempoary)));			// Dobarekari mishavadbe khatere function "FindInputR" ==> Ba'd dorostah kon 
						vector<OUTPUT>	oo;		candidateNOde.inputs.at(j).setType(19);				oo.push_back(INconvertToOutput(candidateNOde.inputs.at(j)));

						node.Node(ii, oo, candidateNOde.inputs.at(j).metabolitNamesIn.at(k), false); 
						tmpPACK.nodesPassOne.push_back(node);

						tmpPACK.ReactionFlag.set(ii.at(ii.size()-1).reacNum);
						tmpPACK.ReactionFlag.set(oo.at(oo.size()-1).reacNum);
						if (ii.at(ii.size()-1).direction)
							tmpPACK.ReactionFlagDir.set(ii.at(ii.size()-1).reacNum);
						if (oo.at(oo.size()-1).direction)
							tmpPACK.ReactionFlagDir.set(oo.at(oo.size()-1).reacNum);

						INNODE tteemmpp = UpdateParent(tmpPACK.ParentsInfoForEachNode, /*ii.at(ii.size()-1),*/ candidateNOde.inputs.at(j).metabolitNamesIn.at(k), THEMETABOLITE/*, node.FWorBK*/);
						if (index != 0)
							tmpPACK.ParentsInfoForEachNode.erase(tmpPACK.ParentsInfoForEachNode.begin()+PINDEX);
						tmpPACK.ParentsInfoForEachNode.push_back(tteemmpp);

						tmpPACK.OutputsToBeChecked.reset(candidateNOde.inputs.at(j).metabolitNamesIn.at(k));
						tmpPACK.MetabolitFlag.set(candidateNOde.inputs.at(j).metabolitNamesIn.at(k));
						//tmpPACK.ifExternalOutReactSeen = false;

						//--------------------------------------------------------------------------------

						tmpPACK.outputNode.erase(tmpPACK.outputNode.begin()			+	tempoary);
						tmpPACK.ifReverseFlag.erase(tmpPACK.ifReverseFlag.begin()	+	tempoary);
						tmpPACK.parentNode.erase(tmpPACK.parentNode.begin()			+	tempoary);
						tmpPACK.lastReaction.erase(tmpPACK.lastReaction.begin()		+	tempoary);
					}

					else //if (!tmpPACK.OutputsToBeChecked.test(candidateNOde.inputs.at(j).metabolitNamesIn.at(k))) // If did not produce beforehand put it in outputs otherwise don't
					{ 

						INOUT inout; inout.setParams(candidateNOde.inputs.at(j).metabolitNamesIn, candidateNOde.inputs.at(j).metabolitNamesOut, candidateNOde.inputs.at(j).reacNum, candidateNOde.inputs.at(j).Coef,
							candidateNOde.inputs.at(j).direction, candidateNOde.inputs.at(j).type);	

						// hey4
						tmpPACK.outputNode.push_back(candidateNOde.inputs.at(j).metabolitNamesIn.at(k));
						tmpPACK.lastReaction.push_back(inout);		
						tmpPACK.ifReverseFlag.push_back(true);
						tmpPACK.parentNode.push_back(candidateNOde.NodeName);

						/*tmpPACK.outputNode.insert(tmpPACK.outputNode.begin(), candidateNOde.inputs.at(j).metabolitNamesIn.at(k));
						tmpPACK.lastReaction.insert(tmpPACK.lastReaction.begin(), inout);	
						tmpPACK.ifReverseFlag.insert(tmpPACK.ifReverseFlag.begin(), true);
						tmpPACK.parentNode.insert(tmpPACK.parentNode.begin(), candidateNOde.NodeName);*/


						tmpPACK.InputsToBeChecked.set(candidateNOde.inputs.at(j).metabolitNamesIn.at(k));
					}

					//---------------------------------------
					//} // end if THEPARENT 
				}
				//------

				// ------ check if we see an external output
				//------ If no output put it as -1 for further processing 
				if(candidateNOde.inputs.at(j).metabolitNamesIn.size()==0){
					tmpPACK.outputNode.push_back(-1);
					//
					INOUT io; 
					tmpPACK.lastReaction.push_back(io);
					tmpPACK.ifReverseFlag.push_back(true);
					tmpPACK.parentNode.push_back(candidateNOde.NodeName);

					//tmpPACK.ifExternalOutReactSeen = true;
				}		

				tempPackages.push_back(tmpPACK);
			} // end if THE REACTION NAME
		}//for j
	} // end THEREVERSE 

	return tempPackages;
}
//*************************************************************************************************
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

// ................................... eliminateMultiWayPaths.....................................................
//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
// ................................... eliminateMultiWayPaths :: findIndeces  ..........................................
//************************************************************************************************* 
int findIndeces(vector<NODE> nodak, int mm)
{
	int ii = -1;
	for (int i=nodak.size()-1; i>=0; i--)
	{
		if (nodak.at(i).NodeName == mm)
		{
			ii = i;
			break;

		}
	}
	return ii; 
}
//*************************************************************************************************
// ................................... eliminateMultiWayPaths :: crossCheckFunc  ..........................................
//************************************************************************************************* 
bool crossCheckFunc(bitset<5> cover, vector<bitset <5>> coverVec) // improve :: if we have choices to assign (not supported yet) :: make a greedy connection 
{
	bool crossCheck = false;
	int check = 0; 

	int findOnecount = -1; 
	bool find = true;

	while (find)
	{
		find = false;
		for (int i = 0; i<(int)coverVec.size(); i++)
		{
			if (coverVec.at(i).count() == 1)
			{
				findOnecount = i;
				find = true;
				break;
			}		
		}

		bool goIn = false;
		if (find){
			int count = (cover & coverVec.at(findOnecount)).count();
			if (count == 1)
			{
				cover &= ((cover & coverVec.at(findOnecount)).flip());
				for (int j=0; j<(int)coverVec.size(); j++)
				{
					coverVec.at(j) &= ((coverVec.at(j) & coverVec.at(findOnecount)).flip());
				}
				coverVec.erase(coverVec.begin() + findOnecount);
				check ++;
				goIn = true;
			}
		}

		if (check >= 2)
		{
			crossCheck = true;
			break; 
		}
		//error :: should change 

		if (find && !goIn)
			break;
		
	}


	/*for (int i=0; i< (int)coverVec.size(); i++)
	{
		int count = coverVec.at(i).count();
		int check = 0;
		switch (count) {
		case 1: {
			//assign :: zero the bit in cover 
			break;}
		case 2: {
			printf ("not planned yet");
			crossCheck = true; 
			break; }
		case 3:{
			printf ("not planned yet");
			crossCheck = true; 
			break; }
		case 4:{
			printf ("not planned yet");
			crossCheck = true; 
			break; }
		case 5:{
			printf ("not planned yet");
			crossCheck = true; 
			break; }
		}
	}*/
	return crossCheck; 
}
//*************************************************************************************************
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// ................................... passTwo.....................................................
//*************************************************************************************************
//*************************************************************************************************
//************************************************************************************************* 
bool checkReady(NODE node, ArrayR R)	
{
	bool CR = true;

	if (node.getFWorBK())
	{
		for (int i=0; i<(int)node.inputs.size(); i++)
		{
			if ((node.inputs.at(i).type == 8) || (node.inputs.at(i).type == 18))
			{
				if (R.flux[node.inputs.at(i).reacNum] == 0)
				{
					CR = false;
					//if (node.inputs.at(i).type != 18)		printf("somthing is wrong :: inputs");		
					return CR;
				}
			}
		}
	}
	//------
	else
	{
		for (int i=0; i<(int)node.outputs.size(); i++)
		{
			if (node.outputs.at(i).type == 19)
			{
				if (R.flux[node.outputs.at(i).reacNum] == 0)
				{
					CR = false;
					//if (node.outputs.at(i).type != 19)		printf("somthing is wrong :: outputs");
					return CR;
				}
			}
		}
	}

	return CR; 
}
//*************************************************************************************************
int caseRecog(NODE node) // '0' => normal			  			
{
	int nodeCase = 0;
	int count = 0; 

	for (int i=0; i<(int)node.inputs.size(); i++)
	{
		if (node.inputs.at(i).type == 8)
		{
			count++; 
		}

		else if ( (node.inputs.at(i).type == -2) || (node.inputs.at(i).type == -3))
		{
			nodeCase = 1;
			//break;
		}	
	}
	//----------------------
	if (count > 0)
	{
		nodeCase = nodeCase | 2;
	}

	count = 0;
	//----------------------------------------------
	for (int i=0; i<(int)node.outputs.size(); i++)
	{
		if (node.outputs.at(i).type == 9)
		{
			count++; 
		}

		else if (node.outputs.at(i).type == -4)
		{
			nodeCase = nodeCase | 4;
			//break;
		}	
		else if (node.outputs.at(i).type == -5)
		{
			nodeCase = nodeCase | 8;
		}
	}
	//---------------------

	if (count > 0)
	{
		nodeCase = nodeCase | 16;
	}

	return nodeCase; 
}
//*************************************************************************************************
int		readyNodes	=	-1; // default ==> just one node is ready
bool	notEFM		=	false;
//*************************************************************************************************
ArrayR fluxCalc(NODE node, ArrayR R, bitset<bitSetNumOfReac> flagD)
{
	// tashkhis dade adiye, tashkhis dade node yale bazgashtiye ezafe dard ==> not important to me 
	// alamate zarayeb ???????????????????????????????????????????????????????????????????????????????????

	float	c_x, c_y = 0;
	float	f_x, f_y = 0; 


	if (node.getFWorBK())
	{
		for (int i=0; i< (int)node.inputs.size(); i++)
		{
			if (node.inputs.at(i).type == 18)	
			{
				c_x = node.inputs.at(i).Coef;
				f_x = R.getFlux(node.inputs.at(i).reacNum);
				break;
			}
		}
		//====
		for (int i=0; i< (int)node.outputs.size(); i++)
		{
			if (node.outputs.at(i).type == 19)	
			{
				c_y = node.outputs.at(i).Coef;
				f_y = abs((c_x*f_x)/c_y);												//alamat

				if (flagD.at(node.outputs.at(i).reacNum))
					f_y = -f_y;

				notEFM = R.setFlux(node.outputs.at(i).reacNum, f_y);

				if (R.getNum(node.outputs.at(i).reacNum) != -1)
					readyNodes = R.getNum(node.outputs.at(i).reacNum);

				break;
			}
		}
	}//========================= end if
	else
	{
		for (int i=0; i< (int)node.outputs.size(); i++)
		{
			if (node.outputs.at(i).type == 19)	
			{
				c_y = node.outputs.at(i).Coef;
				f_y = R.getFlux(node.outputs.at(i).reacNum);
				break;
			}
		}
		//====
		for (int i=0; i< (int)node.inputs.size(); i++)
		{
			if (node.inputs.at(i).type == 18)	
			{
				c_x = node.inputs.at(i).Coef;

				f_x = abs((c_y*f_y)/c_x);											// alamat
				if (flagD.at(node.inputs.at(i).reacNum))
					f_x = -f_x;

				notEFM = R.setFlux(node.inputs.at(i).reacNum, f_x);

				if (R.getNum(node.inputs.at(i).reacNum) != -1)
					readyNodes = R.getNum(node.inputs.at(i).reacNum);

				break;
			}
		}
	}//end else
	return R;
}
//*************************************************************************************************
int FindJ(vector<int> waitList, int RN)
{
	int j = 0;

	for (int i=0; i<(int)waitList.size(); i++)
	{
		if (waitList.at(i) == RN)
		{
			j = waitList.at(i+1);
			return j;
		}
		i++;
	}
	printf("there is no ready node in waitlist");
	return j;
}
//*************************************************************************************************
vector<int> checkReadyCase3(NODE node, ArrayR R) // if hameye 8-type nodes have flux
{
	vector<int> Rs;

	for (int i=0; i<(int)node.inputs.size(); i++)
	{
		if(node.inputs.at(i).type == 8)
		{
			if(R.getFlux(node.inputs.at(i).reacNum) != 0)
				Rs.push_back(node.inputs.at(i).reacNum);
		}
	}
	return Rs;
}
//*************************************************************************************************
ArrayR updateReactionArrayNum(ArrayR R, int NodeName, vector<int> NotReadyRs)
{
	for (int i=0; i<(int)NotReadyRs.size(); i++)
	{
		R.num[NotReadyRs.at(i)] = NodeName;
	}
	return R;
}
//*************************************************************************************************

vector	<int>	nodenames;
vector	<int>	reacs;						//	not needed now
vector	<int>	xVec;						//	not needed now

//*************************************************************************************************
//*************************************************************************************************
float findExtraFlux(NODE node, ArrayR R, int nodeCase)	// return global "xVec"
{
	float PorC = 0;
	float FeCeIN = 0, FeCeOUT= 0;

	//------------------------------------------------- not efficient way 
	for (int j=0; j<(int)node.inputs.size(); j++)
	{
		FeCeIN = FeCeIN + abs(R.flux[node.inputs.at(j).reacNum]*node.inputs.at(j).Coef);	
	}

	for (int j=0; j<(int)node.outputs.size(); j++)
	{
		FeCeOUT = FeCeOUT + abs(R.flux[node.outputs.at(j).reacNum]*node.outputs.at(j).Coef);	
	}

	PorC = FeCeIN - FeCeOUT;		// if  (PorC == 0)	 we are Done! 
	return PorC;

	//------------------------------------------------- not efficient way 

	if (nodeCase <= 3)			// just check inputs
	{
		for (int j=0; j<(int)node.inputs.size(); j++)
		{
			if ((node.inputs.at(j).type != 8) && (node.inputs.at(j).type != 18))
			{
				FeCeIN = FeCeIN + abs(R.flux[node.inputs.at(j).reacNum]*node.inputs.at(j).Coef);		
				xVec.push_back(node.inputs.at(j).reacNum);
				//vector<NODE> nodak(NodeS.begin(), NodeS.begin()+k+1);
				//.. // R = findWayAndSync(FeCe, nodeCase, R, NodeS, k, x);
			}
		}// end for inputs

	}//end if 1 2 3

	else if ((nodeCase >= 4) && (((nodeCase & 3)|3)==3))		// just check outputs
	{
		for (int j=0; j<(int)node.outputs.size(); j++)
		{
			if (node.outputs.at(j).type != 19)
			{
				FeCeOUT = FeCeOUT + abs(R.flux[node.outputs.at(j).reacNum]*node.outputs.at(j).Coef);	
				xVec.push_back(node.outputs.at(j).reacNum);
				//vector<NODE> nodak(NodeS.begin(), NodeS.begin()+k+1);
				// .. // R = findWayAndSync(FeCe, nodeCase, R, NodeS, k, x);
			}
		}// end for outputs

	} // end else 4 8 12
	else																// check both
	{
		for (int j=0; j<(int)node.inputs.size(); j++)
		{
			if ((node.inputs.at(j).type == -2) || (node.inputs.at(j).type == -3))
			{
				FeCeIN = FeCeIN + abs(R.flux[node.inputs.at(j).reacNum]*node.inputs.at(j).Coef);				// FeCe
				xVec.push_back(node.inputs.at(j).reacNum);
				//vector<NODE> nodak(NodeS.begin(), NodeS.begin()+k+1);
				// .. // R = findWayAndSync(FeCe, nodeCase, R, NodeS, k, x);
			}

		}// end for inputs
		for (int j=0; j<(int)node.outputs.size(); j++)
		{
			if (node.outputs.at(j).type != 19)	// -4 -5 9 
			{
				FeCeOUT = FeCeOUT + abs(R.flux[node.outputs.at(j).reacNum]*node.outputs.at(j).Coef);			// FeCe
				xVec.push_back(node.outputs.at(j).reacNum);
				//vector<NODE> nodak(NodeS.begin(), NodeS.begin()+k+1);
				// .. // R = findWayAndSync(FeCe, nodeCase, R, NodeS, k, x);
			}
		}// end for outputs
	}// end else all 


	PorC = FeCeIN - FeCeOUT;		// if  (PorC == 0)	 we are Done! 
	return PorC;
}
//*************************************************************************************************
ArrayR update_flux(float /*PorC*/FeCe, NODE node, ArrayR R, int nodeCase/*, vector<int> x extra reaction indices*/, int loopCheck) // return global "nodenames" and "reacs" :: use global "xVec"  
{
	//vector<int> nodenames; 

	int		ReacEighteenNum	= 0, ReacNineteenNum = 0;
	float	c = 0;
	//int		whereisNinetheenInoutput = -1; 
	//float	SigmaFinCin = 0, SigmaFoutCout = 0;

	if (FeCe > 0)						// Prod		=> -2 -3
	{
		if (loopCheck == 0){
			for (int i=0; i<(int)node.outputs.size(); i++)
			{
				//bool nextIf = false;
				if (node.outputs.at(i).type == 19)
				{
					//if (node.outputs.at(i).metabolitNamesIn.size() > 1)							//output has and-inputs (a way to go back)
					//{

					ReacNineteenNum = node.outputs.at(i).reacNum; 
					c				= node.outputs.at(i).Coef;				//C19
					//nodenames		= node.outputs.at(i).metabolitNamesOut; 
					//whereisNinetheenInoutput = i;

					nodenames		= node.outputs.at(i).metabolitNamesOut;					// push_back In 
					for (int h=0; h<(int)node.outputs.at(i).metabolitNamesOut.size();h++)	
						reacs.push_back(ReacNineteenNum);
					for (int h=0; h<(int)node.outputs.at(i).metabolitNamesIn.size();h++)	// push_back Out 	
					{
						if (node.outputs.at(i).metabolitNamesIn.at(h) != node.NodeName){	
							nodenames.push_back(node.outputs.at(i).metabolitNamesIn.at(h));
							reacs.push_back(ReacNineteenNum);
						}
					}
					//break;				//why didnt work??
					//	nextIf = true;
					//}
				}

				//2
				int sign = 0;
				if (R.flux[ReacNineteenNum] < 0)	sign = -1;		else sign = 1;
				R.flux[ReacNineteenNum] =	sign*abs((abs(R.flux[ReacNineteenNum]*c) + abs(FeCe))/c);		//alamat	

				//--- notEFM added 
				if (R.flux[ReacNineteenNum] == 0)
				{
					notEFM = true;
				}
				//--- notEFM added

				//else if ((node.outputs.at(i).type != 19) && nextIf)	{ if (!ifIinX(ReacNineteenNum, xVec)) SigmaFoutCout = SigmaFoutCout + abs(node.outputs.at(i).Coef	*	R.flux[node.outputs.at(i).reacNum]);	}
			}
		}
		//----------------------------------------------------------------------------------------------------------------------- 
		/*if (!nodenames.empty())
		{
		//update 19 
		//1
		//for (int i=0; i<(int)node.inputs.size(); i++)	{ SigmaFinCin = SigmaFinCin + abs(node.inputs.at(i).Coef*R.flux[node.inputs.at(i).reacNum]);}

		//2
		int sign = 0;
		if (R.flux[ReacNineteenNum] < 0)	sign = -1;		else sign = 1;
		R.flux[ReacNineteenNum] =  sign*abs((abs(R.flux[ReacNineteenNum]*c) + abs(FeCe))/c);			//alamat	

		//--- notEFM added 
		if (R.flux[ReacNineteenNum] == 0)
		{
		notEFM = true;
		}
		//--- notEFM added
		}*/
		//else//------------------------------------------------------------------------------------------------------------------
		//{

		// find 18 and update 18
		// push back 18 inputs 
		else if (loopCheck == 1){
			for (int i=0; i<(int)node.inputs.size(); i++)
			{
				if (node.inputs.at(i).type == 18)
				{
					ReacEighteenNum = node.inputs.at(i).reacNum; 
					c				= node.inputs.at(i).Coef;				//C18
					nodenames		= node.inputs.at(i).metabolitNamesIn;					// push_back In 
					for (int h=0; h<(int)node.inputs.at(i).metabolitNamesIn.size();h++)	
						reacs.push_back(ReacEighteenNum);
					for (int h=0; h<(int)node.inputs.at(i).metabolitNamesOut.size();h++)	// push_back Out 	
					{
						if (node.inputs.at(i).metabolitNamesOut.at(h) != node.NodeName){	
							nodenames.push_back(node.inputs.at(i).metabolitNamesOut.at(h));
							reacs.push_back(ReacEighteenNum);
						}
					}
					//break;				//why didnt work??
				}
				//else	{ if (!ifIinX(ReacEighteenNum, xVec))    SigmaFinCin = SigmaFinCin + abs(node.inputs.at(i).Coef	*	R.flux[node.inputs.at(i).reacNum]);	}
			}
			//-----
			//for (int i=0; i<(int)node.outputs.size(); i++)	{ if (!ifIinX(i, xVec))	SigmaFoutCout = SigmaFoutCout + abs(node.outputs.at(i).Coef*R.flux[node.outputs.at(i).reacNum]);}

			int sign = 0;
			if (R.flux[ReacEighteenNum] < 0)	sign = -1;		else sign = 1;
			float R_flux_ReacEighteenNum_old = R.flux[ReacEighteenNum];
			R.flux[ReacEighteenNum] =  sign*abs((abs(R.flux[ReacEighteenNum]*c) - abs(FeCe))/c);			//alamat	

			//--- notEFM added 
			if ((R.flux[ReacEighteenNum] == 0) || (R_flux_ReacEighteenNum_old == R.flux[ReacEighteenNum]))  //improve ::: better coniditon (is this cond true?)
			{
				notEFM = true;
			}
			//--- notEFM added
		}	
		else if (loopCheck >= 2)
		{
			notEFM = true;
		}
		//------------------------------------------------------------------------------------------------------------------
	}//------------------------------------------------------------------------------------------------------------------
	else if (FeCe < 0)					// Cons     => -4 -5 
	{
		if (loopCheck == 0)			//(nodeCase & 4) != 0
		{
			//update 18
			for (int i=0; i<(int)node.inputs.size(); i++)
			{
				if (node.inputs.at(i).type == 18)
				{
					ReacEighteenNum = node.inputs.at(i).reacNum; 
					c				= node.inputs.at(i).Coef;				//C18
					nodenames		= node.inputs.at(i).metabolitNamesIn;					// push_back In 
					for (int h=0; h<(int)node.inputs.at(i).metabolitNamesIn.size();h++)	
						reacs.push_back(ReacEighteenNum);
					for (int h=0; h<(int)node.inputs.at(i).metabolitNamesOut.size();h++)	// push_back Out 	
					{
						if (node.inputs.at(i).metabolitNamesOut.at(h) != node.NodeName){	
							nodenames.push_back(node.inputs.at(i).metabolitNamesOut.at(h));
							reacs.push_back(ReacEighteenNum);
						}
					}

					//break;	//why didnt work??
				}
				//else	{ if (!ifIinX(ReacEighteenNum, xVec))    SigmaFinCin = SigmaFinCin + abs(node.inputs.at(i).Coef	*	R.flux[node.inputs.at(i).reacNum]);	}
			}
			//-----
			//for (int i=0; i<(int)node.outputs.size(); i++)	{ if (!ifIinX(i, xVec))	SigmaFoutCout = SigmaFoutCout + abs(node.outputs.at(i).Coef*R.flux[node.outputs.at(i).reacNum]);}

			int sign = 0;
			if (R.flux[ReacEighteenNum] < 0)	sign = -1;		else sign = 1;
			R.flux[ReacEighteenNum] =  sign*abs((abs(R.flux[ReacEighteenNum] *c) + abs(FeCe) )/c);			//alamat		

			//--- notEFM added 
			if (R.flux[ReacEighteenNum] == 0)
			{
				notEFM = true;
			}
			//--- notEFM added	
		}//------------------------------------------------------------------------------------------------------------------
		else if (loopCheck == 1)
		{
			//update 19 
			for (int i=0; i<(int)node.outputs.size(); i++)
			{
				if (node.outputs.at(i).type == 19)
				{
					ReacNineteenNum = node.outputs.at(i).reacNum; 
					c				= node.outputs.at(i).Coef;				//C19
					nodenames		= node.outputs.at(i).metabolitNamesOut;					// push_back In 
					for (int h=0; h<(int)node.outputs.at(i).metabolitNamesOut.size();h++)	
						reacs.push_back(ReacNineteenNum);
					for (int h=0; h<(int)node.outputs.at(i).metabolitNamesIn.size();h++)	// push_back Out 	
					{
						if (node.outputs.at(i).metabolitNamesIn.at(h) != node.NodeName){	
							nodenames.push_back(node.outputs.at(i).metabolitNamesIn.at(h));
							reacs.push_back(ReacNineteenNum);
						}
					}
					//break;		//why didnt work??
				}
				//else	{ if (!ifIinX(ReacNineteenNum, xVec)) SigmaFoutCout = SigmaFoutCout + abs(node.outputs.at(i).Coef	*	R.flux[node.outputs.at(i).reacNum]);	}
			}

			//for (int i=0; i<(int)node.inputs.size(); i++)	{ SigmaFinCin = SigmaFinCin + abs(node.inputs.at(i).Coef*R.flux[node.inputs.at(i).reacNum]);}

			//2
			int sign = 0;
			if (R.flux[ReacNineteenNum] < 0)	sign = -1;		else sign = 1;
			float R_flux_ReacNineteenNum_old = R.flux[ReacNineteenNum]; 
			R.flux[ReacNineteenNum] =  sign*abs((abs(R.flux[ReacNineteenNum]*c) - abs(FeCe) )/c);			//alamat	

			//--- notEFM added 
			if ((R.flux[ReacNineteenNum] == 0) || (R_flux_ReacNineteenNum_old == R.flux[ReacNineteenNum]))  //improve ::: better coniditon (is this cond true?) 
			{
				notEFM = true;
			}
			//--- notEFM added

		}

		else if (loopCheck >= 2)
		{
			notEFM = true;
		}

		//------------------------------------------------------------------------------------------------------------------
	}//------------------------------------------------------------------------------------------------------------------

	return R;
}
//*************************************************************************************************
vector<FLUX> MakeFluxVectorFromArray(ArrayR R)
{
	vector<FLUX> path;

	for (int i=0; i<bitSetNumOfReac; i++)
	{
		if (R.flux[i] != 0)
		{
			FLUX f; 
			f.reactionFlux = R.flux[i]; f.reationName = i;
			path.push_back(f);
		}
	}

	return path; 
}
//*************************************************************************************************
vector<vector<int>> update_indices(vector<NODE> NodeS/*nodenames, reacs*/, vector<int> indices, vector<int> reacIndices)
{
	vector<vector<int>> tmp; 

	for (int i=0; i<(int) nodenames.size(); i++)
	{
		int ind = findIndeces(NodeS, nodenames.at(i));
		indices.push_back(ind);
		reacIndices.push_back(reacs.at(i));
	}

	nodenames.clear();
	reacs.clear();

	tmp.push_back(indices);
	tmp.push_back(reacIndices);

	return tmp; 
}
//*************************************************************************************************


