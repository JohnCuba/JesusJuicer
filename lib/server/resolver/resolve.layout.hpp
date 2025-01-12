#include <Arduino.h>

#pragma once

struct resolveLayoutArgs {
	String page;
	String content;
};

String resolveLayout(resolveLayoutArgs options);