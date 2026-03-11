#ifndef MTX_STACK_HPP
#define MTX_STACK_HPP
#include "../utils/noncp_nonmv.hpp"
#include <mutex>
#include <stack>
#define LOCK_GUARD(mtx) std::lock_guard<std::mutex> lock(mtx);
template <typename T>
class MtxStack : protected NonCopyable, protected NonMovable {
public:
  MtxStack() {}

  template <typename U> void push(U &&val) {
    LOCK_GUARD(mutex_)
    stack_.push(std::forward<U>(val));
  }

  bool pop(T &val) {
    LOCK_GUARD(mutex_)
    if (stack_.empty()) {
      return false;
    }
    val = std::move(stack_.top());
    stack_.pop();
    return true;
  }

  std::size_t size() const {
    LOCK_GUARD(mutex_)
    return stack_.size();
  }

  bool empty() const {
    LOCK_GUARD(mutex_)
    return stack_.empty();
  }

private:
  mutable std::mutex mutex_;
  std::stack<T> stack_;
};

#endif // MTX_STACK_HPP