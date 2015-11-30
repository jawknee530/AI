#include <iostream>	//uuuuhDOOOOOOOOOI
#include <map>		//For maps
#include <fstream>	//File I/O
#include <string>	//For strings
#include <vector>	//For vector type
#include <cctype>	//For toupper()


// Our Laplace Smoothing value
#define LAPLACE 1



using namespace std;



// Function Definitions
bool isStrEqual(string, string);

bool train( string, int &, vector <string> &, map< string, int > &, vector<string> &, map< string, vector<string> > & );

double probabilityWordInGenre ( map< string, vector<string> >, string, string, int, int&);

double probabilityOfGenre(map< string, vector<string> >, vector<string>, map< string, int >); 

void getMostLikelyGenre( vector<string>inStr, vector<string> inGenres, map< string, vector<string> > inMap, map< string, int> numMessagesPerGenre, vector<string> inDistWords);
////////////////////////




/* -- MAIN -- */

int main(int argc, char *argv[])
{
	map< string, vector<string> > super_map;
	vector<string> allgenres;
	int numMessages = 0;
	vector <string> distinctWords;
	map< string, int > numMessagesPerGenre;
	
	int it = 2;				//Iterator for moving through argv
	vector<string> input;			//String to store the command-line input

	input.clear();	//Ensure clear

	/*Get the string (movie title) from commandline*/
	if(argv[2] == NULL)
	{
		cout << endl << " -- ERROR: No input provided. Provide at least one word. -- " << endl << endl;
		cout << 	"    Example: ./hamspam testinput adam sandler in the ring"    << endl << endl;
	}
	else
	{
		while(argv[it] != NULL)
		{
			input.push_back(argv[it]);
			it++;
		}

		train(argv[1], numMessages, distinctWords, numMessagesPerGenre, allgenres, super_map);
		getMostLikelyGenre( input, allgenres, super_map, numMessagesPerGenre, distinctWords ); 
	}

	return 0;	
}

// ---------------------------------------------------------------------

/*	
	train()

	Opens the given file, and builds a map based on the file input.
*/
bool train( string filename, int & numMessages, vector <string> & distinctWords, map< string, int > & numMessagesPerGenre, vector <string> &genres, map< string, vector<string> > & inmap)
{
	// Local Variables
	ifstream fin;
	string word;
	string key;
	bool newGenre = true;
	bool newWord = true;
	char ch;

	/* -- Read all file input -- */
	fin.open(filename.c_str());
	while (fin.good())
	{
		//use basic fin - cuts off on spaces and newlines so it's perfect.
		fin >> key;
		numMessages++;		//Add one to # of read messages
		
		//Check if the genre already exists
		for(unsigned int i = 0; i < genres.size(); i++)
		{
			if(isStrEqual(genres[i], key))
			{
				newGenre = false;
			}
		}

		//If it is a new genre, add it to the genres vector.
		if(newGenre == true)
		{
			genres.push_back(key);
		}
		
		//Keep a count of how many messages each key is assigned.
		numMessagesPerGenre[key] += 1;

		//Remove the space that follows the genre, and then advance
		//to the next character.
		fin.get(ch);
		fin.get(ch);

		//Read until we reach a newline in the input file (or EOF)
		while(ch != '\n' && fin.good())
		{
			//Clear out the previous word read
			word.clear();

			//Read until space (end of word), end of line (end of message)
			//or EOF.
			while(ch != ' ' && ch != '\n' && fin.good())
			{
				word.push_back(ch);
				fin.get(ch);
			}

			if(ch == ' ')
			{
				fin.get(ch);		//clear space
			}

			//Reset newWord for this loop
			newWord = true;

			//Check if this is a new distinct word
			for(unsigned int z = 0; z < distinctWords.size(); z++)
			{
				if( isStrEqual(word, distinctWords[z]) )
				{
					newWord = false;
				}
			}

			// IFF we have a new word, push it to the list of distinct words.
			if(newWord == true)
			{
				distinctWords.push_back(word);
			}

			//Add the input word to the read genre's list of words.
			inmap[key].push_back(word);
		}

		//Reset newGenre for next loop.
		newGenre = true;
	}
	
	//close file
	fin.close();

	//IOSTREAM is having an issue with finding EOF - it will always read
	//The last genre twice. This does not affect any data except
	//numMessagesPerGenre and numMessages (verified)
	numMessagesPerGenre[key] -= 1;
	numMessages--;			
					
	//Ensure this deallocates.
	word.clear();

	return 1;
}
// ---------------------------------------------------------------------

/*
	isStrEqual()

	Compares two strings character-by-character.
	 - IS NOT CASE-SENSITIVE! -
*/
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
// ---------------------------------------------------------------------

/*
	probabilityWordInGenre()

	Finds how many matches a string (given from argv)
	has in a genre.

	Uses Laplace Smoothing (LAPLACE is #defined)
*/
double probabilityWordInGenre ( map< string, vector<string> > inMap, string inGenre, string inWord, int laplace_n, int &matchesNotZero)
{
	int numInstancesOfWord = 0;
	double resultWithLaplace = 0;

	for(unsigned int i = 0; i < inMap[inGenre].size(); i++)
	{
		if( isStrEqual(inMap[inGenre][i], inWord))
		{
			numInstancesOfWord++;
		}
	}

	matchesNotZero += numInstancesOfWord;

	resultWithLaplace = ((double)numInstancesOfWord + (double)LAPLACE) / ((double)inMap[inGenre].size() + ((double)LAPLACE * (double)laplace_n));

	return resultWithLaplace;
}
// ---------------------------------------------------------------------

/*
	probabilityOfGenre()

	Finds the probability of a genre. 
	(number of messages in a genre) / (number of messages)

	Uses Laplace Smoothing (LAPLACE is #defined)
*/
double probabilityOfGenre(map< string, vector<string> > inMap, string inGenre, map< string, int > inMsgPerGenre )
{
	int totalNumMessages = 0;
	double resultWithLaplace = 0;
	map< string, int >::iterator it;

	it = inMsgPerGenre.begin();

	while(it != inMsgPerGenre.end())
	{
		totalNumMessages += it->second;
		it++;
	}

	resultWithLaplace = ((double)inMsgPerGenre[inGenre] + (double)LAPLACE) / ((double)totalNumMessages + ((double)LAPLACE * (double)inMsgPerGenre.size()));

	return resultWithLaplace;
}
// ---------------------------------------------------------------------


/*
	getMostLikelyGenre()
	
	The meat-and-potatoes of the program. Calculates: 
		p(message|genre) 	[word_prb] 
		p(genre) 		[genre_prb]
		p(message) 		[prb_m]
		p(genre|message)	[total_prb]

	Once it calculates these values, it finds which genre has the
	greatest probability, then outputs it.
*/
void getMostLikelyGenre( vector<string>inStr, vector<string> inGenres, map< string, vector<string> > inMap, map< string, int> numMessagesPerGenre, vector<string> inDistWords)
{
	vector <double> word_prb;		//p(message|genre)
	vector <double> genre_prb;		//p(genre)
	vector <double> total_prb;		//p(genre|message)
	double prb_m = 0;			//p(message)
	double max = -1;
	string mostLikelyGenre = "NONE";
	int matchesNotZero = 0;


	//For each Genre...
	for(unsigned int i = 0; i < inGenres.size(); i++)
	{
		//Allocate a new cell for our probabilities (one for each genre)
		word_prb.push_back(0);
		genre_prb.push_back(0);
		total_prb.push_back(0);

		// Assign genre_prb [ p(genre) ]
		genre_prb[i] = probabilityOfGenre( inMap, inGenres[i], numMessagesPerGenre);

		// Assign word_prb [ p(message|genre) ]
		for(unsigned int q = 0; q < inStr.size(); q++)
		{
			if(q == 0)
			{
				word_prb[i] = (probabilityWordInGenre(inMap, inGenres[i], inStr[q], inDistWords.size(), matchesNotZero));
			}
			else
			{
				word_prb[i] *= (probabilityWordInGenre(inMap, inGenres[i], inStr[q], inDistWords.size(), matchesNotZero));
			}
		}
	}

	if(matchesNotZero == 0)
	{
		cout << "------------------" << endl;
		cout << "No Words Matched the Training Input AT ALL!" << endl;
		cout << "------------------" << endl;
		return;
	}

	// Generate prb_m  [ p(message) ]
	for(unsigned int i = 0; i < inGenres.size(); i++)
	{
		prb_m += (word_prb[i] * genre_prb[i]);
	}

	// Generate total_prb  [ p(genre|message) ]
	for(unsigned int i = 0; i < inGenres.size(); i++)
	{
		total_prb[i] = ((word_prb[i] * genre_prb[i]) / prb_m);
	}

	// Find the greatest probability
	for(unsigned int i = 0; i < inGenres.size(); i++)
	{
		if(total_prb[i] > max)
		{
			max = total_prb[i];
			mostLikelyGenre = inGenres[i];
		}
	}

	//Output results
	cout << endl;
	cout << "Most Likely Genre:" << endl <<
		"------------------" << endl;
	cout << "   " << mostLikelyGenre << endl;
	cout << "------------------" << endl;
	cout << "   Probability:   " << endl <<
		"------------------" << endl;
	cout << "   " << max << endl;
	cout << "------------------" << endl;
}
