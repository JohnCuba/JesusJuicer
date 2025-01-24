#include "logg.hpp"

// TODO: Disable Serial.print in prod mode

Logg::Logg(String prefix) {
	moduleName = prefix;
};

void Logg::info(String message) {
	// TODO: prod no logs
	Serial.println("[" + moduleName + "]: " + message);
}