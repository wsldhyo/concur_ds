#ifndef LOCKFREE_STACK_HPP
#define LOCKFREE_STACK_HPP
#include <atomic>
template <typename T> class LockFreeStack {
  struct Node {
    T val;
    Node *next;

    template <typename U>
    explicit Node(U &&value) : val(std::forward<U>(value)), next(nullptr) {}
    ~Node() = default;
  };

public:
  template <typename U> void push(U &&val) {
    Node *const new_node{new Node(std::forward<U>(val))};
    new_node->next = head_.load(std::memory_order_acquire);
    while (!head_.compare_exchange_weak(new_node->next, new_node,
                                        std::memory_order_release,
                                        std::memory_order_acquire)) {
    }
  }

  bool try_pop(T &val) {
    Node *old_head{head_.load(std::memory_order_acquire)};
    while (old_head && !head_.compare_exchange_weak(
                           old_head, old_head->next, std::memory_order_acquire,
                           std::memory_order_relaxed)) {
    }
    if (old_head) {
      val = std::move(old_head->val);
      // delete old_head;
      return true;
    }
    return false;
  }

private:
  std::atomic<Node *> head_;
};

#endif // LOCKFREE_STACK_HPP