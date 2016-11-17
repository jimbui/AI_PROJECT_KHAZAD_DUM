#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <fstream>
#include "Dynamic2DArray.h"

class FileManager
{
private:
public:
	static void SaveFile(string FileName, Dynamic2DArray<double> AIData)
	{
		ofstream saveFile(FileName);

		for (int i = 0; i < AIData.Length(0); i++)
		{
			for (int j = 0; j < AIData.Length(1); j++)
			{
				if (j < AIData.Length(1) - 1)
					saveFile << AIData.GetValue(j, i) << " ";
				else
					saveFile << AIData.GetValue(j, i);
			}

			saveFile << "\n";
		}

		saveFile.close();
	}

	static Dynamic2DArray<double>* LoadFile(string FileName)
	{
		ifstream loadFile(FileName);

		if (loadFile.is_open())
		{
			Dynamic2DArray<double>* fData = new Dynamic2DArray<double>(-1, false, 1, 1);
			int input;

			int xPos = 0;
			int yPos = 0;

			while (loadFile >> input)
			{
				fData->ModifyOrInsertAt(xPos, yPos, input);

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