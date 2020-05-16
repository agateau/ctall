import arcade


class TextDrawer:
    """
    Fast, bitmap-based, fixed-width, text drawer
    """
    def __init__(self, font_image_path, alphabet, char_width, char_height,
                 columns=None):
        self.alphabet = alphabet
        self.char_width = char_width
        self.char_height = char_height
        count = len(alphabet)
        if columns is None:
            columns = count
        self.textures = arcade.load_spritesheet(font_image_path, char_width,
                                                char_height, columns, count)

    def draw(self, text, x, y):
        x += self.char_width / 2
        y += self.char_height / 2
        for char in text:
            if char != " ":
                idx = self.alphabet.index(char)
                self.textures[idx].draw_sized(x, y, self.char_width,
                                              self.char_height)
            x += self.char_width
