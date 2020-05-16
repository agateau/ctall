import arcade


def _compute_text_size(text):
    rows = 1
    widest_column = 0
    column = 0
    for char in text:
        if char == "\n":
            rows += 1
            widest_column = max(column, widest_column)
            column = 0
        else:
            column += 1
    widest_column = max(column, widest_column)
    return widest_column, rows


class TextDrawer:
    """
    Fast, bitmap-based, fixed-width, text drawer
    """
    LEFT = 1
    HCENTER = 2
    RIGHT = 4
    TOP = 8
    VCENTER = 16
    BOTTOM = 32

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

    def draw(self, text, x, y, align=None):
        if align is None:
            align = TextDrawer.LEFT | TextDrawer.BOTTOM

        text_columns, text_rows = _compute_text_size(text)
        text_width = text_columns * self.char_width
        text_height = text_rows * self.char_height

        if align & TextDrawer.HCENTER:
            x -= text_width / 2
        elif align & TextDrawer.RIGHT:
            x -= text_width
        if align & TextDrawer.VCENTER:
            y -= text_height / 2
        elif align & TextDrawer.TOP:
            y -= text_height

        # Compensate for the fact we draw text from top to bottom
        y += text_height - self.char_height

        # Compensate for the fact that textures origin is the center
        x += self.char_width / 2
        y += self.char_height / 2
        start_x = x
        for char in text:
            if char == "\n":
                y -= self.char_height
                x = start_x
            else:
                if char != " ":
                    idx = self.alphabet.index(char)
                    self.textures[idx].draw_sized(x, y, self.char_width,
                                                  self.char_height)
                x += self.char_width
