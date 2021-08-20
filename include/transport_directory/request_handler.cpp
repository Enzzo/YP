#include "request_handler.h"

RequestHandler::RequestHandler(const tc::TransportCatalogue& base) : base_(base) {}
std::optional<BusInfo> RequestHandler::GetBusInfo(const std::string_view number) const {
	return base_.GetBusInfo(number);
}

std::shared_ptr<Buses> RequestHandler::GetBusesByStop(const std::string_view stop_name) const {
	return base_.GetBusesByStop(stop_name);
}