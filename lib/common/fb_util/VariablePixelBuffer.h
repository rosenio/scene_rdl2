// Copyright 2023 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#pragma once
#include "FbTypes.h"
#include "ispc/PixelBuffer.hh"

namespace scene_rdl2 {
namespace fb_util {

typedef unsigned int PixelBufferUtilOptions;

class Tiler;

// Use this class instead of PixelBuffer<T> if the concrete pixel type isn't
// known at compile time or is variable at runtime.
class VariablePixelBuffer
{
public:
    enum Format
    {
        RGB888,
        RGBA8888,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,

        // Extend as needed...

        NUM_FORMATS,
        UNINITIALIZED,
    };

    VariablePixelBuffer();
    ~VariablePixelBuffer();

    static uint32_t hudValidation(bool verbose)
    {
        VARIABLE_PIXELBUFFER_VALIDATION;
    }

    bool init(Format format, unsigned w, unsigned h);
    void cleanUp();

    Format getFormat() const    { return mFormat; }

    // Returned in bytes.
    unsigned getSizeOfPixel() const;

    void clear();
    void clear(float val);

    unsigned getWidth() const       { return mBuffer.getWidth(); }
    unsigned getHeight() const      { return mBuffer.getHeight(); }
    unsigned getArea() const        { return getWidth() * getHeight(); }

    std::shared_ptr<uint8_t> getDataShared() {
        return mBuffer.getDataSharedAs<uint8_t>();
    }

    uint8_t *getData()              { return (uint8_t *)mBuffer.getData(); }
    const uint8_t *getData() const  { return (const uint8_t *)mBuffer.getData(); }

    void gammaAndQuantizeTo8bit(const RenderBuffer& srcBuffer, PixelBufferUtilOptions options, float exposure, float gamma);

    bool packSparseTiles(uint8_t *dstPackedBuffer, const std::vector<Tile> &tiles) const;

    bool unpackSparseTiles(const uint8_t *srcPackedData, const std::vector<Tile> &tiles);

    // Takes the tiledBuffer and untiles it into "this".
    void untile(const VariablePixelBuffer &tiledBuffer, const Tiler &tiler, bool parallel);

    Rgb888Buffer &getRgb888Buffer()
    {
        MNRY_ASSERT(mFormat == RGB888);
        return reinterpret_cast<Rgb888Buffer &>(mBuffer);
    }

    const Rgb888Buffer &getRgb888Buffer() const
    {
        return const_cast<VariablePixelBuffer *>(this)->getRgb888Buffer();
    }

    Rgba8888Buffer &getRgba8888Buffer()
    {
        MNRY_ASSERT(mFormat == RGBA8888);
        return reinterpret_cast<Rgba8888Buffer &>(mBuffer);
    }

    const Rgba8888Buffer &getRgba8888Buffer() const
    {
        return const_cast<VariablePixelBuffer *>(this)->getRgba8888Buffer();
    }

    FloatBuffer &getFloatBuffer()
    {
        MNRY_ASSERT(mFormat == FLOAT);
        return reinterpret_cast<FloatBuffer &>(mBuffer);
    }

    const FloatBuffer &getFloatBuffer() const
    {
        return const_cast<VariablePixelBuffer *>(this)->getFloatBuffer();
    }

    Float2Buffer &getFloat2Buffer()
    {
        MNRY_ASSERT(mFormat == FLOAT2);
        return reinterpret_cast<Float2Buffer &>(mBuffer);
    }

    const Float2Buffer &getFloat2Buffer() const
    {
        return const_cast<VariablePixelBuffer *>(this)->getFloat2Buffer();
    }

    Float3Buffer &getFloat3Buffer()
    {
        MNRY_ASSERT(mFormat == FLOAT3);
        return reinterpret_cast<Float3Buffer &>(mBuffer);
    }

    const Float3Buffer &getFloat3Buffer() const
    {
        return const_cast<VariablePixelBuffer *>(this)->getFloat3Buffer();
    }

    Float4Buffer &getFloat4Buffer()
    {
        MNRY_ASSERT(mFormat == FLOAT4);
        return reinterpret_cast<Float4Buffer &>(mBuffer);
    }

    const Float4Buffer &getFloat4Buffer() const
    {
        return const_cast<VariablePixelBuffer *>(this)->getFloat4Buffer();
    }

private:
    // Returned in bytes.
    static unsigned getSizeOfPixel(Format format);

    // This is aliased over all the various buffer types we support.
    // This works since sizeof(PixelBuffer<T>) is the same for all T.
    typedef PixelBuffer<uint8_t> PixelBufferU8;
    VARIABLE_PIXELBUFFER_MEMBERS;
};

MNRY_STATIC_ASSERT((sizeof(VariablePixelBuffer)) % CACHE_LINE_SIZE == 0);

} // namespace fb_util
} // namespace scene_rdl2

