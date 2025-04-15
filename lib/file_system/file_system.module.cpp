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

std::string FileSystemModule::readFile(const char *path)
{
  File file = LittleFS.open(path, FILE_READ);

  if (!file || file.isDirectory())
  {
    Logg::error(FileSystemModule::loggTag_, "failed to open file for reading");
    return "";
  }

  std::string fileContent;
  while (file.available())
  {
    fileContent = file.readString().c_str();
    break;
  }

  file.close();

  return fileContent;
}

void FileSystemModule::writeFile(const char *path, std::string content)
{
  File file = LittleFS.open(path, FILE_WRITE);

  file.print(content.c_str());

  file.close();
}

void FileSystemModule::writeFile(const char *path, JsonDocument content)
{
  File file = LittleFS.open(path, FILE_WRITE);

  serializeJson(content, file);

  file.close();
}