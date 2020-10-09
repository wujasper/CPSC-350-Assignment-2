/*
  Full Name: Jasper Wu
  Student ID: 2339715
  Chapman email: jaswu@chapman.edu
  Course number and section: 350-01
  Assignment Number: 2
*/

#include <iostream>
#include <fstream>

using namespace std;

class FileIo {

public:
  ifstream readStream;
  ofstream writeStream;

  FileIo();
  ~FileIo();

  bool openFileForRead(string fileName);
  bool openFileForWrite(string fileName);
  void closeFiles();
};
