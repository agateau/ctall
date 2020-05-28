#ifndef POOL_H
#define POOL_H

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

template<class T>
class Pool {
public:
    using Creator = std::function<T*(void)>;
    using Container = std::vector<T*>;

    Pool(const Creator& creator) : mCreator(creator) {
    }

    ~Pool() {
        for(T* item : mItems) {
            delete item;
        }
    }

    T* get();

    void recycle(T* item);

private:
    Creator mCreator;
    Container mItems;
    std::size_t mFirstFreeIdx = 0;
};

template<class T>
T* Pool<T>::get() {
    T* item = nullptr;
    if (mFirstFreeIdx == mItems.size()) {
        item = mCreator();
        mItems.push_back(item);
    } else {
        item = mItems.at(mFirstFreeIdx);
    }
    ++mFirstFreeIdx;
    return item;
}

template<class T>
void Pool<T>::recycle(T* item) {
    auto freeItemIt = mItems.begin() + mFirstFreeIdx;
    auto it = std::find(mItems.begin(), freeItemIt, item);
    assert(it != freeItemIt);
    auto lastUsedIt = freeItemIt - 1;
    if (it != lastUsedIt) {
        std::swap(*it, *lastUsedIt);
    }
    --mFirstFreeIdx;
}

#endif /* POOL_H */
