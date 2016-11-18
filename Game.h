#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>

using namespace std;

class Game
{
private:
public:
	// Coding for all in-game activities
	static void Run()
	{
		// Inititalize the game here. ---------------------------------------------------------------------
		char response;
		int numTrainingSessions = 0;
		int playMode = -1;

		cout << "===========================================================================================" << endl;
		cout << "	Blackjack!" << endl;
		cout << "	Artificial intelligence implemented by Jim Bui and John Santoro." << endl;
		cout << "===========================================================================================" << endl;
		cout << endl;

		while (playMode == -1)
		{
			cout << "[+]	How do you want to play?  (t)rain AI or (p)lay with AI:  ";

			cin >> response;
			while (cin.get() != '\n');

			switch (response)
			{
			case 't':
				cout << "[-]	Entering AI training room..." << endl;
				playMode = 0;
				break;

			case 'p':
				cout << "[-]	Entering game room..." << endl;
				playMode = 1;
				break;

			default:
				cout << "[-]	Invalid entry." << endl;
				break;
			}
		}

		// If play mode is 0 (training), we need to know how many random games to use.  Otherwise, we need
		// to display some general info.
		if (playMode == 0)
		{
			cout << "[+]	Enter the number of training sessions you want to use for the AI:  ";
			cin >> numTrainingSessions;
			while (cin.get() != '\n');
		}
		else
		{
			cout << "[-]	You will be playing alongside [the agent's name] against the dealer:  ";
		}

		// ------------------------------------------------------------------------------------------------

		while (true)
		{
			// Play the game here. ------------------------------------------------------------------------
			cout << endl;
			return;
			// --------------------------------------------------------------------------------------------
		}
	}
};

#endif