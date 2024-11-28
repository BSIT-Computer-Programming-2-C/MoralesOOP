#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <unistd.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char spaceship[4][5] = { ' ',' ','^',' ',' ', 
                         ' ','/','|','\\',' ', 
                         ' ','/','|','\\',' ', 
                         ' ',' ',' ',' ',' ' }; 

int spaceshipPosX = WIN_WIDTH / 2;  
int spaceshipPosY = 22; 
int score = 0;

int bulletX = -1, bulletY = -1;  // Bullet position (-1 means no bullet)
bool bulletActive = false;  // Bullet status (whether it's fired)

void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder(){
	SetConsoleTextAttribute(console, FOREGROUND_BLUE);  
	for(int i=0; i<SCREEN_HEIGHT; i++){
		for(int j=0; j<15; j++){
			gotoxy(0+j,i); cout<<"±";
			gotoxy(WIN_WIDTH-j,i); cout<<"±";
		}
		
	} 
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(SCREEN_WIDTH,i); cout<<"±";
	} 
}

void genEnemy(int ind){
	enemyX[ind] = 15 + rand() % (28);
}

void drawEnemy(int ind){
	SetConsoleTextAttribute(console, FOREGROUND_RED); 
	if(enemyFlag[ind] == true){
		gotoxy(enemyX[ind], enemyY[ind]);   cout<<"  @@@  ";  
		gotoxy(enemyX[ind], enemyY[ind] + 1); cout<<" @   @ "; 
		gotoxy(enemyX[ind], enemyY[ind] + 2); cout<<"  @@@  "; 
		gotoxy(enemyX[ind], enemyY[ind] + 3); cout<<" @   @ ";  
	} 
	SetConsoleTextAttribute(console, 7);
}

void eraseEnemy(int ind){
	if(enemyFlag[ind] == true){
		gotoxy(enemyX[ind], enemyY[ind]); cout<<"        ";  
		gotoxy(enemyX[ind], enemyY[ind] + 1); cout<<"        "; 
		gotoxy(enemyX[ind], enemyY[ind] + 2); cout<<"        "; 
		gotoxy(enemyX[ind], enemyY[ind] + 3); cout<<"        "; 
	} 
}

void resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 1;
	genEnemy(ind);
}

void drawSpaceship(){
    SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED); 
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 5; j++){
            gotoxy(j + spaceshipPosX, i + spaceshipPosY); cout << 
			spaceship[i][j];
        }
    }
     SetConsoleTextAttribute(console, 7);
}

void eraseSpaceship(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 5; j++){
			gotoxy(j + spaceshipPosX, i + spaceshipPosY); cout << " ";
		}
	}
}

int collision(){
	if(enemyY[0] + 4 >= spaceshipPosY && enemyY[0] <= spaceshipPosY + 3){
		if(enemyX[0] + 6 - spaceshipPosX >= 0 && enemyX[0] + 6 - spaceshipPosX < 10){
			return 1;
		}
	}
	return 0;
}

bool bulletCollision(){
	if(bulletY >= enemyY[0] && bulletY <= enemyY[0] + 3) {
		if(bulletX >= enemyX[0] && bulletX <= enemyX[0] + 6) {
			resetEnemy(0);
			score += 1;
		 if(score >= 1 && score % 10 == 0) {
                score *= 2; 
            }
            return true;
    	}
	}
	if(bulletY >= enemyY[1] && bulletY <= enemyY[1] + 3) {
		if(bulletX >= enemyX[1] && bulletX <= enemyX[1] + 6) {
			resetEnemy(1);
			score += 1;
		if(score >= 1 && score % 10 == 0) {
                score *= 2;  
            }
            return true;
		}
	}
	return false;
}

void gameover(){
	system("cls");
	cout<<endl;
	cout<<"\t\t±±±±±±±±±±±±±±±±±±±±±±±±±±"<<endl;
	cout<<"\t\t±±±±±±±± Game Over ±±±±±±±"<<endl;
	cout<<"\t\t±±±±±±±±±±±±±±±±±±±±±±±±±±"<<endl<<endl;
	cout<<"\t\tPress any key to go back to menu.";
	getch();
}
void instructions(){
	system("cls");
	cout << "Instructions";
	cout << "\n----------------";
	cout << "\n Avoid asteroids or enemy ships by moving left, right, up, or down.";
	cout << "\n Shoot bullets to destroy the enemies.";
	cout << "\n\n Press 'a' to move left";
	cout << "\n Press 'd' to move right";
	cout << "\n Press 'w' to move up";
	cout << "\n Press 's' to move down";
	cout << "\n Press space to shoot";
	cout << "\n Press 'escape' to exit";
	cout << "\n\nPress any key to go back to menu";
	getch();
}

void updateScore(){
	if (score <= 10) {
		SetConsoleTextAttribute(console, FOREGROUND_RED);  
	} else if (score <= 20){
		SetConsoleTextAttribute(console, FOREGROUND_BLUE); 
	}else if (score <= 30){
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);
	}else {
		SetConsoleTextAttribute(console, FOREGROUND_GREEN);
	}
	
	gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score << endl;
	SetConsoleTextAttribute(console, 7);
}


void setColor(int percentage) {
    if (percentage <= 20) {
        SetConsoleTextAttribute(console, FOREGROUND_RED);  
    } else if (percentage <= 40) {
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN); 
    } else if (percentage <= 60) {
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);  
    } else if (percentage <= 80) {
        SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);  
    } else {
        SetConsoleTextAttribute(console, FOREGROUND_GREEN);  
    }
}

void loadingScreen() {
    system("cls");
    int progress = 0;
    int loadingBarWidth = 50; 
    int barStartX = 10, barStartY = 5;
   
    SetConsoleTextAttribute(console, 7); 
      
    for (int i = 0; i < loadingBarWidth; i++) {
        progress = (i + 1) * 2; 
     
        setColor(progress);       
        gotoxy(barStartX + i, barStartY + 2);
        cout << "±"; 
        gotoxy(10, 5);
        SetConsoleTextAttribute(console, 7); 
        cout << "Loading: " << progress << "%" << flush;
        Sleep(50); 
    }
       
    cout << "]" << endl;
    SetConsoleTextAttribute(console, 7);
    Sleep(500); 
}
int bulletSpeed = 3;
int spaceShipSpeed = 4;
 
void play(){
	spaceshipPosX = WIN_WIDTH / 2;
	spaceshipPosY = 22;  // reset spaceship's vertical position
	score = 0;
	enemyFlag[0] = 1;
	enemyFlag[1] = 0;
	enemyY[0] = enemyY[1] = 1;
	
	system("cls"); 
	drawBorder(); 
	updateScore();
	genEnemy(0);
	genEnemy(1);
	
	gotoxy(WIN_WIDTH + 7, 2); cout << "Space Game";
	gotoxy(WIN_WIDTH + 6, 4); cout << "----------";
	gotoxy(WIN_WIDTH + 6, 6); cout << "----------";
	gotoxy(WIN_WIDTH + 7, 12); cout << "Control ";
	gotoxy(WIN_WIDTH + 7, 13); cout << "-------- ";
	gotoxy(WIN_WIDTH + 2, 14); cout << " A Key - Left";
	gotoxy(WIN_WIDTH + 2, 15); cout << " D Key - Right"; 
	gotoxy(WIN_WIDTH + 2, 16); cout << " W Key - Up";  
	gotoxy(WIN_WIDTH + 2, 17); cout << " S Key - Down"; 
	gotoxy(WIN_WIDTH + 1, 18); cout << " Space Key - Shoot"; 
	
	while(1){
		
		eraseSpaceship();
		// Bullet movement and logic
		if (bulletActive) {
			gotoxy(bulletX, bulletY); cout << " ";
			bulletY-= bulletSpeed;  // move bullet upwards
			if (bulletY < 0 || bulletCollision()) {
				bulletActive = false;  // Reset bullet when it goes off screen or hits an enemy
			}
			if (bulletActive) {
				gotoxy(bulletX, bulletY); cout << "|";  // Draw bullet
			}
		}
		
		if (kbhit()) {
    	char key = getch();
    	if (key == 'a' && spaceshipPosX > 1) {
        spaceshipPosX -= spaceShipSpeed; // Move left
    	} 
    	else if (key == 'd' && spaceshipPosX < WIN_WIDTH - 7) {
        spaceshipPosX += spaceShipSpeed; // Move right
    	} 
    	else if (key == 'w' && spaceshipPosY > 1) {
        spaceshipPosY -= spaceShipSpeed; // Move up
    	} 
    	else if (key == 's' && spaceshipPosY < SCREEN_HEIGHT - 5) {
        spaceshipPosY += spaceShipSpeed; // Move down
    	} 
   		else if (key == ' ' && !bulletActive) { 
        bulletX = spaceshipPosX + 2;  
        bulletY = spaceshipPosY;      
        bulletActive = true;          
    	} 
    	else if (key == 27) {  
        	gameover(); 
        	return;
    	} 
	}
		drawSpaceship();	
        drawEnemy(0);
        drawEnemy(1);
        updateScore();	

		if(collision() == 1){
			gameover();
			return;
		}

		Sleep(85);
		eraseSpaceship();
		drawSpaceship();
		eraseEnemy(0);
		eraseEnemy(1); 
		
		if(enemyY[0] == 9)
			if(enemyFlag[1] == 0)
				enemyFlag[1] = 1;
		
		if(enemyFlag[0] == 1)
			enemyY[0] += 1;
		
		if(enemyFlag[1] == 1)
			enemyY[1] += 1;
		 
		if(enemyY[0] > SCREEN_HEIGHT - 4){
			resetEnemy(0);
				
		}
		if(enemyY[1] > SCREEN_HEIGHT - 4){
			resetEnemy(1);
				
		}
	}
}

int main(){
	setcursor(0, 0); 
	srand((unsigned)time(NULL)); 
	
	// Show the loading screen before the menu
	loadingScreen();
	
	do{
		SetConsoleTextAttribute(console, FOREGROUND_BLUE); 
		system("cls");
		gotoxy(10,5); cout << " ±±±±±±±±±±±±±±±±±±±±±±±±±± "; 
		gotoxy(10,6); cout << " |       Space Game       | "; 
		gotoxy(10,7); cout << " ±±±±±±±±±±±±±±±±±±±±±±±±±±";
		gotoxy(11,9); cout << "1. Start Game";
		gotoxy(11,10); cout << "2. Instructions";	 
		gotoxy(11,11); cout << "3. Quit";
		gotoxy(11,13); cout << "Select option: ";
		char op = getche();
		
		 SetConsoleTextAttribute(console, 7);
		
		if(op == '1') play();
		else if(op == '2') instructions();
		else if(op == '3') exit(0);
	} while(1);
	
	return 0;
}
