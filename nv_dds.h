#pragma once

#include <string>
#include <deque>

#include <assert.h>
#include <stdint.h>

#if defined(MACOS)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

namespace nv_dds {
// surface description flags
const uint32_t DDSF_CAPS = 0x00000001;
const uint32_t DDSF_HEIGHT = 0x00000002;
const uint32_t DDSF_WIDTH = 0x00000004;
const uint32_t DDSF_PITCH = 0x00000008;
const uint32_t DDSF_PIXELFORMAT = 0x00001000;
const uint32_t DDSF_MIPMAPCOUNT = 0x00020000;
const uint32_t DDSF_LINEARSIZE = 0x00080000;
const uint32_t DDSF_DEPTH = 0x00800000;

// pixel format flags
const uint32_t DDSF_ALPHAPIXELS = 0x00000001;
const uint32_t DDSF_FOURCC = 0x00000004;
const uint32_t DDSF_RGB = 0x00000040;
const uint32_t DDSF_RGBA = 0x00000041;

// dwCaps1 flags
const uint32_t DDSF_COMPLEX = 0x00000008;
const uint32_t DDSF_TEXTURE = 0x00001000;
const uint32_t DDSF_MIPMAP = 0x00400000;

// dwCaps2 flags
const uint32_t DDSF_CUBEMAP = 0x00000200;
const uint32_t DDSF_CUBEMAP_POSITIVEX = 0x00000400;
const uint32_t DDSF_CUBEMAP_NEGATIVEX = 0x00000800;
const uint32_t DDSF_CUBEMAP_POSITIVEY = 0x00001000;
const uint32_t DDSF_CUBEMAP_NEGATIVEY = 0x00002000;
const uint32_t DDSF_CUBEMAP_POSITIVEZ = 0x00004000;
const uint32_t DDSF_CUBEMAP_NEGATIVEZ = 0x00008000;
const uint32_t DDSF_CUBEMAP_ALL_FACES = 0x0000FC00;
const uint32_t DDSF_VOLUME = 0x00200000;

// compressed texture types
const uint32_t FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
const uint32_t FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
const uint32_t FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))

struct DXTColBlock {
    uint16_t col0;
    uint16_t col1;

    uint8_t row[4];
};

struct DXT3AlphaBlock {
    uint16_t row[4];
};

struct DXT5AlphaBlock {
    uint8_t alpha0;
    uint8_t alpha1;

    uint8_t row[6];
};

struct DDS_PIXELFORMAT {
    uint32_t dwSize;
    uint32_t dwFlags;
    uint32_t dwFourCC;
    uint32_t dwRGBBitCount;
    uint32_t dwRBitMask;
    uint32_t dwGBitMask;
    uint32_t dwBBitMask;
    uint32_t dwABitMask;
};

struct DDS_HEADER {
    uint32_t dwSize;
    uint32_t dwFlags;
    uint32_t dwHeight;
    uint32_t dwWidth;
    uint32_t dwPitchOrLinearSize;
    uint32_t dwDepth;
    uint32_t dwMipMapCount;
    uint32_t dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    uint32_t dwCaps1;
    uint32_t dwCaps2;
    uint32_t dwReserved2[3];
};

enum TextureType {
    TextureNone, TextureFlat,    // 1D, 2D, and rectangle textures
    Texture3D,
    TextureCubemap
};

class CSurface {
public:
    CSurface();
    CSurface(unsigned int w, unsigned int h, unsigned int d, unsigned int imgsize, const uint8_t *pixels);
    CSurface(const CSurface &copy);
    CSurface &operator=(const CSurface &rhs);
    virtual ~CSurface();

    operator uint8_t*() const;

    virtual void create(unsigned int w, unsigned int h, unsigned int d, unsigned int imgsize, const uint8_t *pixels);
    virtual void clear();

    unsigned int get_width() const {
        return m_width;
    }
    unsigned int get_height() const {
        return m_height;
    }
    unsigned int get_depth() const {
        return m_depth;
    }
    unsigned int get_size() const {
        return m_size;
    }

private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_depth;
    unsigned int m_size;

    uint8_t *m_pixels;
};

class CTexture: public CSurface {
    friend class CDDSImage;

public:
    CTexture();
    CTexture(unsigned int w, unsigned int h, unsigned int d, unsigned int imgsize, const uint8_t *pixels);
    CTexture(const CTexture &copy);
    CTexture &operator=(const CTexture &rhs);
    ~CTexture();

    void create(unsigned int w, unsigned int h, unsigned int d, unsigned int imgsize, const uint8_t *pixels);
    void clear();

    const CSurface &get_mipmap(unsigned int index) const {
        assert(!m_mipmaps.empty());
        assert(index < m_mipmaps.size());

        return m_mipmaps[index];
    }

    void add_mipmap(const CSurface &mipmap) {
        m_mipmaps.push_back(mipmap);
    }

    unsigned int get_num_mipmaps() const {
        return (unsigned int) m_mipmaps.size();
    }

protected:
    CSurface &get_mipmap(unsigned int index) {
        assert(!m_mipmaps.empty());
        assert(index < m_mipmaps.size());

        return m_mipmaps[index];
    }

private:
    std::deque<CSurface> m_mipmaps;
};

class CDDSImage {
public:
    CDDSImage();
    ~CDDSImage();

    void create_textureFlat(unsigned int format, unsigned int components, const CTexture &baseImage);
    void create_texture3D(unsigned int format, unsigned int components, const CTexture &baseImage);
    void create_textureCubemap(unsigned int format, unsigned int components, const CTexture &positiveX, const CTexture &negativeX, const CTexture &positiveY,
            const CTexture &negativeY, const CTexture &positiveZ, const CTexture &negativeZ);

    void clear();
    bool load(std::string filename, bool flipImage = true);
    bool save(std::string filename, bool flipImage = true);

    bool upload_texture1D();
    bool upload_texture2D(unsigned int imageIndex = 0, GLenum target = GL_TEXTURE_2D);
    bool upload_texture3D();
    bool upload_textureRectangle();
    bool upload_textureCubemap();

    operator uint8_t*() {
        assert(m_valid);
        assert(!m_images.empty());

        return m_images[0];
    }

    unsigned int get_width() {
        assert(m_valid);
        assert(!m_images.empty());

        return m_images[0].get_width();
    }

    unsigned int get_height() {
        assert(m_valid);
        assert(!m_images.empty());

        return m_images[0].get_height();
    }

    unsigned int get_depth() {
        assert(m_valid);
        assert(!m_images.empty());

        return m_images[0].get_depth();
    }

    unsigned int get_size() {
        assert(m_valid);
        assert(!m_images.empty());

        return m_images[0].get_size();
    }

    unsigned int get_num_mipmaps() {
        assert(m_valid);
        assert(!m_images.empty());

        return m_images[0].get_num_mipmaps();
    }

    const CSurface &get_mipmap(unsigned int index) const {
        assert(m_valid);
        assert(!m_images.empty());
        assert(index < m_images[0].get_num_mipmaps());

        return m_images[0].get_mipmap(index);
    }

    const CTexture &get_cubemap_face(unsigned int face) const {
        assert(m_valid);
        assert(!m_images.empty());
        assert(m_images.size() == 6);
        assert(m_type == TextureCubemap);
        assert(face < 6);

        return m_images[face];
    }

    unsigned int get_components() {
        return m_components;
    }
    unsigned int get_format() {
        return m_format;
    }
    TextureType get_type() {
        return m_type;
    }

    bool is_compressed() {
        if ((m_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) || (m_format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT) || (m_format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT))
            return true;
        else
            return false;
    }

    bool is_cubemap() {
        return (m_type == TextureCubemap);
    }
    bool is_volume() {
        return (m_type == Texture3D);
    }
    bool is_valid() {
        return m_valid;
    }

    bool is_dword_aligned() {
        assert(m_valid);

        int dwordLineSize = get_dword_aligned_linesize(get_width(), m_components * 8);
        int curLineSize = get_width() * m_components;

        return (dwordLineSize == curLineSize);
    }

private:
    unsigned int clamp_size(unsigned int size);
    unsigned int size_dxtc(unsigned int width, unsigned int height);
    unsigned int size_rgb(unsigned int width, unsigned int height);
    void swap_endian(void *val);

    // calculates 4-byte aligned width of image
    unsigned int get_dword_aligned_linesize(unsigned int width, unsigned int bpp) {
        return ((width * bpp + 31) & -32) >> 3;
    }

    void flip(CSurface &surface);
    void flip_texture(CTexture &texture);

    void swap(void *byte1, void *byte2, unsigned int size);
    void flip_blocks_dxtc1(DXTColBlock *line, unsigned int numBlocks);
    void flip_blocks_dxtc3(DXTColBlock *line, unsigned int numBlocks);
    void flip_blocks_dxtc5(DXTColBlock *line, unsigned int numBlocks);
    void flip_dxt5_alpha(DXT5AlphaBlock *block);

    void write_texture(const CTexture &texture, FILE *fp);

    unsigned int m_format;
    unsigned int m_components;
    TextureType m_type;
    bool m_valid;

    std::deque<CTexture> m_images;
};
}
