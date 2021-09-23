#pragma once
#include <optional>
#include <memory>
#include <set>

#include "transport_catalogue.h"
#include "transport_router.h"
#include "map_renderer.h"

class RequestHandler {
	const tc::TransportCatalogue& base_;
	const renderer::MapRenderer& map_renderer_;
	const TransportRouter& router_;

public:
	RequestHandler(const tc::TransportCatalogue&, const renderer::MapRenderer&, const TransportRouter&);
	std::optional<BusInfo> GetBusInfo(const std::string_view) const;
	std::shared_ptr<Buses> GetBusesByStop(const std::string_view) const;
	void RenderMap(std::ostream& out = std::cout)const;
	std::optional<ReportRouter> GetReportRouter(const std::string_view from, const std::string_view to) const;
};