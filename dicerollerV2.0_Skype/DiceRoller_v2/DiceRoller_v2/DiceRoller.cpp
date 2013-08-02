#include "stdafx.h"
#include "DiceRoller.h"

const string DiceRoller::HELP_MESSAGE = 
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
"    //DICE-EXIT    : (admin) Closes the DiceRoller" "\n" 
"    //help         : lists available commands." "\n"
"    //DICE-ABOUT   : (admin) displays the program credits." "\n"
"                     I appreciate if it you show everyone! Thanks!" "\n"
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
"    //remove:(t/f)  : (admin) sets the success counter to remove " "\n"
"					   1 sucess on rolls of 1. " "\n"
"					   Default off (f)" "\n"
"=======End Command Reference========" "\n";

DiceRoller::DiceRoller(string adminName)
{
    commandSeed = false;
    this->adminName = adminName;
    this->adminNameIsSet = false;

    currentSystem = None;
    plusVal = 0;
    plusUsed = false;

    //d20 system variables
    dAnyAllow = true;

    //d10 system variables
    d10_Difficulty = 8;
    tempDifficulty = 0;
    grThanDiff = true;
    d10AgainRule = false;
    d10DoubleRule = false;
    d1RemoveRule = false;
    rolledDPerc = false;
}

DiceRoller::~DiceRoller()
{
    
}

void DiceRoller::dispose()
{
    if( this != NULL && this->rolledVals.size() > 0)
    {
        this->rolledVals.clear();
    }
}

string DiceRoller::parseCommand(string command, bool isAdmin)
{
    if((int)command.find("//") ==0 && command.length() >= 3)		//starts with //, has more than just //
    {
        command = command.substr(2);
        
        //d-any
        if( command.length() >= 7 && command.compare(0,6, "d-any:")==0 ) 
        {
            if(command[6] =='t' || command[6] =='T')
            {
                setDAnyAllow(true);
                return "SYSTEM: d-Any allowed ON";
            }
            else if ( command[6] == 'f' || command[6] =='F')
            {
                setDAnyAllow(false);
                return "SYSTEM: d-Any allowed OFF";
            }
        }

        //dicerolling commands
        if(command.find("d") != string::npos)
        {
            parseDiceRoll(command);
        }

        //admin commands
        if(isAdmin)
        {
            return parseAdmin(command);
        }

    }
    return "";
}

void DiceRoller::parseDiceRoll(string command)
{
    vector<int> result;
    int letterDPos = string::npos;
    int plusPos = string::npos;
    

    //starts with d
    if(command[0] == 'd')
    {
        if( command.length() == 1) 					
        {
            result.push_back(rollD20());
            quantity = 1;
            sides = 20;
        }
        else if ( command.length() >= 2 )
        {
            if(command[1] == '%') 
            {
                rollDPerc(result);
                quantity = 2;
                sides = 10;
            }
            else if( isdigit(command[1]) )
            {
                quantity = 1;
                sides = atoi(command.substr(1).c_str());
                roll(1, sides, result );
            }
            else
            {
                // the //d-anything
                if(command.compare(0,5, "diff:")!=0)
                {
                    result.push_back(rollD20());
                    quantity = 1;
                    sides = 20;
                }
            }
        }
    }
    //starts with a number and contains 'd'
    else if( isdigit(command[0]) && (letterDPos = command.find("d")) != string::npos )
    {
        quantity = atoi(command.c_str()) ;
        if(command.length() >= letterDPos+2 && isdigit(command[letterDPos+1]) )
        {
            sides = atoi(command.substr(letterDPos+1).c_str());
            roll(quantity, sides, result);
        }
        else
        {
            sides = 10;
            rollD10(quantity, result);
        }
    }

    //check for plus symbol usage
    if( command.find("+") != string::npos)
    {
        plusUsed = true;

        while( (plusPos = command.find("+", plusPos+1)) != string::npos )
        {
            if( command.length() > plusPos+1 && isdigit(command[plusPos+1]) )
            {
                plusVal += atoi(command.substr(plusPos+1).c_str());
            }
        }
    }
    

    this->rolledVals = result;
}

string DiceRoller::parseAdmin(string command)
{
    //Change the command into lower-case
    for(int i = 0; command[i] != '\0'; i++){
        command[i] = tolower(command[i]);
    }

    //std::cout << command <<", lenght:"<<command.size();

    if( command.length()==4 && command.compare(0,4, "help")==0)
    {
        std::cout << HELP_MESSAGE;
        return HELP_MESSAGE;
    }
    if( command.length() >= 7 && (command.compare(0,4, "sys:")==0 || command.compare(0,6, "system:")==0) )
    {
        if(command.compare(4,3, "d20")==0)
        {
            currentSystem = D20;
            return "SYSTEM: Dice System changed to d20";
        }
        else if(command.compare(4,3, "d10")==0)
        {
            currentSystem = D10;
            return "SYSTEM: Dice System changed to d10";
        }
    }
    else if(command.length() >= 6 && command.compare(0,5, "diff:")==0 && isdigit(command[5]))
    {
        grThanDiff = true;
        setDifficulty(atoi(command.substr(5).c_str()));
        return "SYSTEM: Success difficulty set to >= " +  NumberToString(this->d10_Difficulty);
    }
    else if( command.length() >= 12 && command.compare(0,11, "difficulty:")==0 && isdigit(command[11]) )
    {
        grThanDiff = true;
        setDifficulty(atoi(command.substr(11).c_str()));
        return "SYSTEM: Success difficulty set to >= " +  NumberToString(this->d10_Difficulty);
    }
    else if(command.length() >= 7 && command.compare(0,6, "-diff:")==0 && isdigit(command[6]))
    {
        grThanDiff = false;
        setDifficulty( atoi( command.substr(6).c_str() ) );
        return "SYSTEM: Success difficulty set to <= " + NumberToString(this->d10_Difficulty);
    }
    else if( command.length() >= 13 && command.compare(0,12, "-difficulty:")==0 && isdigit(command[12]) )
    {
        grThanDiff = false;
        setDifficulty( atoi( command.substr(12).c_str() ) );
        return "SYSTEM: Success difficulty set to <= " + NumberToString(this->d10_Difficulty);
    }
    else if(command.length() >= 7 && command.compare(0,6, "tdiff:")==0 && isdigit(command[6]))
    {
        setTempDifficulty(atoi(command.substr(6).c_str()));
        return "SYSTEM: Temporary difficulty set to >= " + NumberToString(this->tempDifficulty);
    }
    else if( command.length() >= 13 && command.compare(0,12, "tdifficulty:")==0 && isdigit(command[12]))
    {
        setTempDifficulty(atoi(command.substr(12).c_str()));
        return "SYSTEM: Temporary difficulty set to >= " + NumberToString(this->tempDifficulty);
    }
    else if( command.length() >= 10 && command.compare(0,9, "10double:")==0 )
    {
        if(command[9] =='t' || command[9] =='T')
        {
            set10Double(true);
            return "SYSTEM: d10 Double Rule ON, 10's count for 2 successes";
        }
        else if ( command[9] == 'f' || command[9] =='F')
        {
            set10Double(false);
            return "SYSTEM: d10 Double Rule OFF, 10's count for 1 success";
        }
    }
    else if( command.length() >= 9 &&  command.compare(0,8, "10again:")==0 )
    {
        if(command[8] =='t' || command[8] =='T')
        {
            set10Again(true);
            return "SYSTEM: d10 Again Rule ON, 10's give 1 success and an extra roll";
        }
        else if ( command[8] == 'f' || command[8] =='F')
        {
            set10Again(false);
            return "SYSTEM: d10 Again Rule OFF, 10's count for 1 success";
        }
    }
    else if( command.length() >= 8 &&  command.compare(0,7, "remove:")==0 )
    {
        if(command[7] =='t' || command[7] =='T')
        {
            set1Remove(true);
            return "SYSTEM: d1 Remove Rule ON, 1's removes one success";
        }
        else if ( command[7] == 'f' || command[7] =='F')
        {
            set1Remove(false);
            return "SYSTEM: d1 Remove Rule OFF, 1's dont remove one sucess";
        }
    }
    else if( command.length() >= 8 && command.compare(0,7, "reseed:") == 0 )
    {
        if(command[7] =='t' || command[7] =='T')
        {
            setCommandSeed(true);
            return "SYSTEM: ReSeed by command is ON. Seeds will be generated by the command given.";
        }
        else if ( command[7] == 'f' || command[7] =='F')
        {
            setCommandSeed(false);
            return "SYSTEM: ReSeed by command is OFF. Seeds will be randomly generated.";
        }
    }
    return "";
}

void DiceRoller::setAdminName(string adminName)
{
    if(this->adminNameIsSet == false)
    {
        this->adminName = adminName;
        this->adminNameIsSet == true;
    }
}


void DiceRoller::setDAnyAllow(bool val)
{
    this->dAnyAllow = val;
}

void DiceRoller::set10Again(bool val)
{
    this->d10AgainRule = val;
}

void DiceRoller::set1Remove(bool val)
{
    this->d1RemoveRule = val;
}

void DiceRoller::set10Double(bool val)
{
    this->d10DoubleRule = val;
}

void DiceRoller::setCommandSeed(bool val)
{
    this->commandSeed = val;
}	

void DiceRoller::setDifficulty(int val)
{
    this->d10_Difficulty = val;
}

void DiceRoller::setTempDifficulty(int val)
{
    this->tempDifficulty = val;
}

void DiceRoller::roll(int quantity, int sides, vector<int>& result)
{
    for(int i = 0; i < quantity; i++)
    {
        result.push_back(randGen.randInt(sides-1)+1);
    }
}

int DiceRoller::rollD20()
{
    return randGen.randInt(19)+1;
}

void DiceRoller::rollD10(int quantity, vector<int>& result)
{
    for(int i = 0; i < quantity; i++)
    {
        result.push_back(randGen.randInt(9)+1);
    }
}
void DiceRoller::rollDPerc(vector<int>& result)
{
    result.push_back(randGen.randInt(10)+1);
    result.push_back(randGen.randInt(10)+1);
    rolledDPerc = true;
}

string DiceRoller::performRoll(string sender, string command)
{
    plusVal = 0;
    quantity = 0;
    sides = 0;
    this->rolledVals.clear();
    string returnString = "";

    if(commandSeed)
    {
        randGen.seed( (MTRand::uint32)command.c_str() + randGen.randInt() );
    }
    
    //parse the command string, and performs the roll or admin command.
    returnString = parseCommand(command, isAdmin(sender));

    //creat the output values
    if(rolledVals.size() > 0 && quantity > 0 && sides > 0)
    {
        char dest[128];
        sprintf(dest, "%s rolled %2dd%2d; result:", sender.c_str(), quantity, sides);
        returnString += dest;

        for(int i = 0; i < rolledVals.size(); i++)
        {
            if(i != 0 && i%10 != 0)
            {
                returnString += ", ";
            }
            else if(i % 10 == 0)
            {
                returnString += "\n";
            }
            sprintf(dest, "%2d", (int)rolledVals[i]);
            returnString += dest;
            
        }

        if(plusUsed)
        {
            returnString += "   +";
            returnString += NumberToString(plusVal);
            returnString += "\nSum: ";
            returnString += NumberToString(sumRoll());
        }

        if(currentSystem == D10)
        {
            int successes = sumD10Success();
            if(d10AgainRule)
            {
                returnString += "\nRerolls: ";
                for(int i = 0; i < d10SecondVals.size(); i++)
                {
                    if(i != 0 && i%10 != 0)
                    {
                        returnString += ", ";
                    }
                    else if(i % 10 == 0)
                    {
                        returnString += "\n";
                    }
                    sprintf(dest, "%2d", (int)d10SecondVals[i]);
                    returnString += dest;
                }
            }
            returnString += "\nNumber of Successes: ";
            returnString +=	NumberToString(successes);
        }
    }
    
    std::cout << returnString << std::endl;
    return returnString;
}

string DiceRoller::NumberToString ( int Number )
{
    std::stringstream ss;
    ss << Number;
    return ss.str();
}

int DiceRoller::sumD10Success()
{
    d10SecondVals.clear();
    int successCount = 0;
    int againCount = 0;

    if(currentSystem == D10)
    {
        for(int i = 0; i < rolledVals.size(); i++)
        {
            if( (tempDifficulty != 0 && rolledVals[i] >= tempDifficulty) || (grThanDiff && rolledVals[i] >= d10_Difficulty && tempDifficulty == 0) )
            {
                if( rolledVals[i] == 10 )
                {
                    againCount++;
                    if(d10DoubleRule)
                        successCount++;
                }
                successCount++;
            }
            else if (!grThanDiff && rolledVals[i] <= d10_Difficulty && tempDifficulty == 0)
            {
                successCount++;
            }

            if(this->d1RemoveRule && rolledVals[i] == 1){
                successCount--;
            }

        }

        if(this->d10AgainRule)
        {

            //All of the rerolls
            vector<int> rerolled;

            //Makes sure that we rereoll the rerolls and so on(as long as we get some tens.
            while(againCount != 0){

                d10SecondVals.clear();

                this->rollD10(againCount, d10SecondVals);//Get next batch of rerolls
                againCount=0;

                for(int i = 0; i < d10SecondVals.size(); i++)
                {
                    rerolled.push_back(d10SecondVals[i]);

                    if( (grThanDiff && d10SecondVals[i] >= d10_Difficulty) )
                    {
                        successCount++;
                    }
                    else if (!grThanDiff && d10SecondVals[i] <= d10_Difficulty)
                    {
                        successCount++;
                    }

                    if(d10SecondVals[i] == 10){
                        againCount++;
                    }
                }
            }
            //Reset the vector to all of the rerolls
            d10SecondVals = rerolled;
        }
    }

    tempDifficulty = 0;
    return successCount;
}

int DiceRoller::sumRoll()
{
    int sum = plusVal;
    for(int i = 0; i < rolledVals.size(); i++)
    {
        sum += (int)rolledVals[i];
    }
    return sum;
}

bool DiceRoller::isAdmin(std::string name)
{
    if(name.compare(adminName) == 0)
    {
        return true;
    }
    return false;
}

int numDigits(int n)
{
    int count = 1;
    while(n != 0)
    {
        n/=10;
        count++;
    }
    return count;
}