//Assignment 1, by Leonardo Blas.
// ---------------------------------------------------------------------------
// CS 2C Assignment #1 Part B - iTunes Version

#include <iostream>
#include <vector>
#include "iTunes.h"
using namespace std;

// Global function prototypes, because we were asked to place global functions
// after main.
int operator+(const int &masterSum, const iTunesEntry &iTunesEntry);
ostream& operator<<(ostream& os, const iTunesEntry& entry);

class Sublist
{
public:
	Sublist(vector<iTunesEntry> *orig = NULL)
		: sum(0), originalObjects(orig) { }
	Sublist addItem(int indexOfItemToAdd);
	void showSublist() const;
	int getSum() const { return sum; }

private:
	int sum;
	vector<iTunesEntry> *originalObjects;
	vector<int> indices;
};

int main()
{
	const int TARGET = 3604;
	vector<iTunesEntry> dataSet;
	vector<Sublist> choices;
	int max = 0, masterSum = 0, array_size = 0;
	bool foundPerfect = false;
	Sublist bestChoice;

	// read the data
	iTunesEntryReader tunes_input("itunes_file.txt");
	if (tunes_input.readError())
	{
		cout << "couldn't open " << tunes_input.getFileName()
			<< " for input.\n";
		exit(1);
	}

	// time the algorithm -------------------------
	clock_t startTime, stopTime;
	startTime = clock();

	// create a vector of objects for our own use:
	array_size = tunes_input.getNumTunes();
	for (int k = 0; k < array_size; k++)
		dataSet.push_back(tunes_input[k]);

	choices.clear();
	cout << "Target time: " << TARGET << endl;
	// Initialize the collection Col with one sub-list: the empty sub-list.
	choices.push_back(Sublist(&dataSet));
	// Computing masterSum. Assuming dataSet is a numeric type or has overloaded
	// assignment operators.
	for (auto i : dataSet)
		masterSum = masterSum + i;

	// Loop over all elements x in S.
	// If sum(L) + x == t, break from both loops.
	for (int k = 0; k < dataSet.size() && !foundPerfect; k++)
	{
		int choicesSize = choices.size();
		// Loop over all sub-lists, L, that are already members of Col
		// If sum(L) + x == t, break from both loops.
		for (int j = 0; j < choicesSize && !foundPerfect; j++)
		{
			// Optimization case: TARGET > masterSum.
			if (TARGET > masterSum)
			{
				cout << "   Target is greater than masterSum (" << masterSum << ")\n";
				foundPerfect = true;
				break;
			}
			Sublist oneChoice = choices[j].addItem(k);
			if (TARGET >= oneChoice.getSum())
			{
				// If sum(L) + x <= t, add the sublist L + x to Col.
				choices.push_back(oneChoice);
				//Optimization case: TARGET == sum.
				if (TARGET == oneChoice.getSum())
					foundPerfect = true;
				if (max < oneChoice.getSum())
				{
					bestChoice = oneChoice;
					max = bestChoice.getSum();
				}
			}
		}
	}
	bestChoice.showSublist();

	// how we determine the time elapsed -------------------
	stopTime = clock();

	// report algorithm time
	cout << "\nAlgorithm Elapsed Time: "
		<< (double)(stopTime - startTime) / (double)CLOCKS_PER_SEC
		<< " seconds." << endl << endl;

	return 0;
}

int operator+(const int &masterSum, const iTunesEntry &iTunesEntry)
{
	return masterSum + iTunesEntry.getTime();
}

ostream& operator<<(ostream &os, const iTunesEntry &iTunesEntry)
{
	os << iTunesEntry.getTitle() << " | "
		<< iTunesEntry.getArtist() << " | "
		<< iTunesEntry.getTime();
	return os;
}

Sublist Sublist::addItem(int indexOfItemToAdd)
{
	Sublist augmentedSublist = *this;
	augmentedSublist.indices.push_back(indexOfItemToAdd);
	augmentedSublist.sum = augmentedSublist.getSum() +
		originalObjects->at(indexOfItemToAdd);
	// Sublist as functional return.
	return augmentedSublist;
}

void Sublist::showSublist() const
{
	cout << "Sublist -----------------------------\n";
	cout << "  sum: " << getSum() << endl;
	for (auto i : indices)
		cout << "  array[" << i << "] = " << originalObjects->at(i) << '\n';
}

/*-------------------- SAMPLE RUN --------------------
Target time: 0
Sublist -----------------------------
 sum: 0

Algorithm Elapsed Time: 0.003 seconds.

Target time: 1200
Sublist -----------------------------
 sum: 1200
 array[0] = Cowboy Casanova | Carrie Underwood | 236
 array[1] = Quitter | Carrie Underwood | 220
 array[2] = Russian Roulette | Rihanna | 228
 array[6] = Bad Love | Eric Clapton | 308
 array[11] = Hot Cha | Roy Buchanan | 208

Algorithm Elapsed Time: 0.024 seconds.

Target time: 3600
Sublist -----------------------------
 sum: 3600
 array[0] = Cowboy Casanova | Carrie Underwood | 236
 array[1] = Quitter | Carrie Underwood | 220
 array[2] = Russian Roulette | Rihanna | 228
 array[4] = Monkey Wrench | Foo Fighters | 230
 array[5] = Pretending | Eric Clapton | 283
 array[6] = Bad Love | Eric Clapton | 308
 array[7] = Everybody's In The Mood | Howlin' Wolf | 178
 array[8] = Well That's All Right | Howlin' Wolf | 175
 array[9] = Samson and Delilah | Reverend Gary Davis | 216
 array[11] = Hot Cha | Roy Buchanan | 208
 array[12] = Green Onions | Roy Buchanan | 443
 array[13] = I'm Just a Prisoner | Janiva Magness | 230
 array[14] = You Were Never Mine | Janiva Magness | 276
 array[15] = Hobo Blues | John Lee Hooker | 187
 array[16] = I Can't Quit You Baby | John Lee Hooker | 182

Algorithm Elapsed Time: 1.948 seconds.

Target time: 4799
Sublist -----------------------------
 sum: 4799
 array[0] = Cowboy Casanova | Carrie Underwood | 236
 array[1] = Quitter | Carrie Underwood | 220
 array[2] = Russian Roulette | Rihanna | 228
 array[3] = All My Life | Foo Fighters | 263
 array[4] = Monkey Wrench | Foo Fighters | 230
 array[5] = Pretending | Eric Clapton | 283
 array[6] = Bad Love | Eric Clapton | 308
 array[8] = Well That's All Right | Howlin' Wolf | 175
 array[9] = Samson and Delilah | Reverend Gary Davis | 216
 array[10] = Twelve Sticks | Reverend Gary Davis | 194
 array[11] = Hot Cha | Roy Buchanan | 208
 array[12] = Green Onions | Roy Buchanan | 443
 array[13] = I'm Just a Prisoner | Janiva Magness | 230
 array[14] = You Were Never Mine | Janiva Magness | 276
 array[15] = Hobo Blues | John Lee Hooker | 187
 array[16] = I Can't Quit You Baby | John Lee Hooker | 182
 array[17] = That's The Homie | Snoop Dogg | 343
 array[19] = Ladies and Gentleman | The Rubyz | 201
 array[20] = Watch the Girl | The Rubyz | 192
 array[21] = Donuts for Benny | Veggie Tales | 184

Algorithm Elapsed Time: 64.527 seconds.

Target time: 100000
	Target is greater than masterSum (22110)
Sublist -----------------------------
 sum: 0

Algorithm Elapsed Time: 0.003 seconds.

Target time: -1
Sublist -----------------------------
 sum: 0

Algorithm Elapsed Time: 0.003 seconds.

Target time: 3601
Sublist -----------------------------
 sum: 3601
 array[2] = Russian Roulette | Rihanna | 228
 array[3] = All My Life | Foo Fighters | 263
 array[4] = Monkey Wrench | Foo Fighters | 230
 array[5] = Pretending | Eric Clapton | 283
 array[6] = Bad Love | Eric Clapton | 308
 array[7] = Everybody's In The Mood | Howlin' Wolf | 178
 array[8] = Well That's All Right | Howlin' Wolf | 175
 array[9] = Samson and Delilah | Reverend Gary Davis | 216
 array[10] = Twelve Sticks | Reverend Gary Davis | 194
 array[11] = Hot Cha | Roy Buchanan | 208
 array[12] = Green Onions | Roy Buchanan | 443
 array[13] = I'm Just a Prisoner | Janiva Magness | 230
 array[14] = You Were Never Mine | Janiva Magness | 276
 array[15] = Hobo Blues | John Lee Hooker | 187
 array[16] = I Can't Quit You Baby | John Lee Hooker | 182

Algorithm Elapsed Time: 1.985 seconds.

Target time: 3602
Sublist -----------------------------
 sum: 3602
 array[0] = Cowboy Casanova | Carrie Underwood | 236
 array[1] = Quitter | Carrie Underwood | 220
 array[2] = Russian Roulette | Rihanna | 228
 array[3] = All My Life | Foo Fighters | 263
 array[4] = Monkey Wrench | Foo Fighters | 230
 array[5] = Pretending | Eric Clapton | 283
 array[6] = Bad Love | Eric Clapton | 308
 array[8] = Well That's All Right | Howlin' Wolf | 175
 array[9] = Samson and Delilah | Reverend Gary Davis | 216
 array[10] = Twelve Sticks | Reverend Gary Davis | 194
 array[12] = Green Onions | Roy Buchanan | 443
 array[14] = You Were Never Mine | Janiva Magness | 276
 array[15] = Hobo Blues | John Lee Hooker | 187
 array[17] = That's The Homie | Snoop Dogg | 343

Algorithm Elapsed Time: 2.782 seconds.

Target time: 3603
Sublist -----------------------------
 sum: 3603
 array[0] = Cowboy Casanova | Carrie Underwood | 236
 array[1] = Quitter | Carrie Underwood | 220
 array[2] = Russian Roulette | Rihanna | 228
 array[3] = All My Life | Foo Fighters | 263
 array[4] = Monkey Wrench | Foo Fighters | 230
 array[5] = Pretending | Eric Clapton | 283
 array[6] = Bad Love | Eric Clapton | 308
 array[8] = Well That's All Right | Howlin' Wolf | 175
 array[9] = Samson and Delilah | Reverend Gary Davis | 216
 array[10] = Twelve Sticks | Reverend Gary Davis | 194
 array[11] = Hot Cha | Roy Buchanan | 208
 array[12] = Green Onions | Roy Buchanan | 443
 array[13] = I'm Just a Prisoner | Janiva Magness | 230
 array[15] = Hobo Blues | John Lee Hooker | 187
 array[16] = I Can't Quit You Baby | John Lee Hooker | 182

Algorithm Elapsed Time: 1.796 seconds.

Target time: 3604
Sublist -----------------------------
 sum: 3604
 array[0] = Cowboy Casanova | Carrie Underwood | 236
 array[1] = Quitter | Carrie Underwood | 220
 array[3] = All My Life | Foo Fighters | 263
 array[4] = Monkey Wrench | Foo Fighters | 230
 array[5] = Pretending | Eric Clapton | 283
 array[6] = Bad Love | Eric Clapton | 308
 array[8] = Well That's All Right | Howlin' Wolf | 175
 array[9] = Samson and Delilah | Reverend Gary Davis | 216
 array[10] = Twelve Sticks | Reverend Gary Davis | 194
 array[12] = Green Onions | Roy Buchanan | 443
 array[13] = I'm Just a Prisoner | Janiva Magness | 230
 array[14] = You Were Never Mine | Janiva Magness | 276
 array[15] = Hobo Blues | John Lee Hooker | 187
 array[17] = That's The Homie | Snoop Dogg | 343

Algorithm Elapsed Time: 2.912 seconds.
*/