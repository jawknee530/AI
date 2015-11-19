#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>	//For Windows

using namespace std;

bool train( string, vector<string> &, map< string, vector<string> > & );
string getMostLikelyGenre( vector<string>, map< string, vector<string> > & );

int main(int argc, char *argv[])
{
	map< string, vector<string> > super_map;
	vector<string> allgenres;
	
	train(argv[1], allgenres, super_map);
	
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
		for(int i = 0; i < genres.size(); i++)
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
