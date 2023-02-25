#include <iostream>
#include <windows.h>
#include <limits>
#include <fstream>
#include <string>

using namespace std;

#include "DIFF_Interface.h"
#include "DIFF_Config.h"
#include "colors.h"

static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

bool OpenFile(fstream &_file_stream, string &_file_name, ios_base::openmode _mode)
{
	bool res = false;
	cout << "Enter a valid file name or file path\n"
		 << "Note: must end up with \".txt\" and contains at least 5 charcters\n"
		 << ">> ";

	cin >> _file_name;
	cout << endl;
	/*
	 * The name of the file must contains at least one character and the file extension .txt
	 * so the minimum number of characters is 5
	 */
	if (_file_name.size() < 5)
	{
		SetConsoleTextAttribute(h, RED_COLOR);
		cout << "ERROR: the file name is too short..." << endl;
		SetConsoleTextAttribute(h, WHITE_COLOR);
		res = false;
	}
	else if (_file_name.substr(_file_name.size() - 4) != ".txt") // check for the extension
	{
		SetConsoleTextAttribute(h, RED_COLOR);
		cout << "ERROR: the file extension is invalid..." << endl;
		SetConsoleTextAttribute(h, WHITE_COLOR);
		res = false;
	}
	else
	{
		_file_stream.open(_file_name, _mode);
		if (_file_stream.fail())
		{
			SetConsoleTextAttribute(h, RED_COLOR);
			cout << "ERROR: the file name or the path is invalid..." << endl;
			SetConsoleTextAttribute(h, WHITE_COLOR);
			res = false;
		}
		else
		{
			SetConsoleTextAttribute(h, GREEN_COLOR);
			cout << "INFO: The file is opened successfully..." << endl;
			SetConsoleTextAttribute(h, WHITE_COLOR);
			res = true;
		}
	}
	cout << endl;
	return res;
}

void CloseFile(fstream &_file_stream)
{

	if (_file_stream.is_open())
	{
		_file_stream.close();
		SetConsoleTextAttribute(h, GREEN_COLOR);
		cout << "INFO: The file is closed successfully..." << endl;
		SetConsoleTextAttribute(h, WHITE_COLOR);
	}
	else
	{
		SetConsoleTextAttribute(h, YELLOW_COLOR);
		cout << "WARNING: The file was already colsed...\n";
		SetConsoleTextAttribute(h, WHITE_COLOR);
	}
}

inline bool GotoLine(fstream &_file_stream, unsigned int _num)
{

	// clear the state of _file_stream stream
	_file_stream.clear();
	// Sets the position of the next character to be extracted to beginning of the stream
	_file_stream.seekg(ios::beg);

	// Move to the target position
	// special case that cause the for loop to crush
	for (unsigned int i = 0; i < _num; ++i)
	{
		_file_stream.ignore((numeric_limits<streamsize>::max)(), '\n');
	}
	return !_file_stream.eof();
}

void CompFiles(fstream &_file1_stream, fstream &_file2_stream, fstream &_out_file_stream)
{
	cout << "################### The Beginning Of The Comparison ###################\n";
	/*
	 * Check the validation of the input files
	 */

	if (!_file1_stream.is_open())
	{
		SetConsoleTextAttribute(h, RED_COLOR);
		cout << "ERROR: Invalid input file _file1_stream, make sure the file is opened...\n"
			 << endl;
		SetConsoleTextAttribute(h, WHITE_COLOR);
	}
	else if (!_file2_stream.is_open())
	{
		SetConsoleTextAttribute(h, RED_COLOR);
		cout << "ERROR: Invalid input file _file2_stream, make sure the file is opened...\n"
			 << endl;
		SetConsoleTextAttribute(h, WHITE_COLOR);
	}
	else
	{
		string line_file1;
		string line_file2;

		// The current position in _file2_stream
		int file2_cur_pos = 0;
		while (getline(_file1_stream, line_file1))
		{
			string comp_res = "";
			bool found = false;

			// The match position in _file2_stream at first we assum that the match is at the current position
			int found_pos = file2_cur_pos;

			// read _file2_stream until match is found
			while (getline(_file2_stream, line_file2) && !found)
			{
				found_pos++;
				if (line_file1 == line_file2)
					found = true;
			}

			/*
			 * if no match, this means that the line only exist in _file1_stream,
			 * so puts - symbol before the line and write it in red
			 */
			if (!found)
			{

				comp_res += "-\t" + line_file1 + '\n';

				cout << "-\t" << line_file1 << '\n';
			}
			else
			{
				/*
				 * if match, this means that the line exist in both files,
				 * so puts = symbol before the line and write it in white
				 * but all the lines that is not written in the result from _file2_stream
				 * before that match only exist in _file2_stream
				 * so puts + symbol before the line and write it in green
				 */
				GotoLine(_file2_stream, file2_cur_pos);
				for (int i = file2_cur_pos; i < found_pos - 1; i++)
				{
					getline(_file2_stream, line_file2);
					comp_res += "+\t" + line_file2 + '\n';

					SetConsoleTextAttribute(h, BRIGHT_GREEN_COLOR);
					cout << "+\t" << line_file2 << '\n';
				}

				getline(_file2_stream, line_file2);
				comp_res += "=\t" + line_file2 + '\n';

				SetConsoleTextAttribute(h, WHITE_COLOR);
				cout << "=\t" << line_file2 << '\n';

				file2_cur_pos = found_pos;
			}

			/*cout << comp_res;*/
			if (_out_file_stream.is_open())
			{
				_out_file_stream << comp_res;
			}

			GotoLine(_file2_stream, file2_cur_pos);
		}

		/*
		 * All The remaining of _file2_stream only exist in _file2_stream
		 * so puts + symbol before the line and write it in green
		 */
		while (getline(_file2_stream, line_file2))
		{

			SetConsoleTextAttribute(h, BRIGHT_GREEN_COLOR);
			cout << "+\t" << line_file2 << '\n';
			if (_out_file_stream.is_open())
			{
				_out_file_stream << "+\t" << line_file2 << '\n';
			}
		}
	}
	SetConsoleTextAttribute(h, WHITE_COLOR);
	cout << "###################### The End Of The Comparison ######################\n"
		 << endl;
}
