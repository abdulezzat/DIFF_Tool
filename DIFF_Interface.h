#pragma once
#ifndef _DIFF_INTERFACE_H
#define _DIFF_INTERFACE_H

#include <fstream>
using namespace std;

/*
 * check if the file name/path is valid and open the file with one of ios_base::openmode
 * return :
 * true -> everyting is ok
 * false -> there is a problem
 */
bool OpenFile(fstream &_file_stream, string &_file_name, ios_base::openmode _mode);

/*
 * check if the file is open and then close it
 */
void CloseFile(fstream &_file_stream);

/*
 * Sets the position of the next character to be extracted to a particular position,
 * Return :
 * true -> Everyting is ok
 * false -> The file does not contain this number of lines
 */
inline bool GotoLine(fstream &_file_stream, unsigned int _num);

/*
 * Compares two input text files and show the similarities and the differences between the input files,
 * By adding some symbols before each line in the result:
 * = means that both files has this line
 * - means that only file1 has this line
 * + means that only file2 has this line
 */
void CompFiles(fstream &_file1, fstream &_file2, fstream &_out_file);

#endif
