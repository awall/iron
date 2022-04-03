#pragma once
#include <memory>

template <class T>
class PoolAllocator {
private:

  T* _block;
  T* _cursor;
  std::size_t _offset;

public:

  typedef T value_type;

  PoolAllocator(std::size_t n) noexcept {
    _block = new T[n];
    _cursor = _block;
  }

  T* allocate(std::size_t n) {
    T* before = _cursor;
    _cursor += n;
    return before;
  }

  void deallocate (T* p, std::size_t n) { }

  void rewind(std::size_t n) {
      _cursor -= n;
  }
};