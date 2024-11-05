#include "credentials.h"

APCredentials::APCredentials(String _ssid, String _password) :
	ssid{_ssid},
	password{_password}
	{};

bool APCredentials::isEmpty() {
	return ssid == "" || password == "";
};
