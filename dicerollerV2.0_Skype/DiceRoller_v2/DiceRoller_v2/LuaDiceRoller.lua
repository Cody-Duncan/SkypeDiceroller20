--[[
--DiceRoller.lua -- Script for Skype DiceRoller V2.0
--Author:Cody Duncan
--Note: Somewhat poorly written. I have plans to refactor it now that I have taken compilers. :D

DiceRoller V2.0 expects to call three functions:

string performRoll(string senderID, string displayName, string command) // performs the dice roll, returns a string that prints as a message to skype
void   setAdminName(string adminName)                                  // sets the admin's name (when the diceroller starts, sets it to local user)
bool   isAdmin(string name)                                            // checks if the arguement is the admin's name (checked on every command)

The DiceRand library is just a custom library built into DiceRoller V2.0 that gives access to a random number generator
using the Mersenne Twister Algorithm ( by Makoto Matsumoto, Takuji Nishimura, and Shawn Cokus), 
this particular edition written by Richard J. Wagner.
DiceRand functions:
void seed(int seedNum)  // seeds the random number generator
int rand()              //generates a random integer (often very large)
int rand(int limit)    //generates a random positive integer that is less than the limit

--]]


require("DiceRand");

admin = ""             --string - name of admin
rolledVals = {}        --array of numbers generated
commandSeed = false    --bool - whether to reseed the diceroller using the current command as part of the seed
plusUsed = false       --bool - whether the command contained + ( such as //d20+ )
plusVal = 0            --int  - value after the + symbol in the command( such as //d20+2 )
quantity = 0           --int  - number of dice to roll. //2d10 -> quantity == 2
sides = 0              --int  - number of sides on dice to roll. //2d10 -> sides == 10

-- bool - returns whether String starts with Start
function string.starts(String,Start)
   return string.sub(String,1,string.len(Start))==Start
end

-- string - returns character at index i in String
function string.at(String, i)
    return string.sub(String, i, i)
end

-- bool - Compares the value of str (or a substring) to otherString.
-- returns true if the specified substring of str is the same as otherString in content.
function string.compare(str,start,length,otherString)
    return string.sub(str,start,start+length-1) == otherString
end



-- bool - determines whether the given string is a numerical value
function isNumber(val)
    return tonumber(val) ~= nil 
end

-- array - turns an iterator into an array
function BuildArray(...)
  local arr = {}
  for v in ... do
    arr[#arr + 1] = v
  end
  return arr
end


-- Parses a command, an determines whether to evaluate it as an administrator command
--  or a dice roll.
function parseCommand(command, isAdmin)
    if (string.starts(command, "//") and string.len(command) >= 3) then
        command = string.sub(command,3)

        --d-any
        if (string.len(command) >= 7 and string.compare(command,1,6,"d-any:")) then
        --TODO d-any
        end

        if(string.find(command,"d") ~= nil) then
            parseDiceRoll(command)
        end

        if(isAdmin) then
            return parseAdmin(command)
        end
    end
    return ""
end

--parses and performs an administrator command TODO
function parseAdmin(command)
    return ""
end

--parses and returns the result of a dice roll
--note: this parses just the command string, without the // at the start
function parseDiceRoll(command)
    local result = {}
    letterDPos = -1
    plusPos = -1

    letterDPos = string.find(command,"d")

    if(letterDPos == 1) then
        --//d
        if(string.len(command) == 1) then
            quantity = 1
            sides = 20
            roll(quantity, sides, result );
        --//d%
        elseif( string.at(command,2) == "%") then
            rollDPerc(result)
            quantity = 2
            sides = 10
        --//d#
        elseif ( isNumber(string.match(command,"%d+")) ) then
            quantity = 1;
            sides = tonumber(string.match(command,"%d+")) + 0;
            roll(quantity, sides, result );
        --//d-anything , rolls a d20
        elseif(not string.compare(command,1,5, "diff:")) then
            quantity = 1
            sides = 20
            roll(quantity, sides, result );
        end
    --starts with a number and contains 'd'
    elseif( isNumber(string.sub(command,1,letterDPos-1)) and (letterDPos ~= nil)) then
        local arr = BuildArray(string.gmatch(command,"%d+"))
        quantity = tonumber(arr[1]);
        if #arr > 1 then
            sides = tonumber(arr[2]);
            roll(quantity, sides, result);
        else
            sides = 10;
            roll(quantity, sides, result);
        end
    end

    --check for plus symbol usage
    if( string.find(command,"+") ~= nil) then
        plusUsed = true;
        while( true ) do
        
            plusPos = string.find(command, "+", plusPos+1)
            
            if(plusPos == nil) then
                break;
            elseif( string.len(command) >= plusPos+1 and isNumber(string.at(command,plusPos+1)) ) then
                -- check for another +
                nextPos = string.find(command, "+", plusPos+1);
                parseString = "";
                
                --substring just this number
                if(nextPos ~= nil) then
                    parseString = string.sub(command,plusPos+1, nextPos-1)
                else
                    parseString = string.sub(command,plusPos+1)
                end
                
                --sum onto plusVal
                if(isNumber(parseString)) then
                    plusVal = plusVal + tonumber(parseString);
                end
            end
        end -- end while
    else
        plusUsed = false;
    end

    rolledVals = result
end

-- performs a single dice roll, adds the result to the parameter result array 
function roll(quantity, sides, result)
    for i=1,quantity do
        table.insert(result, DiceRand.rand(sides-1)+1)
        --table.insert(result, math.random(1,sides))
    end
end

------------------------------
-- DICEROLLER API FUNCTIONS --
------------------------------
function performCommand (senderID, displayName, command)
    plusVal = 0
    quantity = 0
    sides = 0
    rolledVals = {}
    returnString = ""

    if commandSeed then
        -- seed random number generator
    end
    
    --parse the message, perform any parsed commands (like a diceroll)
    --returns a blank string if a dice roll was performed.
    --quantity, sides, rolledVals will have a value if a dice roll was performed
    returnString = parseCommand(command, isAdmin(senderID))

        
    --if the rolledVals, quantity, and sides have a value, a roll command was parsed.
    if (#rolledVals > 0 and quantity > 0 and sides > 0) then
       --output who rolled with number of dice and sides on dice rolled
        returnString = returnString .. string.format("%s rolled %2dd%2d; result:", displayName, quantity, sides)
    end
    
    --if the return string is empty by this point, it processed nothing.
    if(string.len(returnString) == 0) then
      return returnString;
    end

    --output the resulting dice roll values
    for i,v in pairs(rolledVals) do
        if (i%10 ~= 0 and i ~= 1) then
            returnString = returnString .. ", ";
        elseif (i % 10 == 0) then
            returnString = returnString .. "\n";
        end

        returnString = returnString .. string.format("%2d", rolledVals[i]);
    end

    --output the sum of the results and the plusVal (if + was used)
    if(plusUsed) then
        returnString = returnString .. "   +";
        returnString = returnString .. plusVal;
        returnString = returnString .. "\nSum: ";
        
        local sum = plusVal;
        for i,v in pairs(rolledVals) do
            sum = sum + rolledVals[i]
        end
        
        returnString = returnString .. sum;
    end

    --TODO: implement D10 section
    print(returnString);
    return returnString;
end

function setAdminName(adminID)
    admin = adminID
    print("set admin name to" .. admin)
end

function isAdmin(personID)
    return (admin == personID)
end


--testing script using built in random number generator
--math.randomseed( os.time() ^2 )
--math.randomseed( os.time() ^2/3+2 + math.random()/7)
--print( performRoll("Cody","//10d20+") )  
