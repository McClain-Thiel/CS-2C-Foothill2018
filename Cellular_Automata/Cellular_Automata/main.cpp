//
//  main.cpp
//  Cellular_Automata
//
//  Created by mcclain thiel on 1/23/18.
//  Copyright © 2018 Anton_Thiel. All rights reserved.
//

#include <iostream>
#include <string>

using namespace std;

class Automaton
{
private:
   static const int RULES_SIZE = 8;

   bool rules[RULES_SIZE];
   string thisGen;
   string extremeBit; // bit, "*" or " ", implied everywhere "outside"
   int displayWidth;  // an odd number so it can be perfectly centered

public:
   static const int MAX_DISPLAY_WIDTH = 121;

   Automaton(int rule);
   string toStringCurrentGen();
   bool setRule(int newRule);      // change rule set
   void resetFirstGen();
   bool setDisplayWidth(int width);
   void propagateNewGeneration();
};


int main() {
   Automaton test1(30);
   test1.setDisplayWidth(51);
   test1.resetFirstGen();
   for(int i =0; i < 100; i++)
   {
      cout << test1.toStringCurrentGen() << endl;
      test1.propagateNewGeneration();

   }
   //test1.resetFirstGen();
}

//class methods

//reorder later

bool Automaton::setRule(int newRule)
{
   if(newRule >= 0 and newRule <= 255)
   {
      for(int i = 0; i < RULES_SIZE; i++)
      {
         rules[i] = newRule & 1;
         newRule /= 2;
      }
      return true;
   }
   else
      return false;
}

Automaton::Automaton(int rule)
{
   thisGen = "*";
   extremeBit = " ";
   setRule(rule);
}

bool Automaton::setDisplayWidth(int width)
{
   if(width % 2 == 1 and width < MAX_DISPLAY_WIDTH)
   {
      displayWidth = width;
      return true;
   }
   else
      return false;
}

string Automaton::toStringCurrentGen()
{
   char *temp;
   string newString = "";
   temp = new char[thisGen.length()];


   if(thisGen.length() == displayWidth)
      return thisGen;
   else if (thisGen.length() > displayWidth)
   {
      for(int i = 0; i < displayWidth; i++)
      {
         temp[i] = thisGen[(thisGen.length()/2) - (displayWidth/2)];
      }
      return temp;
   }
   else
   {
      for(int i = 0; i <=  (displayWidth - thisGen.length())/2; i++ )
      {
         for(int p =0; p < (displayWidth - thisGen.length())/2; p++ )
         {
            temp[i] = extremeBit[0];
         }
         for (int q = 0; q <= thisGen.length(); q++)
         {
            temp[i] = thisGen[i];
         }
         for(int t = 0; t <=  (displayWidth - thisGen.length())/2; t++ )
         {
            temp[i] = extremeBit[0];
         }
      }
      return temp;
   }
}
void Automaton::propagateNewGeneration()
{
   char *nextGen;
   int bit;
   thisGen = this->extremeBit+ this->extremeBit + thisGen +
             this->extremeBit + this->extremeBit; //broke
   nextGen = new char[thisGen.length()];

   for(int i = 0; i < thisGen.length(); i++)
   {
      bit = 4*thisGen[(i-1+thisGen.length())% thisGen.length()]+ 2*thisGen[i]
                       + thisGen[(i+1)%thisGen.length()];
      if(rules[bit])
         nextGen[i] = '*';
      else
         nextGen[i] = ' ';
   }

   thisGen = nextGen;

   if(this->extremeBit == *)
   {
      
   }
      //extremeBit = rules[4*extremeBit[0] + 2*extremeBit[0] + extremeBit[0]];//broke


}

void Automaton::resetFirstGen()
{
   thisGen = "*";
}

