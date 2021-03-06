#pragma once

#include <iostream>
#include "core/util/Util.h"

#if IS_ANDROID()
    #include <GLES3/gl32.h>
#elif IS_IOS()
    #include <OpenGLES/ES3/gl.h>
#elif IS_MAC()
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl3.h>
#elif IS_DESKTOP()
    #include <glad/glad.h>
#endif

namespace GDE {

    class Texture;

    /**
     * @brief Defines how the texture must be rendered so deformations on X and Y won't affect it's quality. It is separated 9 parts as follows:
     * 
     *       ____|____________|____
     *      | 6  |     7      | 8  |
     *      |----|------------|----| top
     *      |    |            |    |
     *      | 3  |     4      | 5  |
     *      |    |            |    |
     *      |----|------------|----| bottom
     *      |_0__|_____1______|_2__|
     *           |            |
     *          left         right
     * 
     * 
     *  Elements 0, 2, 6 and 8 doesn't get streched.
     *  Elements 3 and 5 get streched in Y axis.
     *  Elements 1 and 7 get streched in X axis.
     *  Element 4 gets streched in XY axis.
     * 
     */
    struct NinePatch {
        /**
         * @brief Left separator line
         * 
         */
        int left = -1;

        /**
         * @brief Top separator line
         * 
         */
        int top = -1;

        /**
         * @brief Right separator line
         * 
         */
        int right = -1;

        /**
         * @brief Bottom separator line
         * 
         */
        int bottom = -1;

        /**
         * @brief Returns if the Sprite has or not 9Patch.
         * 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool isEnabled() const {
            return left != -1 && top != -1 && right != -1 && bottom != -1;
        }

        /**
         * @brief The resulting subrects generated by the 9Patch config.
         * 
         */
        IntRect subRects[9];
    };

    class Atlas;

    /**
     * @brief This class represents a SpriteSheet or a simple Sprite that will be uploaded to the GPU.
     */
    class Texture {
        protected:
            /**
             * @brief This is the reference that OpenGL gives us when we upload the texture data to the GPU.
             */
            GLuint openGLTextureID{};

            /**
             * @brief if the texture is RGB or RGBA
             */
            int channels = -1;

            /**
             * @brief The size of the Sprite, not the whole SpriteSheet EXCEPT if this texture is itself a the whole
             * SpriteSheet.
             */
            Vec2I textureSize;

            /**
             * @brief The size of the SpriteSheet this texture comes from or its natural size if this is the SpriteSheet itself.
             */
            Vec2I spriteSheetSize;

            /**
             * @brief Internal format of the texture.
             */
            GLenum internalFormat = -1;

            /**
             * @brief Format of the data of the texture.
             */
            GLenum dataFormat = -1;

            /**
             * @brief Out of the whole SpriteSheet, the portion that the renderer should draw on screen. If this texture
             * is itself the SpriteSheet, then the region is the whole SpriteSheet.
             */
            IntRect region {};

            /**
             * @brief Size in Kilobytes of the SpriteSheet.
             */
            float fileSizeKb = -1;

            /**
             * @brief Where the SpriteSheet is located.
             */
            std::string path;

        public:
            /**
             * @see NinePatch
             */
            NinePatch ninePatch;

        public:
            Texture() {};
            explicit Texture(char* filePath);
            Texture(Atlas* _spriteSheet, const IntRect& _region);
            virtual ~Texture();

            /**
             * @brief Returns the ID of the texture in the GPU. This is probably of no-use for end-users.
             * @return uint
             */
            [[nodiscard]] GLuint getGLTexture() const;

            /**
             * @brief Returns the size of the region to be rendered, not the size of the SpriteSheet, except if this is the
             * SpriteSheet itself.
             * @return Vec2I
             */
            [[nodiscard]] Vec2I getSize() const;

            /**
             * @brief Returns the size of the SpriteSheet where this sub-texture comes from.
             * @return Vec2I
             */
            [[nodiscard]] Vec2I getSpriteSheetSize() const;

            /**
             * @brief Returns the size in Kb of the SpriteSheet.
             * @return float
             */
            [[nodiscard]] float getKb() const;

            /**
             * @attention This is not meant to be called by end-users.
             * @brief Tries to load a SpriteSheet from the File System, if it fails, returns false.
             * @param _path Path where the SpriteSheet is located.
             * @return bool
             */
            bool loadFromFile(const char* _path);

            /**
             * @attention This is not meant to be called by end-users.
             * @brief Tries to load a font into a texture, and if it fails, returns false.
             * @param _width Width of the resulting texture.
             * @param _height Height of the resulting texture.
             * @return
             */
            bool loadTextTexture(int _width, int _height);

            /**
             * @attention This is not meant to be called by end-users.
             * @brief Tries to load a a texture from data generated at runtime.
             * @param _width Width of the resulting texture.
             * @param _height Height of the resulting texture.
             * @param _data Data generated at runtime.
             * @return
             */
            bool loadTextureFromMemory(int _width, int _height, const unsigned char* _data);

            /**
             * @attention This is not meant to be called by end-users.
             * @brief Creates the sub-textures (of each character) of the Font texture.
             * @param _offset Offset from the last character.
             * @param _size Size of the character.
             * @param _data Data needed to generate the texture.
             * @return bool
             */
            bool loadTextSubTextures(Vec2I _offset, Vec2I _size, const void* _data);

            /**
             * @brief Returns the portion of the SpriteSheet that will be rendered.
             * @return IntRect&
             */
            IntRect& getRegion();

            /**
             * @brief Returns the path to the location of the SpriteSheet.
             * @return std::string
             */
            [[nodiscard]]std::string getPath();

        private:
            /**
             * @brief SDL2 loads the texture backwards so this returns it to its normal state.
             * @param surface SDL2 surface needed to load the texture.
             * @return int
             */
            int invertSDLSurface(SDL_Surface *surface);

            /**
             * @brief Generates and SDL2 surface to work with the texture when loading it.
             * @param _imageFile Raw data of the SpriteSheet.
             * @param _pathToFile Path to the file.
             * @return SDL_Surface*
             */
            SDL_Surface* getSDLSurface(SDL_RWops* _imageFile, const std::string& _pathToFile);

            /**
             * @brief Checks for any OpenGL error on a given moment and prints it on screen.
             */
            void checkGLError();
    };

    /**
     * @brief Supported image formats.
     */
    enum ImageType {
        PNG,
        JPG,
        BMP
    };

    /**
     * @attention Not recommended to use as hasn't been updated for many versions.
     * @brief This is another type of texture that resides in the CPU also so we can modify its data on runtime more easily.
     */
    class Image : public Texture {
        private:
            /**
             * @brief Data of the texture.
             */
            unsigned char* pixels = nullptr;

            /**
             * @brief Type of texture.
             */
            ImageType imageType;

        private:
            /**
             * @brief Returns the amount of channels.
             * @param _imageType Type of texture.
             * @return int
             */
            int getChannels(const ImageType& _imageType);

        public:
            Image() {};

            /**
             * @brief Inits a CPU texture given a width, height, data of the texture and type of texture.
             * @param _width Width of the texture.
             * @param _height Height of the texture.
             * @param _pixels Data of the texture.
             * @param _imageType Type of the texture.
             */
            void init(int _width, int _height, unsigned char* _pixels, const ImageType& _imageType);

            /**
             * @brief Inits a CPU texture given a width, height and type of texture but no information.
             * @param _width Width of the texture.
             * @param _height Height of the texture.
             * @param _imageType Type of the texture.
             */
            void init(int _width, int _height, const ImageType& _imageType);

            /**
             * @brief Sends the data to the GPU.
             */
            void uploadToGPU();

            /**
             * @brief Saves a copy of the texture to the file system.
             * @param _pathToSave Path to save the texture.
             */
            void saveAs(const std::string& _pathToSave);

            /**
             * @brief Allows to set a specific pixel of the texture.
             * @param _x X position in the texture starting on the left-bottom corner.
             * @param _y Y position in the texture starting on the left-bottom corner.
             * @param _color The color to paint the pixel.
             */
            void setPixel(int _x, int _y, const Color& _color);

            /**
             * @brief Returns the color of a specific pixel
             * @param _x X position in the texture starting on the left-bottom corner.
             * @param _y Y position in the texture starting on the left-bottom corner.
             * @return
             */
            Color getPixel(int _x, int _y);


            ~Image();
    };
}
