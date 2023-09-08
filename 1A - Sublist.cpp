//Assignment 1, by Leonardo Blas.
// ---------------------------------------------------------------------------
// CS 2C Assignment #1 Part A - int Version

#include <iostream>
#include <vector>
using namespace std;

class Sublist
{
public:
	Sublist(vector<int> *orig = NULL)
		: sum(0), originalObjects(orig) { }
	Sublist addItem(int indexOfItemToAdd);
	void showSublist() const;
	int getSum() const { return sum; }

private:
	int sum;
	vector<int> *originalObjects;
	vector<int> indices;
};

int main()
{
	const int TARGET = 179;
	vector<int> dataSet;
	vector<Sublist> choices;
	int max = 0, masterSum = 0;
	Sublist bestChoice;
	dataSet.push_back(20); dataSet.push_back(12); dataSet.push_back(22);
	dataSet.push_back(15); dataSet.push_back(25);
	dataSet.push_back(19); dataSet.push_back(29);
	dataSet.push_back(18);
	dataSet.push_back(11); dataSet.push_back(13); dataSet.push_back(17);
	choices.clear();
	cout << "Target time: " << TARGET << endl;
	// Initialize the collection Col with one sub-list: the empty sub-list.
	choices.push_back(Sublist(&dataSet));
	// Computing masterSum. Assuming dataSet is a numeric type or has overloaded
	// assignment operators.
	for (auto i : dataSet)
		masterSum += i;
	// Optimization case: TARGET > masterSum.
	if (TARGET > masterSum)
	{
		cout << "   Target is greater than masterSum (" << masterSum << ")\n";
		return 0;
	}
	// Loop over all elements x in S.
	for (int k = 0; k < dataSet.size(); k++)
	{
		int choicesSize = choices.size();
		// Loop over all sub-lists, L, that are already members of Col
		for (int j = 0; j < choicesSize; j++)
		{
			Sublist oneChoice = choices[j].addItem(k);
			if (TARGET >= oneChoice.getSum())
			{
				// If sum(L) + x <= t, add the sublist L + x to Col.
				choices.push_back(oneChoice);
				//Optimization case: TARGET == sum.
				if (TARGET == oneChoice.getSum())
				{
					// If sum(L) + x == t, break from both loops.
					oneChoice.showSublist();
					return 0;
				}
				if (max < oneChoice.getSum())
				{
					bestChoice = oneChoice;
					max = bestChoice.getSum();
				}
			}
		}
	}
	bestChoice.showSublist();
	return 0;
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
Target time: 1
Sublist -----------------------------
 sum: 0

Target time: 67
Sublist -----------------------------
 sum: 67
 array[0] = 20
 array[2] = 22
 array[4] = 25

Target time: 180
Sublist -----------------------------
 sum: 179
 array[0] = 20
 array[1] = 12
 array[3] = 15
 array[4] = 25
 array[5] = 19
 array[6] = 29
 array[7] = 18
 array[8] = 11
 array[9] = 13
 array[10] = 17

Target time: 200
Sublist -----------------------------
 sum: 190
 array[0] = 20
 array[1] = 12
 array[2] = 22
 array[3] = 15
 array[4] = 25
 array[5] = 19
 array[6] = 29
 array[7] = 18
 array[9] = 13
 array[10] = 17

Target time: 1000
	Target is greater than masterSum (201)

Target time: 0
Sublist -----------------------------
 sum: 0

Target time: -1
Sublist -----------------------------
 sum: 0

Target time: 177
Sublist -----------------------------
 sum: 177
 array[0] = 20
 array[1] = 12
 array[2] = 22
 array[3] = 15
 array[4] = 25
 array[5] = 19
 array[6] = 29
 array[7] = 18
 array[10] = 17

Target time: 178
Sublist -----------------------------
 sum: 178
 array[0] = 20
 array[2] = 22
 array[3] = 15
 array[4] = 25
 array[5] = 19
 array[6] = 29
 array[7] = 18
 array[9] = 13
 array[10] = 17

Target time: 179
Sublist -----------------------------
 sum: 179
 array[0] = 20
 array[1] = 12
 array[3] = 15
 array[4] = 25
 array[5] = 19
 array[6] = 29
 array[7] = 18
 array[8] = 11
 array[9] = 13
 array[10] = 17
*/