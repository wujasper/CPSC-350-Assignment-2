/*
  Full Name: Jasper Wu
  Student ID: 2339715
  Chapman email: jaswu@chapman.edu
  Course number and section: 350-01
  Assignment Number: 2
*/

#include "assign2.h"

// overloaded constructor with height, width, boundary mode and population density
GameOfLife::GameOfLife(int height, int width, double density, int boundaryMode){
  // Initialize the world
  if (setWorld(height, width) != -1) {
    // Fill the world randomly based on the population density
    fillRandomWorld(density);
    // Set boundary mode
    setMode(boundaryMode);
  }
}
// overlaoded constructor with world configuration file stream and boundary mode
GameOfLife::GameOfLife(ifstream &readStream, int boundaryMode){
  // Initialize the world with height, width, and pattern from file
  if (fillWorldFromFile(readStream) != -1) {
    // Set boundary mode
    setMode(boundaryMode);
  }
}
// default destructor
GameOfLife::~GameOfLife(){
  // delete worlds
  deleteWorlds();
}
// Initialize world with height and width
int GameOfLife::setWorld(int height, int width) {
  if (height <= 0 || width <= 0) {
    cout << "=== Invalid width or height ===" << endl;
    return -1;
  }
  this->height = height;
  this->width = width;

  // create 2d array for curWorld and shadowWorld
  curWorld = new char*[height];
  shadowWorld = new char*[height];
  // create array of size width for each row
  for (int r = 0; r < height; ++r) {
    curWorld[r] = new char[width];
    shadowWorld[r] = new char[width];
  }

  // fill the current world with '-'
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      curWorld[r][c] = '-';
    }
  }

  return 0;
}
// Set boundary mode
int GameOfLife::setMode(int boundaryMode) {
  if (boundaryMode < 0 || boundaryMode > 2) {
    cout << "=== Invalid boundary mode ===" << endl;
    return -1;
  }
  this->boundaryMode = boundaryMode;

  return 0;
}
// Fill the world randomly based on the population density
int GameOfLife::fillRandomWorld(double density) {
  if (density <= 0 || density > 1) {
    cout << "=== Invalid population density ===" << endl;
    return -1;
  }
  int randR;
  int randC;
  // Compute number of 'X' to be marked
  int count = (int)(density * (double)(width * height));

  // Mark the cell with 'X' chosen by random row and random column
  // till number of 'X' satisfies the population count
  do {
    randR = rand() % height;
    randC = rand() % width;
    // Mark cell with 'X' if it is '-' and decrement the population count
    if (curWorld[randR][randC] == '-') {
      curWorld[randR][randC] = 'X';
      count--;
    }
  } while (count > 0);

  return 0;
}
// Fill the world from coniguraiton file
int GameOfLife::fillWorldFromFile(ifstream &readStream) {
  if (!readStream) {
    cout << "=== Invalid read file stream ===" << endl;
    return -1;
  }

  // read the file input and assign world dimension and initial world configuraiton
  // setup the world after height and width are available, don't continue if either one is not valid
  // then read one row at a time from the file and copy it into current world
  string line;
  int lineNum = 0;

  while (getline(readStream, line)) {
    // get height from line # 0
    if (lineNum == 0) {
      try {
        height = stoi(line);
      }
      catch (exception& e) {
        return -1;
      }
    }
    // get width from line # 1
    else if (lineNum == 1) {
      try {
        width = stoi(line);
      }
      catch (exception& e) {
        return -1;
      }
      // initialize the world
      if (setWorld(height, width) == -1) {
        return -1;
      }
    }
    // get line from the file and populate to the row of the world
    else {
      for (int c = 0; c < line.length(); ++c) {
        curWorld[lineNum - 2][c] = line[c];
      }
    }
    ++lineNum;
    // stop to read the file if row execeeds the maximum
    if (lineNum > (height + 1)) {
      break;
    }
  }

  return 0;
}
// Count number of neightbors that are marked as 'X'
int GameOfLife::countNeighbors(int row, int col) {
  int newR;
  int newC;
  int count = 0;

  // Neightbors are in a 3x3 array and transform the cell coordinate
  // accordingly based on the boundary mode. Skip the cell if necessary
  for (int r = row - 1; r < row + 2; ++r) {
    for (int c = col - 1; c < col + 2; ++c) {
      // assign (row, col) to (newRow, newCol) as coordinate may be modified
      newR = r;
      newC = c;
      // Get the transformed cell coordinate or skip the cell
      if (transformCell(newR, newC, row, col)) {
        continue;
      }
      // Increment the count if neightbor is marked as 'X'
      if (curWorld[newR][newC] == 'X') {
        ++count;
      }
    }
  }
  return count;
}
// Generate the next cell for current world simulation based on the rule
char GameOfLife::generateNewCell(int row, int col) {
  char mark = curWorld[row][col];
  int count = countNeighbors(row, col);

  // Mark cell as '-' if number of neighbors are marked either is less or equal
  // then 1 or is greater or equal then 4
  if (count <= 1 || count >= 4) {
    mark = '-';
  }
  // Mark cell as 'X' if number of neightbors are marked equal to 3
  else if (count == 3){
    mark = 'X';
  }

  return mark;
}
// Transform the cell coordinate based on boundary mode
// (cellRow, cellCol) is cell coordinate from the neighbors
// (originRow, originCol) is the cell coordinate of the cell to be exaimed
// (cellRow, cellCol) are passed as reference so they can be modified
// skip the cell based on the condition for each boundary mode
bool GameOfLife::transformCell(int &cellRow, int &cellCol, int originRow, int originCol) {
  bool skip = false;
  bool self = (cellRow == originRow) && (cellCol == originCol);

  // skip this neighbor cell if it is itself
  if (self) {
    skip = true;
  }
  // In classic mode, skip this neighbor cell if it is out of bound
  // cell is out of bound if one of its cell coordinate is negative or
  // greater than its maximum value. Maximum value for row is height - 1 and
  // maximum value for column is width - 1. No cell coordinate transformation is needed
  else if (boundaryMode == 0) {
    if ((cellRow < 0) || (cellCol < 0) || (cellRow > (height - 1)) || (cellCol > (width - 1))) {
      skip = true;
    }
  }
  // For doughnut mode, the cell coordinate is wrapped around if it is out of bound
  else if (boundaryMode == 1) {
    // negative column value becomes width - 1
    if (cellCol < 0) {
      cellCol = width - 1;
    }
    // column greater than width - 1 becomes 0
    else if (cellCol > (width - 1)){
      cellCol = 0;
    }
    // negative row value becomes height - 1
    if (cellRow < 0) {
      cellRow = height - 1;
    }
    // row greater than height - 1 becomes 0
    else if (cellRow > (height - 1)) {
      cellRow = 0;
    }
  }
  // For mirror mode, out of bound cell coordinate is mirrored from its immediate
  // inbound neighbor veritcally or horizontally. The out of bound cell with only
  // inbound corner cell as neighbor mirrors the corner cell
  else {
    // negative column value becomes 0
    if (cellCol < 0) {
      cellCol = 0;
    }
    // column greater than width - 1 becomes width - 1
    else if (cellCol > (width -1)) {
      cellCol = width - 1;
    }
    // negative row value becomes 0
    if (cellRow < 0) {
      cellRow = 0;
    }
    // row greater than height - 1 becomes height - 1
    else if (cellRow > (height - 1)) {
      cellRow = height - 1;
    }
  }
  return skip;
}
// Generate next simulation world through generating each next cell
// and return true boolean if the world is not stablized yet and not empty
bool GameOfLife::generateNewWorld() {
  bool worldIsChanged = false;
  bool worldIsEmpty = true;

  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      shadowWorld[r][c] = generateNewCell(r, c);
      worldIsChanged |= (curWorld[r][c] != shadowWorld[r][c]);
      worldIsEmpty &= (shadowWorld[r][c] == '-');
    }
  }
  copyWorld(shadowWorld, curWorld);
  return (worldIsChanged && !worldIsEmpty);
}
// Copy the world, mainly used to copy shadow world to current world after world generation
void GameOfLife::copyWorld(char** world1, char** world2){

  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      world2[r][c] = world1[r][c];
    }
  }
}
// Print world to output file or screen
void GameOfLife::printWorld(bool outputToFile, ofstream &writeStream, string generationNumStr) {

  string rowStr;

  // print the generation number
  if (outputToFile) {
    writeStream << generationNumStr << endl;
  }
  else {
    cout << generationNumStr << endl;
  }

  // print the world
  for (int r = 0; r < height; ++r) {
    rowStr = "";
    // build row string
    for (int c = 0; c < width; ++c) {
      rowStr += curWorld[r][c];
    }
    // print row string
    if (outputToFile) {
      writeStream << rowStr << endl;
    }
    else {
      cout << rowStr << endl;
    }
  }
}
// Run the game till the world is empty or stablized, pause the simulation
// for each new world generation based on user selection
int GameOfLife::runGame(bool outputToFile, ofstream &writeStream, bool pauseGame){

  if (!writeStream) {
    cout << "=== Invalid write file stream ===" << endl;
    return -1;
  }

  int generationNum = 0;
  string generationNumStr;
  bool gameContinue;

  // print current world start with generation number = 0
  // generate next world, if next world is stablized then exit the game
  // pause the game if user selects the option
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  // print initial world to either screen or to a file
  printWorld(outputToFile, writeStream, to_string(generationNum++));
  do {
    // print world to either screen or to a file
    generationNumStr = "generation " + to_string(generationNum++);
    printWorld(outputToFile, writeStream, generationNumStr);
    // generate next world
    gameContinue = generateNewWorld();
    // pause if user selects to pause between world generation and game does not finish
    if (pauseGame && gameContinue) {
      pause ("Press Enter to continue...");
    }
  } while (gameContinue);

  return 0;
}
// Pause the new world generation till Enter is typed
void GameOfLife::pause(string message){

  cout << message << flush;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
// Delete the worlds
void GameOfLife::deleteWorlds() {

  // delete array for each row
  if (!curWorld) {
    for (int r = 0; r < height; ++r) {
      delete [] curWorld[r];
    }
    delete [] curWorld;
  }

  if (!shadowWorld) {
    for (int r = 0; r < height; ++r) {
      delete [] shadowWorld[r];
    }
    delete [] shadowWorld;
  }
}
