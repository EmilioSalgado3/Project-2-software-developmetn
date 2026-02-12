/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * updated by Deborah Wilson, February 2026
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 * refers to the actual string entered in the form's "verse" field.
 *
 * STUDENT NAME: Jesus Emilio Salgado Suarez
 */

#include <iostream>
#include <stdio.h>
#include <string>
#include "Bible.h"  // to create the bible object
#include "Ref.h"    // to create the reference object
#include "Verse.h"  // to create the verse object

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"

using namespace std;
using namespace cgicc;

int main() {
   /* A CGI program must send a response header with content type
    * back to the web client before any other output.
    * For an AJAX request, our response is not a complete HTML document,
    * so the response type is just plain text to insert into the web page.
    */
   cout << "Content-Type: text/plain\n\n";

   Cgicc cgi;  // create object used to access CGI request data

   // GET THE INPUT DATA
   // browser sends us a string of field name/value pairs from HTML form
   // retrieve the value for each appropriate field name
   form_iterator st = cgi.getElement("search_type");
   form_iterator book = cgi.getElement("book");
   form_iterator chapter = cgi.getElement("chapter");
   form_iterator verse = cgi.getElement("verse");
   form_iterator nv = cgi.getElement("num_verse");

   // Convert and check input data
   bool validInput = false; // Start false; set to true only if all checks pass

   if (chapter == cgi.getElements().end())
   {
      cout << "<p>Missing chapter input.</p>" << endl;
   }
   else
   {
      int chapterNum = chapter->getIntegerValue();
      if (chapterNum > 150)
      {
         cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
      }
      else if (chapterNum <= 0)
      {
         cout << "<p>The chapter must be a positive number.</p>" << endl;
      }
      else
      {
         validInput = true;
      }
   }

   // TODO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you!

   string bookStr = (book != cgi.getElements().end()) ? **book : "";
   int bookNum = 0;
   if (bookStr.empty()) {
      cout << "<p>Book is required.</p>" << endl;
      validInput = false;
   } else {
      try {
         bookNum = stoi(bookStr);
         if (bookNum < 1 || bookNum > 66) {
            cout << "<p>Invalid book number: " << bookStr << " (must be 1-66).</p>" << endl;
            validInput = false;
         }
      } catch (...) {
         cout << "<p>Book must be a number (1-66).</p>" << endl;
         validInput = false;
      }
   }

   int chapterNum = (chapter != cgi.getElements().end()) ? chapter->getIntegerValue() : 0;
   if (chapterNum <= 0 || chapterNum > 150) {
      cout << "<p>Invalid chapter: must be between 1 and 150.</p>" << endl;
      validInput = false;
   }

   int verseNum = (verse != cgi.getElements().end()) ? verse->getIntegerValue() : 0;
   if (verseNum <= 0 || verseNum > 176) {
      cout << "<p>Invalid verse: must be between 1 and 176.</p>" << endl;
      validInput = false;
   }

   int numVerses = (nv != cgi.getElements().end()) ? nv->getIntegerValue() : 1;
   if (numVerses <= 0 || numVerses > 50) {
      cout << "<p>Invalid number of verses: must be between 1 and 50.</p>" << endl;
      validInput = false;
   }

   // TODO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
   //        TO LOOK UP THE REQUESTED VERSES

   Verse resultVerse; 
   LookupResult status = OTHER; // initialize status to other

   string versesOutput = ""; // Added declaration

   if (validInput)
   {
      const string bibleFile = "/home/class/csc3004/Bibles/web-complete";//its like the link to the bible 
      Bible bibleObj(bibleFile); // the bible object points to the bible file

      Ref ref(bookNum, chapterNum, verseNum);

      resultVerse = bibleObj.lookup(ref, status); 
      if (status != SUCCESS)
      {
         cout << "<p>Lookup failed: " << bibleObj.error(status) << "</p>" << endl;
         validInput = false; 
      } else {
         // Start with the first verse
         versesOutput = resultVerse.getVerse();
      }
   }
   
   
    /* SEND BACK THE RESULTS
    * Finally we send the result back to the client on the standard output stream
    * in HTML text format.
    * This string will be inserted as is inside a container on the web page,
    * so we must include HTML formatting commands to make things look presentable!
    */
   if (validInput)
   {
      cout << "Search Type: <b>" << **st << "</b>" << endl;
      cout << "<p>Your result: "
           << resultVerse.getRef().getBookName() << " " << **chapter << ":" << **verse
           << "<em> " << versesOutput
           << "</em></p>" << endl;
   }
   else
   {
      cout << "<p>Invalid Input: <em>report the more specific problem.</em></p>" << endl;
   }

   return 0;
}