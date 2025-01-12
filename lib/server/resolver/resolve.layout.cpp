#include "resolve.layout.hpp"
#include "file_system.module.hpp"

String resolveLayout(resolveLayoutArgs options) {
	FileSystemModule* fs_module = FileSystemModule::GetInstance();
	String layoutMarkup = fs_module->readFile("/public/components/layout.html");

	layoutMarkup.replace("%LAYOUT_PAGE%", options.page);
	layoutMarkup.replace("%CONTENT%", options.content);
	
	return layoutMarkup;
}