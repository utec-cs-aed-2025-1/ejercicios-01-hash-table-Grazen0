#include <vector>

using namespace std;

const int maxColision = 3;
const float maxFillFactor = 0.8;

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

template <typename T> class LinkedList {
    struct Node {
        T data;
        Node *next = nullptr;

        Node(T data) : data(std::move(data))
        {
        }

        Node(T data, Node *const next) : data(std::move(data)), next(next)
        {
        }
    };

    class iterator {
        Node **cur;

        friend class LinkedList<T>;

      public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        iterator(Node **const head) : cur(head)
        {
        }

        iterator &operator++()
        {
            cur = &(*cur)->next;

            if (*cur == nullptr)
                cur = nullptr;

            return *this;
        }

        iterator operator++(int)
        {
            iterator retval = *this;
            ++(*this);
            return retval;
        }

        bool operator==(const iterator &other) const
        {
            return this->cur == other.cur;
        }

        bool operator!=(const iterator &other) const
        {
            return !(*this == other);
        }

        value_type &operator*()
        {
            return (*cur)->data;
        }
    };

    Node *m_head = nullptr;
    size_t m_size = 0;

  public:
    LinkedList() = default;

    LinkedList(const LinkedList<T> &other) : m_size(other.m_size)
    {
        Node **this_cur = &m_head;
        Node *other_cur = other.m_head;

        while (other_cur != nullptr) {
            *this_cur = new Node(other_cur->data);
            this_cur = &(*this_cur)->next;
            other_cur = other_cur->next;
        }
    }

    LinkedList(LinkedList<T> &&other) noexcept
    {
        std::swap(m_head, other.m_head);
        std::swap(m_size, other.m_size);
    }

    ~LinkedList()
    {
        clear();
    }

    LinkedList<T> &operator=(const LinkedList<T> &other) = delete;

    LinkedList<T> &operator=(LinkedList<T> &&other) noexcept = delete;

    [[nodiscard]] T &front()
    {
        if (m_head == nullptr)
            throw std::runtime_error("list is empty");

        return m_head->data;
    }

    [[nodiscard]] const T &front() const
    {
        if (m_head == nullptr)
            throw std::runtime_error("list is empty");

        return m_head->data;
    }

    [[nodiscard]] T &back()
    {
        if (m_head == nullptr)
            throw std::runtime_error("list is empty");

        Node *cur = m_head;

        while (cur->next != nullptr)
            cur = cur->next;

        return cur->data;
    }

    [[nodiscard]] const T &back() const
    {
        if (m_head == nullptr)
            throw std::runtime_error("list is empty");

        Node *cur = m_head;

        while (cur->next != nullptr)
            cur = cur->next;

        return cur->data;
    }

    void push_front(T data)
    {
        m_head = new Node(std::move(data), m_head);
        ++m_size;
    }

    void push_back(T data)
    {
        Node **cur = &m_head;

        while (*cur != nullptr)
            cur = &(*cur)->next;

        *cur = new Node(std::move(data));
        ++m_size;
    }

    T pop_front()
    {
        if (m_head == nullptr)
            throw std::runtime_error("list is empty");

        const T data = std::move(m_head->data);

        Node *const new_head = m_head->next;
        delete std::exchange(m_head, new_head);
        --m_size;
        return data;
    }

    T pop_back()
    {
        if (m_head == nullptr)
            throw std::runtime_error("list is empty");

        Node **cur = &m_head;

        while ((*cur)->next != nullptr)
            cur = &(*cur)->next;

        const T data = std::move((*cur)->data);
        delete std::exchange(*cur, nullptr);
        --m_size;
        return data;
    }

    void remove(const iterator &it)
    {
        delete std::exchange(*it.cur, (*it.cur)->next);
        --m_size;
    }

    [[nodiscard]] const T &operator[](const size_t index) const
    {
        if (index >= m_size)
            throw std::out_of_range("list index out of bounds");

        Node *cur = m_head;

        for (size_t i = 0; i < index; ++i)
            cur = cur->next;

        return cur->data;
    }

    [[nodiscard]] T &operator[](const size_t index)
    {
        if (index >= m_size)
            throw std::out_of_range("list index out of bounds");

        Node *cur = m_head;

        for (size_t i = 0; i < index; ++i)
            cur = cur->next;

        return cur->data;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return m_size == 0;
    }

    [[nodiscard]] size_t size() const noexcept
    {
        return m_size;
    }

    void clear()
    {
        Node *cur = std::exchange(m_head, nullptr);

        while (cur != nullptr)
            delete std::exchange(cur, cur->next);

        m_size = 0;
    }

    void reverse()
    {
        if (m_size <= 1)
            return;

        Node *cur_first = m_head;
        Node *cur_second = m_head->next;

        while (cur_second != nullptr) {
            Node *const next = cur_second->next;
            cur_second->next = cur_first;

            cur_first = cur_second;
            cur_second = next;
        }

        m_head->next = nullptr;
        m_head = cur_first;
    }

    iterator begin()
    {
        return iterator(m_head == nullptr ? nullptr : &m_head);
    }

    iterator end()
    {
        return iterator(nullptr);
    }
};

template <typename K, typename T> class ChainHash {
    struct Entry {
        K key;
        size_t hash;
        T value;

        Entry(K key, const size_t hash, T value)
            : key(std::move(key)), hash(hash), value(std::move(value)) {};
    };

    size_t size = 0;
    size_t used_buckets = 0;
    size_t capacity = 0;
    LinkedList<Entry> *buckets = nullptr;

    static size_t get_hash_code(const K &key)
    {
        return std::hash<K>()(key);
    }

    [[nodiscard]] double fill_factor() const
    {
        return (double)used_buckets / (double)capacity;
    }

    void rehash()
    {
        const size_t new_capacity = capacity * 2;
        auto *const new_buckets = new LinkedList<Entry>[new_capacity]();

        for (size_t i = 0; i < capacity; ++i) {
            for (const Entry &entry : buckets[i]) {
                const size_t new_index = entry.hash % new_capacity;
                new_buckets[new_index].push_front(std::move(entry));
            }
        }

        delete[] std::exchange(buckets, new_buckets);
        capacity = new_capacity;
    }

  public:
    explicit ChainHash(const size_t initial_capacity = 8)
        : capacity(initial_capacity), buckets(new LinkedList<Entry>[capacity]())
    {
    }

    ChainHash(const ChainHash<K, T> &other) = delete;

    ChainHash(ChainHash<K, T> &&other)
    {
        std::swap(size, other.size);
        std::swap(used_buckets, other.used_buckets);
        std::swap(capacity, other.capacity);
        std::swap(buckets, other.buckets);
    }

    ChainHash &operator=(const ChainHash<K, T> &other) = delete;
    ChainHash &operator=(ChainHash<K, T> &&other) = delete;

    ~ChainHash()
    {
        delete[] buckets;
    }

    void set(K key, T value)
    {
        const size_t hash = get_hash_code(key);
        const size_t index = hash % capacity;

        for (auto &entry : buckets[index]) {
            if (entry.key == key) {
                entry.value = std::move(value);
                return;
            }
        }

        buckets[index].push_front(
            Entry(std::move(key), hash, std::move(value)));
        ++size;

        if (buckets[index].size() == 1)
            ++used_buckets;

        if (buckets[index].size() > maxColision ||
            fill_factor() > maxFillFactor)
            rehash();
    }

    T &get(const K &key)
    {
        const size_t hash = get_hash_code(key);
        const size_t index = hash % capacity;

        for (auto &entry : buckets[index]) {
            if (entry.key == key)
                return entry.value;
        }

        throw std::out_of_range("Key not found");
    }

    const T &get(const K &key) const
    {
        const size_t hash = get_hash_code(key);
        const size_t index = hash % capacity;

        for (const auto &entry : buckets[index]) {
            if (entry.key == key)
                return entry.value;
        }

        throw std::out_of_range("Key not found");
    }

    bool remove(const K &key)
    {
        const size_t hash = get_hash_code(key);
        const size_t index = hash % capacity;
        auto &bucket = buckets[index];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if ((*it).key == key) {
                buckets[index].remove(it);
                return true;
            }
        }

        return false;
    }

    bool contains(const K &key) const
    {
        const size_t hash = get_hash_code(key);
        const size_t index = hash % capacity;

        for (auto &el : buckets[index]) {
            if (el.key == key)
                return true;
        }

        return false;
    }

    [[nodiscard]] size_t bucket_count() const
    {
        return capacity;
    }

    [[nodiscard]] size_t bucket_size(size_t index) const
    {
        return buckets[index].size();
    }

    auto begin(const size_t index)
    {
        if (index >= capacity)
            throw std::out_of_range("Bucket index out of bounds");

        return buckets[index].begin();
    }

    auto end(const size_t index)
    {
        if (index >= capacity)
            throw std::out_of_range("Bucket index out of bounds");

        return buckets[index].end();
    }
};
