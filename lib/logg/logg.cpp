#include "logg.hpp"

Logg::Logg(String prefix) {
	moduleName = prefix;
};

void Logg::info(String message) {
	// TODO: prod no logs
	Serial.println("[" + moduleName + "]: " + message);
}