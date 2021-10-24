#include "request_handler.h"

RequestHandler::RequestHandler(const Serialization& serializator, const tc::TransportCatalogue& base, const renderer::MapRenderer& map_renderer, const tr::TransportRouter& router)
	: serializator_(serializator)
	, base_(base)
	, map_renderer_(map_renderer)
	, router_(router) {}

std::optional<BusInfo> RequestHandler::GetBusInfo(const std::string_view number) const {
	return base_.GetBusInfo(number);
}

std::shared_ptr<Buses> RequestHandler::GetBusesByStop(const std::string_view stop_name) const {
	return base_.GetBusesByStop(stop_name);
}

proto_tc::Map RequestHandler::RenderMap() const {
	return serializator_.LoadMap();
}

std::optional<tr::ReportRouter> RequestHandler::GetReportRouter(const std::string_view from, const std::string_view to) const {
	return router_.GetReportRouter(from, to);
}