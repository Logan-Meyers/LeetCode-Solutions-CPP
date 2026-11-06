#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

struct Node {
    int val;
    std::unique_ptr<Node> next;
    explicit Node(int v) : val(v), next(nullptr) {}
};

class LinkedList {
public:
    LinkedList() = default;

    // TODO: rule of five. Copying must be disallowed; moving must work.
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;
    LinkedList(LinkedList&& other) noexcept;
    LinkedList& operator=(LinkedList&& other) noexcept;
    ~LinkedList();

    // Adds a new node holding val at the front. O(1).
    void pushFront(int val);

    // Removes the front node and return its value.
    // Throws std::out_of_range if the list is empty. O(1).
    int popFront();

    // Returns the front value without removing it.
    // Throws std::out_of_range if the list is empty.
    int front() const;

    bool empty() const;
    std::size_t size() const;

    // STRETCH: reverse the list in place, O(n) time, O(1) extra space.
    // No new nodes allowed; just re-link the existing ones with std::move.
    void reverse();

private:
    std::unique_ptr<Node> head_;
    std::size_t size_ = 0;

    void clear() noexcept;
};

// ---- stub bodies: replace these with real implementations ----
LinkedList::LinkedList(LinkedList&& other) noexcept : head_(std::move(other.head_)), size_(other.size_) {
    other.size_ = 0;
}
LinkedList& LinkedList::operator=(LinkedList&& other) noexcept {
    if (this != &other) {
        // clear current list
        this->clear();

        // take other's head and size
        this->head_ = std::move(other.head_);
        this->size_ = other.size_;

        // reset other's size
        other.size_ = 0;
    }

    return *this;
}
LinkedList::~LinkedList() {
    this->clear();
}
void LinkedList::pushFront(int val) {
    std::unique_ptr<Node> newPtr = std::make_unique<Node>(val);  // create node
    newPtr->next = std::move(this->head_);  // move head pointer to new node's next
    this->head_ = std::move(newPtr);  // move new node's next to head pointer
    this->size_++;
}
int LinkedList::popFront() {
    // if empty list, throw
    if (this->head_ == nullptr) {
        throw std::out_of_range("List was empty when you tried to pop front!");
    }
    
    std::unique_ptr<Node> front = std::move(this->head_);  // move head to temp front node
    this->head_ = std::move(front->next);  // move front node's next to head
    this->size_--;  // decrement size
    return front->val;  // return value ... front is automatically deallocated
}
int LinkedList::front() const {
    // if empty list, throw
    if (this->head_ == nullptr) {
        throw std::out_of_range("List was empty when you tried to access front!");
    }

    return this->head_->val;
}
bool LinkedList::empty() const {
    return this->head_ == nullptr;
}
std::size_t LinkedList::size() const {
    return this->size_;
}
void LinkedList::reverse() {}

void LinkedList::clear() noexcept {
    while (this->head_) this->head_ = std::move(this->head_->next);
    this->size_ = 0;
}

// ---------------- tests (don't edit) ----------------
template <typename F>
bool throwsOutOfRange(F f) {
    try { f(); } catch (const std::out_of_range&) { return true; } catch (...) { return false; }
    return false;
}

int main() {
    static_assert(!std::is_copy_constructible<LinkedList>::value, "list must not be copyable");
    static_assert(!std::is_copy_assignable<LinkedList>::value, "list must not be copy-assignable");
    static_assert(std::is_nothrow_move_constructible<LinkedList>::value, "move ctor should be noexcept");
    static_assert(std::is_nothrow_move_assignable<LinkedList>::value, "move assign should be noexcept");

    // 1. new list is empty
    {
        LinkedList l;
        assert(l.empty());
        assert(l.size() == 0);
        std::cout << "1 ok: new list is empty\n";
    }

    // 2. push/pop order is LIFO and size tracks
    {
        LinkedList l;
        l.pushFront(1); l.pushFront(2); l.pushFront(3);
        assert(!l.empty());
        assert(l.size() == 3);
        assert(l.front() == 3);
        assert(l.popFront() == 3);
        assert(l.size() == 2);
        assert(l.popFront() == 2);
        assert(l.popFront() == 1);
        assert(l.empty());
        assert(l.size() == 0);
        std::cout << "2 ok: push/pop order and size\n";
    }

    // 3. empty list throws
    {
        LinkedList l;
        assert(throwsOutOfRange([&] { l.popFront(); }));
        assert(throwsOutOfRange([&] { l.front(); }));
        l.pushFront(7);
        l.popFront();
        assert(throwsOutOfRange([&] { l.popFront(); }));
        std::cout << "3 ok: empty list throws std::out_of_range\n";
    }

    // 4. move construction transfers ownership and leaves source empty
    {
        LinkedList a;
        a.pushFront(1); a.pushFront(2); a.pushFront(3);
        LinkedList b(std::move(a));
        assert(b.size() == 3);
        assert(b.front() == 3);
        assert(a.empty());
        assert(a.size() == 0);   // watch out: a defaulted move copies size_ and forgets to zero it
        a.pushFront(9);          // moved-from list must still be usable
        assert(a.front() == 9 && a.size() == 1);
        std::cout << "4 ok: move constructor\n";
    }

    // 5. move assignment frees the old contents and handles self-move
    {
        LinkedList a, b;
        a.pushFront(1); a.pushFront(2);
        b.pushFront(10); b.pushFront(20); b.pushFront(30);
        b = std::move(a);
        assert(b.size() == 2 && b.front() == 2);
        assert(a.empty() && a.size() == 0);
        LinkedList& self = b;
        b = std::move(self);     // self-move must not destroy the list
        assert(b.size() == 2 && b.front() == 2);
        std::cout << "5 ok: move assignment\n";
    }

    // 6. a very long list must be destroyed without blowing the stack
    {
        LinkedList big;
        for (int i = 0; i < 1000000; ++i) big.pushFront(i);
        assert(big.size() == 1000000);
        std::cout << "6 ok: 1,000,000 nodes built; destroying now...\n";
    }
    std::cout << "6 ok: long list destroyed without a stack overflow\n";

#ifdef TEST_REVERSE
    // 7. STRETCH: reverse
    {
        LinkedList l;
        l.reverse();                         // empty is fine
        assert(l.empty());
        l.pushFront(1);
        l.reverse();                         // single element is fine
        assert(l.size() == 1 && l.front() == 1);
        l.popFront();
        l.pushFront(1); l.pushFront(2); l.pushFront(3);   // list is 3,2,1
        l.reverse();                                       // now 1,2,3
        assert(l.size() == 3);
        assert(l.popFront() == 1);
        assert(l.popFront() == 2);
        assert(l.popFront() == 3);
        std::cout << "7 ok: reverse\n";
    }
#endif

    std::cout << "ALL TESTS PASSED\n";
    return 0;
}