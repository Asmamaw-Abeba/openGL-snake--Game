#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <cstdio>
#include <Windows.h>
#include <mmsystem.h>
#include <GL/glut.h>
#include <fstream>  // For file handling

using namespace std;

// Object and snake properties
struct Segment {
    GLdouble x, y;
};

std::vector<Segment> snake; // Snake body
GLdouble fx = 100, fy = 100; // Food position
GLdouble ps = 10;            // Point size
GLint d = 0;                 // Direction indicator
GLint score = 0;             // Score
GLint highScore = 0;         // High score
bool gameOver = false;
int speed = 100;              // Initial speed (in milliseconds)
bool isFullScreen = false;    // Track whether the game is in fullscreen mode

//New state for the game
enum GameState { OVERVIEW, INGAME };
GameState currentState = OVERVIEW; // Start in the overview screen

// Function prototypes
void initializeSnake();
void updateSnakePosition();
void growSnake();
void randomFood();
void playSound(const char* soundFile);
void drawSnake();
void myDisplay();
void myTimer(int value);
void myInit();
void displayChar(void* font, const char* str, int x, int y);
void loadHighScore();
void saveHighScore();

// Initialize OpenGL
void myInit() {
    glClearColor(0.5, 1.0, 1.0, 0);
    glPointSize(ps);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 480, 0);
}

// Initialize the snake
void initializeSnake() {
    snake.push_back({320.0, 240.0}); // Start with a single segment
}

// Generate random food position
void randomFood() {
    fx = rand() % (640 - 10) + 10; // Random number between 10 and 630
    fy = rand() % (480 - 10) + 10; // Random number between 10 and 480
}


// Play sound
void playSound(const char* soundFile) {
    PlaySound(soundFile, NULL, SND_FILENAME | SND_ASYNC);
}

// Update snake position
void updateSnakePosition() {
    for (size_t i = snake.size() - 1; i > 0; i--) {
        snake[i] = snake[i - 1]; // Each segment follows the one ahead
    }

    // Move the head
    if (d == 0) snake[0].x += 10; // Right
    else if (d == 1) snake[0].x -= 10; // Left
    else if (d == 2) snake[0].y += 10; // Down
    else if (d == 3) snake[0].y -= 10; // Up
}

// Grow the snake
void growSnake() {
    Segment newSegment = snake.back(); // Add a new segment at the tail's position
    snake.push_back(newSegment);
}

// Draw the snake
// Draw the snake with alternating colors for segments
void drawSnake() {
    for (size_t i = 0; i < snake.size(); i++) {
        // Alternate colors for each segment
        if (i % 2 == 0) {
            glColor3f(0.0, 0.0, 1.0); // Blue for even segments
        } else {
            glColor3f(0.0, 1.0, 0.0); // Green for odd segments
        }

        // Draw border (black)
        glColor3f(0.0, 0.0, 0.0); // Border color (black)
        glBegin(GL_QUADS);
        glVertex2i(snake[i].x - 5, snake[i].y - 5);
        glVertex2i(snake[i].x + 5, snake[i].y - 5);
        glVertex2i(snake[i].x + 5, snake[i].y + 5);
        glVertex2i(snake[i].x - 5, snake[i].y + 5);
        glEnd();

        // Draw body inside the border
        glColor3f(i % 2 == 0 ? 0.0f : 0.0f, i % 2 == 0 ? 0.0f : 1.0f, 1.0f); // Color based on index
        glBegin(GL_QUADS);
        glVertex2i(snake[i].x - 4, snake[i].y - 4);
        glVertex2i(snake[i].x + 4, snake[i].y - 4);
        glVertex2i(snake[i].x + 4, snake[i].y + 4);
        glVertex2i(snake[i].x - 4, snake[i].y + 4);
        glEnd();
    }
}


// Display characters
void displayChar(void* font, const char* str, int x, int y) {
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(1.0, 0.0, 0.0); // Text color
    glRasterPos2i(x, y);
    for (size_t i = 0; i < strlen(str); i++) {
        glutBitmapCharacter(font, str[i]);
    }
    glPopAttrib();
}

// Timer function for controlling update speed
// Global flag to indicate whether the game is paused
bool isPaused = false;

// Function to write high score to a binary file
void writeHighScore() {
    std::ofstream outfile("C:\\Users\\mf\\Desktop\\Graphics Project\\flappy_Bird_Gr4\\bin\\score.bin", std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(reinterpret_cast<const char*>(&highScore), sizeof(highScore));
        outfile.close();
    } else {
        cerr << "Error: Could not open file to save high score!" << endl;
    }
}

// Function to read high score from a binary file
void readHighScore() {
    std::ifstream infile("C:\\Users\\mf\\Desktop\\Graphics Project\\flappy_Bird_Gr4\\bin\\score.bin", std::ios::binary);
    if (infile.is_open()) {
        infile.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));
        infile.close();
    } else {
        // If the file does not exist, initialize high score to 0
        highScore = 0;
    }
}

// Timer function for controlling update speed
void myTimer(int value) {
    if (gameOver || isPaused || currentState != INGAME) return;  // Skip updating if the game is over or not in game state

    updateSnakePosition();

    // Check if the snake eats food
    if ((snake[0].x + 5 >= fx - 5) && (snake[0].y + 5 >= fy - 5) &&
        (snake[0].x - 5 <= fx + 5) && (snake[0].y - 5 <= fy + 5)) {
        score++;
        randomFood();
        growSnake();
    }

    // Check collision with walls
    if (snake[0].x < 10 || snake[0].x >= 630 || snake[0].y < 10 || snake[0].y >= 470) {
        playSound("hit");
        gameOver = true;
        if (score > highScore) {
            highScore = score;
            writeHighScore();  // Save the new high score
        }
    }

    // Check self-collision
    for (size_t i = 1; i < snake.size() - 1; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            playSound("hit");
            gameOver = true;
            if (score > highScore) {
            highScore = score;
            writeHighScore();  // Save the new high score
        }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(100, myTimer, 0); // Repeat after 100 milliseconds
}

// Draw the overview screen
void drawOverviewScreen() {

    glClear(GL_COLOR_BUFFER_BIT);
    // border for it
    glColor3f(0.0, 0.0, 0.0); // Border color (black)
    glLineWidth(10);
    glBegin(GL_LINE_LOOP);
    glVertex2i(50, 50);
    glVertex2i(600, 50);
    glVertex2i(600, 470);
    glVertex2i(50, 470);
    glEnd();

    glColor3f(1.0, 1.0, 0.0); // Yellow color for the text
    displayChar(GLUT_BITMAP_TIMES_ROMAN_24, "******SNAKE GAME(By Asmamaw)******", 100, 100);
    displayChar(GLUT_BITMAP_TIMES_ROMAN_24, "=> Use Arrow Keys to control the snake", 150, 150);
    displayChar(GLUT_BITMAP_TIMES_ROMAN_24, "=> Press ENTER to Start", 150, 200);
    displayChar(GLUT_BITMAP_TIMES_ROMAN_24, "=> Press F to make full screen", 150, 250);
    displayChar(GLUT_BITMAP_TIMES_ROMAN_24, "=> Press P to pause (after start a game)", 150, 300);
    displayChar(GLUT_BITMAP_TIMES_ROMAN_24, "=> Press C to resume (after start a game)", 150, 350);
    displayChar(GLUT_BITMAP_TIMES_ROMAN_24, "=> Press R to restart (after start a game)", 150, 400);
    displayChar(GLUT_BITMAP_TIMES_ROMAN_24, "=> Press ESC to Exit", 150, 450);

    glutSwapBuffers();
}



void restartGame() {
    playSound("sunshineskirmish.wav");
    snake.clear();
    initializeSnake(); // Reinitialize the snake
    randomFood();      // Place a new random food
    d = 0;             // Reset direction
    score = 0;         // Reset score
    gameOver = false;  // Reset game over state
    speed = 100;       // Reset speed
    glutTimerFunc(100, myTimer, 0); // Restart the timer
    glutPostRedisplay(); // Redraw the screen
}

// Display speed (for debugging purposes)
void displaySpeed() {
    char speedStr[20];
    sprintf(speedStr, "Speed: %d", 100 - (score / 2)); // Based on score
    displayChar(GLUT_BITMAP_TIMES_ROMAN_24, speedStr, 500, 110);
}

// Draw border around the screen
void drawBorder() {
    glColor3f(0.0, 0.0, 0.0); // Border color (black)
    glLineWidth(10);
    glBegin(GL_LINE_LOOP);
    glVertex2i(0, 0);
    glVertex2i(640, 0);
    glVertex2i(640, 480);
    glVertex2i(0, 480);
    glEnd();
}

// screen toggle function
void toggleFullScreen() {
    if (isFullScreen) {
        glutReshapeWindow(640, 480); // Set windowed mode size
        glutPositionWindow(100, 100); // Optionally set the window's position
        isFullScreen = false;
    } else {
        glutFullScreen(); // Set fullscreen mode
        isFullScreen = true;
    }
}



// Display function
void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
     if (currentState == OVERVIEW) {
        drawOverviewScreen();  // Draw the overview screen
    } else if (currentState == INGAME) {
        // Draw the border around the screen
        drawBorder();
        // Draw snake and food
        drawSnake();
        glColor3f(1.0, 0.0, 0.0); // Food color
        glBegin(GL_POINTS);
        glVertex2i(fx, fy);
        glEnd();

        // Display score and high score
        char scoreStr[20];
        sprintf(scoreStr, "Score: %d", score);
        displayChar(GLUT_BITMAP_TIMES_ROMAN_24, scoreStr, 500, 50);

        char highScoreStr[20];
        sprintf(highScoreStr, "High Score: %d", highScore);
        displayChar(GLUT_BITMAP_TIMES_ROMAN_24, highScoreStr, 500, 80);

        // Display game over message
        if (gameOver) {
            displayChar(GLUT_BITMAP_TIMES_ROMAN_24, "Game Over! Press 'R' to restart or 'ESC' to exit.", 50, 240);
        }

        glutSwapBuffers();
        glFlush();
    }
}

// Handle keyboard input
// boolean isPaused = false; // Flag to indicate whether the game is paused

// Handle keyboard input
// Handle keyboard input
void NormalKey(unsigned char key, int x, int y) {
    switch (key) {
        case 'p': case 'P':
            if (!isPaused) {
                isPaused = true; // Set paused flag to true
            }
            break;
        case 'c': case 'C':
            if (isPaused) {
                isPaused = false; // Set paused flag to false
                glutTimerFunc(100, myTimer, 0); // Resume the timer
            }
            break;
        case 27: // ESC to exit
            if (currentState == OVERVIEW) {
                exit(0);
            } else {
                currentState = OVERVIEW;  // Go back to the overview screen
                gameOver = false;         // Reset game over state
            }
            break;
        case 13: // ENTER to start the game
            if (currentState == OVERVIEW) {
                currentState = INGAME; // Start the game
                restartGame();
            }
            break;
        case 'r': case 'R':
            if (gameOver) {
                restartGame(); // Restart the game if it's over
            }
            break;
        case 'f': case 'F':
            toggleFullScreen(); // Toggle between fullscreen and windowed
            break;
    }
    glutPostRedisplay();
}


void mySpecial(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            if(d != 0) {
                 d = 1;
                 break;
            }
        case GLUT_KEY_RIGHT:
            if(d != 1) {
                d = 0; break;
            }
        case GLUT_KEY_UP:
            if(d != 2) {
                 d = 3; break;
            }

        case GLUT_KEY_DOWN:
            if(d != 3) {
                d = 2; break;
            }
    }
    glutPostRedisplay();
}

// Load the high score from a file


// Main function
int main(int argc, char* argv[]) {
     playSound("sunshineskirmish.wav");
    readHighScore(); // read the high score
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Snake Game");
    initializeSnake();
    randomFood();
    myInit();
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(NormalKey);
    glutSpecialFunc(mySpecial);
    glutTimerFunc(100, myTimer, 0); // Start the timer with a 100ms interval
    glutMainLoop();
    return 0;
}
