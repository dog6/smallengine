#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

// A list that owns its items. T must be a complete type.
template <typename T>
class List {
public:
    using Storage       = std::vector<std::unique_ptr<T>>;
    using const_iterator = typename Storage::const_iterator;

    List() = default;
    ~List() = default;

    // The list can move, but it cannot copy.
    List(const List&) = delete;
    List& operator=(const List&) = delete;
    List(List&&) = default;
    List& operator=(List&&) = default;

    // Takes ownership of item. Do not call delete on it.
    // Returns nullptr if item is null. Returns item if it is already stored.
    T* Add(T* item)
    {
        if (item == nullptr) return nullptr;
        if (Find(item) != items.end()) return item;

        std::unique_ptr<T> owner(item);   // owner deletes item if push_back throws
        items.push_back(std::move(owner));
        return item;
    }

    // Preferred: the type shows that the list takes ownership.
    T* Add(std::unique_ptr<T> item)
    {
        if (!item) return nullptr;
        T* handle = item.get();
        items.push_back(std::move(item));
        return handle;
    }

    // Deletes the item. Returns false if the list does not have it.
    bool Remove(T& item)
    {
        auto it = Find(&item);
        if (it == items.end()) return false;
        items.erase(it);
        return true;
    }

    // Deletes the item at index. Returns false if the index is not valid.
    bool Remove(std::size_t index)
    {
        if (index >= items.size()) return false;
        items.erase(items.begin() + static_cast<std::ptrdiff_t>(index));
        return true;
    }

    // Returns nullptr if the index is not valid.
    T* Get(std::size_t index) const
    {
        return index < items.size() ? items[index].get() : nullptr;
    }

    std::size_t Size() const  { return items.size(); }
    bool        Empty() const { return items.empty(); }
    void        Clear()       { items.clear(); }   // deletes all items

    // Calls fn(T&) for each item.
    template <typename F>
    void ForEach(F&& fn) const
    {
        for (const auto& item : items) {
            if (item) fn(*item);
        }
    }

    // Calls Update() on each item. The compiler checks this only if you call it.
    void Update() const
    {
        for (const auto& item : items) {
            if (item) item->Update();
        }
    }

    const_iterator begin() const { return items.begin(); }
    const_iterator end() const   { return items.end(); }

    // Read-only access. No copy. The list keeps the ownership.
    const Storage& GetAll() const { return items; }

    // Moves all items out. The caller gets the ownership.
    // The list is empty after this call.
    Storage Release()
    {
        Storage out = std::move(items);
        items.clear();   // a moved-from vector is valid, but its state is not defined
        return out;
    }

private:
    typename Storage::iterator Find(const T* item)
    {
        return std::find_if(items.begin(), items.end(),
            [item](const std::unique_ptr<T>& p) { return p.get() == item; });
    }

    Storage items;
};