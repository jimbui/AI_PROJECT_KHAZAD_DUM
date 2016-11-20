#ifndef PLAYERAI_H
#define PLAYERAI_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <time.h>

#include "FileManager.h"

using namespace std;

class PlayerAI
{
private:
	string name;

	// Vector uses information from text file to generate a knowledge table.  For missing/inconclusive values,
	// a linear trendline will be used instead (this is a function).
	vector<vector<double>> knowledgeTable;

	// Vector includes information from saved text file.
	vector<vector<double>>* pastExperiences;

	// Info from decision
	int prevCardValue;
	int decision;

	// Helper Methods ----------------------------------------------------------------------------------------
	void ConvertExperienceToKnowledge(int Buffer)
	{
		// Initialize table
		knowledgeTable = vector<vector<double>>(19);

		for (int i = 0; i < knowledgeTable.size(); i++)
			knowledgeTable[i] = vector<double>(5);

		vector<double> standingWin(19);
		vector<double> standingLoss(19);
		vector<double> standingNoLoss(19);  // Occurs if tie
		vector<double> hittingWin(19);
		vector<double> hittingLoss(19);
		vector<double> hittingNoLoss(19);

		// Get table data
		for (int i = 0; i < (*pastExperiences).size(); i++)
		{
			if ((*pastExperiences)[i][1] == 0 && (*pastExperiences)[i][2] == -1)
				standingNoLoss[(*pastExperiences)[i][0] - 2]++;

			if ((*pastExperiences)[i][1] == 0 && (*pastExperiences)[i][2] == 0)
				standingLoss[(*pastExperiences)[i][0] - 2]++;

			if ((*pastExperiences)[i][1] == 0 && (*pastExperiences)[i][2] == 1)
				standingWin[(*pastExperiences)[i][0] - 2]++;

			if ((*pastExperiences)[i][1] == 1 && (*pastExperiences)[i][2] == 1)
				hittingWin[(*pastExperiences)[i][0] - 2]++;

			if ((*pastExperiences)[i][1] == 1 && (*pastExperiences)[i][2] == 0)
				hittingLoss[(*pastExperiences)[i][0] - 2]++;

			if ((*pastExperiences)[i][1] == 1 && (*pastExperiences)[i][2] == -1)
				hittingNoLoss[(*pastExperiences)[i][0] - 2]++;
		}

		// Determine averages if possible; else fill in zeros.  Use data to fill in table.
		for (int i = 0; i < knowledgeTable.size(); i++)
		{
			// VAL_CARDS
			knowledgeTable[i][0] = i + 2;

			// NOLOSS_IF_HIT
			knowledgeTable[i][1] = (hittingWin[i] > 0 || hittingLoss[i] > 0 || hittingNoLoss[i] > 0 ? 
				(hittingNoLoss[i] + hittingWin[i] + Buffer) / (hittingWin[i] + hittingLoss[i] + hittingNoLoss[i] + Buffer) : 1);

			// NOLOSS_IF_STAND
			knowledgeTable[i][2] = (standingWin[i] > 0 || standingLoss[i] > 0 || standingNoLoss[i] > 0 ?
				(standingWin[i] + standingNoLoss[i]) / (standingWin[i] + standingLoss[i] + standingNoLoss[i] + Buffer) : 0);

			// ERR_HIT
			knowledgeTable[i][3] = (hittingWin[i] + hittingLoss[i] + hittingNoLoss[i] + Buffer > 0 ? 
				1 / (hittingWin[i] + hittingLoss[i] + hittingNoLoss[i] + Buffer) : 1);

			// ERR_STAND
			knowledgeTable[i][4] = (standingWin[i] + standingLoss[i] + standingNoLoss[i] + Buffer > 0 ? 
				1 / (standingWin[i] + standingLoss[i] + standingNoLoss[i] + Buffer) : 1);
		}
	}

public:
	PlayerAI(string Name)
	{
		name = Name;
		
		// Get past experiences if possible.
		pastExperiences = FileManager::LoadFile(name.append(".txt"));

		if (pastExperiences == NULL)
			pastExperiences = new vector<vector<double>>();

		ConvertExperienceToKnowledge(0);

		// Initialize random number generator
		srand(time(NULL));
	}

	int Decision() const { return decision; };

	// Decides whether to request a hit or to stand.
	int MakeADecision(int HandValue)
	{
		prevCardValue = HandValue;
		
		// decision = (knowledgeTable[HandValue - 2][1] > knowledgeTable[HandValue - 2][2] ? 1 : 0);

		if (knowledgeTable[HandValue - 2][1] - knowledgeTable[HandValue - 2][3] * 2 >
			knowledgeTable[HandValue - 2][2] + knowledgeTable[HandValue - 2][4] * 2)
		{
			// If the chance of a successful outcome for a hit minus its error is greater than 
			// the chance of a successful outcome when standing plus its error
			decision = 1;
		}
		else if (knowledgeTable[HandValue - 2][1] + knowledgeTable[HandValue - 2][3] * 2 <
			knowledgeTable[HandValue - 2][2] - knowledgeTable[HandValue - 2][4] * 2)
		{
			// The opposite from the above
			decision = 0;
		}
		else
		{
			// If the margin of error is too great, make a random decision.
			if (knowledgeTable[HandValue - 2][1] > knowledgeTable[HandValue - 2][2])
				decision = 1 - (rand() % 4 == 0);  // How to do a 1 / n...
			else if (knowledgeTable[HandValue - 2][1] < knowledgeTable[HandValue - 2][2])
				decision = (rand() % 4 == 0);
			else
				decision = rand() % 2;
		}


		return decision;
	}

	// Records the results of a move.  PrevCardValue:  2 - 20; Decision:  0 (Stand), 1 (Hit); Result:  0 (Loss), 1 (Win), -1 (NoLoss)
	void RecordResults(int PrevCardValue, int Decision, int Result)
	{
		// Adds to past experiences
		vector<double> newExperience(3);

		newExperience[0] = PrevCardValue;
		newExperience[1] = Decision;
		newExperience[2] = Result;

		pastExperiences->push_back(newExperience);

		// Saves updated past experiences
		FileManager::SaveFile(name, *pastExperiences);

		// Clears the knowledge table before creating a new one.
		for (int i = 0; i < knowledgeTable.size(); i++)
			knowledgeTable[i].clear();

		knowledgeTable.clear();

		// Creates a new knowledge table from experience.
		ConvertExperienceToKnowledge(0);
	}

	// Overload --> Uses saved values of prevCardValue and decision to save results.
	void RecordResults(int Result)
	{
		// Adds to past experiences
		vector<double> newExperience(3);

		newExperience[0] = prevCardValue;
		newExperience[1] = decision;
		newExperience[2] = Result;

		pastExperiences->push_back(newExperience);

		// Saves updated past experiences
		FileManager::SaveFile(name, *pastExperiences);

		// Clears the knowledge table before creating a new one.
		for (int i = 0; i < knowledgeTable.size(); i++)
			knowledgeTable[i].clear();

		knowledgeTable.clear();

		// Creates a new knowledge table from experience.
		ConvertExperienceToKnowledge(0);
	}

	// Use this to clear the AI of its past experiences and reset the file.
	void ResetAI()
	{
		// Clear past experiences + file
		for (int i = 0; i < (*pastExperiences).size(); i++)
			(*pastExperiences)[i].clear();

		pastExperiences->clear();

		FileManager::SaveFile(name, *pastExperiences);

		// Clear the knowledge table
		for (int i = 0; i < knowledgeTable.size(); i++)
			knowledgeTable[i].clear();

		knowledgeTable.clear();
	}

	// Prints the knowledge table.  For debugging/demo purposes only
	void PrintKnowledgeTable()
	{
		cout << "------------------------------------- Knowledge -------------------------------------" << endl << endl;
		cout << "\tVAL_CARDS\tNOLOSS_IF_HIT\tNOLOSS_IF_STAND\tERR_HIT\t\tERR_STAND" << endl;

		for (int i = 0; i < knowledgeTable.size(); i++)
		{
			cout << "\t";

			for (int j = 0; j < knowledgeTable[i].size(); j++)
				cout << round(knowledgeTable[i][j] * 1000) / 1000.0 << "\t\t";

			cout << endl;
		}
		cout << "-------------------------------------------------------------------------------------" << endl << endl;
	}
};

#endif