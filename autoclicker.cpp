#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>

using namespace std;

int maxcps, mincps, randcps, spikepercentage;
bool toggled = false;
char key;
string validkeys = "1234567890QWERTYUIOPASDFGHJKLZXCVBNM";
LPCSTR WindowName = NULL;
POINT pt;
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void loadConfig()
{
	ifstream fin("config.txt");
	if (fin.fail())
	{
		ofstream fout("config.txt");
		fout << "Maxcps = 13          # It's recommended to stay below 15 cps\n";
		fout << "Mincps = 11          # It's recommended to stay below 14 cps\n";
		fout << "Togglekey = R        # Can only be a number or a letter\n";
		fout << "Spikepercentage = 3  # Can only be between 1 and 9";
		fout.close();

		maxcps = 13;
		mincps = 11;
		key = 'R';
		spikepercentage = 3;
	}

	else
	{
		string line;
		while (getline(fin, line))
		{
			istringstream sin(line.substr(line.find("=") + 1));
			if (line.find("Maxcps") != -1)
			{
				sin >> maxcps;
			}
			else if (line.find("Mincps") != -1)
			{
				sin >> mincps;
			}
			else if (line.find("Togglekey") != -1)
			{
				sin >> key;
				key = toupper(key);
			}
			else if (line.find("Spikepercentage") != -1)
			{
				sin >> spikepercentage;
			}
		}
		if (!(validkeys.find(key) != string::npos))
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			cout << "[ERROR]" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << "Togglekey can only be a number or a letter exiting in 5 seconds";
			Sleep(5000);
			exit(EXIT_FAILURE);
		}
		if (maxcps < 2)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			cout << "[ERROR]" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << "Maxcps must be bigger than 1 exiting in 5 seconds";
			Sleep(5000);
			exit(EXIT_FAILURE);
		}
		if (mincps < 1)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			cout << "[ERROR]" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << "Mincps must be bigger than 0 exiting in 5 seconds";
			Sleep(5000);
			exit(EXIT_FAILURE);
		}
		if (maxcps <= mincps)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			cout << "[ERROR]" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << "Maxcps must be bigger than Mincps exiting in 5 seconds";
			Sleep(5000);
			exit(EXIT_FAILURE);
		}
		if (!(0 < spikepercentage && spikepercentage < 10))
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			cout << "[ERROR]" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << "Spikepercentage can only be between 1 and 9 exiting in 5 seconds";
			Sleep(5000);
			exit(EXIT_FAILURE);
		}
		if (maxcps > 15)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			cout << "[Warning] ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << "Maxcps is above 15, we recommend staying below 15. Press Enter to Continue";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		if (mincps > 14)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			cout << "[Warning] ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			cout << "Mincps is above 14, we recommend staying below 14. Press Enter to Continue";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		fin.close();
	}
}

void Menu()
{
	cout << "You can change the settings in the config file" << endl;
	cout << "Max Cps: " << maxcps << endl;
	cout << "Min Cps: " << mincps << endl;
	cout << "Toggle Key: " << key << endl;
	cout << "Spikepercentage: " << spikepercentage << endl
		 << endl;

	cout << "Status: ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	cout << "[OFF]";
}

string get_right_of_delim(string const &str, string const &delim)
{
	return str.substr(str.find(delim) + delim.size());
}

int main()
{
	loadConfig();
	Menu();

    HWND hwnd;

	while (1)
	{
		if (GetAsyncKeyState(key))
		{
			if (toggled)
			{
				toggled = false;
				cout << string(5, '\b');
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
				cout << "[OFF]";
				Sleep(250);
			}
			else
			{
				toggled = true;
				cout << string(5, '\b');
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				cout << "[ON] ";
				Sleep(250);
			}
		}

		if (toggled == true)
		{
			hwnd = GetForegroundWindow();

			if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
			{
				int randcps = rand() % (maxcps * 10) + (mincps * 10);
				if (rand() % 100 < spikepercentage)
				{
					randcps = randcps / (rand() % 3 + 2);
				}
				GetCursorPos(&pt);
				SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
				Sleep(6100 / randcps);
				SendMessage(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(pt.x, pt.y));
				Sleep(6300 / randcps);
			}
		}
	}
}