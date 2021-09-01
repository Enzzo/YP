#pragma once

#include "json.h"

namespace json {
	class Builder {
		Node node_;
		std::string key_;

	public:
		Builder& StartDict();
		Builder& EndDict();
		Builder& StartArray();
		Builder& EndArray();
		Builder& Key(const std::string&);
		Builder& Value(const Node&);
		Node Build();
	};
}