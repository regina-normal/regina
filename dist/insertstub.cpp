
/**************************************************************************
 *                                                                        *
 *  Written by Ben Burton                                                 *
 *  Copyright (c) 1999-2001                                               *
 *                                                                        *
 *  Contact Ben Burton (benb@acm.org) for more details.                   *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

// Usage: insertstub <sentry-string> <stub-file>
//
// The input file is read from standard input.
//
// If the sentry string is found, any text above it will be replaced by
// the contents of <stub-file>, the resulting file will be written to
// standard output and 0 will be returned.
//
// If the sentry string is not found or an error occurs, nothing will be
// written to standard output and 1 will be returned.

#include <cstring>
#include <iostream>
#include <fstream>

#define MAXLINE 1000

char line[MAXLINE+1];

void catfile(istream& file) {
    file.getline(line, MAXLINE);
    while (! file.eof()) {
        cout << line << '\n';
        file.getline(line, MAXLINE);
    }
}

void catfile(const char* filename) {
    ifstream infile(filename);
    if (! infile) {
        cerr << "Could not open file: " << filename << '\n';
        exit(1);
    } else
        catfile(infile);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: insertstub <sentry-string> <stub-file>\n";
        return 1;
    }
    cin.getline(line, MAXLINE);
    while (! cin.eof()) {
        if (strcmp(line, argv[1]) == 0)
            break;
        cin.getline(line, MAXLINE);
    }
    if (cin.eof()) {
        cerr << "Warning: Could not locate sentry: " << argv[1] << '\n';
        return 1;
    } else {
        catfile(argv[2]);
        cout << argv[1] << '\n';
    }
    catfile(cin);
    return 0;
}

