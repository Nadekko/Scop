#include "../include/config.hpp"

// commande image :
// convert image.png -type TrueColor image.bmp <= command ImageMagick (in true color format (24 bits per pixel)
// xxd image.bmp <= this command will output the binary content of the BMP file in a hexadecimal format
// file image.bmp <= this command will output the file type of the image


// a BPM file is a bitmap image file format used to store digital images.
// It is a simple format that stores pixel data in a binary format, along with a header that contains information about the image such as its width, height, and color depth.
// The header is typically 54 bytes long and contains information such as the file type, file size, image width, image height, number of color planes, bits per pixel, compression method, and more.
// The pixel data follows the header and is stored in a specific order based on the color depth of the image (e.g., RGB or RGBA). 
// BMP files can be uncompressed or compressed using various methods, 
// but they are generally larger in size compared to other image formats like JPEG or PNG due to their simplicity and lack of compression.

// Visualizing the structure of a BMP file
//  +----------------------+
//  octet 0 1 : 'B''M' (signature)
//  octet 2-5 : file size in bytes
//  octet 6-9 : reserved (not used)
//  octet 10-13 : offset to start of pixel data
//  octet 14-17 : header size (40 bytes for BITMAPINFOHEADER)
//  octet 18-21 : image width in pixels
//  octet 22-25 : image height in pixels
//  octet 26-27 : number of color planes (must be 1)
//  octet 28-29 : bits per pixel (e.g., 24 for RGB, 32 for RGBA)
//  octet 30-33 : compression method (0 for non-compressed)
//  octet 34-37 : image size (can be 0 for uncompressed images)
//  octet 38-41 : horizontal resolution (pixels per meter)
//  octet 42-45 : vertical resolution (pixels per meter)
//  octet 46-49 : number of colors in the color palette (0 for default)
//  octet 50-53 : number of important colors (0 for all colors)


int Texture::parseTexturePath(const char *path)
{
    std::cout << "path for texture = " << path << std::endl;
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        std::cerr << "ERROR::TEXTURE::FILE_NOT_SUCCESFULLY_READ\n" << path << std::endl;
        return -1;
    }

    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54)
    {
        std::cerr << "ERROR::TEXTURE::FILE_NOT_A_BMP\n" << path << std::endl;
        fclose(file);
        return -1;
    }
    if (header[0] != 'B' || header[1] != 'M')
    {
        std::cerr << "ERROR::TEXTURE::FILE_NOT_A_BMP\n" << path << std::endl;
        fclose(file);
        return -1;
    }

    int dataOffset;
    std::memcpy(&dataOffset, &header[10], sizeof(int));
    
    int width, height;
    std::memcpy(&width, &header[18], sizeof(int));
    std::memcpy(&height, &header[22], sizeof(int));
    
    short bpp;  // BMP files store colors in BGR (blue-green-red) format, so we need to convert it to RGB
    std::memcpy(&bpp, &header[28], sizeof(short));

    if (dataOffset < 54 || (bpp != 24 && bpp != 32) || (width <= 0 || height <= 0))
    {
        std::cerr << "ERROR::TEXTURE::UNSUPPORTED_BMP_FORMAT\n" << path << std::endl;
        fclose(file);
        return -1;
    }

    //calculate the size of the pixel data
    unsigned int rowSize = ((bpp * width + 31) / 32) * 4; // each row is padded to be a multiple of 4 bytes
    unsigned int imageSize = rowSize * height; // total size of pixel data
    std::vector<unsigned char> data(imageSize); // allocate memory for pixel data

    // pos at the beginning of the pixel data
    fseek(file, dataOffset, SEEK_SET);
    if (fread(data.data(), 1, imageSize, file) != imageSize)
    {
        std::cerr << "ERROR::TEXTURE::FILE_NOT_SUCCESFULLY_READ\n" << path << std::endl;
        data.clear();
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

Texture::Texture(const char *path)
{
    // generate and bind the texture
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    // set the texture wrapping parameters
    // s = horizontal axis, t = vertical axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set the texture filtering parameters // GL_LINEAR = smooth transition between mipmap levels.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // texture minifying (object is far away)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // texture magnifying (object is close)

    GLenum format = (bpp == 24) ? GL_BGR : GL_BGRA; // BMP files store colors in BGR format, so we need to convert it to RGB
    GLenum internalFormat = (bpp == 24) ? GL_RGB : GL_RGBA; // we want to store the texture in RGB or RGBA format on the GPU
    
    // upload the texture data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data.data());

    glGenerateMipmap(GL_TEXTURE_2D);
    data.clear();
}

Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, ID);
}

// 0 means, "no more link between the texture and the OpenGL context"
void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}