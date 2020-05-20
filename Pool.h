#ifndef POOL_H
#define POOL_H

#include <algorithm>
#include <cassert>
#include <functional>
#include <set>

#include <iostream>

// FIXME: leak: items are never deleted
template<class T>
class Pool {
public:
    using Creator = std::function<T*(void)>;

    Pool(const Creator& creator) : mCreator(creator) {
    }

    T* get();

    void recycle(T* item);

    std::set<T*> getActiveItems() const {
        return mActiveItems;
    }

private:
    Creator mCreator;
    std::set<T*> mActiveItems;
    std::set<T*> mRecycledItems;
};

template<class T>
T* Pool<T>::get() {
    T* item = nullptr;
    if (mRecycledItems.empty()) {
        item = mCreator();
    } else {
        auto it = mRecycledItems.begin();
        item = *it;
        mRecycledItems.erase(it);
    }
    mActiveItems.insert(item);
    return item;
}

template<class T>
void Pool<T>::recycle(T* item) {
    auto it = std::find(mActiveItems.begin(), mActiveItems.end(), item);
    assert(it != mActiveItems.end());
    mActiveItems.erase(it);
    mRecycledItems.insert(item);
}

#endif /* POOL_H */
