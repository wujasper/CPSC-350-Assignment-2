/*
  Full Name: Jasper Wu
  Student ID: 2339715
  Chapman email: jaswu@chapman.edu
  Course number and section: 350-01
  Assignment Number: 2
*/

#include "assign2.h"
#include "fileio.h"

string getUserInput(string message);
int stringToInt(string str);
double stringToDouble(string str);

int main(int argc, char** argv) {
  string userInput;
  int height;
  int width;
  int boundaryMode;
  double density;
  bool pauseGame = false;
  bool configFromFile = false;
  bool outputToFile = false;

  FileIo *files = new FileIo();
  GameOfLife *game;

  // Ask if user wants to provide the world configuration file.
  // Repeat the question if user does not respond with either 'y' or 'n'
  do {
    userInput = getUserInput("Do you want to provide configuration file?(y/n) ");
  } while (userInput != "y" && userInput != "n");
  // Ask the file name if user selects to provide the configuration file
  // Repeat the question if user does not respond with valid configuraiton file
  if (userInput == "y") {
    do {
      userInput = getUserInput("Enter configuration file name: ");
    } while (!files->openFileForRead(userInput));
    configFromFile = true;
  }
  else {
    // If user selects random, then ask for height, width, and population density
    // repeat the questions if user does not respond with valid input
    // Non-zero positive integer are valid inputs for dimension
    // Valid population density is decimal value between 0 and 1 excluding 0
    do {
      userInput = getUserInput("Enter height: ");
      height = stringToInt(userInput);
    } while (height <= 0);
    do {
      userInput = getUserInput("Enter width: ");
      width = stringToInt(userInput);
    } while (width <= 0);
    do {
      userInput = getUserInput("Enter population density(decimal value greater than 0 and less or equal to 1): ");
      density = stringToDouble(userInput);
    } while (density <= 0 || density > 1);

  }
  // Ask user what kind of boundary mode for the simulation
  // 0 is for classic mode
  // 1 is for doughnut mode
  // 2 is for mirror mode
  // Repeat the question if user does not respond with value between 0 - 2
  do {
    userInput = getUserInput("Select boundary mode(0-Classic, 1-Doughnut, 2-Mirror): ");
    boundaryMode = stringToInt(userInput);
  } while (boundaryMode < 0 || boundaryMode > 2);

  // Ask user if he wants the world simulation to be output to a file
  // Repeat the question if user does not respond with either 'y' or 'n'
  do {
    userInput = getUserInput("Do you want simulation output to a file?(y/n) ");
  } while (userInput != "y" && userInput != "n");
  // If user selects output file option, ask for output file name
  // Repeat the question if user does not respond with valid output file name
  if (userInput == "y") {
    do {
      userInput = getUserInput("Enter output file name: ");
    } while (!files->openFileForWrite(userInput));
    outputToFile = true;
  }
  else {
    // If user selects screen output optoon, ask if he wants a brief pause
    // between each new world simulation.
    // Repeat the question if user does not respond with either 'y' or 'n'
    do {
      userInput = getUserInput("Do you want brief pause between simulation?(y/n) ");
    } while (userInput != "y" && userInput != "n");
    if (userInput == "y") {
      pauseGame = true;
    }
  }
  // Instantiate game object
  if (configFromFile) {
    // Instantiate game object from the file
    game = new GameOfLife(files->readStream, boundaryMode);
  }
  else {
    // Instantiate game object using height, width, and population density
    game = new GameOfLife(height, width, density, boundaryMode);
  }
  // run the game till world becomes empty or stablized
  game->runGame(outputToFile, files->writeStream, pauseGame);
  // Ask user to press Enter to exit program
  game->pause("Press Enter to exit the program...");

  // delete the game object
  delete game;
  // delete opened files
  delete files;
}
// Get user input and return as a string
string getUserInput(string message) {
  string userInput;

  cout << message;
  cin >> userInput;

  return userInput;
}
// convert string to integer
int stringToInt(string str) {
  int convertedValue;

  try {
    convertedValue = stoi(str);
  }
  catch(exception& e) {
    return -1;
  }

  return convertedValue;
}
// convert string to double
double stringToDouble(string str) {
  double convertedValue;

  try {
    convertedValue = stod(str);
  }
  catch(exception& e) {
    return -1;
  }

  return convertedValue;
}
