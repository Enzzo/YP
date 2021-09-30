#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>
#include <memory>

//-------------------------RawMemory-------------------------
template<typename T>
class RawMemory {
	T* buffer_ = nullptr;
	size_t capacity_ = 0;

public:
	RawMemory(const RawMemory&) = delete;
	RawMemory& operator=(const RawMemory&) = delete;

	RawMemory() = default;

	explicit RawMemory(size_t capacity)
		: buffer_(Allocate(capacity))		
		, capacity_(capacity){}

	RawMemory(RawMemory&& other) noexcept {
		capacity_ = other.capacity_;
		buffer_ = std::move(other.buffer_);		
	}

	RawMemory& operator=(RawMemory&& rhs) noexcept {
		capacity_ = std::move(rhs.capacity_);
		buffer_ = std::move(rhs.buffer_);
	}

	~RawMemory() {
		Deallocate(buffer_);
	}

	T* operator+(size_t offset) noexcept {
		//Разрешается получать адрес ячейки памяти, следующей за последним элементом массива
		assert(offset <= capacity_);
		return buffer_ + offset;
	}

	const T* operator+(size_t offset) const noexcept {
		return const_cast<RawMemory&>(*this) + offset;
	}

	const T& operator[](size_t index) const noexcept {
		return const_cast<RawMemory&>(*this)[index];
	}

	T& operator[](size_t index) noexcept {
		assert(index < capacity_);
		return buffer_[index];
	}

	void Swap(RawMemory& other) noexcept {
		std::swap(buffer_, other.buffer_);
		std::swap(capacity_, other.capacity_);
	}

	const T* GetAddress() const noexcept {
		return buffer_;
	}

	T* GetAddress() noexcept {
		return buffer_;
	}

	size_t Capacity() const {
		return capacity_;
	}

private:
	//Выделяет сырую память под n элементов и возвращает указатель на неё
	static T* Allocate(size_t n) {
		return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
	}

	//Освобождает сырую память, выделенную ранее по адресу buf при помощи Allocate
	static void Deallocate(T* buf) noexcept {
		operator delete(buf);
	}
};

//-------------------------Vector-------------------------
template <typename T>
class Vector {
	RawMemory<T> data_;
	size_t size_ = 0;

public:
	Vector() = default;

	explicit Vector(size_t size)
		: data_(size)
		, size_(size) //
	{
		std::uninitialized_value_construct_n(data_.GetAddress(), size_);
	}

	Vector(const Vector& other)
		: data_(other.size_)
		, size_(other.size_) //
	{		
		std::uninitialized_copy_n(other.data_.GetAddress(), size_, data_.GetAddress());
	}

	Vector(Vector&& other) {
		size_ = std::move(other.size_);
		data_(std::move(other.data_));
	}

	Vector& operator=(const Vector& rhs) {
		if (this != &rhs) {
			if (rhs.size_ > data_.Capacity()) {
				//copy-and-swap
			}
			else {
				//скопировать элементы из rhs, создав при необходимости новые
				//или удалив существующие
			}
		}
		return *this;
	}

	Vector& operator=(Vector&& rhs) {
		return *this;
	}

	//------------------------------dtor------------------------------
	~Vector() {
		std::destroy_n(data_.GetAddress(), size_);
	}

	//------------------------------Reserve------------------------------
	void Reserve(size_t new_capacity) {

		if (new_capacity <= data_.Capacity()) {
			return;
		}

		RawMemory<T> new_data(new_capacity);

		if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
			std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
		}
		else {
			std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
		}

		std::destroy_n(data_.GetAddress(), size_);
		data_.Swap(new_data);
	}

	size_t Size() const noexcept {
		return size_;
	}

	size_t Capacity() const noexcept {
		return data_.Capacity();
	}

	const T& operator[](size_t index) const noexcept {
		return const_cast<Vector&>(*this)[index];
	}

	T& operator[](size_t index) noexcept {
		assert(index < size_);
		return data_[index];
	}

private:
	//Вызывает деструкторы n объектов массива по адресу buf
	static void DestroyN(T* buf, size_t n) noexcept {
		for (size_t i = 0; i != n; ++i) {
			Destroy(buf + i);
		}
	}

	//Создаёт копию объекта elem в сырой памяти по адресу buf
	static void CopyConstruct(T* buf, const T& elem) {
		new (buf) T(elem);
	}

	//Вызывает деструктор объекта по адресу buf
	static void Destroy(T* buf) noexcept {
		buf->~T();
	}

	void Swap(Vector& other) noexcept {
		std::swap(size_, other.size_);
		std::swap(data_, other.data_);
	}
};