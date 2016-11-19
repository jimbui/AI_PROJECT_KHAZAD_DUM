#ifndef PLAYERAI_H
#define PLAYERAI_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class PlayerAI
{
private:
	string name;

	// Vector uses information from text file to generate a knowledge table.  For missing/inconclusive values,
	// a linear trendline will be used instead (this is a function).
	vector<vector<double>> knowledgeTable;

	// Vector includes information from saved text file.
	vector<vector<double>> pastExperiences;

	// Helper Methods ----------------------------------------------------------------------------------------
	void ConvertExperienceToKnowledge()
	{
		// Initialize table
		knowledgeTable = vector<vector<double>>(19);

		for (int i = 0; i < knowledgeTable.size(); i++)
			knowledgeTable[i] = vector<double>(3);

		vector<double> standingWin(19);
		vector<double> standingLoss(19);
		vector<double> hittingWin(19);
		vector<double> hittingLoss(19);
		vector<double> hittingNoLoss(19);

		// Get table data
		for (int i = 0; i < pastExperiences.size(); i++)
		{
			if (pastExperiences[i][1] == 0 && pastExperiences[i][2] == 0)
				standingLoss[pastExperiences[i][0] - 2]++;

			if (pastExperiences[i][1] == 0 && pastExperiences[i][2] == 1)
				standingWin[pastExperiences[i][0] - 2]++;

			if (pastExperiences[i][1] == 1 && pastExperiences[i][2] == 1)
				hittingWin[pastExperiences[i][0] - 2]++;

			if (pastExperiences[i][1] == 1 && pastExperiences[i][2] == 0)
				hittingLoss[pastExperiences[i][0] - 2]++;

			if (pastExperiences[i][1] == 1 && pastExperiences[i][2] == -1)
				hittingNoLoss[pastExperiences[i][0] - 2]++;
		}

		// Determine averages if possible; else fill in zeros.  Use data to fill in table.
		for (int i = 0; i < knowledgeTable.size(); i++)
		{
			// VAL_CARDS
			knowledgeTable[i][0] = i + 2;

			// NOLOSS_IF_HIT
			knowledgeTable[i][1] = (hittingWin[i] > 0 || hittingLoss[i] > 0 || hittingNoLoss[i] > 0 ? 
				(hittingNoLoss[i] + hittingWin[i]) / (hittingWin[i] + hittingLoss[i] + hittingNoLoss[i]) : 0);

			// WIN_IF_STAND
			knowledgeTable[i][2] = (standingWin[i] > 0 || standingLoss[i] > 0 ?
				(standingWin[i]) / (standingWin[i] + standingLoss[i]) : 0);
		}
	}

public:
	PlayerAI(string Name, vector<vector<double>> PastExperiences)
	{
		name = Name;
		pastExperiences = PastExperiences;

		ConvertExperienceToKnowledge();
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