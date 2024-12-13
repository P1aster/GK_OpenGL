#include "Texture.h"


// Metoda wczytująca plik TGA
// Według dokumentacji biblioteki OpenGL
GLbyte* Texture::LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat)
{

    // Struktura przechowująca nagłówek pliku TGA
#pragma pack(1)
    typedef struct
    {
        GLbyte idlength;
        GLbyte colormaptype;
        GLbyte datatypecode;
        unsigned short colormapstart;
        unsigned short colormaplength;
        unsigned char colormapdepth;
        unsigned short x_orgin;
        unsigned short y_orgin;
        unsigned short width;
        unsigned short height;
        GLbyte bitsperpixel;
        GLbyte descriptor;
    } TGAHEADER;

    // Zmienne przechowujące informacje o wczytanym pliku
    FILE* pFile;
    TGAHEADER tgaHeader;
    unsigned long lImageSize;
    short sDepth;
    GLbyte* pbitsperpixel = NULL;

    // Domyślne wartości, wykorzystane w przypadku problemów z wczytaniem pliku
    *ImWidth = 0;
    *ImHeight = 0;
    *ImFormat = GL_BGR_EXT;
    *ImComponents = GL_RGB8;

    // Wczytanie pliku o nazwie określonej w parametrach funkcji
    pFile = fopen(FileName, "rb");
    if (pFile == NULL) return NULL;

    // Wczytanie nagłówka pliku do zdefiniowanej powyżej struktury
    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

    // Wczytanie parametrów obrazu
    *ImWidth = tgaHeader.width;
    *ImHeight = tgaHeader.height;
    sDepth = tgaHeader.bitsperpixel / 8;

    // Sprawdzenie, czy parametr głębi kolorów speałnia wymagania OpenGL (ma 8, 24 lub 32 bity)
    if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32) return NULL;

    // Określenie rozmiaru tekstury i alokacja odpowiedniej ilości pamięci
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

    // Weryfikacja poprawności wczytanej tekstury
    if (pbitsperpixel == NULL) return NULL;
    if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
    {
        free(pbitsperpixel);
        return NULL;
    }

    // Ustawienie formatu na format OpenGL
    switch (sDepth)
    {
    case 3:
        *ImFormat = GL_BGR_EXT;
        *ImComponents = GL_RGB8;
        break;
    case 4:
        *ImFormat = GL_BGRA_EXT;
        *ImComponents = GL_RGBA8;
        break;
    case 1:
        *ImFormat = GL_LUMINANCE;
        *ImComponents = GL_LUMINANCE8;
        break;
    };

    // Zamknięcie pliku i zwrócenie wskaźnika na miejsce wczytanych danych w pamięci
    fclose(pFile);
    return pbitsperpixel;
}