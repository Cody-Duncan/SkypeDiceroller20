Introduction
	Welcome to Skype-DiceRoller V2.0. Its newly written in C++ to allow for a faster runtime and better
	random number generation. A host of new features for d10 system has also been added that weren't
	present in the former version, along with a couple fun things for reseeding and to 
	make dice roll commands a bit more entertaining.
	
How to Use
	If this version is a skype extra, access the skype extras menu to open up the application. I have not done testing
	for this as of yet, so I can't say what messages will appear.
	
	If this version is just an exe file, open skype and log in. Open the exe file, and a command prompt should
	appear with the a message noting an attempt to connect to the skype API. Back in the skype window, it will 
	ask if you want to "allow Skype-DiceRoller.exe". Say yes. The command prompt window will confirm the connection.
	
	Go to a chat window in skype. Typing dice-roll commands in the chat will alert
	the diceroller to calculate a random roll, and send a message with the result.

A Basic Command
	A Basic command with most features would be
		//2d20+5        (roll two 20-sided dice and add 5 to the result, calculate the sum)
	
	Though typically you don't roll two d20's. So lets try a more convenient command.
		//d20 			(roll one 20-sided die)
		
	That's useful for Dungeons and Dragons, or games with a similar nature.
	To make things more convenient, it can also be used for sums. Typically,
	dice games have a player roll, and then add on some value on top. So a 
	command that was shown before is available to automate that.
		//d20+5			(roll one 20-sided die and add 5)
	
	And for those that needs to sum several values.
		//d20+5+5+5		(roll one 20-sided die and add 15)

		
Entertainment Features
	As dice games are meant to be fun, the testers thought this bug should become a feature.
	This can be turned off, but by default, the system allows a //d(anything) command
	to roll a d20. So for examples:
		//d_Farfignugen
		//dPotatoTop
		//dKILL-IT-WITH-FIRE
		//d a swing and a miss
	will all roll a single 20-sided die. Have fun with it.
	

Administrator Commands
	Sys:d20, Sys:d10. This chances the system for the roller. Right now there is no functionality specifically for the d20 system.
					  The d10 system calculates success rolls 
	
	ReSeed			  This sets the random number generator to use the command string as a seeding value. A random number from the former seed is
					  added in to ensure that values won't be repeatable by using the same command. However, this regeneration may undo the even 
					  distribution of randomly rolled values. This means that the setup may make some numbers more likely than others. Really 
					  this functioality is only included  to add another level of random generation for those that don't trust pseudorandom numbers, 
					  or for those that feel superstitious that typing //dI-CAN-WIN will give them a better roll.
					  
	Difficulty:#, -Difficulty:#, TDifficulty:#, 10Double:(t/f), 10Again:(t/f), remove:(t/f)
					  These values are specific to the d10 success counter when using the d10 system option. Typically, d10 systems roll some
					  number of d10 dice, and all the dice that roll higher (or lower) than a certain threshhold count as a success. The 
					  Difficulty options allow control over the threshhold value. The 10Double can enable the counter to count a 
					  10 as two successes. The 10Again rule counts the number of 10 rolls, and rolls one additional d10 for every 10 rolled.
					  Remove makes it so that rolls of 1 removes one success.
	
	//DICE-EXIT		  Closes the DiceRoller
	//help			  lists available commands.

					  
Reading Commands and Definitions
	Prefix: Comes before the command syntax to note that it is a command.
	Postfix: Comes after command syntax for extra functionality.
	System: What dice system the program is set to for particular functionality.
	# or #1: represents a number value. #1 represents a number that can be different from #2, and so on.
	(t/f): put "t" or "f" (without quotes or parenthesis) to represent turning a capability ON or OFF respectively.
	(anything): put anything there (disregard parenthesis)
	
	
Command Reference
Prefix to Commands
	//					: all commands are prefixed with // 

Postfix to Commands
	+#					: add on # to the resulting value or sum
	+#1+#2+#3+#4		: add on the sum of the # given
	+					: sum the roll

System Commands (admin only)
	//Sys:d20			: sets the system to d20   (no functionality specific to d20 yet)
	//Sys:d10			: sets the system to d10
	//ReSeed:(t/f)		: sets the system to generate a new seed from the command given. It still adds in a random number from
						  the former seed to ensure values stay random. Warning: usage does not guarantee even distribution of roll
						  values.
	//DICE-EXIT			: Closes the DiceRoller
	//help				: lists available commands.

Commands
	default commands
		//d#			: roll a single die with # sides 
		//d-any:(t/f)	: (admin) mode for typing //d(anything here) for a d20 roll. Default on.
		//d(anything)	: roll a single 20-sided die, only enabled by admin command.
		//#1d#2			: rolls the #1 of dice with #2 of sides, will also print the sum of the numbers
		//d%			: rolls two 10-sided dice with one representing 10s digit, the other representing 1's digit

	d10 - system commands
		//#d(anything)	: rolls the # of 10-sided dice, and reports the successes
		//Difficulty:#  : (admin) sets the threshold for successes to be # or higher, inclusive. Default 8
		//-Difficulty:# : (admin) sets the threshold for successes to be # or lower, inclusive. Default 8
		//TDifficulty:# : (admin) sets the difficulty for the next roll to be #, resets to the original afterward
		//10Double:(t/f): (admin) sets the success counter to count rolls of 0 (means 10) as 2 successes. Default off (f)
		//10Again:(t/f) : (admin) sets the success counter to add 1 success and roll a d10 for every 0 counted. Default off (f)


Credits
	Skype Diceroller V2.0 (C++)
	Original Code Author: Cody Duncan
	Code Assistance: Mr. Scott Bell , Skype Moderator TheUberOverlord
	Future Code Authors: LURKEN
	Icon Artist: Donald Simon, Andrew Sinclair
	Testers: Roy Lee, Matt M., Chad(wick)
	Version 1.2, August 8, 2009 (Java)
	Version 2.0, June 1, 2011 (C++)
	This project was created independently by Cody Duncan, a Computer Science student at Northwest Missouri State University,
	with moral support and concept help from The Fellowship of the Tower Gaming Society (an extremely enjoyable NWMSU campus organization).
	Links: Northwest Missouri State University:  http://www.nwmissouri.edu
	The Fellowship of the Tower Gaming Society:   http://catpages.nwmissouri.edu/m/FOT/
	You can contact the Fellowship of the Tower by emailing: fot@nwmissouri.edu  
	Please support the Fellowship of the Tower, all donations are accepted.

	

"========Command Reference============" "\n" 
"Prefix to Commands---------------------------------------" "\n"
"    // : all commands are prefixed with //" "\n" 
"\n" 
"Postfix to Commands--------------------------------------" "\n" 
"    +#           : add on # to the resulting value or sum" "\n" 
"    +#1+#2+#3+#4 : add on the sum of the # given" "\n" 
"    +            : sum the roll" "\n" 
"\n" 
"System Commands (admin only)-----------------------------""\n" 
"    //Sys:d20      : sets the system to d20" "\n" 
"                     (no functionality specific to d20 yet)" "\n" 
"    //Sys:d10      : sets the system to d10" "\n" 
"    //ReSeed:(t/f) : sets the system to generate a new seed from the" "\n"
"                     command given. It still adds in a random number" "\n"
"                     from the former seed to ensure values stay random" "\n"
"                     Warning: usage does not guarantee even" "\n"
"                     distribution of roll values." "\n"
"    //DICE-EXIT    : Closes the DiceRoller" "\n" 
"    //help         : lists available commands." "\n" 
"\n" 
"Commands-------------------------------------------------" "\n" 
"    //d# : roll a single die with # sides " "\n" 
"    //d-any:(t/f) : (admin) mode for typing //d(anything here)" "\n" 
"                    for a d20 roll. Default on." "\n" 
"    //d(anything) : roll a single 20-sided die." "\n" 
"                    only enabled by admin command." "\n" 
"    //#1d#2       : rolls the #1 of dice with #2 of sides." "\n" 
"                    will also print the sum of numbers." "\n"
"    //d%          : rolls two 10-sided dice with one representing " "\n"
"                    10s digit, the other representing 1's digit." "\n" 
"\n" 
"  d10 - system commands" "\n" 
"    //#d(anything)  : rolls the # of 10-sided dice," "\n" 
"                      and reports the successes" "\n" 
"    //Difficulty:#  : (admin) sets the threshold for successes to be" "\n" 
"                      # or higher, inclusive. Default 8" "\n" 
"    //-Difficulty:# : (admin) sets the threshold for successes to be" "\n" 
"                      # or lower, inclusive. Default 8" "\n" 
"    //TDifficulty:# : (admin) sets the difficulty for the next roll" "\n" 
"                      to be #, resets to the original afterward" "\n" 
"    //10Double:(t/f): (admin) sets the success counter to" "\n" 
"                       count rolls of 0 (means 10) as 2" "\n" 
"                       successes. Default off (f)" "\n" 
"    //10Again:(t/f) : (admin) sets the success counter to add " "\n"
"                      1 success and roll a d10 for every 10 counted." "\n"
"                      Default off (f)" "\n"
"=======End Command Reference========" "\n";

Possible Errors
    "Cannot make an instance of Skype" - the skype4COM.dll hasn't been registered
    This is automatically registered when skype extras is installed. Alternatively:
        1. go to http://developer.skype.com/accessories
        2. download Skype4COM version 1.0.36 (zip archive)
        3. unzip the file
        4. open a command prompt via
            XP: start -> run -> type "cmd" and hit ok
            win7: start -> type "cmd", open 'cmd.exe' from the results
        5. navigate the command prompt to the unzipped folder
            cd "C:\Directory where you unzipped it"
        6. enter
            regsvr32 skype4COM.dll
        7. recieve a confirmation message that the RegisterServer succeeded
	
	Alternatively
		1. go to http://developer.skype.com/accessories
        2. download Skype4COM version 1.0.36 (zip archive)
        3. unzip the file
		2. move Skype4COM.dll to the DiceRoller's directory (wherever you extracted that)
		3. run RegisterSkype4COM.bat (double click on it).
		now it should be registered. This fixes most problems.
        