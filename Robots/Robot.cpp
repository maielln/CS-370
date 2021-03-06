/*File Name: Robot.cpp
  Contributer(s): Nicole Maiello
  Date: 2/3/2018
  Language: C++ (gcc compiler)
  Version 1.01
  Description: Currently not finished, will contain multiple variables and info for each
  robot. Just a concept right now
*/
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <array>

using namespace std;

#include "Robot.h"

//all variables can be accessed outside of the class, so all variables
//can be incremented and utilized outside without update and get functions

Robot::Robot() //creates an empty robot
{
    health = 100;
    kills = 0;
    deaths = 0;
    shots = 0;
    wins = 0;
    time_left = 0;
    delay_left = 0;
    robot_time_limit=0;
    scanrange = 1500;
    shotstrength = 1;
    damageadj = 1;
    speedadj = 1;
    mines = 0;
    
    hd = rand() % 256; //from reset_hardware
    
    plen = 0;
    for(int i = 0; i < max_ram; i++)
      RAM[i] = 0;
    RAM[71] = 768;
    thd = hd;
    tspd = 0;
    scanarc = 8;
    shift = 0;
    error_count = 0;
    err = 0;
    overburn = false;
    keepshift = false;
    ip = 0;
    accuracy = 0;
    meters = 0;
    delay_left = 0;
    time_left = 0;
    shields_up = false; //up to here is reset_software
    
    meters = 0;
    shift = 0;
    lhd = hd + 1;
    lshift = shift + 1;
    spd = 0;
    speed = 0;
    cooling = false;
    larmor = 0;
    heat = 0;
    lheat = 1; 
    match_shots = 0;
    won = false;
    last_damage = 0;
    last_hit = 0;
    transponder = 2;
    shutdown = 400;
    channel = transponder;
    startkills = kills;

    robot_config(5,2,2,2,1,0,0); //scanner, weapon, armor, engine, heatsinks, shield, max_mines

    acceleration = 4;
    lx = 0;
    ly = 0;
    xv = 0;
    yv = 0;

    
}

Robot::Robots(string fileName, string compFile) //basically the create_robot (line 1305)
{
  //here will initialize all global variables from the compile file
  //porting create_robot from ATR2.PAS
  //instead of passing individual variables, use a typeDef array and put
  //each variable in a specific slot and initialize it in this function
    Name = fileName;
    compiledFile = compFile;
    cout<<Name<<" has been created."<<endl;
    x = rand() % 1000; //base code for now, getting a random number then having it go from 0-100
    y = rand() % 1000; //same as above but for y
    cout<<"Coordinates: "<<"("<<x<<", "<<y<<")"<<endl;
    attackNum = 0;
    arraySize = 0;

    set_AttackArray();

    turretAngle = rand() % 255;
}

//void Robot::init_Robot() //have to work with this more
//{
 /* for(int i = 0; i < max_ram; i++)
  {
    ram[i] = 0; //frees up all memory
  }
  ram[71] = 768; //not sure why this is needed but ok
  for(i = 0; i < max_code; i++)
  {
    for(int k = 0; k < max_op; k++)
    {
      code[i].op[k] = 0; //taking what operation was determined and resetting it?
    }
  }
  reset_hardware(n); //line 1220 in my files
  reset_software(n); //line 1202 in my files
  //if we look to include these funcitons, it should probably be placed in a different file?
    */
//}

void Robot::robot_config(int scan, int weap, int arm, int eng, int mine, int shielding, int heat) //line 1149 in my file
{
  //the original code uses switch cases to update scanrange and other configs
  //robot[n]^ is the singular robot, since this is a class of a single robot, we will already be in that robot's data
  //because we are already in the data, have the numbers of config passed to this function
  switch(scan){ //configures the scan range
    case 5:
      scanrange = 1500;
      break;
    case 4:
      scanrange = 1000;
      break;
    case 3:
      scanrange = 700;
      break;
    case 2:
      scanrange = 500;
      break;
    case 1:
      scanrange = 350;
      break;
    default:
      scanrange = 250;
      break;
  }
  switch(weap){
    case 5:
      shotstrength = 1.5;
      break;
    case 4:
      shotstrength = 1.35;
      break;
    case 3:
      shotstrength = 1.2;
      break;
    case 2:
      shotstrength = 1;
      break;
    case 1:
      shotstrength = 0.8;
      break;
    default:
      shotstrength = 0.5;
      break;
  }
  switch(arm){
    case 5:
      damageadj = 0.66;
      speedadj = 0.66;
      break;
    case 4:
      damageadj = 0.77;
      speedadj = 0.75;
      break;
    case 3:
      damageadj = 0.83;
      speedadj = 0.85;
      break;
    case 2:
      damageadj = 1;
      speedadj = 1;
      break;
    case 1:
      damageadj = 1.5;
      speedadj = 1.2;
      break;
    default:
      damageadj = 2;
      speedadj = 1.33;
  }
  switch(eng){
    case 5:
      speedadj = speedadj * 1.5;
      break;
    case 4:
      speedadj = speedadj * 1.35;
      break;
    case 3:
      speedadj = speedadj * 1.2;
      break;
    case 2:
      speedadj = speedadj * 1;
      break;
    case 1:
      speedadj = speedadj * 0.8;
      break;
    default:
      speedadj = speedadj * 0.5;
      break;
  }
  switch(mine){
    case 5:
      mines = 24;
      break;
    case 4:
      mines = 16;
      break;
    case 3:
      mines = 10;
      break;
    case 2:
      mines = 6;
      break;
    case 1:
      mines = 4;
      break;
    default:
      mines = 2;
      //config.mines = 0; not entirely sure what this is
      break;
  }
  shields_up = false;

  if((shielding < 3) || (shielding > 5))
    shield = 0;
  if((heat < 0) || (heat > 5)) //code states that heatsingks are handled separately
    heatsinks = 0; //will come back to
}

void Robot::set_AttackArray()
{
    string Line = "";
    int cnt = 0;
    for(int i = 0;i < compiledFile.length(); i++)
    {
        if(compiledFile[i] == '\n')
        {
            Stack[cnt] = Line;
            cnt++;
            Line = "";
        }
        else
            Line += compiledFile[i];
    }
    arraySize = cnt;
}

string Robot::sendAttack()
{
    if(attackNum > arraySize)
    {
        attackNum = 0;
        return Stack[arraySize];
    }
    else
        return Stack[attackNum];
}
