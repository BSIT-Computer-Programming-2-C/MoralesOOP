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

const int MIN_HEIGHT = 2; 
const int MAX_HEIGHT = SCREEN_HEIGHT - 4 ; 

int enemyY[3];
int enemyX[3];
int enemyFlag[3];

// Two spaceship designs
char spaceship1[4][5] = { ' ',' ','^',' ',' ', 
                          ' ','/','|','\\',' ', 
                          ' ','/','|','\\',' ', 
                          ' ',' ',' ',' ',' ' };

char spaceship2[4][5] = { ' ',' ','#',' ',' ', 
                          ' ','±','|','±',' ', 
                          ' ','±','|','±',' ', 
                          ' ',' ',' ',' ',' ' };
                          
// Pointer to selected spaceship
char (*spaceship)[4][5];
int spaceshipPosX = WIN_WIDTH / 2;  
int spaceshipPosY = 22; 
int score = 0;
int bulletX = -1, bulletY = -1; 
bool bulletActive = false;  

// Function to move the cursor to a specific position on the console
void gotoxy(int x, int y){
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

// Function to set the visibility of the console cursor
void setcursor(bool visible, DWORD size) {
    if(size == 0)
        size = 20;	
	
    CONSOLE_CURSOR_INFO lpCursor;	
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

// Function to draw the border around the game area
void drawBorder(){
    SetConsoleTextAttribute(console, FOREGROUND_BLUE);  
    for(int i=0; i<SCREEN_HEIGHT; i++){
        for(int j=0; j<17; j++){
            gotoxy(0+j,i); cout<<"±";
            gotoxy(WIN_WIDTH-j,i); cout<<"±";
        }
    } 
    for(int i=0; i<SCREEN_HEIGHT; i++){
        gotoxy(SCREEN_WIDTH,i); cout<<"±";
    } 
}

	void genEnemy(int ind){
	enemyX[ind] = 17 + rand() % (30);
}
// Function to draw the enemy at their current position
void drawEnemy(int ind) {
    if (enemyFlag[ind] == true){
        SetConsoleTextAttribute(console, FOREGROUND_RED); 
        gotoxy(enemyX[ind], enemyY[ind]);     cout << "  @@@  ";  
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout << " @   @ "; 
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "  @@@  "; 
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout << " ±   ± ";  
    }
    SetConsoleTextAttribute(console, 7);
}

// Function to erase the enemy from the screen
void eraseEnemy(int ind){
    if(enemyFlag[ind] == true){
        gotoxy(enemyX[ind], enemyY[ind]); cout<<"        ";  
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout<<"        "; 
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout<<"        "; 
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout<<"        "; 
    } 
}

// Function to reset the enemy to a new position
void resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 1;
	genEnemy(ind);
}


// Function to draw the spaceship on the screen
void drawSpaceship(){
    SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_RED); 
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 5; j++){
            gotoxy(j + spaceshipPosX, i + spaceshipPosY); cout << 
            (*spaceship)[i][j];
        }
    }
    SetConsoleTextAttribute(console, 7);
}

// Function to erase the spaceship from the screen
void eraseSpaceship(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 5; j++){
            gotoxy(j + spaceshipPosX, i + spaceshipPosY); cout << " ";
        }
    }
}


// Function to check collision between spaceship and enemy
int collision(){
    if(enemyY[0] + 4 >= spaceshipPosY && enemyY[0] <= spaceshipPosY + 3){
        if(enemyX[0] + 6 - spaceshipPosX >= 0 && enemyX[0] + 6 - spaceshipPosX < 10){
            return 1;
        }
    }
    return 0;
}

// Function to check if a bullet hits an enemy
bool bulletCollision(){
    if(bulletY >= enemyY[0] && bulletY <= enemyY[0] + 3) {
        if(bulletX >= enemyX[0] && bulletX <= enemyX[0] + 6) {
            resetEnemy(0);
            score += 1;
            return true;
        }
    }
    if(bulletY >= enemyY[1] && bulletY <= enemyY[1] + 3) {
        if(bulletX >= enemyX[1] && bulletX <= enemyX[1] + 6) {
            resetEnemy(1);
            score += 1;
            return true;
        }
    }
    return false;
}


// Function to display the game over screen
void gameover(){
    system("cls");
    cout<<endl;
    SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);  
    cout<<"\t\t±±±±±±±±±±±±±±±±±±±±±±±±±±"<<endl;
    cout<<"\t\t±±±±±±±± Game Over ±±±±±±±"<<endl;
    cout<<"\t\t±±±±±±±±±±±±±±±±±±±±±±±±±±"<<endl<<endl;
    cout<<"\t\tPress any key to go back to menu.";
    getch();
}

// Function to display instructions on how to play the game
void instructions(){
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
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

// Function to update the score display on the screen
void updateScore(){
    if (score <= 100) {
        SetConsoleTextAttribute(console, FOREGROUND_RED);  
    } else if (score <= 200){
        SetConsoleTextAttribute(console, FOREGROUND_BLUE); 
    }else if (score <= 300){
        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN);
    }else {
        SetConsoleTextAttribute(console, FOREGROUND_GREEN);
    }
    
    gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score << endl;
    SetConsoleTextAttribute(console, 7);
}

// Function to set the color of the loading bar based on percentage
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

// Function to display the loading screen
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

// Function to display the Spaceship design 
void displaySpaceshipDesign(int x, int y, char (*spaceship)[4][5]) {
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            gotoxy(x + j, y + i); 
            cout << (*spaceship)[i][j];
        }
    }
}

// Function to display the Spaceship choices
void selectSpaceship() {
    system("cls");

    // Display the header and choices
    SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "Select Your Spaceship: \n"; 
    
    SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    gotoxy(5, 4); 
    cout << "1. Spaceship 1";
    SetConsoleTextAttribute(console, FOREGROUND_BLUE| FOREGROUND_RED);
    displaySpaceshipDesign(5, 5, &spaceship1);  
	
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    gotoxy(5, 10); 
    cout << "2. Spaceship 2";
    SetConsoleTextAttribute(console, FOREGROUND_BLUE| FOREGROUND_RED);
    displaySpaceshipDesign(5, 11, &spaceship2);  
		SetConsoleTextAttribute(console, 7);
		
	SetConsoleTextAttribute(console, FOREGROUND_INTENSITY| FOREGROUND_GREEN);
    gotoxy(1, 15);  
    cout << "Enter your choice of spaceship (either 1 or 2): ";

    char choice = getch();

    // Set the selected spaceship
    if (choice == '1') {
        spaceship = &spaceship1;
    } else if (choice == '2') {
        spaceship = &spaceship2;
    } else {
        cout << "Invalid choice, selecting Spaceship 1 by default.\n";
        spaceship = &spaceship1;
    }

    system("cls");  
    cout << "You selected Spaceship " << (choice == '1' ? "1" : "2") << endl;
    displaySpaceshipDesign(1, 2, spaceship);  
    cout << "\n\nPress any key to continue to the game.";
    getch();  
}
 
void play(){
	spaceshipPosX = WIN_WIDTH / 2;
	spaceshipPosY = 22; 
	score = 0;
	enemyFlag[0] = 1;
	enemyFlag[1] = 0;
	enemyY[0] = enemyY[1] = 1;
	
	system("cls"); 
    drawBorder(); 
    updateScore();
    genEnemy(0);
    genEnemy(1);
    
	
	SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(WIN_WIDTH + 6, 2); cout << "Space Game";
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
		if (bulletActive) {
			gotoxy(bulletX, bulletY); cout << " ";
			bulletY-= bulletSpeed; 
			if (bulletY < 0 || bulletCollision()) {
				bulletActive = false;  
			}
			if (bulletActive) {
				gotoxy(bulletX, bulletY); cout << "|";  
			}
		}
		
		if (kbhit()) {
    	char key = getch();
    	if (key == 'a' && spaceshipPosX > + 21) {
        spaceshipPosX -= spaceShipSpeed; 
    	} 
    	else if (key == 'd' && spaceshipPosX <WIN_WIDTH -23) {
        spaceshipPosX += spaceShipSpeed; 
    	} 
    	else if (key == 'w' && spaceshipPosY > MIN_HEIGHT) {
        spaceshipPosY -= spaceShipSpeed; 
    	} 
    	else if (key == 's' && spaceshipPosY < MAX_HEIGHT) {
        spaceshipPosY += spaceShipSpeed; 
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
        drawEnemy(2);  
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
        

        if (enemyFlag[0] == 1) enemyY[0] += 1;
        if (enemyFlag[1] == 1) enemyY[1] += 1;
        if (enemyFlag[2] == 1) enemyY[2] += 1;
        
        // Reset enemies if they go off the screen
        if (enemyY[0] > SCREEN_HEIGHT - 4) resetEnemy(0);
        if (enemyY[1] > SCREEN_HEIGHT - 4) resetEnemy(1);
        if (enemyY[2] > SCREEN_HEIGHT - 4) resetEnemy(2);
    }
}
int main(){
	setcursor(0, 0); 
	srand((unsigned)time(NULL)); 
	
	loadingScreen();
	
	do{
		SetConsoleTextAttribute(console, FOREGROUND_BLUE); 
			
		system("cls");
		gotoxy(10,5); cout << " ±±±±±±±±±±±±±±±±±±±±±±±±±± "; 
		gotoxy(10,6); cout << " |       Space Game       | "; 
		gotoxy(10,7); cout << " ±±±±±±±±±±±±±±±±±±±±±±±±±±";
		SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		gotoxy(11,9); cout << "1. Start Game";
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
		gotoxy(11,10); cout << "2. Instructions";	 
		SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN);
		gotoxy(11,11); cout << "3. Quit";
		gotoxy(11,13); cout << "Select option: ";
		char op = getche();
		
		SetConsoleTextAttribute(console, 7);
		
		if(op == '1') {
            selectSpaceship();
            play();
        }
		
		if(op == '1') play();
		else if(op == '2') instructions();
		else if(op == '3') exit(0);
	} while(1);
	
	return 0;
}
