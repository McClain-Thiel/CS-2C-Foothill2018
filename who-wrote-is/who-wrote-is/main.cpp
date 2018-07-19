//
//  main.cpp
//  who-wrote-is
//
//  Created by mcclain thiel on 6/18/18.
//  Copyright © 2018 Anton_Thiel. All rights reserved.
//

//for string processing and input
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//data structures and functinallity
#include <vector>
#include <map>
#include <math.h>

using namespace std;

//functions
bool newBook(string test);
bool buildMap(vector<map<string, double> > & list);
bool modifyTables(vector<map<string, double> > & list);
vector<string> collectInput(string quote);
vector<string> collectInput();
map<double, string> compare(vector<map<string, double> > list,
                            vector<string> test);
void printResults(vector<map<string, double> > list);
void sortMap(map<string, double> list);
vector<string> choseInputMethod();
void answer(map<double, string>);



int main() {
   //new table for each author:
   map<string, double> carroll, dickens, grimm, plato, twain, wilde,
                                 conrad, austin, hesse, homer, doyle;

   carroll = *new map<string, double>; conrad = *new map<string, double>;
   dickens = *new map<string, double>; austin = *new map<string, double>;
   grimm = *new map<string, double>; hesse = *new map<string, double>;
   plato = *new map<string, double>; homer = *new map<string, double>;
   twain = *new map<string, double>; doyle = *new map<string, double>;
   wilde = *new map<string, double>;

   // create vector of author hash tables and push them into vector
   vector<map<string, double> > list;
   list.push_back(carroll); list.push_back(dickens); list.push_back(grimm);
   list.push_back(plato); list.push_back(twain); list.push_back(wilde);
   list.push_back(conrad); list.push_back(austin); list.push_back(hesse);
   list.push_back(homer); list.push_back(doyle);

   //housekeeping
   buildMap(list);
   modifyTables(list);

   //user interaction
   //maybe in loop so it doesnt have to recompile

   bool valid = false;

   while(valid == false)
   {
      vector<string> usrQuote;
      usrQuote = choseInputMethod();
      //usrQuote.push_back("why");usrQuote.push_back("does");
      //usrQuote.push_back("this");usrQuote.push_back("happen");
      map<double, string> probTable;
      probTable = compare(list, usrQuote);//compare causes seg fault
      answer(probTable);

      string ans;
      cout << "To quit enter 'quit'. Otherwise the program will restart . . .";

      cin.get();
      getline(cin, ans);
      if(ans == "quit")
         valid = true;
      else
         cout << "Welcome Back!" << endl;
   }


    return 0;
}


bool newBook(string test)
{
   if( test == "***==========NEW-BOOK=========***")
      return true;
   else
      return false;
}


bool buildMap(vector<map<string, double> >  & list) //this needs to be passsed by reffrence and modify the thing
{
   static const char* fileName = "train-2.txt";

   ifstream fileStream(fileName);

   // Check if we've opened the file (as we should have).
   if (!fileStream.is_open()) {
      cerr << "Couldn't open the file." << endl;
      return false;
   }
   cout << "Reading file " << endl;

   int i = -1;
   while (fileStream.good())
   {
      // Store the next word in the file in a local variable.
      string word;
      fileStream >> word;
      if(newBook(word) == true)
      {
         i++; //start populating map for a new book, idx i
         cout << "new book " << i << endl;
      }
      else {
         if (list[i].find(word) == list[i].end()) list[i][word] = 1; //init to 1 first encounter
         else list[i][word]++; //increment if found again
      }
   }
   for (int i = 0; i < list.size(); ++i) {
      cout << "size of dictionary for " << i << " is " << list[i].size() << endl;
   }
   return true;
}

bool modifyTables(vector<map<string, double> > & list)
{

   //removes and groups all single occurance words
   int count = 0;

   for(int i = 0; i <  list.size(); i++)
   {
      for (map<string,double>::iterator it=list[i].begin(); it!=list[i].end(); )
      {
         if(it->second == 1)
         {
            count++;
            list[i].erase(it++);
         }
         else
         {
            ++it;
         }
      }

       list[i]["***UNKNOWN***"] = count;
      count = 0;
   }


   //finds probability and translates to log space
   long numElements = 0;
   for(int i = 0; i <  list.size(); i++)
   {
      numElements = list[i].size();
      for (map<string,double>::iterator it=list[i].begin(); it!=list[i].end(); ++it)
      {
         it->second = (it->second) / numElements;
         it->second = log(it->second);
      }
   }
   return true;
}

map<double, string> compare(vector<map<string, double> > list, vector<string> test)
{
   vector<double> prob;
   for(int i = 0; i < 11; i++)
      prob.push_back(0.0);

   map<double, string> authorProb;

   for (int i = 0; i < list.size(); i++) //clear
   {
      for (int j = 0; j < test.size(); j++) //clear
      {
         if(prob[i] == 0) //not clear
         {
            if (list[i].find(test[j]) == list[i].end()) //word not in corpus
            {
               prob[i] = log(1/list[i].size()); // assigns very small non-zero
            }                                  //prob to any unkown word
            else
               prob[i] = list[i][test[j]];
         }
         else
         {
            if (list[i].find(test[j]) == list[i].end()) //word not in corpus
            {
               prob[i] *= log(1/list[i].size()); // assigns very small non-zero
            }                                  //prob to any unkown word
            else
            prob[i] *= list[i][test[j]];
         }
      }
   }


   authorProb[prob[0]] = "Lewis Carroll"; authorProb[prob[6]] = " Joseph Conrad";
   authorProb[prob[1]] = "Charles Dickens"; authorProb[prob[7]] = "Jane Austin";
   authorProb[prob[2]] = "The Brothers Grimm"; authorProb[prob[8]] = "Herman Hesse";
   authorProb[prob[3]] = "Plato"; authorProb[prob[9]] = "Homer";
   authorProb[prob[4]] = "Mark Twain"; authorProb[prob[10]] = "Sir. Doyle";
   authorProb[prob[5]] = "Oscar Wilde";


   return authorProb;
}

vector<string> collectInput()
{
   static const char* fileName = "train-2.txt"; //hard code input data
   vector<string> comp;

   // Begin reading from file:
   ifstream fileStream(fileName);

   // Check if we've opened the file (as we should have).
   if (fileStream.is_open())
      while (fileStream.good())
      {
      // Store the next word in the file in a local variable.
      string word;
      fileStream >> word;
      comp.push_back(word);
      }
   else  // We couldn't open the file. Report the error in the error stream.
   {
      cerr << "Couldn't open the file." << endl;
   }
   return comp;
}


vector<string> choseInputMethod()
{
   bool valid = false;
   vector<string> error;
   error.push_back("there was an error");

   while(valid == false)
   {

   cout << "How would you like to input text? By file or by typing" << endl;
   cout << "Enter F for file(must be hardcoded in) or T for typing" << endl;
   char input;
   cin >> input;
   if(input == 'F')
   {
      cout << "You have chosen file. Program will now begin ..." << endl;
      valid = true;
      return collectInput();
   }

   else if(input == 'T')
   {

      string passage;
      cout << "You have chosen to enter your passage manually." << endl;
      cout << "Please enter it below: " << endl;
      cin >> passage;
      //getline(cin, passage);


      cout << "Warning! Text entered by this method must already be cleaned!"
      << endl  << "This means: correct spelling, no punctuation, abbriviations"
      << " or capital letters and correct formatting. Tread carfully." << endl;


      //collect and toenkize data
      stringstream ss(passage);
      string item;
      vector<string> tokens;
      cin.ignore();
      while (getline(ss, item,' ')) {
         tokens.push_back(item);
      }
      valid = true;
      return tokens;

   }

   else
   {
      cout << "Please enter a valid character" << endl;
      valid = false;
   }
   }
   return error;
}


vector<string> collectInput(const string  & quote) //might not really be nesseary
{
   cout << "Warning! Text entered by this method must already be cleaned!"
   << endl  << "This means: correct spelling, no punctuation, abbriviations" <<
   " or capital letters and correct formatting. Tread carfully." << endl;

   //collect and toenkize data
   stringstream ss(quote);
   string item;
   vector<string> tokens;
   while (getline(ss, item, ' ')) {
      tokens.push_back(item);
   }
   return tokens;
}

void answer(map<double, string> prob)
{
   double max = -INFINITY;
   for(map<double,string>::iterator it = prob.begin();it != prob.end(); it++ )
   {
      if(it->first > max)
         max = it->first;
   }
   string author = prob[max];
   cout << "Thinking . . ." << endl;
   cout << "My best guess is that " << author << " wrote this" << endl;
}






//spacer
