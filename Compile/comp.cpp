#include <iostream>
#include <string>
#include <algorithm>
#include "Comp.h"
/*
    This is the compiler.
    It checks every character in a string.
    Successfully removes all white space
    This file formats code to be worked with in the switch case
    Receives file from fOpen
*/

using namespace std;


Comp::Comp(string fPath)
{
    fOpen inFile(fPath);

    scanner=0;
    armor=0;
    engines=0;
    mines=0;
    heatsinks=0;
    weapon=0;
    shield=0;

    fText = inFile.getBuffer();

    formatFile();
    setConfig();
}


string Comp::getText()
{
    return fText;
}


void Comp::formatFile()
{
    string Line = "";
    string clearFile = "";
    int i = 0;

    while(fText[i] != '\0') //going through whole file
    {
        if(fText[i] != '\r' && fText[i + 1] != '\n')//if not empty line
        {
            if(fText[i] == ';') //checking for comments
            {
                while(fText[i] != '\r' && fText[i + 1] != '\n') //ignores comment only lines
                    i++;
            }
            else
            {
                while(fText[i] != '\r' && fText[i + 1] != '\n') //gets whole line
                {
                    if(fText[i] != ';') //ignoring comments after lines of code
                    {
                        Line += fText[i];
                        i++;
                    }
                    else
                    {
                        while(fText[i] != '\r' && fText[i + 1] != '\n') //goes to end of line if a comment is found
                            i++;
                    }

                }
                Line = trim(Line);
                if(Line[0] >= 33 && Line[0] <= 126) //if what was trimmed is an accurate ascii value, then continue with filling clearFile
                {
                    Line = formatLine(replaceSpace(Line));//completely removes all useless white space
                    clearFile += Line + '\n';
                }
            }
            i++;
            Line = ""; //resetting line
        }
        else
            fText.erase(i, 1); //deletes lines that are empty
        i++;
    }

    fText = clearFile;

}


string Comp::trim(string l) //trimming front and back
{
    int i = 0; //keep track of position

    while(!l.empty() && std::isspace(l.back())) //getting rid of back first
        l.pop_back();

    while(i < l.size() && std::isspace(l[i])) //taking care of front now
        i++;

    return l.substr(i);
}

string Comp::replaceSpace(string l) //puts commas in place of lone characters, using later for switch case
{
    int i = 1; //pos 0 is confirmed not white space
    while(i < l.size())
    {
        if((l[i] >= 33 && l[i] <= 126) && (l[i] != 44 || l[i] != ',')) //checks ascii table to make sure the right things (and not commas) are getting a comma behind them
        {
            i++;//character found, so if next character is white space, the next character will be changed
            if(l[i] == ' ' || l[i] == 32 || l[i] == 9 || l[i] == '\t')
            {
                l[i] = ','; //next character dealt with
                i++;//inc i to check for more white space
                while(l[i] == ' ') //skipping the rest of the white space
                    i++;
            }
        }
        else
            i++;
    }
    return l;
}


string Comp::formatLine(string l) //erases ALL forms of white space
{
    l.erase(std::remove_if(l.begin(), l.end(), ::isspace), l.end()); //removes remaining white space
    return l;
}


void Comp::setConfig(void)
{
    int i=0;
    string temp;

    while (fText[i] != '\0')
    {
        if (fText[i] == '#')//'#'CONFIG,item,number
        {
            temp = fText.substr(i+1, 6); //#'CONFIG',item,number
            if (temp == "config")
            {
                i+=8; //i should be after the first comma now
                temp = fText.substr(i, 4);
                if (temp == "scan")
                {
                    i+=8;
                    setVars(&scanner, fText[i]);
                }
                else if (temp == "armo")
                {
                    i+=6;
                    setVars(&armor, fText[i]);
                }
                else if (temp == "engi")
                {
                    i+=7;
                    setVars(&engines, fText[i]);
                }
                else if (temp == "mine")
                {
                    i+=6;
                    setVars(&mines, fText[i]);
                }
                else if (temp == "heat")
                {
                    i+=10;
                    setVars(&heatsinks, fText[i]);
                }
                else if (temp == "weap")
                {
                    i+=7;
                    setVars(&weapon, fText[i]);
                }
                else if (temp == "shie")
                {
                    i+=7;
                    setVars(&shield, fText[i]);
                }
            }
        }
        while (fText[i] != '\n' && fText[i] != '\0')
        {
            i++;
        }
        i++;
    }
}


void Comp::setVars(int * part, char val)
{
    switch (val)
    {
    case 49:
        *part = 1;
        break;
    case 50:
        *part = 2;
        break;
    case 51:
        *part = 3;
        break;
    case 52:
        *part = 4;
        break;
    case 53:
        *part = 5;
        break;
    case 48:
    default:
        *part = 0;
        break;
    }
}


void Comp::dispConfig(void)
{
    cout << "scanner: " << scanner << endl;
    cout << "armor: " << armor << endl;
    cout << "engines: " << engines << endl;
    cout << "mines: " << mines << endl;
    cout << "heat sinks: " << heatsinks << endl;
    cout << "weapon: " << weapon << endl;
    cout << "shield: " << shield << endl;
}
