#include <iostream>
#include <fstream>
#include <string>

#include "json.h"
#include "json_reader.h"
#include "map_renderer.h"

int main() {
	tc::TransportCatalogue base;	
	MapRenderer renderer;
	RequestHandler request(base, renderer);
	JSONreader reader(base, renderer, request);

	std::ifstream in("C:\\input05.txt");
	std::ofstream o("out.txt");
	//const json::Document doc = json::Load(std::cin);
	const json::Document doc = json::Load(in);
	const auto& mode = doc.GetRoot().AsDict();
	
	if (mode.count("base_requests") || mode.count("render_settings")) {
		reader.ReadRequest(doc);
	}
	if (mode.count("stat_requests")) {
		reader.ReadRequests(doc);
		reader.ReadTransportCatalogue(o);

		std::ofstream out("out.svg");
		request.RenderMap(out);
	}

	return 0;
}