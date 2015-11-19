#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>	//For Windows

using namespace std;

bool train( string, map< string, vector<string> > & );

int main(int argc, char *argv[])
{
	map< string, vector<string> > super_map;

	train(argv[1], super_map);
}


bool train( string filename, map< string, vector<string> > & inmap)
{
	ifstream fin;
	string word;
	string key;
	char ch;

	/* -- Read all file input -- */
	fin.open(filename.c_str());
	while (fin.good())
	{
		//use basic fin - cuts off on spaces and newlines so it's perfect.
		fin >> key;
		fin.get(ch);
		fin.get(ch);
		cout << key << endl;
		while(ch != '\n' && fin.good())
		{
			word.clear();
			while(ch != ' ' && ch != '\n' && fin.good())
			{
				word.push_back(ch);
				fin.get(ch);
			}
			fin.get(ch);		//clear space
			cout << word << endl;
			inmap[key].push_back(word);
		}
	}
	fin.close();

	//Ensure this deallocates.
	word.clear();

	return 1;
}
