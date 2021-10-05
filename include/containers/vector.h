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
		Swap(other);
	}

	RawMemory& operator=(RawMemory&& rhs) noexcept {
		Swap(rhs);
		return *this;
	}

	~RawMemory() {
		if(buffer_ != nullptr)
			Deallocate(buffer_);
	}

	T* operator+(size_t offset) noexcept {
		return buffer_ + offset;
	}

	const T* operator+(size_t offset) const noexcept {
		return buffer_ + offset;
	}

	T& operator[](size_t index) noexcept {
		return buffer_[index];
	}

	const T& operator[](size_t index) const noexcept {
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
		assert(buf != nullptr);
		operator delete(buf);
	}
};

//-------------------------Vector-------------------------
template <typename T>
class Vector {
	RawMemory<T> data_;
	size_t size_ = 0;

public:
	using iterator = T*;
	using const_iterator = const T*;

	Vector() = default;

	//------------------------------ctor------------------------------
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

	Vector(Vector&& other) noexcept {
		Swap(other);
	}

	//------------------------------assign------------------------------
	Vector& operator=(const Vector& rhs) {
		if (rhs.size_ > data_.Capacity()) {
			//copy-and-swap
			Vector temp(rhs);
			Swap(temp);
		}
		else {
			for (size_t i = 0; i < std::min(Size(), rhs.Size()); ++i) {
				data_[i] = rhs[i];
			}
			if (Size() > rhs.Size()) {
				std::destroy_n(data_ + rhs.Size(), Size() - rhs.Size());
			}
			else if(Size() < rhs.Size()){ //if Size() <= rhs.Size()
				std::uninitialized_copy_n(rhs.data_.GetAddress() + Size(), rhs.Size() - Size(), data_.GetAddress() + Size());
			}
		}
		size_ = rhs.Size();
		
		return *this;
	}

	Vector& operator=(Vector&& rhs) noexcept{
		Swap(rhs);
		return *this;
	}

	//------------------------------dtor------------------------------
	~Vector() {
		std::destroy_n(data_.GetAddress(), size_);
	};

	//------------------------------iterators------------------------------
	iterator begin() noexcept {
		return data_.GetAddress();
	};

	iterator end() noexcept {
		return data_.GetAddress() + size_;
	};
	
	const_iterator begin() const noexcept {
		return data_.GetAddress();
	};

	const_iterator end() const noexcept {
		return data_.GetAddress() + size_;
	};

	const_iterator cbegin() const noexcept {
		return data_.GetAddress();
	};
	
	const_iterator cend() const noexcept {
		return data_.GetAddress() + size_;
	};

	//------------------------------methods------------------------------
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

	void Resize(size_t new_size) {   
		Reserve(new_size);

		if (new_size < size_) {
			std::destroy_n(data_ + new_size, size_ - new_size);
		}
		else if (new_size > size_) {
			std::uninitialized_default_construct_n(data_ + size_, new_size - size_);
		}
		size_ = new_size;
	}
	
	void PushBack(const T& value) {
		if (size_ == data_.Capacity()) {
			size_t temp_size = (size_ == 0) ? 1 : size_ * 2;

			RawMemory<T> new_data(temp_size);

			new(new_data.GetAddress() + size_) T(value);

			if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
				std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
			}
			else {
				std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
			}

			std::destroy_n(data_.GetAddress(), size_);
			data_.Swap(new_data);
		}
		else {
			new (data_.GetAddress() + size_) T(value);
		}
		++size_;
	}

	void PushBack(T&& value) {
		if (size_ == data_.Capacity()) {
			size_t temp_size = (size_ == 0) ? 1 : size_ * 2;

			RawMemory<T> new_data(temp_size);

			new(new_data.GetAddress() + size_) T(std::move(value));

			if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
				std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
			}
			else {
				std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
			}

			std::destroy_n(data_.GetAddress(), size_);
			data_.Swap(new_data);
		}
		else {
			new (data_.GetAddress() + size_) T(std::move(value));
		}
		++size_;
	}

	void PopBack() noexcept {
		assert(size_ > 0);
		--size_;
		std::destroy_at(data_ + size_);
	}

	template<typename... Args>
	T& EmplaceBack(Args&&... args) {
		if (size_ == data_.Capacity()) {
			size_t temp_size = (size_ == 0) ? 1 : size_ * 2;

			RawMemory<T> new_data(temp_size);

			new(new_data.GetAddress() + size_) T(std::forward<Args>(args)...);

			if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
				std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
			}
			else {
				std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
			}

			std::destroy_n(data_.GetAddress(), size_);
			data_.Swap(new_data);
		}
		else {
			new (data_.GetAddress() + size_) T(std::forward<Args>(args)...);
		}
		++size_;
		return data_[size_ - 1];
	}

	size_t Size() const noexcept {
		return size_;
	}

	size_t Capacity() const noexcept {
		return data_.Capacity();
	}

	void Swap(Vector& other) noexcept {
		data_.Swap(other.data_);
		std::swap(size_, other.size_);		
	}

	iterator Insert(const_iterator pos, const T& value) {
		return Emplace(pos, value);
	}
	
	iterator Insert(const_iterator pos, T&& value) {
		return Emplace(pos, std::move(value));
	}

	template<typename... Args>
	iterator Emplace(const_iterator p, Args&&... args) {

		iterator pos = const_cast<iterator>(p);
		int dist = std::distance(begin(), pos);
		iterator it = &data_[dist];

		if (size_ == Capacity()) {
			//realloc
			size_t temp_size = (size_ == 0) ? 1 : size_ * 2;
			RawMemory<T> new_data(temp_size);

			try {
				new(new_data.GetAddress() + dist) T(std::forward<Args>(args)...);

				if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
					std::uninitialized_move_n(data_.GetAddress(), dist, new_data.GetAddress());
					std::uninitialized_move_n(data_.GetAddress() + dist, size_ - dist, new_data.GetAddress() + (dist + 1));
				}
				else {
					std::uninitialized_copy_n(data_.GetAddress(), dist, new_data.GetAddress());
					std::uninitialized_copy_n(data_.GetAddress() + dist, size_ - dist, new_data.GetAddress() + (dist + 1));
				}
				std::destroy_n(data_.GetAddress(), size_);
				data_.Swap(new_data);
				it = &data_[dist];
				++size_;
			}
			catch (...) {
				new_data.~RawMemory();
			}
		}
		else {

			if (pos == nullptr || pos == end()) {
				new(end()) T(std::forward<Args>(args)...);
			}
			else {
				new(end()) T(std::forward<T>(*(end() - 1)));
				std::move_backward(it, end()-1, end());
				*it = T(std::forward<Args>(args)...);
			}
			++size_;
		}		
		return it;
	}

	iterator Erase(const_iterator p) {
		iterator pos = const_cast<iterator>(p);
		Destroy(pos);
		std::move(pos + 1, end(), pos);
		--size_;
		return pos;
	}

	//------------------------------index------------------------------
	const T& operator[](size_t index) const noexcept {
		return data_[index];
	}

	T& operator[](size_t index) noexcept {
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
};