#pragma once

#include <iostream>
#include "core/util/Util.h"

#if IS_MOBILE()
    #include <GLES3/gl32.h>
#elif IS_DESKTOP()
    #include <glad/glad.h>
#endif

namespace GDE {

    class Texture;
    struct NinePatch {
        int left = -1;
        int top = -1;
        int right = -1;
        int bottom = -1;

        [[nodiscard]] bool isEnabled() const {
            return left != -1 && top != -1 && right != -1 && bottom != -1;
        }

        IntRect subRects[9];
    };

    class Atlas;
    class Texture {
        protected:
            unsigned int refCount = 0;
            GLuint openGLTextureID{};
            int channels = -1;
            Vec2I textureSize, spriteSheetSize;
            GLenum internalFormat = -1, dataFormat = -1;
            IntRect region {};
            float fileSizeKb = -1;
            std::string path;

        public:
            NinePatch ninePatch;

        public:
            Texture() = default;
            explicit Texture(char* filePath);
            Texture(Atlas* _spriteSheet, const IntRect& _region);
            ~Texture();
            [[nodiscard]] GLuint getGLTexture() const;
            [[nodiscard]] Vec2I getSize() const;
            [[nodiscard]] Vec2I getSpriteSheetSize() const;
            [[nodiscard]] float getKb() const;

            bool loadFromFile(const char* _path);
            bool loadTextTexture(int _width, int _height);
            bool loadTextureFromMemory(int _width, int _height, const unsigned char* _data);
            bool loadTextSubTextures(Vec2I _offset, Vec2I _size, const void* _data);

            IntRect& getRegion();
            [[nodiscard]]std::string getPath();

        private:
            int invertSDLSurface(SDL_Surface *surface);
            SDL_Surface* getSDLSurface(SDL_RWops* _imageFile, const std::string& _pathToFile);
            void checkGLError();
    };

    enum ImageType {
        PNG,
        JPG,
        BMP
    };

    class Image : public Texture {
        private:
            unsigned char* pixels = nullptr;
            ImageType imageType;

        private:
            int getChannels(const ImageType& _imageType);

        public:
            Image() = default;
            void init(int _width, int _height, unsigned char* _pixels, const ImageType& _imageType);
            void init(int _width, int _height, const ImageType& _imageType);
            void uploadToGPU();
            void saveAs(const std::string& _pathToSave);
            void setPixel(int _x, int _y, const Color& _color);
            Color getPixel(int _x, int _y);
            ~Image();
    };
}