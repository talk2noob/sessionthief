#include "request.h"

// constructor parses out the url for the get
// and limits size to 750 chars (darn line length limit!)
Request::Request(wxString & get, unsigned int ip) {
    wxString line = _T("");
    wxString url = _T("");
    unsigned int linestartpos = 0, lineendpos = 0;

    //Fill in the url by parsing the get
    // find end of line
    while (get.Len() > lineendpos && get[lineendpos] != '\n')
        lineendpos++;
    //if not multiline or does not start with "GET " 
    //check by character is more direct than making a substring and checking StartsWith
    if(lineendpos >= get.Len() || lineendpos - linestartpos <= 13 //enough room for "GET / HTTP1.1"
            || get[linestartpos] != 'G' || get[linestartpos+1] != 'E'
            || get[linestartpos+2] != 'T' || get[linestartpos+3] != ' ')
        throw _T("Incomplete Get");

    //get GET
    for (unsigned int i = linestartpos + 4; get[i] != ' ' && i < lineendpos; i++) // until we hit a space
        line.append(1,get[i]);

    //next line
    lineendpos++;
    linestartpos = lineendpos;

    // continue to find host. look at each line
    while (true) {
        // find end of line
        while (get.Len() > lineendpos && get[lineendpos] != '\n')
            lineendpos++;
        if (lineendpos >= get.Len() || lineendpos - linestartpos < 3)
            break; // end of get

        // get host
        if (lineendpos - linestartpos > 7 //enough room for "Host: \r"
                && get[linestartpos] == 'H' && get[linestartpos+1] == 'o'
                && get[linestartpos+2] == 's' && get[linestartpos+3] == 't'
                && get[linestartpos+4] == ':' && get[linestartpos+5] == ' ') {
            for (unsigned int i = linestartpos + 6; get[i] != '\r' && i < lineendpos; i++)
                url.append(1,get[i]);
        }
        // move to next line
        lineendpos++;
        linestartpos = lineendpos;
        if (get.length() <= lineendpos || url.Len() > 0) // end of given string or host found
            break;
    }// end each line

    url.append(line);    // append /ig/stuff to host
    if (url.Len() > 748) // truncate long lines
        url.Remove(749);
    url.append(_T("\n"));
    
    // set data members
    this->URL = url;
    this->HTTP = get;
    this->IP = ip;
}
