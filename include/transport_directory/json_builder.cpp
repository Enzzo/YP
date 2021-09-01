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
	Builder& Builder::Key(const std::string&) {
		return *this;
	};
	Builder& Builder::Value(const Node& node) {
		return *this;
	};
	Node Builder::Build() {

	};
}