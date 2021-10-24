#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

#include "json.h"
#include "json_reader.h"
#include "map_renderer.h"
#include "transport_router.h"
#include "serialization.h"

using namespace std::literals;

void PrintUsage(std::ostream& stream = std::cerr) {
	stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		PrintUsage();
		return 1;
	}

	const std::string_view mode(argv[1]);

	tc::TransportCatalogue base;
	tr::TransportRouter router(base);
	renderer::MapRenderer renderer;
	Serialization serializator(base, renderer, router);
	RequestHandler request(serializator, base, renderer, router);
	JSONreader reader(serializator, base, renderer, request, router);

	if (mode == "make_base"sv) {
		reader.ReadRequest();
	}
	else if (mode == "process_requests"sv) {
		reader.ReadRequests();
		reader.ReadTransportCatalogue();
	}
	else {
		PrintUsage();
		return 1;
	}
	return 0;
}