// Assignment 6, by Leonardo Blas.

#ifndef FHHASHQPWFIND_H
#define FHHASHQPWFIND_H

#include <iostream>
#include <string>
#include <time.h>
#include "FHhashQP.h"
#include "EBookEntry.h"

using namespace std;

int Hash(int key) { return key; }
int Hash(const string &key);
int Hash(const EBookEntry &item) { return Hash(item.getETextNum()); }
//int Hash(const EBookEntry &item) { return Hash(item.getCreator()); }
int getKey(const EBookEntry &item) { return item.getETextNum(); }
//string getKey( const EBookEntry &item) { return item.getCreator(); }
const int NUM_RANDOM_INDICES = 25;

template <class Object, typename KeyType>
class FHhashQPwFind : public FHhashQP<Object>
{
public:
	const Object find(const KeyType &key);
	class NotFoundException { };
protected:
	int myHashKey(const KeyType &key) const;
	int findPosKey(const KeyType &key) const;
};

int main()
{
	EBookEntryReader bookInput("catalog-short4.txt");
	if (bookInput.readError())
	{
		cout << "couldn't open " << bookInput.getFileName() << " for input.\n";
		exit(1);
	}
	EBookEntry book;
	FHhashQPwFind<EBookEntry, int> hash_table;  // for ID equality
	//FHhashQPwFind<EBookEntry, string> hash_table; // for any string equality

	// we want two books to be identical in different ways:  ID or author
	EBookEntry::setSortType(EBookEntry::SORT_BY_ID);
	//EBookEntry::setSortType(EBookEntry::SORT_BY_CREATOR);

	cout << bookInput.getFileName() << endl;
	cout << bookInput.getNumBooks() << endl;

	srand(time(NULL));

	for (int i = 0; i < bookInput.getNumBooks(); i++)
		hash_table.insert(bookInput[i]);

	int randomIndices[NUM_RANDOM_INDICES];
	for (int i = 0; i < NUM_RANDOM_INDICES; i++)
		randomIndices[i] = int(rand()) % bookInput.getNumBooks();

	// display NUM_RANDOM_INDICES books from array
	cout << "\nDISPLAYING RANDOM BOOKS:\n\n";
	for (int i = 0; i < NUM_RANDOM_INDICES; i++)
	{
		cout << "   Random book #" << i << ":\n";
		cout << "   Title: " << (bookInput[randomIndices[i]]).getTitle() << "\n";
		cout << "   Creator: " << (bookInput[randomIndices[i]]).getCreator()
			<< "\n";
		cout << "   ID (ETextNum): "
			<< (bookInput[randomIndices[i]]).getETextNum() << "\n\n";
	}

	// attempt to find on the selected key
	cout << "DISPLAYING FIND CALLS:\n";
	cout << "The same random books from the hash table\n" << endl;
	for (int k = 0; k < NUM_RANDOM_INDICES; k++)
	{
		try
		{
			book = hash_table.find((bookInput[randomIndices[k]]).getETextNum());
			//book = hash_table.find((bookInput[randomIndices[k]]).getCreator());
			cout << "   Random book # " << k << " was found:\n";
			cout << "   Title: " << book.getTitle() << "\n";
			cout << "   Creator: " << book.getCreator() << "\n";
			cout << "   ID (ETextNum): " << book.getETextNum() << "\n\n";
		}
		catch (...)
		{
			cout << "Not Found Exception triggered\n";
		}
	}

	// test known failures exceptions:
	try
	{
		book = hash_table.find(-3);
		//book = hash_table.find( "Jack Kerouac" );
		cout << "NO EXCEPTION TRIGGERED " << book.getTitle() << " WAS FOUND";
	}
	catch (...)
	{
		cout << "Not Found Exception triggered\n";
	}
	try
	{
		book = hash_table.find(-2);
		//book = hash_table.find( "BoJack Horseman" );
		cout << "NO EXCEPTION TRIGGERED " << book.getTitle() << " WAS FOUND";
	}
	catch (...)
	{
		cout << "Not Found Exception triggered\n";
	}
	try
	{
		book = hash_table.find(-1);
		//book = hash_table.find( "SpongeBob Squarepants" );
		cout << "NO EXCEPTION TRIGGERED " << book.getTitle() << " WAS FOUND";
	}
	catch (...)
	{
		cout << "Not Found Exception triggered\n";
	}

}

int Hash(const string &key)
{
	unsigned int k, retVal = 0;
	for (k = 0; k < key.length(); k++)
		retVal = 37 * retVal + key[k];
	return retVal;
}

template <class Object, typename KeyType>
const Object FHhashQPwFind<Object, KeyType>::find(const KeyType &key)
{
	if (this->mArray[findPosKey(key)].state == FHhashQP<Object>::ACTIVE)
		return (this->mArray[findPosKey(key)]).data;
	throw NotFoundException();
}

template <class Object, typename KeyType>
int FHhashQPwFind<Object, KeyType>::myHashKey(const KeyType &key) const
{
	int hashVal;
	hashVal = Hash(key) % this->mTableSize;
	if (hashVal < 0)
		hashVal += this->mTableSize;
	return hashVal;
}

template <class Object, typename KeyType>
int FHhashQPwFind<Object, KeyType>::findPosKey(const KeyType &key) const
{
	int kthOddNum = 1;
	int index = myHashKey(key);
	while (this->mArray[index].state != FHhashQP<Object>::EMPTY
		&& getKey(this->mArray[index].data) != key)
	{
		index += kthOddNum;  // k squared = (k-1) squared + kth odd #
		kthOddNum += 2;   // compute next odd #
		if (index >= this->mTableSize)
			index -= this->mTableSize;
	}
	return index;
}
#endif

/*--------//--------//--------//--------//--------//--------//--------//--------
SAMPLE RUNS:
//--------//--------//--------//--------//--------//--------//--------//--------
USING STRINGS:

catalog-short4.txt
4863

DISPLAYING RANDOM BOOKS:

	Random book #0:
	Title: The Peanut PlantIts Cultivation And Uses
	Creator: Jones, B. W.
	ID (ETextNum): 28594

	Random book #1:
	Title: On Calvinism
	Creator: Hull, William
	ID (ETextNum): 28339

	Random book #2:
	Title: Due South or Cuba Past and Present
	Creator: Ballou, Maturin Murray, 1820-1895
	ID (ETextNum): 30130

	Random book #3:
	Title: Captain CookHis Life, Voyages, and Discoveries
	Creator: Kingston, William Henry Giles, 1814-1880
	ID (ETextNum): 24755

	Random book #4:
	Title: Bo-Peep Story Books
	Creator: (no data found)
	ID (ETextNum): 25461

	Random book #5:
	Title: Leaves of LifeFor Daily Inspiration
	Creator: Steinmetz, Margaret Bird
	ID (ETextNum): 14849

	Random book #6:
	Title: Mr. Bingle
	Creator: McCutcheon, George Barr, 1866-1928
	ID (ETextNum): 5963

	Random book #7:
	Title: Two Thousand Miles Below
	Creator: Diffin, Charles Willard, 1884-1966
	ID (ETextNum): 29965

	Random book #8:
	Title: Brief Reflections relative to the Emigrant French Clergy
	Creator: Burney, Fanny, 1752-1840
	ID (ETextNum): 29125

	Random book #9:
	Title: Uncle TerryA Story of the Maine Coast
	Creator: Munn, Charles Clark, 1848-1917
	ID (ETextNum): 28446

	Random book #10:
	Title: A Woman's Will
	Creator: Warner, Anne, 1869-1913
	ID (ETextNum): 27225

	Random book #11:
	Title: Tales from Dickens
	Creator: Rives, Hallie Erminie, 1876-
	ID (ETextNum): 30127

	Random book #12:
	Title: A Letter to the Right Honorable the Lord Chancellor, on the Nature and Interpretation of Unsoundness of Mind, and Imbecility of Intellect
	Creator: Haslam, John, 1764-1844
	ID (ETextNum): 27740

	Random book #13:
	Title: Games for the Playground, Home, School and Gymnasium
	Creator: Bancroft, Jessie Hubbell, 1867-
	ID (ETextNum): 25660

	Random book #14:
	Title: A Terminal Market SystemNew York's Most Urgent Need; Some Observations, Comments,and Comparisons of European Markets
	Creator: Black, Madeleine
	ID (ETextNum): 28575

	Random book #15:
	Title: Right Use of Lime in Soil Improvement
	Creator: Agee, Alva, 1858-1943
	ID (ETextNum): 25389

	Random book #16:
	Title: The Naturewoman
	Creator: Sinclair, Upton, 1878-1968
	ID (ETextNum): 3301

	Random book #17:
	Title: The Sylvan CabinA Centenary Ode on the Birth of Lincoln and Other Verse
	Creator: Jones, Edward Smyth, 1881-
	ID (ETextNum): 26036

	Random book #18:
	Title: The Library of William Congreve
	Creator: Congreve, William, 1670-1729
	ID (ETextNum): 27606

	Random book #19:
	Title: When We Two Parted
	Creator: Byron, George Gordon Byron, Baron, 1788-1824
	ID (ETextNum): 21182

	Random book #20:
	Title: The River Prophet
	Creator: Spears, Raymond S. (Raymond Smiley), 1876-1950
	ID (ETextNum): 28848

	Random book #21:
	Title: Ox-Team Days on the Oregon Trail
	Creator: Driggs, Howard R. (Howard Roscoe), 1873-1963
	ID (ETextNum): 29543

	Random book #22:
	Title: The Traitors
	Creator: Oppenheim, E. Phillips (Edward Phillips), 1866-1946
	ID (ETextNum): 29162

	Random book #23:
	Title: Society for Pure English, Tract 03 (1920)A Few Practical Suggestions
	Creator: Smith, Logan Pearsall, 1865-1949
	ID (ETextNum): 12390

	Random book #24:
	Title: Edmund Dulac's Fairy-BookFairy Tales of the Allied Nations
	Creator: Dulac, Edmund, 1882-1953
	ID (ETextNum): 25513

DISPLAYING FIND CALLS:
The same random books from the hash table

	Random book # 0 was found:
	Title: The Peanut PlantIts Cultivation And Uses
	Creator: Jones, B. W.
	ID (ETextNum): 28594

	Random book # 1 was found:
	Title: On Calvinism
	Creator: Hull, William
	ID (ETextNum): 28339

	Random book # 2 was found:
	Title: Due Westor Round the World in Ten Months
	Creator: Ballou, Maturin Murray, 1820-1895
	ID (ETextNum): 28222

	Random book # 3 was found:
	Title: The Three Midshipmen
	Creator: Kingston, William Henry Giles, 1814-1880
	ID (ETextNum): 24812

	Random book # 4 was found:
	Title: Dick and His Cat and Other Tales
	Creator: (no data found)
	ID (ETextNum): 28351

	Random book # 5 was found:
	Title: Leaves of LifeFor Daily Inspiration
	Creator: Steinmetz, Margaret Bird
	ID (ETextNum): 14849

	Random book # 6 was found:
	Title: What's-His-Name
	Creator: McCutcheon, George Barr, 1866-1928
	ID (ETextNum): 28512

	Random book # 7 was found:
	Title: The Finding of Haldgren
	Creator: Diffin, Charles Willard, 1884-1966
	ID (ETextNum): 29717

	Random book # 8 was found:
	Title: Cecilia; Or, Memoirs of an Heiress ΓÇö Volume 1
	Creator: Burney, Fanny, 1752-1840
	ID (ETextNum): 6346

	Random book # 9 was found:
	Title: Uncle TerryA Story of the Maine Coast
	Creator: Munn, Charles Clark, 1848-1917
	ID (ETextNum): 28446

	Random book # 10 was found:
	Title: A Woman's Will
	Creator: Warner, Anne, 1869-1913
	ID (ETextNum): 27225

	Random book # 11 was found:
	Title: Tales from Dickens
	Creator: Rives, Hallie Erminie, 1876-
	ID (ETextNum): 30127

	Random book # 12 was found:
	Title: A Letter to the Right Honorable the Lord Chancellor, on the Nature and Interpretation of Unsoundness of Mind, and Imbecility of Intellect
	Creator: Haslam, John, 1764-1844
	ID (ETextNum): 27740

	Random book # 13 was found:
	Title: Games for the Playground, Home, School and Gymnasium
	Creator: Bancroft, Jessie Hubbell, 1867-
	ID (ETextNum): 25660

	Random book # 14 was found:
	Title: A Terminal Market SystemNew York's Most Urgent Need; Some Observations, Comments,and Comparisons of European Markets
	Creator: Black, Madeleine
	ID (ETextNum): 28575

	Random book # 15 was found:
	Title: Right Use of Lime in Soil Improvement
	Creator: Agee, Alva, 1858-1943
	ID (ETextNum): 25389

	Random book # 16 was found:
	Title: Samuel the Seeker
	Creator: Sinclair, Upton, 1878-1968
	ID (ETextNum): 5961

	Random book # 17 was found:
	Title: The Sylvan CabinA Centenary Ode on the Birth of Lincoln and Other Verse
	Creator: Jones, Edward Smyth, 1881-
	ID (ETextNum): 26036

	Random book # 18 was found:
	Title: The Library of William Congreve
	Creator: Congreve, William, 1670-1729
	ID (ETextNum): 27606

	Random book # 19 was found:
	Title: When We Two Parted
	Creator: Byron, George Gordon Byron, Baron, 1788-1824
	ID (ETextNum): 21182

	Random book # 20 was found:
	Title: The River Prophet
	Creator: Spears, Raymond S. (Raymond Smiley), 1876-1950
	ID (ETextNum): 28848

	Random book # 21 was found:
	Title: Ox-Team Days on the Oregon Trail
	Creator: Driggs, Howard R. (Howard Roscoe), 1873-1963
	ID (ETextNum): 29543

	Random book # 22 was found:
	Title: The Master Mummer
	Creator: Oppenheim, E. Phillips (Edward Phillips), 1866-1946
	ID (ETextNum): 28161

	Random book # 23 was found:
	Title: Society for Pure English, Tract 03 (1920)A Few Practical Suggestions
	Creator: Smith, Logan Pearsall, 1865-1949
	ID (ETextNum): 12390

	Random book # 24 was found:
	Title: Edmund Dulac's Fairy-BookFairy Tales of the Allied Nations
	Creator: Dulac, Edmund, 1882-1953
	ID (ETextNum): 25513

Not Found Exception triggered
Not Found Exception triggered
Not Found Exception triggered

//--------//--------//--------//--------//--------//--------//--------//--------
USING INTS:

catalog-short4.txt
4863

DISPLAYING RANDOM BOOKS:

	Random book #0:
	Title: Mutineer
	Creator: Shea, Robert, 1933-1994
	ID (ETextNum): 29196

	Random book #1:
	Title: Experiences of a Bandmaster
	Creator: Sousa, John Philip, 1854-1932
	ID (ETextNum): 2589

	Random book #2:
	Title: Far from the Madding Crowd
	Creator: Hardy, Thomas, 1840-1928
	ID (ETextNum): 27

	Random book #3:
	Title: The Secret Garden
	Creator: Burnett, Frances Hodgson, 1849-1924
	ID (ETextNum): 113

	Random book #4:
	Title: The American Empire
	Creator: Nearing, Scott, 1883-1983
	ID (ETextNum): 27787

	Random book #5:
	Title: Jewish Children
	Creator: Sholem Aleichem, 1859-1916
	ID (ETextNum): 27001

	Random book #6:
	Title: Domestic Life in Virginia in the Seventeenth Century
	Creator: Jester, Annie Lash
	ID (ETextNum): 27482

	Random book #7:
	Title: Chronicles (3 of 6): Historie of England (1 of 9)Henrie IV
	Creator: Holinshed, Raphael, -1580?
	ID (ETextNum): 28188

	Random book #8:
	Title: Dorothy's House Party
	Creator: Raymond, Evelyn, 1843-1910
	ID (ETextNum): 28805

	Random book #9:
	Title: The Panchronicon
	Creator: MacKaye, Harold Steele, 1866-1928
	ID (ETextNum): 27682

	Random book #10:
	Title: Notes and Queries, Number 77, April 19, 1851A Medium of Inter-communication for Literary Men, Artists,Antiquaries, Genealogists, etc
	Creator: Various
	ID (ETextNum): 26897

	Random book #11:
	Title: Competition
	Creator: Causey, James
	ID (ETextNum): 29632

	Random book #12:
	Title: Narrative of the Captivity of William Biggs among the Kickapoo Indians in Illinois in 1788
	Creator: Biggs, William, 1755-1827
	ID (ETextNum): 26799

	Random book #13:
	Title: The Boy Ranchersor Solving the Mystery at Diamond X
	Creator: Baker, Willard F.
	ID (ETextNum): 27093

	Random book #14:
	Title: The Russian Garlandbeing Russian Folk Tales
	Creator: (no data found)
	ID (ETextNum): 30109

	Random book #15:
	Title: How to Draw a Straight LineA Lecture on Linkages
	Creator: Kempe, A. B. (Alfred Bray), 1849-1922
	ID (ETextNum): 25155

	Random book #16:
	Title: The Defenders
	Creator: Dick, Philip K., 1928-1982
	ID (ETextNum): 28767

	Random book #17:
	Title: The Lost Valley
	Creator: Walsh, James Morgan, 1897-1952
	ID (ETextNum): 19162

	Random book #18:
	Title: Harper's Young People, June 29, 1880An Illustrated Weekly
	Creator: Various
	ID (ETextNum): 29016

	Random book #19:
	Title: Not Like Other Girls
	Creator: Carey, Rosa Nouchette, 1840-1909
	ID (ETextNum): 28463

	Random book #20:
	Title: Astounding Stories of Super-Science, May, 1930
	Creator: Various
	ID (ETextNum): 29809

	Random book #21:
	Title: Samantha at Coney Islandand a Thousand Other Islands
	Creator: Holley, Marietta, 1836-1926
	ID (ETextNum): 28467

	Random book #22:
	Title: The Vagrant Duke
	Creator: Gibbs, George, 1870-1942
	ID (ETextNum): 29617

	Random book #23:
	Title: Montaigne and Shakspere
	Creator: Robertson, J. M. (John Mackinnon), 1856-1933
	ID (ETextNum): 25535

	Random book #24:
	Title: The Infernal Marriage
	Creator: Disraeli Earl of Beaconsfield, Benjamin, 1804-1881
	ID (ETextNum): 20003

DISPLAYING FIND CALLS:
The same random books from the hash table

	Random book # 0 was found:
	Title: Mutineer
	Creator: Shea, Robert, 1933-1994
	ID (ETextNum): 29196

	Random book # 1 was found:
	Title: Experiences of a Bandmaster
	Creator: Sousa, John Philip, 1854-1932
	ID (ETextNum): 2589

	Random book # 2 was found:
	Title: Far from the Madding Crowd
	Creator: Hardy, Thomas, 1840-1928
	ID (ETextNum): 27

	Random book # 3 was found:
	Title: The Secret Garden
	Creator: Burnett, Frances Hodgson, 1849-1924
	ID (ETextNum): 113

	Random book # 4 was found:
	Title: The American Empire
	Creator: Nearing, Scott, 1883-1983
	ID (ETextNum): 27787

	Random book # 5 was found:
	Title: Jewish Children
	Creator: Sholem Aleichem, 1859-1916
	ID (ETextNum): 27001

	Random book # 6 was found:
	Title: Domestic Life in Virginia in the Seventeenth Century
	Creator: Jester, Annie Lash
	ID (ETextNum): 27482

	Random book # 7 was found:
	Title: Chronicles (3 of 6): Historie of England (1 of 9)Henrie IV
	Creator: Holinshed, Raphael, -1580?
	ID (ETextNum): 28188

	Random book # 8 was found:
	Title: Dorothy's House Party
	Creator: Raymond, Evelyn, 1843-1910
	ID (ETextNum): 28805

	Random book # 9 was found:
	Title: The Panchronicon
	Creator: MacKaye, Harold Steele, 1866-1928
	ID (ETextNum): 27682

	Random book # 10 was found:
	Title: Notes and Queries, Number 77, April 19, 1851A Medium of Inter-communication for Literary Men, Artists,Antiquaries, Genealogists, etc
	Creator: Various
	ID (ETextNum): 26897

	Random book # 11 was found:
	Title: Competition
	Creator: Causey, James
	ID (ETextNum): 29632

	Random book # 12 was found:
	Title: Narrative of the Captivity of William Biggs among the Kickapoo Indians in Illinois in 1788
	Creator: Biggs, William, 1755-1827
	ID (ETextNum): 26799

	Random book # 13 was found:
	Title: The Boy Ranchersor Solving the Mystery at Diamond X
	Creator: Baker, Willard F.
	ID (ETextNum): 27093

	Random book # 14 was found:
	Title: The Russian Garlandbeing Russian Folk Tales
	Creator: (no data found)
	ID (ETextNum): 30109

	Random book # 15 was found:
	Title: How to Draw a Straight LineA Lecture on Linkages
	Creator: Kempe, A. B. (Alfred Bray), 1849-1922
	ID (ETextNum): 25155

	Random book # 16 was found:
	Title: The Defenders
	Creator: Dick, Philip K., 1928-1982
	ID (ETextNum): 28767

	Random book # 17 was found:
	Title: The Lost Valley
	Creator: Walsh, James Morgan, 1897-1952
	ID (ETextNum): 19162

	Random book # 18 was found:
	Title: Harper's Young People, June 29, 1880An Illustrated Weekly
	Creator: Various
	ID (ETextNum): 29016

	Random book # 19 was found:
	Title: Not Like Other Girls
	Creator: Carey, Rosa Nouchette, 1840-1909
	ID (ETextNum): 28463

	Random book # 20 was found:
	Title: Astounding Stories of Super-Science, May, 1930
	Creator: Various
	ID (ETextNum): 29809

	Random book # 21 was found:
	Title: Samantha at Coney Islandand a Thousand Other Islands
	Creator: Holley, Marietta, 1836-1926
	ID (ETextNum): 28467

	Random book # 22 was found:
	Title: The Vagrant Duke
	Creator: Gibbs, George, 1870-1942
	ID (ETextNum): 29617

	Random book # 23 was found:
	Title: Montaigne and Shakspere
	Creator: Robertson, J. M. (John Mackinnon), 1856-1933
	ID (ETextNum): 25535

	Random book # 24 was found:
	Title: The Infernal Marriage
	Creator: Disraeli Earl of Beaconsfield, Benjamin, 1804-1881
	ID (ETextNum): 20003

Not Found Exception triggered
Not Found Exception triggered
Not Found Exception triggered

//--------//--------//--------//--------//--------//--------//--------//------*/
