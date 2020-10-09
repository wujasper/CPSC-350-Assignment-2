/*
  Full Name: Jasper Wu
  Student ID: 2339715
  Chapman email: jaswu@chapman.edu
  Course number and section: 350-01
  Assignment Number: 2
*/

#include "fileio.h"

// default constructor
FileIo:: FileIo() {}
// default destrcutor
FileIo::~FileIo(){
  closeFiles();
}
// Open read file stream from input file name
// return true if file opens successfully
bool FileIo::openFileForRead(string fileName) {
  readStream.open(fileName);
  if (!readStream) {
    cout << "=== " << fileName << " cannot be opened ===" << endl;
    return false;
  }
  return true;
}
// Open write file stream from output file name
// return true if file opens successfully
bool FileIo::openFileForWrite(string fileName){
  writeStream.open(fileName);
  if (!writeStream) {
    cout << "=== " << fileName << " cannot be opened ===" << endl;
    return false;
  }
  return true;
}
// close opened file streams
void FileIo::closeFiles(){
  if (!readStream) {
    readStream.close();
  }
  if (!writeStream) {
    writeStream.close();
  }
}
