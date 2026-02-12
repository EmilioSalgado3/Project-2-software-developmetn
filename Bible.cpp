/* Bible class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME: Jesus Emilio Salgado Suarez
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Default constructor
Bible::Bible()
{ 
   infile = "/home/class/csc3004/Bibles/web-complete";
   isOpen = false;
}

// Constructor – pass bible filename
Bible::Bible(const string s) { infile = s; isOpen = false;}

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status)
{ 
   // TODO: scan the file to retrieve the line that holds ref ...
   
 	if (isOpen) {
       instream.close();
       isOpen = false;
   }

   instream.open(infile.c_str(), ios::in);

   if (!instream)
   {
	   status = OTHER;
	   cerr << "Error - can't open input file: " << infile << endl;
	   return Verse();
   }//check if the file opens
	else
   {
	   isOpen = true;
   }

	bool sawBook = false;
	bool sawChapter = false;

	int targetBook = ref.getBook();
	int targetChapter = ref.getChapter();
	int targetVerse = ref.getVerse();
	
	if (targetBook < 1 || targetBook > 66)
	{
    	status = NO_BOOK;
		instream.close();
		isOpen = false;
    	return Verse();
	}

	if (targetChapter < 1)
	{
    		status = NO_CHAPTER;
			instream.close();
			isOpen = false;
    		return Verse();
	}

	if (targetVerse < 1)
	{
    		status = NO_VERSE;
			instream.close();
			isOpen = false;
    		return Verse();
	}

	
	string line;
	while (getline(instream, line))// this loop is what looks for the verse
	{
			Ref current(line);

			if (current.getBook() > targetBook) {
				if (!sawBook)
					status = NO_BOOK;
				else if (!sawChapter)
					status = NO_CHAPTER;
				else
					status = NO_VERSE;

				instream.close();
				isOpen = false;
				return Verse();
			}

			if (current.getBook() < targetBook) {
				// still scanning earlier books
				continue;
			}

			// current book matches target book
			sawBook = true;

			if (current.getChapter() < targetChapter) {
				// still scanning earlier chapters in the book
				continue;
			}

			if (current.getChapter() > targetChapter) {
				status = sawChapter ? NO_VERSE : NO_CHAPTER;
				instream.close();
				isOpen = false;
				return Verse();
			}

			// current chapter equals targetChapter
			if (current.getChapter() == targetChapter) {
				sawChapter = true;
				if (current.getVerse() < targetVerse) {
					continue; // keep scanning within chapter
				}
				if (current.getVerse() > targetVerse) {
					status = NO_VERSE;
					instream.close();
					isOpen = false;
					return Verse();
				}

				// exact match
				status = SUCCESS;
				return Verse(line);
			}
	}

   // update the status variable
   // placeholder until retrieval is attempted
	if (!sawBook)
	{
		status = NO_BOOK;
	}
	else if (!sawChapter)
	{
		status = NO_CHAPTER;
	}
	else
	{
		status = NO_VERSE;
	};
	
	instream.close();
   isOpen = false;
   return Verse();
   // create and return the verse object
   // default verse, to be replaced by a Verse object
	// that is constructed from a line in the file.
   
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status)
{
   string line;
   
   if (!isOpen) 
   { instream.open(infile.c_str(), ios::in);
		if (!instream) 
		{ status = OTHER; 
			cerr << "Error - can't open input file: " << infile << endl;
			return Verse(); }
		isOpen = true; 
	}
	
	if (getline(instream, line)) 
	{ 	
		status = SUCCESS; 
		return Verse(line); 
	} 
	
	status = OTHER;
	
	instream.close();
	
	isOpen = false;
	
	return Verse();
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status)
{
   if (status == SUCCESS)
	   return "Verse was found";
   else if (status == NO_BOOK)
	   return "The book doesnt exist in the Bible";
   else if (status == NO_CHAPTER)
	   return "This chapter doesnt exist in this book";
   else if (status == NO_VERSE)
	   return "This verse doesnt exist in this book";
   else if (status == OTHER)
	   return "I have no idea what you did but thats awesome!";
   
   return "";
   
}

void Bible::display()
{
   cout << "Bible file: " << infile << endl;
}
	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
Ref Bible::next(const Ref ref, LookupResult& status)
{
   return ref;
}

// OPTIONAL: Return the reference before the given ref
Ref Bible::prev(const Ref ref, LookupResult& status)
{
   return ref;
}
