#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>

constexpr int tileSize = 32;

struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    Pixel &operator *(uint8_t coef) {
        r *= coef;
        g *= coef;
        b *= coef;
        a *= coef;
        return *this;
    }
    Pixel &operator +(uint8_t coef) {
        r += coef;
        g += 2 * coef / 5;
        b += 2 * coef / 5;
        return *this;
    }
    Pixel &operator /(uint8_t coef) {
        r /= coef;
        g /= coef;
        b /= coef;
        return *this;
    }
};

constexpr Pixel backgroundColor{0, 0, 0, 0};
Pixel blend(Pixel oldPixel, Pixel newPixel);

struct Image {

    explicit Image(const std::string &a_path);

    Image(int a_width, int a_height, int a_channels);

    Image(const Image &other);

    int Save(const std::string &a_path);

    int Width() const { return width; }

    int Height() const { return height; }

    int Channels() const { return channels; }

    size_t Size() const { return size; }

    Pixel *Data() { return data; }

    Pixel GetPixel(int x, int y) { return data[width * y + x]; }

    void PutPixel(int x, int y, const Pixel &pix) { data[width * y + x] = pix; }

    ~Image();

private:
    int width{-1};
    int height{-1};
    int channels{3};
    size_t size{0};
    Pixel *data{nullptr};
    bool self_allocated{false};
};


#endif //MAIN_IMAGE_H
