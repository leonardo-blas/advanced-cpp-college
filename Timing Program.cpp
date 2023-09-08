////YOU MODIFIED THIS. YOU ADDED A CHUNK OF CODE FROM THE
////MODULES. CHECK LINE 44
//
//#include <iostream>
//#include <string>
//#include <cstdlib>
//#include <time.h>
//
//using namespace std;
//
//#include "Foothill_Sort.h"
//
//// --------------- main ---------------
//#define ARRAY_SIZE 2000
//
//int main()
//{
//	int k;
//	int arrayOfInts[ARRAY_SIZE];
//
//	for (k = 0; k < ARRAY_SIZE; k++)
//		arrayOfInts[k] = rand() % ARRAY_SIZE;
//
//	// how we time our algorithms -------------------------
//	clock_t startTime, stopTime;
//	startTime = clock();
//
//	// sort using a home made bubble sort (in Foothill_Sort.h)
//	arraySort(arrayOfInts, ARRAY_SIZE);
//
//	// how we determine the time elapsed -------------------
//	stopTime = clock();
//
//	// report algorithm time
//	cout << "\nAlgorithm Elapsed Time: "
//		<< (double)(stopTime - startTime) / (double)CLOCKS_PER_SEC
//		<< " seconds." << endl << endl;
//
//	return 0;
//}
//
//
//
////-------------------------------------------------------------------------
//// ITUNES XML FILES READER TEMPLATE, FROM MODULES
//
//
//// Main file for iTunes project.  See Read Me file for details
//// CS 2C, Foothill College, Michael Loceff, creator
//
//#include <iostream>
//using namespace std;
//
//#include "iTunes.h"
//
//// for timing our algorithms
//#include <time.h>
//
//// ----------- prototypes -------------
//void displayOneTune(const iTunesEntry & tune);
//
//// --------------- main ---------------
//int main()
//{
//	// how we read the data from files
//	iTunesEntryReader tunesInput("itunes_file.txt");
//	int arraySize;
//
//	// how we test the success of the read:
//	if (tunesInput.readError())
//	{
//		cout << "couldn't open " << tunesInput.getFileName() << " for input.\n";
//		exit(1);
//	}
//
//	cout << tunesInput.getFileName() << endl;
//	cout << tunesInput.getNumTunes() << endl;
//
//	// create an array of objects for our own use:
//	arraySize = tunesInput.getNumTunes();
//	iTunesEntry *tunesArray = new iTunesEntry[arraySize];
//	for (int k = 0; k < arraySize; k++)
//		tunesArray[k] = tunesInput[k];
//
//	// how we time our algorithms -------------------------
//	clock_t startTime, stopTime;
//	startTime = clock();
//
//	// do something interesting like search or sort or build a hash-table, then...
//
//	// how we determine the time elapsed -------------------
//	stopTime = clock();
//
//	// show the array
//	for (int k = 0; k < arraySize; k++)
//		displayOneTune(tunesArray[k]);
//	cout << endl << endl;
//
//	// report algorithm time
//	cout << "\nAlgorithm Elapsed Time: "
//		<< (double)(stopTime - startTime) / (double)CLOCKS_PER_SEC
//		<< " seconds." << endl << endl;
//
//	delete[] tunesArray;
//	return 0;
//}
//
//void displayOneTune(const iTunesEntry & tune)
//{
//	cout << tune.getArtist() << " | ";
//	cout << tune.getTitle() << " | ";
//	// cout << tune.getTime() << " | "; 
//	cout << " " << tune.convertTimeToString() << endl;
//}