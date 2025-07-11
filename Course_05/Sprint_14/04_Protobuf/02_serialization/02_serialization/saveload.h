#include <iostream>
#include <map>
#include <string>
#include <vector>

// Serialization

template <typename T>
void Serialize(T val, std::ostream& out);

inline void Serialize(const std::string& str, std::ostream& out);

template <typename T>
void Serialize(const std::vector<T>& val, std::ostream& out);

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& val, std::ostream& out);

// Deserialization

template <typename T>
void Deserialize(std::istream& in, T& val);

inline void Deserialize(std::istream& in, std::string& str);

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& val);

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& val);

// ��������� ������� ���������, �� ���������� ����� ������ ����� � h-�����.
// �� �������� �������� ����������� ������� ��� inline.

template<typename T>
void Serialize(T val, std::ostream& out) {
	out.write(reinterpret_cast<const char*>(&val), sizeof(val));
}

inline void Serialize(const std::string& str, std::ostream& out) {
	size_t s = str.size();
	out.write(reinterpret_cast<const char*>(&s), s);
	for (const char ch : str) {
		Serialize(ch, out);
	}
}

template <typename T>
void Serialize(const std::vector<T>& val, std::ostream& out) {
	size_t s = val.size();
	out.write(reinterpret_cast<const char*>(&s), s);
	for (const T v : val) {
		Serialize(v, out);
	}
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& val, std::ostream& out) {
	size_t s = val.size();
	out.write(reinterpret_cast<const char*>(&s), s);
	for (const auto [key, value] : val) {
		Serialize(key, out);
		Serialize(value, out);
	}
}

// Deserialization

template <typename T>
void Deserialize(std::istream& in, T& val) {
	in.read(reinterpret_cast<char*>(&val), sizeof(val));
}

inline void Deserialize(std::istream& in, std::string& str) {
	size_t size = 0;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (size_t i = 0; i < size; ++i) {
		char ch;
		Deserialize(in, ch);
		str += ch;
	}
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& val) {
	size_t size = 0;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (size_t i = 0; i < size; ++i) {
		T v;
		Deserialize(in, v);
		val.push_back(v);
	}
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& val) {
	size_t size = 0;
	in.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (size_t i = 0; i < size; ++i) {
		T1 key;
		T2 value;

		Deserialize(in, key);
		Deserialize(in, value);
		val.emplace(key, value);
	}
}