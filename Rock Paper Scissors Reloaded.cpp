#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <type_traits>
#include <unordered_map>
#include <sstream>
#define NOMINMAX
#include <windows.h>
#define ESC "\033["
#define GREEN "32"
#define RED "31"
#define YELLOW "33"
#define PURPLE "35"
#define BLUE "34"
#define RESET "\033[m"
#define NMAX 105
#define INF 2012345678
using namespace std;

struct Dubla {
	string name;
	int score = 0;

	bool operator < (const Dubla w) const {
		return score > w.score;
	};
};

int score, nrElements, pointsForWin, pointsForLose, pointsForDraw;
string element[NMAX];
vector <string> nameSave;
unordered_map <string, string> umap; // change from save name to save input
bool outcome[NMAX][NMAX]; // outcome[i][j] <- i bate j

void ChangeSave()
{
	int i, j;
	ifstream fin("Save.txt");
	fin >> pointsForWin >> pointsForLose >> pointsForDraw >> nrElements;
	for (i = 1; i <= nrElements; i++)
		fin >> element[i];
	
	memset(outcome, 0, sizeof(outcome));

	while (fin >> i >> j)
		outcome[i][j] = 1;

	score = 0;
	fin.close();
}

void Line()
{
	cout << "----------------------------------------" << "\n";
}

void ClearScreen()
{
	cout << "\033[2J\033[H";
}

void Pause()
{
	cout << ESC << PURPLE << "m" << "Press Enter to continue..." << RESET;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
}

void Leaderboard()
{
	vector <Dubla> top;
	string name, yourName;
	string romanNumbers[11] = { "   I. ", "  II. ", " III. ", "  IV. ", "   V. ", "  VI. ", " VII. ", "VIII. ", "  IX. ", "   X. " };
	int i, fileScore, nrPlacements;
	bool saveNameChoice;

	ifstream finTop("Top10.txt");
	nrPlacements = 0;
	while (finTop >> name >> fileScore)
	{
		top.push_back({ name, fileScore });
		nrPlacements++;
	}
	finTop.close();

	if (nrPlacements == 0)
		cout << ESC << RED << "m" << "Leaderboard empty\n" << RESET;
	else
	{
		// show top 10
		Line();
		cout << ESC << BLUE << "m" << "=====          " << RESET;
		cout << ESC << YELLOW << "m" << "Leaderboard" << RESET;
		cout << ESC << BLUE << "m" << "         =====" << RESET << "\n";

		i = 10;
		for (Dubla w : top)
			if (i > 0)
			{
				cout << ESC << YELLOW << "m" << romanNumbers[10 - i] << w.name << " " << w.score << RESET << "\n";
				i--;
			}
		Line();
		cout << "\n";
	}
}

void GameOver()
{
	cout << ESC << RED << "m" << "Game shut down...\n\n" << RESET;
	exit(0);
}

string IntToString(int value)
{
	string s;
	ostringstream help;
	help << value;
	s += help.str();
	return s;
}

void CreateSave()
{
	int x, y, i, saveNrElements;
	vector <string> saveNameElements;
	string nameElement, nameOfSave, save;
	bool viz[NMAX][NMAX];
	memset(viz, 0, sizeof(viz));

	ofstream fout("Save.txt");
	Line();
	cout << ESC << BLUE << "m" << "Enter name of save [string with no spaces]: " << RESET;
	cin >> nameOfSave;
	Line();

	Pause();
	ClearScreen();

	/// points
	Line();
	cout << ESC << GREEN << "m" << "Enter number of points per win [-1000; 1000]: "; 
	cin >> x;
	save += IntToString(x) + ' ';
	//fout << x << " ";
	cout << RESET << "\n";
	cout << ESC << RED << "m" << "Enter number of points per lose [-1000; 1000]: ";
	cin >> x;
	save += IntToString(x) + ' ';
	//fout << x << " ";
	cout << RESET << "\n";
	cout << ESC << YELLOW << "m" << "Enter number of points per draw [-1000; 1000]: ";
	cin >> x;
	save += IntToString(x) + '\n';
	//fout << x << "\n";
	cout << RESET << "\n";
	Line();

	Pause();
	ClearScreen();

	// elements
	Line();
	cout << ESC << BLUE << "m" << "Enter number of elements [maximum of 100]: ";
	cin >> saveNrElements;
	save += IntToString(saveNrElements) + '\n';
	//fout << x << "\n";
	cout << "\n";
	for (i = 1; i <= saveNrElements; i++)
	{
		cout << "Enter name of element " << i << " [string with no spaces]: ";
		cin >> nameElement;
		save += nameElement + '\n';
		//fout << nameElement << "\n";
		saveNameElements.push_back(nameElement);
		cout << "\n";
	}
	cout << RESET;
	Line();
	Pause();
	ClearScreen();

	//relations
	Line();
	cout << ESC << PURPLE << "m";
	for (i = 0; i < saveNrElements; i++)
		cout << i + 1 << ") " << saveNameElements[i] << "\n";
	cout << RESET;

	Line();
	cout << ESC << PURPLE << "m" << "Enter 2 ids from above to choose who beats who [first one beats the second one] [enter 0 0 to stop]: " << RESET;
	while (cin >> x >> y)
		if (x != 0 && y != 0)
		{
			if (x != y && viz[x][y] == 0 && x >= 1 && y >= 1 && x <= saveNrElements && y <= saveNrElements)
			{
				save += IntToString(x) + ' ' + IntToString(y) + '\n';
				viz[x][y] = viz[y][x] = 1;
				//fout << x << " " << y << "\n";
				cout << ESC << GREEN << "m" << "Relation \"" << saveNameElements[x - 1] << " beats " << saveNameElements[y - 1] << "\" added succesfully\n" << RESET;
			}
			else
				cout << ESC << RED << "m" << "Invalid relation\n" << RESET;
		}
		else
			break;

	fout << save;

	fout.close();
	umap[nameOfSave] = save;
	nameSave.push_back(nameOfSave);
}

void SaveMenu()
{
	int nrSaves, choose;
	string inputSave;

	Line();
	cout << ESC << BLUE << "m" << "==========       SAVES         =========\n";
	cout << "0) Go back\n";
	nrSaves = 0;
	for (auto w : nameSave)
	{
		nrSaves++;
		cout << nrSaves << ") " << w << "\n";
	}
	cout << RESET;
	Line();

	cout << ESC << PURPLE << "m" << "Enter choice: ";
	while (cin >> choose)
		if (choose > nrSaves || choose < 0)
			cout << ESC << PURPLE << "m" << "Incorrect input, re-enter choice: " << RESET;
		else
			break;
	ClearScreen();

	if (choose == 0) // goes back
		return;

	cout << ESC << PURPLE << "m" << "Save " << nameSave[choose - 1] << " is loading...\n";

	ofstream fout("Save.txt");
	fout << umap[nameSave[choose - 1]];
	fout.close();

	ChangeSave();
	Pause();
}

void CheckScore()
{
	cout << ESC;
	if (score < pointsForLose)
		cout << RED;
	else if (score > pointsForWin)
		cout << GREEN;
	else
		cout << YELLOW;

	cout << "m" << "Your current score is: " << score << "\n" << RESET;
}

void SaveScore()
{
	vector <Dubla> top;
	string name, yourName;
	int i, fileScore, minScore, nrPlacements;

	ifstream finTop("Top10.txt");

	minScore = INF;
	nrPlacements = 0;
	while (finTop >> name >> fileScore)
	{
		minScore = min(minScore, fileScore);
		top.push_back({ name, fileScore });
		nrPlacements++;
	}
	finTop.close();

	if (nrPlacements >= 10 && score <= minScore)
	{
		cout << ESC << RED << "m" << "Score too low for the leaderboard\n" << RESET;
		return;
	}

	// enter name
	Line();
	cout << ESC << BLUE << "m" << "Enter your name (no spaces): " << RESET << "\n";
	cin >> yourName;
	Line();

	// recalculates leaderboard
	ofstream foutTop("Top10.txt");
	top.push_back({ yourName, score });
	sort(top.begin(), top.end());
	for (Dubla w : top)
		foutTop << w.name << " " << w.score << "\n";


	Pause();
	ClearScreen();
	cout << ESC << GREEN << "m" << "Score saved" << RESET << "\n";

	foutTop.close();
}

void Options(bool &nextRound, bool &optionsOn)
{
	int yourPick;
	Line();
	cout << ESC << RED << "m" << "==========      SETTINGS       =========\n";
	cout << "0) Shut Down\n";
	cout << "1) Check score\n";
	cout << "2) Save score\n";
	cout << "3) Leaderboard\n";
	cout << "4) Create new save\n";
	cout << "5) Load a save\n";
	cout << "6) Leave options\n" << RESET;
	Line();
	cout << ESC << BLUE << "m" << "Enter option: " << RESET;

	// your pick
	while (1)
	{
		cin >> yourPick;
		Line();
		if (yourPick < 0 || yourPick > 6)
			cout << ESC << PURPLE << "m" << "Incorrect input, re-enter choice: " << RESET;
		else
			break;
	}

	switch (yourPick)
	{
	case 0: // shutdown
		nextRound = 0;
		Pause();
		ClearScreen();
		GameOver();
		break;
	case 1: // check score
		ClearScreen();
		CheckScore();
		Pause();
		ClearScreen();
		break;
	case 2: // save score
		ClearScreen();
		SaveScore();
		Pause();
		ClearScreen();
		break;
	case 3: // leaderboard
		ClearScreen();
		Leaderboard();
		Pause();
		ClearScreen();
		break;
	case 4: // create new save
		ClearScreen();
		CreateSave();
		Pause();
		ClearScreen();
		break;
	case 5: // save menu
		ClearScreen();
		SaveMenu();
		ClearScreen();
		break;
	case 6: // leave options
		optionsOn = 0;
		ClearScreen();
		break;
	}
}

void BeginningPick(int& yourPick, bool& nextRound, bool &fromSettings)
{
	bool optionsOn;
	Line();
	cout << ESC << PURPLE << "m" << "Welcome to Rock-Paper-Scissors RELOADED!" << RESET << "\n";
	Line();
	cout << ESC << PURPLE << "m" << "Gather as many points as you can!" << RESET << "\n\n";
	cout << ESC << GREEN << "m" << "Each win is " << pointsForWin << " points" << RESET << "\n";
	cout << ESC << RED << "m" << "Each lose is " << pointsForLose << " points" << RESET << "\n";
	cout << ESC << YELLOW << "m" << "Each draw is " << pointsForDraw << " points" << RESET << "\n";
	Line();
	cout << ESC << PURPLE << "m" << "What are you choosing?\n";
	cout << "Pick a number from 0 to " << nrElements << ":\n" << RESET;
	cout << ESC << BLUE << "m" << "0) Settings\n";

	for (int i = 1; i <= nrElements; i++)
		cout << i << ") " << element[i] << "\n";

	cout << RESET;
	Line();
	cout << ESC << PURPLE << "m" << "Enter choice: " << RESET;

	// your choice
	do
	{
		cin >> yourPick;
		Line();
		if (yourPick == 0)
		{
			ClearScreen();
			optionsOn = 1;
			fromSettings = 1;
			while(optionsOn)
				Options(nextRound, optionsOn);
			return;
		}
		else if (yourPick < 0 || yourPick > nrElements)
		{
			cout << ESC << PURPLE << "m" << "Incorrect input, re-enter choice: " << RESET;
			continue;
		}
		else
		{
			ClearScreen();
			Line();
			cout << ESC << PURPLE << "m" << "You chose " << element[yourPick] << ".\n" << RESET;
			return;
		}
	} while (1);
}

void PCPick(int& computersPick)
{
	/// computer's pick
	computersPick = rand() % nrElements + 1;
	cout << ESC << BLUE << "m" << "The computer chose " << element[computersPick] << "\n";
	Line();
}

void GamePossibilities(int yourPick, int computersPick)
{
	/// posibilities for computer's pick
	if (outcome[computersPick][yourPick] == 1)
	{
		cout << ESC << RED << "m" << "You lost... Try again...\n" << RESET;
		Line();
		score += pointsForLose;
	}
	else if (outcome[yourPick][computersPick] == 1)
	{
		cout << ESC << GREEN << "m" << "You won! Good job!\n" << RESET;
		Line();
		score += pointsForWin;
	}
	else
	{
		cout << ESC << YELLOW << "m" << "It's a draw. Try again...\n" << RESET;
		Line();
		score += pointsForDraw;
	}
}

bool Round()
{
	int computersPick, yourPick;
	bool nextRound = 1, fromSettings = 0;
	srand(time(NULL));

	BeginningPick(yourPick, nextRound, fromSettings);

	if (nextRound && !fromSettings)
	{
		PCPick(computersPick);
		GamePossibilities(yourPick, computersPick);

		Pause();
		ClearScreen();
	}
	return nextRound;
}

void AddBasicSave()
{
	string joc;
	nameSave.push_back("Piatra-Hartie-Foarfeca");
	joc = "3 -3 1\n3\nPiatra\nHartie\nFoarfeca\n1 3\n2 1\n3 2\n";
	umap["Piatra-Hartie-Foarfeca"] = joc;
}

int main()
{
	// enables color when ran thru .exe file
	//HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//DWORD dwMode = 0;
	//GetConsoleMode(hOut, &dwMode);
	//dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	//SetConsoleMode(hOut, dwMode);

	AddBasicSave();

	while (Round())
		continue;

	return 0;
}