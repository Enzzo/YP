#include "json_builder.h"

using namespace std::literals;

namespace json {
	Builder& Builder::StartDict() {
		if (UnableUseStartDict()) {
			throw std::logic_error("StartDict can't be applied"s);
		}
		nodes_stack_.push_back(std::make_unique<Node>(Dict()));
		return *this;
	};

	Builder& Builder::EndDict() {
		if (UnableUseEndDict()) {
			throw std::logic_error("EndDict can't be applied"s);
		}
		return AddNode(*nodes_stack_.back().release());
	};

	Builder& Builder::StartArray() {
		if (UnableUseStartArray()) {
			throw std::logic_error("StartArray can't be applied"s);
		}
		nodes_stack_.push_back(std::make_unique<Node>(Array()));
		return *this;
	};

	Builder& Builder::EndArray() {
		if (UnableUseEndArray()) {
			throw std::logic_error("EndArray can't be applied"s);
		}
		return AddNode(*nodes_stack_.back().release());
	};
	
	Builder& Builder::Key(const std::string& node) {
		if (UnableUseKey()) {
			throw std::logic_error("Key can't be applied"s);
		}
		nodes_stack_.push_back(std::make_unique<Node>(node));
		return *this;
	};
	
	Builder& Builder::Value(const Node& node) {
		nodes_stack_.push_back(std::make_unique<Node>(node));
		node_ = node;
		return *this;
	};
	
	Node Builder::Build() {
		return node_;
	};

	Builder& Builder::AddNode(const Node& node) {
		nodes_stack_.pop_back();
		if (nodes_stack_.empty()) {
			node_ = node;
		}
		else if (nodes_stack_.back()->IsArray()) {
			nodes_stack_.back()->AsArray().push_back(node);
		}
		else {
			const Node& key = *nodes_stack_.back().release();
			nodes_stack_.pop_back();
			nodes_stack_.back()->AsDict().emplace(key.AsString(), node);
		}
		return *this;
	}

	bool Builder::UnableAdd()const {
		if (!nodes_stack_.empty()) {
			return true;
		}
		return (nodes_stack_.back()->IsArray() || nodes_stack_.back()->IsString());
	}
	bool Builder::MakingNode() const {
		return !node_.IsNull();
	}

	bool Builder::UnableUseKey() const {
		if (MakingNode()) {
			return true;
		}
		return nodes_stack_.empty() || !nodes_stack_.back()->IsDict();
	}

	bool Builder::UnableUseValue() const {
		return MakingNode() || UnableAdd();
	}

	bool Builder::UnableUseStartDict() const {
		return UnableUseValue();
	}

	bool Builder::UnableUseEndDict() const {
		return MakingNode() || nodes_stack_.empty() || !nodes_stack_.back()->IsDict();
	}

	bool Builder::UnableUseBuild() const {
		return !MakingNode();
	}

	bool Builder::UnableUseStartArray() const {
		return UnableUseValue();
	}

	bool Builder::UnableUseEndArray() const {
		return MakingNode() || nodes_stack_.empty() || !nodes_stack_.back()->IsArray();
	}
}