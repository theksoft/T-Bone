//==============================================================================
// Vector of unique pointer.
// Note: std::set requires key. This implementation has no key.
//==============================================================================

#ifndef TB_PTR_VECTOR_HXX
#define TB_PTR_VECTOR_HXX

#include "tb_locks.hpp"
#include <vector>

namespace tbone {

//==============================================================================

template <typename T>
class PtrVector : public std::vector<T> {

public:
  PtrVector() {
    std::vector<T>::clear();
  }
  virtual ~PtrVector() {
    std::vector<T>::clear();
  }

  typename std::vector<T>::const_iterator find(const T v) const {
    typename std::vector<T>::const_iterator it;
    for (it = std::vector<T>::cbegin(); it != std::vector<T>::cend(); it++) {
      if (*it == v)
        break;
    }
    return it;
  }

  typename std::vector<T>::iterator find(const T v) {
    typename std::vector<T>::iterator it;
    for (it = std::vector<T>::begin(); it != std::vector<T>::end(); it++) {
      if (*it == v)
        break;
    }
    return it;
  }

  bool exist(const T v) const {
    return (std::vector<T>::cend() != find(v));
  }

  bool add(T v) {
    if (v && !exist(v)) {
      std::vector<T>::push_back(v);
      return true;
    }
    return false;
  }

  void remove(T v) {
    auto it = find(v);
    if (std::vector<T>::end() != it) {
      std::vector<T>::erase(it);
    }
  }

};

//==============================================================================

template <typename T>
class GuardedPtrVector : public PtrVector<T> {

public:
  GuardedPtrVector() {}
  virtual ~GuardedPtrVector() {}

  typename std::vector<T>::iterator find(const T v) {
    Locker locker(_guard);
    return PtrVector<T>::find(v);
  }

  bool exist(const T v) {
    Locker locker(_guard);
    return PtrVector<T>::exist(v);
  }

  bool add(T v) {
    Locker locker(_guard);
    return PtrVector<T>::add(v);
  }

  void remove(T v) {
    Locker locker(_guard);
    PtrVector<T>::remove(v);
  }

protected:
  Lock _guard;

};

//==============================================================================

} // namespace tbone

#endif// TB_PTR_VECTOR_HXX

// End of file
