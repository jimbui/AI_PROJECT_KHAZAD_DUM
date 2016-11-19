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
	void ConvertExperienceToKnowledge()
	{
		// Initialize table
		knowledgeTable = vector<vector<double>>(19);

		for (int i = 0; i < knowledgeTable.size(); i++)
			knowledgeTable[i] = vector<double>(3);

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
				(hittingNoLoss[i] + hittingWin[i]) / (hittingWin[i] + hittingLoss[i] + hittingNoLoss[i]) : 1);

			// NOLOSS_IF_STAND
			knowledgeTable[i][2] = (standingWin[i] > 0 || standingLoss[i] > 0 || standingNoLoss[i] > 0 ?
				(standingWin[i] + standingNoLoss[i]) / (standingWin[i] + standingLoss[i] + standingNoLoss[i]) : 0);
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

		ConvertExperienceToKnowledge();

		// Initialize random number generator
		srand(time(NULL));
	}

	int Decision() const { return decision; };

	// Decides whether to request a hit or to stand.
	int MakeADecision(int HandValue)
	{
		prevCardValue = HandValue;
		decision = (knowledgeTable[HandValue - 2][1] > knowledgeTable[HandValue - 2][2] ? 1 : 0); // Insert AI function here.

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
		ConvertExperienceToKnowledge();
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
		ConvertExperienceToKnowledge();
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
		cout << "----------------------- Knowledge -----------------------" << endl << endl;
		cout << "\tVAL_CARDS\tNOLOSS_IF_HIT\tWIN_IF_STAND" << endl;

		for (int i = 0; i < knowledgeTable.size(); i++)
		{
			cout << "\t";

			for (int j = 0; j < knowledgeTable[i].size(); j++)
				cout << round(knowledgeTable[i][j] * 100) / 100.0 << "\t\t";

			cout << endl;
		}
		cout << "---------------------------------------------------------" << endl << endl;
	}
};

#endif