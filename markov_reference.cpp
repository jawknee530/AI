#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>	//For Windows

using namespace std;

bool buildMap(string, map< pair<string, string>, vector<string> > &);
void exportMap(map< pair<string, string>, vector<string> >, string);
void injectStarterWords(string filename, map< pair<string, string>, vector<string> > &in_suffix_map);
void generateWord(map< pair<string, string>, vector<string> > in_suffix_map);


int main(int argc, char *argv[])
{
	map< pair<string, string>, vector<string> > suffix_map;
	bool command_error = 1;
	string msg = "No Arguments Provided!";
	ifstream fin;

	srand(time(NULL));

	if(argv[1] != NULL)
	{
		std::string arg1(argv[1]);

		if(argv[1][0] != '-')
		{
			fin.open(argv[1]);
			if(fin.good())
			{
				fin.close();
				command_error = 0;
				injectStarterWords("db", suffix_map);
				buildMap(argv[1], suffix_map);
			}
			else
			{
				fin.close();
				msg = "Input file could not be opened.";
			}
		}

		else
		{
			if(arg1 == "-altdb")
			{
				fin.open(argv[2]);
				if(fin.good())
				{
					fin.close();
					fin.open(argv[3]);
					if(fin.good())
					{
						fin.close();
						command_error = 0;
						injectStarterWords(argv[3], suffix_map);
						buildMap(argv[2], suffix_map);
						exportMap(suffix_map, argv[3]);
					}
					else
					{
						fin.close();
						command_error = 0;
						buildMap(argv[2], suffix_map);
						exportMap(suffix_map, argv[3]);
					}
				}
				else
				{
					msg = "Input file could not be opened.";
				}			
			}
			else if (arg1 == "-nodb")
			{
				fin.open(argv[2]);
				if(fin.good())
				{
					fin.close();
					command_error = 0;
					buildMap(argv[2], suffix_map);
				}
				else
				{
					msg = "Input file could not be opened.";
				}
			}
			else if (arg1 == "-dbonly")
			{
				if(argv[2] != NULL)
				{
					fin.open(argv[2]);
					if(fin.good())
					{
						fin.close();
						command_error = 0;
						injectStarterWords(argv[2], suffix_map);
					}
					else
					{
						msg = "Database does not exist.";
					}				
				}
				else
				{
					command_error = 0;
					injectStarterWords("db", suffix_map);
				}
			}
		}
	}

	//Print usage if this hasn't been toggled
	if(command_error == 1)
	{
		cout << endl << "Arguments given are invalid: " << msg << endl;

		cout << endl << "Usage:" << endl;
		cout << "     ./markov [input_file]" << endl;
		cout << "          + Uses default database to seed histogram," << endl << "          then injects new data from the given file." << endl;
		cout << endl << "     ./markov -altdb [input_file] [desired_database]" << endl;
		cout << "          + If [desired_database] exists, will use it to seed" << endl << "            historgram. If [desired_database] doesn't exist," << endl << "            it will be created." << endl;
		cout << endl << "     ./markov -nodb [input_file]" << endl;
		cout << "          + No saved histogram is used/updated. Uses" << endl << "            only the given file to create a histogram." << endl;
		cout << endl << "     ./markov -dbonly" << endl;
		cout << "          + Generates output using only the default" << endl << "            database's saved histogram." << endl;
		cout << endl << "     ./markov -dbonly [desired_database]" << endl;
		cout << "          + Generates output using the indicated" << endl << "            database's histogram." << endl;
	}
	else
	{
		generateWord(suffix_map);
	}
}

bool buildMap(string filename, map< pair<string, string>, vector<string> > &in_suffix_map)
{
	ifstream fin;
	string word;
	string key1, key2, value;
	pair<string, string> key;

	word.clear();
	value[0] = '\0';
	key2[0] = '\0';

	/* -- Read all file input -- */
	fin.open(filename.c_str());
	while (fin.good())
	{
		//use basic fin - cuts off on spaces and newlines so it's perfect.
		fin >> word;
		key1 = key2;
		key2 = value;
		key = make_pair(key1, key2);
		value = word;
		in_suffix_map[key].push_back(value);
	}
	fin.close();

	//Ensure this deallocates.
	word.clear();

	return 1;
}


void exportMap(map< pair<string, string>, vector<string> > outMap, string filename)
{
	ofstream fout;
	map< pair<string, string>, vector<string> >::iterator it;
	vector<string>::iterator it2;

	fout.open(filename.c_str());

	it = outMap.begin();

	while(it!= outMap.end())
	{
		it2 = it->second.begin();
		fout <<"-KEYBAWS- " << it->first.first << " -KEYBAWS- " << it->first.second << " ";
		while(it2 != it->second.end())
		{
			fout << *it2 << " ";
			it2++;
		}
		it++;
		
		fout << "-ENDBAWS-" << endl;
	}

	fout.close();
}

void injectStarterWords(string filename, map< pair<string, string>, vector<string> > &in_suffix_map)
{
	ifstream fin;
	string word;
	string key1, key2;
	pair<string, string> key;

	word.clear();
	word[0] = '\0';
	key1[0] = '\0';
	key2[0] = '\0';

	/* -- Read all file input -- */
	fin.open(filename.c_str());

	key = make_pair(key1, key2);

	fin >> word;

	while(word == "-KEYBAWS-")
	{
		fin >> word;
	}

	while(word != "-ENDBAWS-" && fin.good())
	{
		in_suffix_map[key].push_back(word);
		fin >> word;
	}

	fin >> word;
	fin >> word;

	//Get next key
	while(word == "-KEYBAWS-")
	{
		fin >> word;
		key2 = word;
		key = make_pair(key1, key2);
		fin >> word;

		while(word != "-ENDBAWS-" && fin.good())
		{
			in_suffix_map[key].push_back(word);
			fin >> word;
		}

		fin >> word;
		fin >> word;
	}

	while (fin.good())
	{
		//use basic fin - cuts off on spaces and newlines so it's perfect.
		if(word != "-ENDBAWS-")
		{
			key1 = word;
		}
		
		fin >> word;
		fin >> word;

		if(word != "-ENDBAWS-")
		{
			key2 = word;
		}

		key = make_pair(key1, key2);

		fin >> word;
		while(word != "-ENDBAWS-" && fin.good())
		{
			in_suffix_map[key].push_back(word);
			fin >> word;
		}

		fin >> word;
		fin >> word;
	}
	fin.close();

	//Ensure this deallocates.
	word.clear();
}

void generateWord(map< pair<string, string>, vector<string> > in_suffix_map)
{
	string prevkey1 = "\0", prevkey2 = "\0", output;
	pair<string, string> key;
	int size_in_kp, randomized_word_index, random_end, random_break;
	string::size_type str_search;
	bool finished = false;

	prevkey1[0] = '\0';
	prevkey2[0] = '\0';

	while (finished == false)
	{
		key = make_pair(prevkey1, prevkey2);

		size_in_kp = in_suffix_map[key].size();

		randomized_word_index = rand() % size_in_kp;

		output = in_suffix_map[key][randomized_word_index];
		prevkey1 = prevkey2;
		prevkey2 = output;

		key = make_pair(prevkey1, prevkey2);

		if (in_suffix_map[key].size() == 0)	//Close if map pair is invalid.
		{
			finished = true;
		}

		random_end = rand() % 1000 + 1;

		if (random_end < 20)			//Randomly close on a period.
		{
			str_search = output.find(".");
			if (str_search != string::npos)
			{
				finished = true;
			}
		}

		random_break = rand() % 1000 + 1;

		cout << output << " ";

		if (random_break < 125)			//Random line breaks on periods
		{
			str_search = output.find(".");
			if (str_search != string::npos)
			{
				cout << endl << endl;
			}
		}
	}
	cout << endl;
}
