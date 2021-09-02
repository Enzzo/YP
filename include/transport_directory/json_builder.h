#pragma once

#include <sstream>

#include "json.h"

namespace json {
	class Builder {
		Node node_;
		std::vector<std::unique_ptr<Node>> nodes_stack_;

	public:
		Builder& StartDict();
		Builder& EndDict();
		Builder& StartArray();
		Builder& EndArray();
		Builder& Key(const std::string&);
		Builder& Value(const Node&);
		Node Build();

	private:
		Builder& AddNode(const Node&);
		bool UnableAdd() const;
		bool MakingNode() const;
		bool UnableUseValue() const;
		bool UnableUseStartDict() const;
		bool UnableUseEndDict() const;
		bool UnableUseBuild() const;		
	};
}