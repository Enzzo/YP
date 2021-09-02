#include "json_builder.h"

namespace json {
	Builder& Builder::StartDict() {
		
		return *this;
	};
	Builder& Builder::EndDict() {
		return *this;
	};
	Builder& Builder::StartArray() {
		return *this;
	};
	Builder& Builder::EndArray() {
		return *this;
	};
	Builder& Builder::Key(const Node& node) {
		return *this;
	};
	Builder& Builder::Value(const Node& node) {
		ist.
		return *this;
	};
	Document Builder::Build() {
		return document_;
	};
}