class Poolable:
    def set_pool(self, pool):
        self._pool = pool

    def recycle(self):
        self._pool.recycle(self)
