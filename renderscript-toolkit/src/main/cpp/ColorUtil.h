//
// Created by admin on 2022/11/28.
//

#ifndef RENDERSCRIPT_INTRINSICS_REPLACEMENT_TOOLKIT_COLORUTIL_H
#define RENDERSCRIPT_INTRINSICS_REPLACEMENT_TOOLKIT_COLORUTIL_H
#include <array>
#include <cmath>
#include <float.h>

template <typename T>
static constexpr const T& SkTPin(const T& x, const T& lo, const T& hi) {
    return std::max(lo, std::min(x, hi));
}

#define SkASSERT(cond)            static_cast<void>(0)

/** Fast type for unsigned 8 bits. Use for parameter passing and local
    variables, not for storage
*/
typedef unsigned U8CPU;

/** Fast type for unsigned 16 bits. Use for parameter passing and local
    variables, not for storage
*/
typedef unsigned U16CPU;

typedef float SkScalar;


/** 8-bit type for an alpha value. 255 is 100% opaque, zero is 100% transparent.
*/
typedef uint8_t SkAlpha;

/** 32-bit ARGB color value, unpremultiplied. Color components are always in
    a known order. This is different from SkPMColor, which has its bytes in a configuration
    dependent order, to match the format of kBGRA_8888_SkColorType bitmaps. SkColor
    is the type used to specify colors in SkPaint and in gradients.

    Color that is premultiplied has the same component values as color
    that is unpremultiplied if alpha is 255, fully opaque, although may have the
    component values in a different order.
*/
typedef uint32_t SkColor;

/** Returns color value from 8-bit component values. Asserts if SK_DEBUG is defined
    if a, r, g, or b exceed 255. Since color is unpremultiplied, a may be smaller
    than the largest of r, g, and b.

    @param a  amount of alpha, from fully transparent (0) to fully opaque (255)
    @param r  amount of red, from no red (0) to full red (255)
    @param g  amount of green, from no green (0) to full green (255)
    @param b  amount of blue, from no blue (0) to full blue (255)
    @return   color and alpha, unpremultiplied
*/
static constexpr inline SkColor SkColorSetARGB(U8CPU a, U8CPU r, U8CPU g, U8CPU b) {
return SkASSERT(a <= 255 && r <= 255 && g <= 255 && b <= 255),
(a << 24) | (r << 16) | (g << 8) | (b << 0);
}

/** Returns color value from 8-bit component values, with alpha set
    fully opaque to 255.
*/
#define SkColorSetRGB(r, g, b)  SkColorSetARGB(0xFF, r, g, b)

/** Returns alpha byte from color value.
*/
#define SkColorGetA(color)      (((color) >> 24) & 0xFF)

/** Returns red component of color, from zero to 255.
*/
#define SkColorGetR(color)      (((color) >> 16) & 0xFF)

/** Returns green component of color, from zero to 255.
*/
#define SkColorGetG(color)      (((color) >>  8) & 0xFF)

/** Returns blue component of color, from zero to 255.
*/
#define SkColorGetB(color)      (((color) >>  0) & 0xFF)

#define SK_Scalar1                  1.0f
#define SK_ScalarHalf               0.5f
#define sk_float_floor(x)       floorf(x)
#define SkIntToScalar(x)        static_cast<SkScalar>(x)
#define SkIntToFloat(x)         static_cast<float>(x)
#define SkScalarRoundToInt(x)   static_cast<int>(x)
#define SkScalarAbs(x)              std::abs(x)
#define SK_ScalarNearlyZero         (SK_Scalar1 / (1 << 12))
#define SkScalarFloorToScalar(x)    sk_float_floor(x)


static inline bool SkScalarNearlyZero(SkScalar x,
                                      SkScalar tolerance = SK_ScalarNearlyZero) {
    SkASSERT(tolerance >= 0);
    return SkScalarAbs(x) <= tolerance;
}


static inline SkScalar ByteToScalar(U8CPU x) {
    SkASSERT(x <= 255);
    return SkIntToScalar(x) / 255;
}

static inline SkScalar ByteDivToScalar(int numer, U8CPU denom) {
    // cast to keep the answer signed
    return SkIntToScalar(numer) / (int)denom;
}


void RGBToHSV(U8CPU r, U8CPU g, U8CPU b, SkScalar hsv[3]) ;

SkColor HSVToColor(U8CPU a, const SkScalar hsv[3]);


#endif //RENDERSCRIPT_INTRINSICS_REPLACEMENT_TOOLKIT_COLORUTIL_H
