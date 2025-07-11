#pragma once
#include <optional>
#include <memory>
#include <set>

#include "transport_router.h"
#include "transport_catalogue.h"
#include "map_renderer.h"
#include "request_handler.h"
#include "serialization.h"

class RequestHandler {
	const Serialization& serializator_;
	const tc::TransportCatalogue& base_;
	const renderer::MapRenderer& map_renderer_;
	const tr::TransportRouter& router_;

public:
	RequestHandler(	const Serialization& serializator, 
					const tc::TransportCatalogue& base, 
					const renderer::MapRenderer& map_renderer, 
					const tr::TransportRouter& router);

	std::optional<BusInfo> GetBusInfo(const std::string_view) const;
	std::shared_ptr<Buses> GetBusesByStop(const std::string_view) const;
	proto_tc::Map RenderMap()const;
	std::optional<tr::ReportRouter> GetReportRouter(const std::string_view, const std::string_view)const;
};