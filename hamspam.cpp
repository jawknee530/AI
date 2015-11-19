#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>	//For Windows
#include <cctype>	//For toupper()

using namespace std;

bool isStrEqual(string, string);

bool train( string, vector<string> &, map< string, vector<string> > & );
string getMostLikelyGenre( vector<string>, vector<string>, map< string, vector<string> > );

int main(int argc, char *argv[])
{
	map< string, vector<string> > super_map;
	vector<string> allgenres;
	int it = 2; 	//Iterator for moving through argv
	vector<string> input;

	input.clear();	//Ensure clear

	/*Get the string (movie title) from commandline*/
	if(argv[2] == NULL)
	{
		cout << endl << " -- ERROR: No input provided. Provide at least one word. -- " << endl << endl;
	}
	else
	{
		while(argv[it] != NULL)
		{
			input.push_back(argv[it]);
			it++;
		}

		train(argv[1], allgenres, super_map);
		getMostLikelyGenre( input, allgenres, super_map);
	}

	return 0;	
}


bool train( string filename, vector <string> &genres, map< string, vector<string> > & inmap)
{
	ifstream fin;
	string word;
	string key;
	bool pass = true;
	char ch;

	/* -- Read all file input -- */
	fin.open(filename.c_str());
	while (fin.good())
	{
		//use basic fin - cuts off on spaces and newlines so it's perfect.
		fin >> key;
		
		//Check if the genre already exists
		for(unsigned int i = 0; i < genres.size(); i++)
		{
			if(genres[i] == key)
			{
				pass = false;
			}
		}
		if(pass == true)
		{
			genres.push_back(key);
		}

		fin.get(ch);
		fin.get(ch);
		while(ch != '\n' && fin.good())
		{
			word.clear();
			while(ch != ' ' && ch != '\n' && fin.good())
			{
				word.push_back(ch);
				fin.get(ch);
			}
			if(ch == ' ')
			{
				fin.get(ch);		//clear space
			}
			inmap[key].push_back(word);
		}
		pass = true;
	}
	fin.close();

	//Ensure this deallocates.
	word.clear();

	return 1;
}

string getMostLikelyGenre( vector<string> inputstr, vector<string> genres, map< string, vector<string> > inmap)
{

	/*Incomplete - currently only counts the number of words in a genre*/

	vector<int> instances;		//Counts of instances will be in the same order as their corresponding genre.
	vector<int> numMatches;		//Counts the number of matches between the input string and the trained tables.
	int max = 0, it = 0;		//holder for finding the genre with the most hits; index holder for the most likely genre.

	cout << endl << "Number of Words per Genre" << endl <<
			"-------------------------" << endl;

	for(unsigned int i = 0; i < genres.size(); i++)
	{
		//Allocate memory for the new genre's count
		instances.push_back(0);

		for(unsigned int q = 0; q < inmap[genres[i]].size(); q++)
		{
			instances[i] += 1;
		}
		cout << genres[i] << ":: " << instances[i] << endl;
	}

	cout << "-------------------------" << endl;

	cout << endl << "Number of Matches with Input per Genre" << endl <<
			"--------------------------------------" << endl;
	
	for(unsigned int i = 0; i < genres.size(); i++)
	{
		//Allocate memory for the genre's match count
		numMatches.push_back(0);

		for(unsigned int q = 0; q < inmap[genres[i]].size(); q++)
		{
			for(unsigned int r = 0; r < inputstr.size(); r++)
			{
				if(isStrEqual(inputstr[r], inmap[genres[i]][q]))
				{
					numMatches[i]++;
				}
			}
		}
		cout << genres[i] << ":: " << numMatches[i] << endl;
	}

	cout << "--------------------------------------" << endl;

	//Basic, -placeholder- decision algo
	for(unsigned int i = 0; i < genres.size(); i++)
	{
		if(numMatches[i] > max)
		{
			max = numMatches[i];
			it = i;
		}
	}


	cout << endl << "Most Likely Genre" << endl <<
			"-----------------" << endl;
	cout << genres[it] << endl;
	cout << "-----------------" << endl;

	return genres[it];
}

bool isStrEqual(string A, string B)
{
	if(A.size() != B.size())
	{
		return 0;
	}

	for(unsigned int i = 0; i < A.size(); i++)
	{
		if(toupper(A[i]) != toupper(B[i]))
		{
			return 0;
		}
	}

	return 1;
}
