#include "wifi_list_entity.resolver.hpp"

String wifiListEntityResolver(String markupTemplate, String title, String target) {
	String markup = String(markupTemplate);

	markup.replace("%TITLE%", title);
	markup.replace("%ID%", target);
	markup.replace("%TARGET%", target);

	return markup;
}