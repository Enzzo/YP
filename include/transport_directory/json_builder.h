#pragma once

#include <sstream>

#include "json.h"

namespace json {
	class Builder {
		Node node_;
		Document document_;

	public:
		Builder& StartDict();
		Builder& EndDict();
		Builder& StartArray();
		Builder& EndArray();
		Builder& Key(const Node&);
		Builder& Value(const Node&);

		//создаёт документ
		Document Build();
	};
}