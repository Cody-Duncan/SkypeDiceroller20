#include "stdafx.h"
#include "MersenneTwister.h"
using namespace std;
/*

Prefix to Commands
	//					: all commands are prefixed with // 

Postfix to Commands
	+#					: will add on # to the resulting value or sum

System Commands (admin only)
//Sys:d20				: sets the system to d20
//Sys:d10				: sets the system to d10
//ReSeed:(t/f)			: uses the command text to reseed the random number generator. Default OFF. 
						  Warning: does not guarantee even distribution of possible roll values.

Systems
	d20
		//d#            : to roll single dice with # sides 
		//d-any:(t/f)   : (admin) mode for typing //d(anything here) for a d20 roll. Default true.
		//d(anything)	: roll a single 20-sided die, only enabled by admin command.
		//(#1)d(#2)		: rolls the #1 of dice with #2 of sides, will also print the sum of the numbers
		//d%			: rolls two 10-sided dice with one representing 10s digit, the other representing 1's digit

	d10
		//#d(anything)	: rolls the # of 10-sided dice, and reports the successes
		//Difficulty:#  : (admin) sets the threshold for successes to be # or higher, inclusive. Default 8
		//-Difficulty:# : (admin) sets the threshold for successes to be # or lower, inclusive. Default 8
		//TDifficulty:# : (admin) sets the difficulty for the next roll to be #, resets to the original afterward
		//10Double:(t/f): (admin) (t) sets the success counter to count rolls of 0 (means 10) as 2 successes. (f) turns off this functionality. Default off (f)
		//10Again:(t/f) : (admin) (t) sets the success counter to add 1 success and roll a d10 for every 0 counted. Default off (f)

(admin commands, coming soon)

*/

enum System
{
	None, D20, D10
};

class DiceRoller
{
	private:
		string parseAdmin(string command);
		void parseDiceRoll(string command);
		string NumberToString ( int Number );
		void printRoll(std::string sender);
		static const string HELP_MESSAGE;
		bool adminNameIsSet;
		

	protected:
		MTRand randGen;

		System currentSystem;
		int plusVal;
		bool plusUsed;
		bool commandSeed;
		int quantity;
		int sides;
		vector<int> rolledVals;
		vector<int> d10SecondVals;

		//d20 system variables
		bool dAnyAllow;

		//d10 system variables
		int d10_Difficulty;
		int tempDifficulty;
		bool grThanDiff;
		bool d10AgainRule;
		bool d1RemoveRule;
		bool d10DoubleRule;
		bool rolledDPerc;

		string adminName;

	public:
		DiceRoller(string adminName = "");
		~DiceRoller();

		void roll(int quantity, int sides, vector<int>& result);
		int rollD20();
		void rollD10(int quantity, vector<int>& result);
		void rollDPerc(vector<int>& result);
		void setAdminName(string adminName);
		void setDAnyAllow(bool val);
		void setDifficulty(int val);
		void setTempDifficulty(int val);
		void set10Again(bool val);
		void set1Remove(bool val);
		void set10Double(bool val);
		void setCommandSeed(bool val);
		int sum(int* values);
		int sumD10Success();
		int sumRoll();
		string parseCommand(string command, bool isAdmin = false);
		string performRoll(string sender, string command);
		bool isAdmin(std::string name);
};



