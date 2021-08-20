#include <iostream>
#include <fstream>
#include <string>

#include "json.h"
#include "json_reader.h"
#include "map_renderer.h"

int main() {

	tc::TransportCatalogue base;
	RequestHandler request(base);
	MapRenderer renderer;
	JSONreader reader(base, renderer, request);

	const json::Document doc = json::Load(std::cin);
	const auto& mode = doc.GetRoot().AsMap();
	
	if (mode.count("base_requests") || mode.count("render_settings")) {
		reader.ReadBase(doc);
	}
	if (mode.count("stat_requests")) {
		reader.ReadRequests(doc);
		reader.Answer();
	}

	return 0;
}