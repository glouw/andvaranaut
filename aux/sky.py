# This Python2 script generates two Quake-like cloud textures
# (cloud.bmp and cloud2.bmp) using Tileable Perlin Noise.

# cloud.bmp is the back cloud layer and cloud2.bmp
# is the front cloud layer with a transperancy alpha.

import random, copy, math
from PIL import Image, ImageOps

def noise(x, y, per, perm, dirs):

    def surflet(gridX, gridY):
        distX, distY = abs(x - gridX), abs(y - gridY)
        polyX = 1 - 6 * distX ** 5 + 15 * distX ** 4 - 10 * distX ** 3
        polyY = 1 - 6 * distY ** 5 + 15 * distY ** 4 - 10 * distY ** 3
        hashed = perm[perm[int(gridX) % per] + int(gridY) % per]
        grad = (x - gridX) * dirs[hashed][0] + (y - gridY) * dirs[hashed][1]
        return polyX * polyY * grad

    intX, intY = int(x), int(y)

    return (surflet(intX + 0, intY + 0) + surflet(intX + 1, intY + 0) +
            surflet(intX + 0, intY + 1) + surflet(intX + 1, intY + 1))

def fbm(x, y, per, octs, perm, dirs):

    val = 0
    for o in range(octs):
        val += 0.5 ** o * noise(x * 2 ** o, y * 2 ** o, per * 2 ** o, perm, dirs)
    return val

def gen(size, perm, dirs):

    freq = 1 / 32.0
    octs = 5
    data = []
    for y in range(size):
        for x in range(size):
            now = fbm(x * freq, y * freq, int(size * freq), octs, perm, dirs)
            data.append(now)
    return data

def go(path, clamped, size):

    perm = range(256)
    random.shuffle(perm)
    perm += perm
    dirs = [(math.cos(a * 2.0 * math.pi / 256), math.sin(a * 2.0 * math.pi / 256)) for a in range(256)]

    grey = Image.new("L", (size, size))

    grey.putdata(gen(size, perm, dirs), 128, 128)

    color = ImageOps.colorize(grey, (0, 0, 0), (211, 69, 73))

    if clamped:
        for x in range(size):
            for y in range(size):
                if grey.getpixel((x, y)) < 128:
                    key = 0xFFFF00
                    color.putpixel((x, y), key)

    color.save(path)

def main():
    size = 128
    go("cloud.bmp", False, size)
    go("cloud2.bmp", True, size)

if __name__ == "__main__":
    main()
