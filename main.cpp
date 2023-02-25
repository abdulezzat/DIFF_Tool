#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <windows.h>
#include "colors.h"

using namespace std;

#include "DIFF_Interface.h"
#include "DIFF_Config.h"

int main(void)
{
	cout << endl
		 << "**********************************Welcome**********************************" << endl;

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	while (1)
	{
		int user_ans;
		string in_file1_name, in_file2_name, out_file_name;
		fstream in_file1_stream, in_file2_stream, out_file_stream;

		/*
		 * Open the first file and check for the validation of the file
		 */
		cout << "first file:-\n"
			 << "------------\n";
		while (!OpenFile(in_file1_stream, in_file1_name, ios::in))
			;

		/*
		 * Open the second file and check for the validation of the file
		 */
		cout << "second file:-\n"
			 << "-------------\n";
		while (!OpenFile(in_file2_stream, in_file2_name, ios::in))
			;

		/*
		 * Check if the user entered the same file twice, if so give him some options
		 * Either to continue or to restart or to exit the program
		 */
		if (in_file1_name == in_file2_name)
		{
			SetConsoleTextAttribute(h, YELLOW_COLOR);
			cout << "WARNING: The two files are the same...!" << endl;
			SetConsoleTextAttribute(h, WHITE_COLOR);
			cout << "- To continue, Press \"" << CONTINUE_PROGRAM << "\"\n"
				 << "- To restart the program, Press \"" << RESTART_PROGRAM << "\"\n"
				 << "- To exit the program, Press \"" << EXIT_PROGRAM << "\"\n"
				 << ">> ";

			cin >> user_ans;
			cout << endl;

			/*
			 * Loop until the user enter a valid choice
			 */
			while (user_ans != CONTINUE_PROGRAM && user_ans != RESTART_PROGRAM && user_ans != EXIT_PROGRAM)
			{
				SetConsoleTextAttribute(h, RED_COLOR);
				cout << "ERROR: Invalid choice...\n";
				SetConsoleTextAttribute(h, WHITE_COLOR);
				cout << ">> ";
				cin >> user_ans;
				cout << endl;
			}
			switch (user_ans)
			{
			case CONTINUE_PROGRAM:
				break;
			case RESTART_PROGRAM:
				continue;
				break;
			case EXIT_PROGRAM:
				/*
				 * Close the files before the end of the program
				 */
				CloseFile(in_file1_stream);
				CloseFile(in_file2_stream);
				return 0;
				break;
			}
		}

		/* --------------- Start comparing the two files --------------- */

		/*
		 * Asking for saving the result in a separate file
		 */
		cout << "Save the result in a separate file ?\n"
			 << "- If YES, Press \"" << YES << "\"\n"
			 << "- If NO, Press \"" << NO << "\"\n"
			 << ">> ";
		cin >> user_ans;
		cout << endl;

		/*
		 * Loop until the user enter a valid choice
		 */
		while (user_ans != YES && user_ans != NO)
		{
			SetConsoleTextAttribute(h, RED_COLOR);
			cout << "ERROR: Invalid choice...\n";
			SetConsoleTextAttribute(h, WHITE_COLOR);
			cout << ">> ";
			cin >> user_ans;
			cout << endl;
		}
		switch (user_ans)
		{
		case YES:
			cout << "Note: If you entered one of the two input files, the data will be removed before the comparison...\n";
			while (!OpenFile(out_file_stream, out_file_name, ios::out))
				;
			break;
		case NO:
			break;
		}

		/*
		 * Start comparing the two files
		 */
		CompFiles(in_file1_stream, in_file2_stream, out_file_stream);

		/*
		 * Close the files before the end of the program
		 */
		CloseFile(in_file1_stream);
		CloseFile(in_file2_stream);
		if (out_file_stream.is_open())
			CloseFile(out_file_stream);

		/*
		 * See if the user want to do another operation
		 */
		cout << "\nDo you want to do another operation?\n"
			 << "- If YES, Press \"" << YES << "\"\n"
			 << "- If NO, Press \"" << NO << "\"\n"
			 << ">> ";
		user_ans = -1;
		cin >> user_ans;
		cout << endl;

		while (user_ans != YES && user_ans != NO)
		{
			SetConsoleTextAttribute(h, RED_COLOR);
			cout << "ERROR: Invalid choice...\n";
			SetConsoleTextAttribute(h, WHITE_COLOR);
			cout << ">> ";
			cin >> user_ans;
			cout << endl;
		}
		switch (user_ans)
		{
		case YES:
			break;
		case NO:
			return 0;
			break;
		}
	}
	return 0;
}