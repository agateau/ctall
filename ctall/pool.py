import arcade


class Pool:
    def __init__(self, creator):
        self._creator = creator
        self.sprite_list = arcade.SpriteList()
        self._recycled_items = []

    def get(self):
        if self._recycled_items:
            obj = self._recycled_items.pop()
        else:
            obj = self._creator()
            obj.set_pool(self)
        self.sprite_list.append(obj)
        return obj

    def recycle(self, obj):
        self.sprite_list.remove(obj)
        self._recycled_items.append(obj)
