#include "Pool.h"

#include <catch2/catch.hpp>

#include <set>

TEST_CASE("Pool") {
    struct Item {

    };

    auto creator = []() -> Item* { return new Item; };

    Pool<Item> pool(creator);

    SECTION("new") {
        Item* item1 = pool.get();
        REQUIRE(item1);
        Item* item2 = pool.get();
        REQUIRE(item2);
        REQUIRE(item1 != item2);
    }

    SECTION("full-recycle") {
        const int N = 7;
        std::set<Item*> initialItems;
        for (int i = 0; i < N; ++i) {
            initialItems.insert(pool.get());
        }

        // Recycle them all
        for(Item* item : initialItems) {
            pool.recycle(item);
        }

        // Get new items
        std::set<Item*> newItems;
        for (int i = 0; i < N; ++i) {
            newItems.insert(pool.get());
        }

        REQUIRE(initialItems == newItems);
    }

    SECTION("half-recycle") {
        Item* item1 = pool.get();
        REQUIRE(item1);
        pool.recycle(item1);

        Item* item2 = pool.get();
        REQUIRE(item2 == item1);

        Item* item3 = pool.get();
        REQUIRE(item3 != item2);
    }
}
