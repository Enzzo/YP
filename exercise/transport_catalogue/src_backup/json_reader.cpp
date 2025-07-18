#include "json_reader.h"
#include "json_builder.h"

#include <sstream>

using namespace std::literals;

void JSONreader::ReadRequest(std::istream& ist) {
    const auto load = json::Load(ist).GetRoot().AsDict();
    base_requests_ = load.at("base_requests"s).AsArray();
    serializator_.SetSetting(MakeSerializationSetting(load.at("serialization_settings"s).AsDict()));
    map_renderer_.SetSettings(MakeRenderSettings(load.at("render_settings").AsDict()));
    router_.SetSettings(MakeRouterSettings(load.at("routing_settings"s).AsDict()));

    LoadStops();
    LoadDistances();
    LoadBuses();

    map_renderer_.SetBorder(base_.GetStops());
    map_renderer_.SetTrail(base_.GetBuses());
    map_renderer_.SetStation(base_.GetStops());

    serializator_.CreateBase();
}

void JSONreader::ReadRequests(std::istream& ist) {
    const auto load = json::Load(ist).GetRoot().AsDict();
    stat_requests_ = load.at("stat_requests"s).AsArray();
    serializator_.SetSetting(MakeSerializationSetting(load.at("serialization_settings"s).AsDict()));
    serializator_.AccessBase();
    LoadTransportRouter();
}

void JSONreader::ReadTransportCatalogue(std::ostream& ost) {
    for (const auto& request : stat_requests_) {
        const auto& description = request.AsDict();
        const auto& type = description.at("type"s).AsString();
        if (type == "Stop"s) {
            answers_.push_back(ReadStop(description));
        }
        else if (type == "Bus"s) {
            answers_.push_back(ReadBus(description));
        }
        else if (type == "Map"s) {
            answers_.push_back(ReadMap(description));
        }
        else if (type == "Route"s) {
            answers_.push_back(ReadRoute(description));
        }
    }
    const json::Document answer(answers_);
    Print(answer, ost);
}

void JSONreader::LoadStops() {
    for (const auto& request : base_requests_) {
        const auto& description = request.AsDict();
        if (description.at("type"s).AsString() == "Stop"s) {
            base_.AddStop(MakeStop(description));
        }
    }
}

void JSONreader::LoadBuses() {
    for (const auto& request : base_requests_) {
        const auto& description = request.AsDict();
        if (description.at("type"s).AsString() == "Bus"s) {
            base_.AddBus(MakeBus(description));
        }
    }
}

void JSONreader::LoadDistances() {
    for (const auto& request : base_requests_) {
        const auto& description = request.AsDict();
        if (description.at("type"s).AsString() == "Stop"s) {
            SetDistancesFromStop(description);
        }
    }
}

void JSONreader::LoadTransportRouter() {
    router_.MakeGraph();
}

Stop JSONreader::MakeStop(const json::Dict& description) {
    return { description.at("name"s).AsString(), description.at("latitude"s).AsDouble(), description.at("longitude"s).AsDouble() };
}

void JSONreader::SetDistancesFromStop(const json::Dict& description) {
    const auto from = base_.FindStop(description.at("name"s).AsString());
    for (const auto& [stop_name, distance] : description.at("road_distances"s).AsDict()) {
        const auto to = base_.FindStop(stop_name);
        base_.AddDistanceBetweenStops(from, to, (distance.AsInt()));
    }
}

Bus JSONreader::MakeBus(const json::Dict& description) {
    Bus bus;
    bus.is_ring = description.at("is_roundtrip"s).AsBool();
    bus.number = description.at("name"s).AsString();
    const auto& stops = description.at("stops"s).AsArray();
    for (const auto& stop : stops) {
        const auto& stop_name = stop.AsString();
        bus.stops.push_back(base_.FindStop(stop_name));
    }
    return bus;
}

json::Node JSONreader::ReadStop(const json::Dict& description) {
    json::Builder builder;
    builder.StartDict().Key("request_id"s).Value(description.at("id"s).AsInt());
    const auto buses = request_handler_.GetBusesByStop(description.at("name"s).AsString());

    if (buses) {
        builder.Key("buses"s).StartArray();

        for (const auto& bus : *buses) {
            builder.Value(bus->number);
        }
        builder.EndArray();
    }
    else {
        builder.Key("error_message"s).Value("not found"s);
    }
    return builder.EndDict().Build();
}

json::Node JSONreader::ReadBus(const json::Dict& description) {
    json::Builder builder;
    builder.StartDict().Key("request_id"s).Value(description.at("id"s).AsInt());

    const auto& info = request_handler_.GetBusInfo(description.at("name"s).AsString());
    if (info) {
        builder.Key("curvature"s).Value(info->curvature)
            .Key("route_length"s).Value(static_cast<float>(info->route_length))
            .Key("stop_count"s).Value(static_cast<int>(info->stops))
            .Key("unique_stop_count"s).Value(static_cast<int>(info->unique_stops));
    }
    else {
        builder.Key("error_message"s).Value("not found"s);
    }
    return builder.EndDict().Build();
}

json::Node JSONreader::ReadMap(const json::Dict& description) {
    json::Builder builder;
    builder.StartDict().Key("request_id"s).Value(description.at("id"s).AsInt());

    builder.Key("map"s).Value(request_handler_.RenderMap().description());
    return builder.EndDict().Build();
}

svg::Point JSONreader::SetPoint(const json::Node& node)const {
    using namespace renderer;
    const auto& array = node.AsArray();
    return MakePoint(array.front().AsDouble(),
        array.back().AsDouble());
}

svg::Color JSONreader::SetColor(const json::Node& node)const {
    using namespace renderer;
    if (node.IsArray()) {
        const auto& array = node.AsArray();
        if (array.size() == 3) {
            return MakeColor(array.at(0).AsInt(),
                array.at(1).AsInt(), array.at(2).AsInt());
        }
        else if (array.size() == 4) {
            return MakeColor(array.at(0).AsInt(),
                array.at(1).AsInt(), array.at(2).AsInt(),
                array.at(3).AsDouble());
        }
    }
    return MakeColor(node.AsString());
}

renderer::Settings JSONreader::MakeRenderSettings(const json::Dict& description) const {

    renderer::Settings settings;
    settings.width = description.at("width"s).AsDouble();
    settings.height = description.at("height"s).AsDouble();
    settings.padding = description.at("padding"s).AsDouble();
    settings.line_width = description.at("line_width"s).AsDouble();
    settings.stop_radius = description.at("stop_radius"s).AsDouble();
    settings.bus_label_font_size = description.at("bus_label_font_size"s).AsInt();
    settings.bus_label_offset = SetPoint(description.at("bus_label_offset"s));
    settings.stop_label_font_size = description.at("stop_label_font_size"s).AsInt();
    settings.stop_label_offset = SetPoint(description.at("stop_label_offset"s));
    settings.underlayer_color = SetColor(description.at("underlayer_color"s));
    settings.underlayer_width = description.at("underlayer_width"s).AsDouble();
    const auto& array_color = description.at("color_palette"s).AsArray();
    settings.color_palette.reserve(array_color.size());
    for (const auto& color : array_color) {
        settings.color_palette.push_back(SetColor(color));
    }
    return settings;
}

tr::Settings JSONreader::MakeRouterSettings(const json::Dict& description) const {
    return { description.at("bus_wait_time"s).AsInt(), description.at("bus_velocity"s).AsDouble() };
}

json::Node JSONreader::ReadRoute(const json::Dict& description) {
    using namespace graph;
    json::Builder builder_node;
    builder_node.StartDict()
        .Key("request_id"s).Value(description.at("id"s).AsInt());
    const auto& report = request_handler_.GetReportRouter(description.at("from"s).AsString(),
        description.at("to"s).AsString());
    if (report) {
        builder_node.Key("total_time"s).Value(report->total_minutes)
            .Key("items"s).StartArray();
        for (const auto& info : report->information) {
            builder_node.StartDict()
                .Key("type"s).Value("Wait"s)
                .Key("time"s).Value(info.wait.minutes)
                .Key("stop_name"s).Value(info.wait.stop_name)
                .EndDict()
                .StartDict()
                .Key("type"s).Value("Bus"s)
                .Key("time"s).Value(info.bus.minutes)
                .Key("bus"s).Value(info.bus.number)
                .Key("span_count"s).Value(info.bus.span_count)
                .EndDict();
        }
        builder_node.EndArray();
    }
    else {
        builder_node.Key("error_message"s).Value("not found"s);
    }
    return builder_node.EndDict().Build();
}

JSONreader::Path JSONreader::MakeSerializationSetting(const json::Dict& description) const {
    return Path(description.at("file"s).AsString());
}