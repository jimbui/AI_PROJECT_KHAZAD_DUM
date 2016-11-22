#ifndef EL_BLACKJACK_GAME
#define EL_BLACKJACK_GAME

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include "PlayerAI.h"

class BlackJack_Game
{
	private:

	public:

	void play_ball() // this got messier than intended.
	{
		int play_mode = -1 ;
		char user_response ;
		char rounds_to_play[] = "r" ;
		int rounds_to_play_int = 1 ;
		int remember = -1 ; 

		PlayerAI* p = new PlayerAI("Phil");

		std::cout << " \n" ;
		std::cout << "    =========================================================================================== \n" ;
		std::cout << "        Blackjack! \n" ;
		std::cout << "        implemented by Jim Bui and John Santoro. \n" ;
		std::cout << "    =========================================================================================== \n\n" ;

		std::cout << "           Welcome to HardRock! \n\n" ;

		// get game type.

		while (play_mode == -1)
		{
			std::cout << "    [+]    How do you want to play? \n\n" ;
			std::cout << "           1.    AI versus the house. \n" ;
			std::cout << "           2.    User versus the house. \n\n" ;
			std::cout << "                 " ;

			std::cin >> user_response ;
			while (std::cin.get() != '\n') ; // clear input buffer.
			std::cout << " \n" ;
			// clear_screen() ;

			switch (user_response)
			{
				case '1':
					std::cout << "    [+]    AI is sitting down (1). \n\n" ;
					play_mode = 0 ;
					break ;
				case '2':
					std::cout << "    [+]    User is sitting down (2). \n\n" ;
					play_mode = 1 ;
					break ;
				default:
					std::cout << "    [-]    Invalid entry. \n\n" ;
					break ;
			}
		}

		if (play_mode == 0) // AI versus house.
		{
			while (true)
			{
				std::cout << "    [+]    Enter the number of games for the AI to play: \n\n" ;
				std::cout << "           " ;

				std::cin >> rounds_to_play ;
				while (std::cin.get() != '\n') ; // clear input buffer.
				std::cout << " \n" ;

				if (isdigit(rounds_to_play[0]))
				{
					rounds_to_play_int = atoi(rounds_to_play) ;
					break ;
				}

				else std::cout << "    [-]    Invalid entry. \n\n" ;
			}

		}

		if (play_mode == 0) // AI versus house.
		{
			while (remember != 0 && remember != 1)
			{
				std::cout << "    [+]    Should the AI remember these games?: \n\n" ;
				std::cout << "           1.    Remember. \n" ;
				std::cout << "           2.    Do not remember. \n\n" ;
				std::cout << "                 " ;

				std::cin >> user_response ;
				while (std::cin.get() != '\n') ; // clear input buffer.
				std::cout << " \n" ;
				// clear_screen() ;

				switch (user_response)
				{
					case '1':
						std::cout << "    [+]    AI will remember (1). \n\n" ;
						remember = 1 ;
						break ;
					case '2':
						std::cout << "    [+]    AI will not remember (2). \n\n" ;
						remember = 0 ;
						break ;
					default:
						std::cout << "    [-]    Invalid entry. \n\n" ;
						break ;
				}
			}

		}

		// starting the game.

		std::cout << "    [+]    Starting the game. \n\n" ;

		bool flag = true ;
		bool victory_flag = false ;
		int game_count = 1 ;
		srand(time(NULL)) ;
		int win = 0 ;
		int lose = 0 ;
		int draw = 0 ;

		while (flag) // play the game as many times as needed.
		{
			victory_flag = false ;

			while (!victory_flag)
			{
				std::vector<int> the_deck = {0 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4 , 4  , 4 , 4 , 4 , 4} ; // {X , A , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , J , Q , K}.
				
				// array of eleven since that is the most cards you can without being a loser.

				std::vector<int> player_hand(11) ; // can view all.
				int player_hand_count = 0 ;
				std::vector<int> dealer_hand(11) ; // first card will be hidden from view.
				int dealer_hand_count = 0 ;

				if (rounds_to_play_int > 1) // for games of multiple rounds with AI.
				{
					std::cout << "    [+]    GAME " << game_count << " \n\n" ;
					game_count++ ;
				}

				// draw first card for each.

				player_hand[player_hand_count] = rand() % 12 + 1 ;
				the_deck[player_hand[player_hand_count]]-- ;
				player_hand_count++ ;
				dealer_hand[dealer_hand_count] = rand() % 12 + 1 ;
				the_deck[dealer_hand[dealer_hand_count]]-- ;
				dealer_hand_count++ ;

				// draw second card for each.

				player_hand[player_hand_count] = rand() % 12 + 1 ;
				the_deck[player_hand[player_hand_count]]-- ;
				player_hand_count++ ;
				dealer_hand[dealer_hand_count] = rand() % 12 + 1 ;
				the_deck[dealer_hand[dealer_hand_count]]-- ;
				dealer_hand_count++ ;

				print_game_so_far(player_hand , player_hand_count , dealer_hand , dealer_hand_count) ;

				// let's check if you're blackjacked.

				if (check_hand(player_hand , player_hand_count) == 21)
				{
					if (play_mode == 1) std::cout << "           You got blackjack!  You win! \n\n" ;
					else std::cout << "           The AI got blackjack!  AI wins! \n\n" ;
					win++ ;
					break ; // yeah!  it works!
				}

				// this is where we hit or stand.
				while (true)
				{
					// hit or stand for mr. user.

					if (play_mode == 1)
					{
						game_count++ ;

						while (true)
						{
							std::cout << "           Hit or stand? (h / s) \n           " ;
							std::cin >> user_response ;
							while (std::cin.get() != '\n') ; // clear input buffer.

							if (user_response == 'h' || user_response == 's') break ;
							else std::cout << "           Invalid response. \n" ;
						}

						if (user_response == 's')
						{
							std::cout << "           You chose to stand. \n" ;
							break ;
						}

						else 
						{
							std::cout << "           You chose to draw. \n" ;
							player_hand[player_hand_count] = rand() % 12 + 1 ;
							the_deck[player_hand[player_hand_count]]-- ;
							player_hand_count++ ;

							// std::this_thread::sleep_for(std::chrono::milliseconds(1000));

							print_game_so_far(player_hand , player_hand_count , dealer_hand , dealer_hand_count) ;

							if (check_hand(player_hand , player_hand_count) >= 21) break ;
						}
					}

					// AI MOVE.

					else
					{
						// let the AI decide to do shit or not.

						if (rounds_to_play_int == 1)  // Display I/O if only one game is being played.
						{
							std::cout << "           Hit or stand? \n";
							// std::this_thread::sleep_for(std::chrono::milliseconds(1000 + rand() % 2000));
						}

						p->MakeADecision(check_hand(player_hand, player_hand_count));
						
						// if (rounds_to_play_int == 1)  // Display I/O if only one game is being played.
						{
							if (p->Decision() == 0)
								std::cout << "           AI chose to stand. \n";
							else
								std::cout << "           AI chose to draw. \n";
						}
						
						if (p->Decision() == 1)  // Hit... AI hand receives a card.
						{
							player_hand[player_hand_count] = rand() % 12 + 1;
							the_deck[player_hand[player_hand_count]]--;
							player_hand_count++;

							// std::this_thread::sleep_for(std::chrono::milliseconds(1000));

							print_game_so_far(player_hand, player_hand_count, dealer_hand, dealer_hand_count);

							if (check_hand(player_hand, player_hand_count) >= 21)
								break;
							else
								if (remember == 1) p->RecordResults(-1);  // AI records a hit that was not a loss.
						}
						else  // Stand
							break;

						// break ; --> Line removed so that game will not exit unless AI decides to stop receiving cards, wins, or goes bust.
					}
				}

				// checking for 21 , etc.

				if (check_hand(player_hand , player_hand_count) == 21) // check for win.
				{
					if (play_mode == 1) std::cout << "           You got blackjack!  You win! \n\n" ;
					else
					{
						//if (rounds_to_play_int == 1)
							std::cout << "           The AI got blackjack!  AI wins! \n\n";

						if (remember == 1) p->RecordResults(1);  // AI records a hitting win.
					}
					win++ ;
					break ; // yeah!  it works!
				}

				if (check_hand(player_hand , player_hand_count) > 21) // check for lose.
				{
					if (play_mode == 1) std::cout << "           You went over!  You lost! \n\n" ;
					else
					{
						//if (rounds_to_play_int == 1)
							std::cout << "           The AI went over!  AI loses! \n\n";

						if (remember == 1) p->RecordResults(0);  // AI records a hitting loss.
					}
					lose++ ;
					break ; // yeah!  it works!
				}

				// now let the dealer do its thing.  same for both AI and user players.

				std::cout << "           It is now the dealer's turn. \n" ;

				while (check_hand(dealer_hand , dealer_hand_count) < 17)
				{
					std::cout << "           The dealer draws. \n" ;
					dealer_hand[dealer_hand_count] = rand() % 12 + 1 ;
					the_deck[dealer_hand[dealer_hand_count]]-- ;
					dealer_hand_count++ ;
					
					// std::this_thread::sleep_for(std::chrono::milliseconds(2000));
					
					if (check_hand(dealer_hand , dealer_hand_count) < 21) print_game_so_far(player_hand , player_hand_count , dealer_hand , dealer_hand_count) ;
				}

				if (check_hand(dealer_hand , dealer_hand_count) > 21) // check for lose.
				{
					print_game_reveal(player_hand , player_hand_count , dealer_hand , dealer_hand_count) ;
					if (play_mode == 1) std::cout << "           Dealer went over!  You win! \n\n" ;
					else
					{
						// if (rounds_to_play_int == 1)
							std::cout << "           The dealer went over!  AI wins! \n\n";

						if (remember == 1) p->RecordResults(1);
					}
					win++ ;
					break ; // yeah!  it works!
				}

				std::cout << "           The dealer stays. \n" ;

				// std::this_thread::sleep_for(std::chrono::milliseconds(2000));

				std::cout << "           These are the final hands! \n" ;
				print_game_reveal(player_hand , player_hand_count , dealer_hand , dealer_hand_count) ;

				if (check_hand(dealer_hand , dealer_hand_count) > check_hand(player_hand , player_hand_count)) // dealer has more.
				{
					std::cout << "           The dealer ended up having a higher hand.  Dealer wins. \n\n" ;
					lose++ ;
					if (play_mode == 0)
						if (remember == 1) p->RecordResults(0);  // AI records a standing loss.

					break ;
				}

				if (check_hand(dealer_hand , dealer_hand_count) < check_hand(player_hand , player_hand_count)) // player / AI has more.
				{

					if (play_mode == 1) std::cout << "           You ended up having a higher hand.  You win. \n\n" ;
					else
					{
						// if (rounds_to_play_int == 1)
							std::cout << "           The AI ended up having a higher hand.  AI win. \n\n";

						if (remember == 1) p->RecordResults(1);  // AI records a standing win.
					}
					win++ ;
					break ;
				}

				else 
				{
					if (play_mode == 1)
						std::cout << "           It is a draw. \n\n" ;  // Print output if it should be shown.
					
					if (play_mode == 0)
					{
						if (remember == 1) p->RecordResults(-1);  // AI records standing but not losing.
						std::cout << "           It is a draw. \n\n" ;
					}
					draw++ ;
					break ;
				}

				// victory_flag = true ;
			}

			// playing again for user.

			if (play_mode == 1)
			{
				while (true)
				{
					std::cout << "           Play again? (y / n) \n           " ;
					std::cin >> user_response ;
					while (std::cin.get() != '\n') ; // clear input buffer.

					if (user_response == 'y' || user_response == 'n') break ;
					else std::cout << "           Invalid response. \n" ;
				}

				if (user_response == 'y') 
				{
					game_count = 2 ;
					std::cout << "\n    [+]    Starting the game. \n\n" ;
				}

				else 
				{
					std::cout << " \n" ;
					break ;
				}
			}
			
			// p->PrintKnowledgeTable();
			// std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			if (play_mode != 1)
			{
				if (game_count > rounds_to_play_int) break ; // delete_me.
				if (rounds_to_play_int == 1) break ;
			}
		}

		// std::cout << "seeya bitch \n" ;

		std::cout << "    [-]    Exiting the the HardRock with " << win << " wins, " << draw << " draws, and " << lose << " loses. \n\n" ;
	}

	void print_game_so_far(std::vector<int> player_array , int player_count , std::vector<int> dealer_array , int dealer_count)
	{
		std::cout << "           Player Hand: " ; print_hand(player_array , player_count) ; std::cout << " count: " << check_hand(player_array , player_count) << " \n" ;
		std::cout << "           Dealer Hand: " ; print_hand_hidden(dealer_array , dealer_count) ; std::cout << " count: ? \n" ;
	}

	void print_game_reveal(std::vector<int> player_array , int player_count , std::vector<int> dealer_array , int dealer_count)
	{
		std::cout << "           Player Hand: " ; print_hand(player_array , player_count) ; std::cout << " count: " << check_hand(player_array , player_count) << " \n" ;
		std::cout << "           Dealer Hand: " ; print_hand(dealer_array , dealer_count) ; std::cout << " count: " << check_hand(dealer_array , dealer_count) << " \n" ;
	}

	void print_hand(std::vector<int> array , int count) // prints the hand.
	{
		std::cout << "[" ;

		for (int i = 0 ; i < count - 1 ; i++) 
		{
			switch (array[i])
			{
				case 1: std::cout << "A , " ; break ;
				case 11: std::cout << "J , " ; break ;
				case 12: std::cout << "Q , " ; break ; 
				case 13: std::cout << "K , " ; break ;
				default: std::cout << array[i] << " , " ;
			}
		}

		switch (array[count - 1])
		{
			case 1: std::cout << "A]" ; break ;
			case 11: std::cout << "J]" ; break ;
			case 12: std::cout << "Q]" ; break ; 
			case 13: std::cout << "K]" ; break ;
			default: std::cout << array[count - 1] << "]" ;
		}
	}

	void print_hand_hidden(std::vector<int> array , int count) // prints the hand.
	{
		std::cout << "[? , " ;

		for (int i = 1 ; i < count - 1 ; i++) 
		{
			switch (array[i])
			{
				case 1: std::cout << "A , " ; break ;
				case 11: std::cout << "J , " ; break ;
				case 12: std::cout << "Q , " ; break ; 
				case 13: std::cout << "K , " ; break ;
				default: std::cout << array[i] << " , " ;
			}
		}

		switch (array[count - 1])
		{
			case 1: std::cout << "A]" ; break ;
			case 11: std::cout << "J]" ; break ;
			case 12: std::cout << "Q]" ; break ; 
			case 13: std::cout << "K]" ; break ;
			default: std::cout << array[count - 1] << "]" ;
		}
	}

	int check_hand(std::vector<int> array , int count) // checks the value of hand.  returns upper value of sum.  clean this shit up.
	{
		bool is_ace = false ;
		int sum = 0 ;

		for (int i = 0 ; i < count ; i++) 
		{
			if (array[i] == 11 || array[i] == 12 || array[i] == 13) sum = sum + 10 ;
			else sum = sum + array[i] ;

			if (array[i] == 1) 
			{
				if (!is_ace)
				{
					is_ace = true ;
					sum = sum + 10 ;
				}
			}
		}

		if (sum > 21 && is_ace) return sum - 10 ;
		return sum ;
	}
} ;

#endif