import arcade

SPRITE_SCALING = 1


class Player(arcade.Sprite):
    MOVE_SPEED = 200

    def __init__(self, game):
        super().__init__("./images/player.png", SPRITE_SCALING)
        self.velocity = [0, 0]
        self.game = game

    def update(self, delta):
        keys = self.game.keys

        if keys[arcade.key.LEFT]:
            self.velocity[0] = int(-self.MOVE_SPEED * delta)
        elif keys[arcade.key.RIGHT]:
            self.velocity[0] = int(self.MOVE_SPEED * delta)
        else:
            self.velocity[0] = 0

        if keys[arcade.key.UP]:
            self.velocity[1] = int(self.MOVE_SPEED * delta)
        elif keys[arcade.key.DOWN]:
            self.velocity[1] = int(-self.MOVE_SPEED * delta)
        else:
            self.velocity[1] = 0

        self.center_x += self.velocity[0]
        self.center_y += self.velocity[1]
