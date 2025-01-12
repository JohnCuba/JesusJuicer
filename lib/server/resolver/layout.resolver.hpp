#include <Arduino.h>

#pragma once

struct layoutResolverArgs {
	String page;
	String content;
};

String layoutResolver(layoutResolverArgs options);