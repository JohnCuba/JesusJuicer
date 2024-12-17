#include "LittleFS.h"
#include "file_system.module.hpp"

FileSystemModule* FileSystemModule::pinstance_{nullptr};

FileSystemModule *FileSystemModule::GetInstance() {
	if (pinstance_ == nullptr) {
		pinstance_ = new FileSystemModule();
	}

	return pinstance_;
}

void FileSystemModule::onSetup() {
  if (!LittleFS.begin(true)) {
    logg.info("An error has occurred while mounting LittleFS");
  }
  logg.info("mounted successfully");
}

String FileSystemModule::readFile(String path) {
  File file = LittleFS.open(path, FILE_READ);

  if(!file || file.isDirectory()){
    logg.info("failed to open file for reading");
    return String();
  }

  String fileContent;
  while(file.available()){
    fileContent = file.readString();
    break;     
  }

  file.close();

  return fileContent;
}

void FileSystemModule::writeFile(String path, String content) {
  File file = LittleFS.open(path, FILE_WRITE);

  file.print(content);

  file.close();
}

void FileSystemModule::writeFile(String path, JsonDocument content) {
  File file = LittleFS.open(path, FILE_WRITE);

  serializeJson(content, file);

  file.close();
}