#include "LittleFS.h"
#include "file_system.module.hpp"

const char FileSystemModule::loggTag_[3] = "FS";

void FileSystemModule::onSetup()
{
  if (!LittleFS.begin(true))
  {
    Logg::error(FileSystemModule::loggTag_, "An error has occurred while mounting LittleFS");
  }
  Logg::info(FileSystemModule::loggTag_, "mounted successfully");
}

String FileSystemModule::readFile(String path)
{
  File file = LittleFS.open(path, FILE_READ);

  if (!file || file.isDirectory())
  {
    Logg::error(FileSystemModule::loggTag_, "failed to open file for reading");
    return String();
  }

  String fileContent;
  while (file.available())
  {
    fileContent = file.readString();
    break;
  }

  file.close();

  return fileContent;
}

void FileSystemModule::writeFile(String path, String content)
{
  File file = LittleFS.open(path, FILE_WRITE);

  file.print(content);

  file.close();
}

void FileSystemModule::writeFile(String path, JsonDocument content)
{
  File file = LittleFS.open(path, FILE_WRITE);

  serializeJson(content, file);

  file.close();
}