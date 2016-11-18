#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class FileManager
{
private:
public:
	static void SaveFile(string FileName, vector<vector<double>> AIData)
	{
		ofstream saveFile(FileName);

		for (int i = 0; i < AIData.size(); i++)
		{
			for (int j = 0; j < AIData[i].size(); j++)
			{
				if (j < AIData[i].size() - 1)
					saveFile << AIData[i][j] << " ";
				else
					saveFile << AIData[i][j];
			}

			saveFile << "\n";
		}

		saveFile.close();
	}

	static vector<vector<double>>* LoadFile(string FileName)
	{
		ifstream loadFile(FileName);

		if (loadFile.is_open())
		{
			vector<vector<double>>* fData = new vector<vector<double>>();
			int input;

			int xPos = 0;
			int yPos = 0;

			while (loadFile >> input)
			{
				if (yPos == 0)
				{
					vector<double>* v = new vector<double>();
					fData->push_back(*v);
				}

				(*fData)[yPos].push_back(input);

				char duh = loadFile.get();

				xPos++;

				if (duh == '\n')
				{
					xPos = 0;
					yPos++;
				}
			}

			loadFile.close();
			return fData;
		}
		else
		{
			cout << "We are unable to read the file." << endl;
			return NULL;
		}
	}
};

#endif