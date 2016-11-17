/******************************************************************************************************************
*	Blackjack Learning Program
*	Jim Bui and John Santoro (U87402138)
*	Description:  This program learns to play Blackjack in a strategic manner.
*
*	In a game of blackjack, the value of the cards is as follows:
*	A = 1, 11
*	2 ... 10 = value on card
*	J, Q, K = 11
*
*	Knowledge Table (Base):
*				Player-Independent				Player-Dependent (Formulate strategy based on play-style of player)
*	VAL_CARDS	NOLOSS_IF_HIT	LOSS_IF_HIT		WIN_IF_STAND	LOSS_IF_STAND
*	2			1				0				0				1
*	3			1				0				0				1
*	4			1				0				0				1
*	5			1				0				0				1
*	6			1				0				0				1
*	7			1				0				0				1
*	8			1				0				0				1
*	9			1				0				0				1
*	10			1				0				0				1
*	11			1				0				0				1
*	12			1				0				0				1
*	13			1				0				0				1
*	14			1				0				0				1
*	15			1				0				0				1
*	16			1				0				0				1
*	17			1				0				0				1
*	18			1				0				0				1
*	19			1				0				0				1
*	20			1				0				0				1
*
*	The base cases imply the following:  
*	1)  The agent will initially continue accepting cards until it acheives Blackjack or goes bust.  
*	2)  The agent assumes that the player plays the same way.
*	
*	The knowledge table will be used to build two linear trendlines.  The trendlines will be built off of 
*	experimental data only (not base cases), and will be used once [#] games have been played.  Once built, 
*	they will be used to approximate all other values in the table.
*
*	Home Game:  Limited # of cards (home game = 1 deck = 4 of each card)... AI should determine chance of getting 
*	next card from its current hand and what the remaining cards probably are.
*		P(card) = 
******************************************************************************************************************/

#include "FileManager.h"
#include <time.h>

int main()
{
	// Test Code - Create and save a random 2D array ---------------------------------------------------------------
	srand(time(NULL));

	Dynamic2DArray<double>* dn = new Dynamic2DArray<double>(-1, false, 1, 1);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			dn->ModifyOrInsertAt(j, i, rand() % 100);
		}
	}

	FileManager::SaveFile("Data.txt", *dn);
	// End Test Code -----------------------------------------------------------------------------------------------

	// Test Code - Load a 2D array ---------------------------------------------------------------------------------
	Dynamic2DArray<double>* dnl = FileManager::LoadFile("Data.txt");
	dnl->Print();
	// End Test Code -----------------------------------------------------------------------------------------------

	cout << "Your dog ate my homework." << endl;

	cin.get();
	// delete dn;

	return 0;
}