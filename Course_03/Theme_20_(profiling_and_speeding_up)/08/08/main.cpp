#include "log_duration.h"
#include "search_server.h"

#include <iostream>

using namespace std::literals;

int main() {
	
	SearchServer server("fj asdf dsf fs"s);
	server.AddDocument(1, "dsf sdf fs"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
	server.FindTopDocuments("fs"s);

	return 0;
}