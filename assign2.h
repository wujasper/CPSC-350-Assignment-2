/*
  Full Name: Jasper Wu
  Student ID: 2339715
  Chapman email: jaswu@chapman.edu
  Course number and section: 350-01
  Assignment Number: 2
*/

#include <iostream>
#include <fstream>
#include <exception>
#include <stdlib.h>
#include <limits>
#include <string>

using namespace std;

class GameOfLife {
private:
  int height;
  int width;
  int boundaryMode;
  char **curWorld;
  char **shadowWorld;

  // private functions
  int setWorld(int height, int width);
  int setMode(int boundaryMode);
  int fillRandomWorld(double density);
  int fillWorldFromFile(ifstream &readStream);
  int countNeighbors(int row, int col);
  char generateNewCell(int row, int col);
  bool transformCell(int &cellRow, int &cellCow, int originRow, int originCol);
  bool generateNewWorld();
  void printWorld(bool outputToFile, ofstream &writeStream, string generationNumStr);
  void copyWorld(char** world1, char** world2);
  void deleteWorlds();

public:
  // constructors
  GameOfLife();
  GameOfLife(int height, int width, double density, int boundaryMode);
  GameOfLife(ifstream &readStream, int boundaryMode);
  // destructor
  ~GameOfLife();

  // public functions
  int runGame(bool outputToFile, ofstream &writeStream, bool pauseGame);
  void pause(string message);
};
