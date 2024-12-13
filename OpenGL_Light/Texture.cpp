#include "Texture.h"

// Function to load a TGA file as a texture
GLbyte* Texture::LoadTGAImage(const char* FileName)
{
    // Structure to store the header of a TGA file
#pragma pack(1)
    typedef struct
    {
        GLbyte idlength;          // Length of the ID field
        GLbyte colormaptype;      // Color map type (0 = none, 1 = present)
        GLbyte datatypecode;      // Data type code (e.g., 2 = uncompressed true-color image)
        unsigned short colormapstart; // Offset for the color map
        unsigned short colormaplength; // Length of the color map
        unsigned char colormapdepth; // Depth of the color map
        unsigned short x_orgin;   // X origin of the image
        unsigned short y_orgin;   // Y origin of the image
        unsigned short width;     // Width of the image in pixels
        unsigned short height;    // Height of the image in pixels
        GLbyte bitsperpixel;      // Number of bits per pixel (e.g., 8, 24, 32)
        GLbyte descriptor;        // Image descriptor byte
    } TGAHEADER;

    // Variables for storing information about the loaded file
    FILE* pFile;
    TGAHEADER tgaHeader;
    unsigned long lImageSize;
    short sDepth;
    GLbyte* pbitsperpixel = NULL;

    // Default values used in case of issues while loading the file
    ImWidth = 0;
    ImHeight = 0;
    ImFormat = GL_BGR_EXT;  // Default image format for TGA files
    ImComponents = GL_RGB8; // Default component type for OpenGL

    // Open the file specified by the function parameter
    pFile = fopen(FileName, "rb");
    if (pFile == NULL) return NULL;

    // Read the file header into the defined structure
    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

    // Extract image parameters from the header
    ImWidth = tgaHeader.width;
    ImHeight = tgaHeader.height;
    sDepth = tgaHeader.bitsperpixel / 8;

    // Verify that the color depth is valid for OpenGL (must be 8, 24, or 32 bits)
    if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32) return NULL;

    // Calculate the size of the texture and allocate the necessary memory
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

    // Verify the integrity of the loaded texture
    if (pbitsperpixel == NULL) return NULL;
    if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
    {
        free(pbitsperpixel);
        return NULL;
    }

    // Set the format to OpenGL-specific formats based on depth
    switch (sDepth)
    {
    case 3:
        ImFormat = GL_BGR_EXT;   // 24-bit color format
        ImComponents = GL_RGB8; // RGB components
        break;
    case 4:
        ImFormat = GL_BGRA_EXT;  // 32-bit color format
        ImComponents = GL_RGBA8; // RGBA components
        break;
    case 1:
        ImFormat = GL_LUMINANCE; // Grayscale
        ImComponents = GL_LUMINANCE8; // Single intensity channel
        break;
    };

    // Close the file and return a pointer to the loaded data in memory
    fclose(pFile);
    pBytes = pbitsperpixel;
    return pbitsperpixel;
}

// Function to configure the texture and generate its OpenGL ID
Texture& Texture::setTexture() {
    // Generate a unique texture ID
    glGenTextures(1, &textureID);

    // Bind the texture to the 2D texture target
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Define the 2D texture using the previously loaded data
    glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

    // Free the memory allocated for the texture data as it's now on the GPU
    free(pBytes);
    return *this;
}

// Function to retrieve the OpenGL texture ID
GLuint Texture::getTetureID() {
    return textureID;
}

// Function to bind the texture to the current OpenGL context
Texture& Texture::bindTexture() {
    // Bind the texture ID to the 2D texture target
    glBindTexture(GL_TEXTURE_2D, textureID);
    return *this;
}