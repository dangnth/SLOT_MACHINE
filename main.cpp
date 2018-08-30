#include <iostream>
#include <Windows.h>
#include <stdint.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

//Game Tools
static HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
static HANDLE inpHandle = GetStdHandle(STD_INPUT_HANDLE);

const string MenuItemName[4]={"Start","Credit","Help","Exit"};

int selectionMenuItem;
bool wantExit=false;
int money; int bet; int mark; int s[4];
int highscore = 1000; int maxMoney=1000;

void resizeConsole(int width, int height)
{
	_COORD coord;
	coord.X = width;
	coord.Y = height;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = height - 1;
	Rect.Right = width - 1;

	SetConsoleScreenBufferSize(outHandle, coord);
	SetConsoleWindowInfo(outHandle, TRUE, &Rect);
}

void gotoxy(SHORT x, SHORT y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(outHandle, c);
}

void setColor(int colorText, int colorBackground)
{
	SetConsoleTextAttribute(outHandle, colorText + 16 * colorBackground);
}

void printxy(long n, int x, int y, int colorText, int colorBackground)
{
	setColor(colorText, colorBackground);
	gotoxy(x, y);
	cout << n;
}

void printxy(string s, int x, int y, int colorText, int colorBackground)
{
	setColor(colorText, colorBackground);
	gotoxy(x, y);
	cout << s;
}
/////////////////////////////////////////////////////////////////////////////////////////

void Intro() {
    for (int i=0; i<4; i++) {
        gotoxy(45,i); cout << "K";
        gotoxy(50-i,i); cout << "N";
        gotoxy(51-i,i+1); cout << "E";
        gotoxy(49,8-i); cout << "W";
        //gotoxy()
        Sleep(200);
        gotoxy(45,i); cout << " ";
        gotoxy(50-i,i); cout << " ";
        gotoxy(51-i,i+1); cout << " ";
        gotoxy(49,8-i); cout << " ";
    }
}

void GameName(bool x) {
    int kind;
    if (x) {
        kind=1;
    } else kind=0;
    printxy("KNEW            _STUDIO_",45,4, 3*kind,7*kind);
    printxy("SLOT_MACHINE$", 49,6, 2*kind, 14*kind);
    for (int i=0; i<10; i++) {
        printxy("><", i*2, 20+i, 15*kind, 16*kind);
        printxy("><", 100+i*2, i, 15*kind, 16*kind);
    }
    printxy("Master_Version 1.0", 90, 27, 4*kind,0);
}

void showMenu(bool x) {
    int tmp;
    if (x) {
        tmp=1;
    } else {
        tmp=0;
    }
    for (int i=0; i<4; i++) {
        printxy(MenuItemName[i],53, 11+i*2 , 11*tmp, 16*tmp);
    }
}

int readKey()
{
	fflush(stdin);
	char c = _getch();
	if ((int)c == -32)
	{
		c = _getch();
		return -((int)c);
	}
	else
		return (int)c;
}

void showLine(bool x)
{
	int kind;
	if (x)	{
        kind = 1;
	} else kind = 0;
	for (int i = 0; i <= 119; i++)
		printxy("=", i, 16, 15 * kind, 0);
	for (int i = 0; i<=15; i++)
        printxy("|", 55, i, 15 * kind, 0 );
}

void showSelectionMenuItem(bool show, int selectionMenuItem)
{
	int kind;
	if (show)
		kind = 1;
	else
		kind = 0;
	printxy(">>", 50, 11 + selectionMenuItem * 2, kind * 12, 0);
}

void showMoney(bool x);

void XoaManHinh()
{
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

void showHelp(bool x) {
    int kind;
    if (x) {
        kind=1;
    } else kind = 0;
    //
    printxy("<-", 65, 5, 0, kind*11);
    printxy("or", 68, 5, 15*kind,16*kind);
    printxy("->", 71, 5, 0, kind*11);
    printxy("to change your BET", 74, 5, 15*kind, 16*kind);
    //
    printxy("Enter",65 ,7,0,kind*11);
    printxy("to place BET", 74, 7, 15*kind, 16*kind);
}

int countNum(int x) {
    int cNum=0;
    do {
        cNum++;
        x/=10;
    } while (x != 0);
    return cNum;
}

void showMoney(bool x) {
	int kind;
	if (x) {
        kind=1;
	} else {
        kind=0;
	}

    printxy("Money  $", 15, 7, 11*kind, 0);
    printxy(money, 23, 7, 11*kind, 0);
    printxy("               ", 23+countNum(money),7,0,0);
}

void showEarnMoney(bool x) {
    int kind;
    if (x) {
        kind=1;
    } else kind=0;
    if (mark > 0) {
        printxy("+  $", 19, 8, 14*kind, 0);
        printxy(bet, 23, 8, 14*kind, 0);
        printxy("           ",23+countNum(bet),8,0,0);
    } else if (mark != -1) {
        printxy("-  $", 19, 8, 12*kind, 0);
        printxy(bet, 23, 8, 12*kind, 0);
        printxy("           ",23+countNum(bet),8,0,0);
    }
}

void showBetChanging(bool x) {
    int kind;
    if (x) {
        kind=1;
    } else {
        kind=0;
    }
    printxy("< Your bet > :$ " , 15,12, 15*kind, 0 );
}

void showBet(bool x) {
    int kind;
    if (x) {
        kind=1;
    } else {
        kind=0;
    }
    printxy(bet, 32, 12, 14*kind, 12*kind);
    printxy("             ", 32+ countNum(bet),12,0,0);
}

void placeBet() {
    int key;
    showMoney(true);
    showEarnMoney(true);
    showBetChanging(true);
    bet=money;
    do {
        showBet(true);
        key=readKey();
        if (key==-75) {
            if (bet-(money/50) >= 0) {
                bet-=money/50;
            }
        } else if (key==-77) {
            if (bet +(money/50) <= money) {
                bet+=money/50;
            }
        }
    } while (key != 13);
    money-=bet;
    showMoney(false);
    showMoney(true);
    showEarnMoney(false);
    showEarnMoney(true);
    showBetChanging(false);
    showBet(false);
}

int random(int low, int high)
{
	return (low + rand() % (high - low + 1));
}

void calculation() {
    int kind=1;
    for (int i=1; i< 10; i++) {
        s[1]=random(2,7);
        printxy(s[1],45,24,15*kind,16*kind);
        Sleep(100);
    }
    for (int i=1; i< 10; i++) {
        s[2]=random(2,7);
        printxy(s[2],55,24,15*kind,16*kind);
        Sleep(100);
    }
    for (int i=1; i<10; i++) {
        s[3]=random(2,7);
        printxy(s[3],65,24,15*kind,16*kind);
        Sleep(100);
    }
    //
    if (s[1]==s[2] && s[1]==s[3]) {
        if (s[1]==7) {
            bet*=10;
            mark=4;
        } else {
            bet*=5;
            mark=3;
        }
        money+=bet;
        maxMoney=max(maxMoney,money);
        return;
    } else if (s[1]==s[2] || s[2]==s[3] || s[3]==s[1]) {
        bet*=3;
        mark=2;
        money+=bet;
        maxMoney=max(maxMoney,money);
        return;
    }
    mark=0;
}

void showResult(bool x) {
    int kind=0;
    if (x) {
        kind=1;
    } else {
        printxy("                           ", 43,20,0,0);
    }
    if (x) {
    if (mark==4) {
        printxy("Perfect !!!!! X 10", 45,20, 10*kind, 16*kind );
    } else if (mark==3) {
        printxy("Congratulations ! X 5", 46,20, 11*kind, 16*kind);
    } else if (mark==2) {
        printxy("You Win X 3", 50,20, 14*kind, 16*kind);
    } else if (mark==0) {
        printxy("Aw, Better Luck Next Time !", 43,20, 12*kind,16*kind);
    }
    }
}

void showSlot(bool x)
{
	if (!x)
	{
		for (int i = 0; i < 3; i++)
		{
			printxy(' ', 48 + i * 5, 25, 0, 0);
			printxy("____________", 44 + i * 5, 25, 0, 0);
		}
		return;
	}
	int color;
	if (mark == 4)
		color = 14;
	else
		if (mark == 3)
			color = 11;
		else
			if (mark == 2)
				color = 10;
			else
				color = 12;
	for (int i = 0; i < 3; i++)
	{
		//printxy(s[i], 48 + i * 5, 16, 15, 0);
		printxy("____________", 44 + i * 5, 25, color, 0);
	}
	if (mark == 2)
	{
		if (s[2] == s[3])
			printxy("________", 44, 25, 12, 0);
		else
        if (s[1] == s[3])
            printxy("________", 44 + 8, 25, 12, 0);
        else
            printxy("________", 44 + 14, 25, 12, 0);
	}
}

void printNum(bool x) {
     int kind=0;
     if (x) kind=1;
     printxy(s[1],45,24,15*kind,16*kind);
     printxy(s[2],55,24,15*kind,16*kind);
     printxy(s[3],65,24,15*kind,16*kind);
}

void startGame(bool x) {
    if (x) {
        showLine(true);
        showHelp(true);
        placeBet();
        calculation();
        printNum(true);
        showResult(false);
        showResult(true);
        showSlot(true);
        showMoney(false);
        showMoney(true);
        showEarnMoney(false);
        showEarnMoney(true);
    }
}

void Menu() {
    //showLine(false);
    //showHelp(false);
    showMenu(true);
    showSlot(false);
    showMoney(false);
    showResult(false);
    printNum(false);
    showEarnMoney(false);
    GameName(true);
    //calculation(false);
    int key;
    selectionMenuItem=0;
    do {
        showSelectionMenuItem(true,selectionMenuItem);
        key=readKey();
        showSelectionMenuItem(false,selectionMenuItem);
        if (key==-72) {
            selectionMenuItem=max(0, selectionMenuItem-1);
        } else if (key==-80) {
            selectionMenuItem=min(3,selectionMenuItem+1);
        }
    } while (key != 13);
    showMenu(false);
}

bool CmpScore() {
    if (maxMoney>highscore) {
        return true;
    }
    else return false;
}

void writeHighScore(bool x) {
    int kind=0;
    if (x) {
        kind=1;
    }
    printxy("Oh ! New High Score: ", 40,22,14*kind,16*kind );
    printxy(highscore, 65,22,7*kind,0 );
}

void GameOver(bool x) {
	int kind;
	if (!x)
		kind = 0;
	else
		kind = 1;
	printxy("GAME OVER", 51, 18, kind * 12, 0);
	printxy("Enter", 45, 20, kind * 15, 5*kind);
	printxy("to return Menu", 51, 20, kind*15,0);
	if (kind == 1)
		while (readKey() != 13);
}

void ScreenHelp(bool x) {
    int kind=0;
    if (x) kind=1;
    for (int i=0; i<10; i++) {
        printxy("><", i*2, 20+i, 15*kind, 16*kind);
        printxy("><", 100+i*2, i, 15*kind, 16*kind);
    }
    printxy("HELP",3,5,11*kind,0);
    printxy("After the player has input his or her bet, the program must calculate three random numbers in the range [2, 7]",3,10,14*kind,0);
    printxy("and output them neatly to the screen. If all three numbers are sevens, then the player wins ten times their betting",3,11,14*kind,0);
    printxy("money; else if, the three numbers are all the same, but not sevens, then the player wins five times their betting",3,12,14*kind,0);
    printxy("money; else if, two out of the three numbers are the same then the player wins three times their betting money;",3,13,14*kind,0);
    printxy("else, the player loses his or her bet.  ", 3,14,14*kind,0);
    printxy("Enter", 45,17,9*kind,11*kind);
    printxy("to return Menu", 51, 17, 15*kind,0);
}

void ScreenCredit(bool x) {
    int kind = 0;
    if (x) kind = 1;
    for (int i=0; i<10; i++) {
        printxy("><", i*2, 20+i, 15*kind, 16*kind);
        printxy("><", 100+i*2, i, 15*kind, 16*kind);
    }
    printxy("CREDIT:", 15, 7, 11*kind,0);
    printxy("Coder:", 20, 9,11*kind,0);
    printxy("#Nguyen Tran Huu Dang", 23,11,11*kind,0);
    printxy("-KTPM_K13. UIT", 23,12,11*kind,0);
    printxy("-Practice Smarter-",23,13,11*kind,0);
    printxy("Contact:", 65, 7, 10*kind,0);
    printxy("-pogback1f4@gmail.com",70, 8, 10*kind,0);
    printxy("-github.com/dangnth",70,9,10*kind,0);
    printxy("-01686968950",70,10,10*kind,0);
    printxy("THANK   YOU!", 65, 25,13*kind,0);
    printxy("8-30-2018.",65,26,13*kind,0);
    printxy("Enter", 25,27,9*kind,11*kind);
    printxy("to return Menu", 31, 27, 15*kind,0);

}

void PlayGame(bool x) {
    if (x) {
        Menu();
        switch (selectionMenuItem)
        {
        case 0 :
            wantExit=false;
            GameName(false);
            showMenu(false);
            GameOver(false);
            showEarnMoney(false);
            do {
                mark=-1;
                startGame(true);
            } while (!wantExit && money>0);
            showResult(false);
            if (money==0) {
                if (CmpScore()) {
                    highscore=maxMoney;
                    writeHighScore(true);
                }
                GameOver(true);
                writeHighScore(false);
            }
            showLine(false);
            showHelp(false);
            break;
        case 1 :
            wantExit=false;
            GameName(false);
            ScreenCredit(true);
            int key;
            while (key != 13) {
                key=readKey();
            }
            key=0;
            ScreenCredit(false);
            break;
        case 2 :
            wantExit=false;
            GameName(false);
            key=0;
            ScreenHelp(true);
            while (key != 13) {
                key=readKey();
            }
            key=0;
            ScreenHelp(false);
            break;
        default : wantExit = true; break;
        }
     GameOver(false);
     showEarnMoney(false);
     //ScreenHelp(false);
    }
}

void Game() {
    //string MenuItemName[4]={"Start","Credit","Help","Exit"};
    //XoaManHinh();
    resizeConsole(100,100);
    Intro();
    GameName(true);
    srand((unsigned int)time(0));
    showMoney(false);
    do {
        money=1000;
        mark=-1;
        PlayGame(true);
    } while (!wantExit);
    GameOver(false);
}

int main()
{
    Game();
    return 0;
}
