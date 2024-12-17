#include "logg.hpp"

Logg::Logg(String prefix) {
	moduleName = prefix;
};

void Logg::info(String message) {
	Serial.println("[" + moduleName + "]: " + message);
}