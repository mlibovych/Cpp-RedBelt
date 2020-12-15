#pragma once

#include <cstdlib>
#include <algorithm>

using namespace std;

// Ð ÐµÐ°Ð»Ð¸Ð·ÑƒÐ¹Ñ‚Ðµ ÑˆÐ°Ð±Ð»Ð¾Ð½ SimpleVector
template <typename T>
class SimpleVector {
public:
  	SimpleVector() = default;
	explicit SimpleVector(size_t size) : data(new T[size]),
										 size_(size),
										 capacity_(size)
	{
	}
	SimpleVector(const SimpleVector<T>& other) : data(new T[other.capacity_]),
										 size_(other.size_),
										 capacity_(other.capacity_)
	{
		copy(other.begin(), other.end(), begin());
	}
	SimpleVector(SimpleVector<T>&& other) : data(other.data),
										 size_(other.size_),
										 capacity_(other.capacity_)
	{
		other.data = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;
	}
	~SimpleVector() {
			delete[] data;
	}

	void operator=(const SimpleVector& other) {
		// delete[] data;
		// data = new T[other.capacity_];
		// size_ = other.size_;
		// capacity_ = other.capacity_;
		// copy(other.begin(), other.end(), begin());

		if (other.size_ <= capacity_) {
			copy(other.begin(), other.end(), begin());
			size_ = other.size_;
    	}
		else {
			SimpleVector<T> tmp(other);
			swap(tmp.data, data);
			swap(tmp.size_, size_);
			swap(tmp.capacity_, capacity_);
		}
	}

	void operator=(SimpleVector&& other) {
		delete[] data;
		data = other.data;
		size_ = other.size_;
		capacity_ = other.capacity_;

		other.data = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;
	}

	T& operator[](size_t index);

	const T* begin() const ;
	const T* end() const;
	T* begin();
	T* end();

	size_t Size() const;
	size_t Capacity() const;
	void PushBack(const T& value);

private:
  	T* data = nullptr;
	size_t size_ = 0;
	size_t capacity_ = 0;
};

template <typename T>
T& SimpleVector<T>:: operator[](size_t index) {
    return data[index]; //*(data + index)
}

template <typename T>
T* SimpleVector<T>:: begin() {
	return data;
}

template <typename T>
T* SimpleVector<T>:: end() {
	return data + size_;
}

template <typename T>
const T* SimpleVector<T>:: begin() const{
	return data;
}

template <typename T>
const T* SimpleVector<T>:: end() const{
	return data + size_;
}

template <typename T>
size_t SimpleVector<T>:: Size() const {
    return size_;
}

template <typename T>
size_t SimpleVector<T>:: Capacity() const {
    return capacity_;
}

template <typename T>
void SimpleVector<T>:: PushBack(const T& value) {
    if (size_ == capacity_) {
		if (!capacity_) {
			++capacity_;
		}
		capacity_ *= 2;
		T* tmp = new T[capacity_];
		std::copy(begin(), end(), tmp);
		delete[] data;
		data = tmp;
	}
	data[size_++] = value;
}