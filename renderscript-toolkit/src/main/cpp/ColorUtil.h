//
// Created by admin on 2022/11/28.
//

#ifndef RENDERSCRIPT_INTRINSICS_REPLACEMENT_TOOLKIT_COLORUTIL_H
#define RENDERSCRIPT_INTRINSICS_REPLACEMENT_TOOLKIT_COLORUTIL_H
#include <array>
#include <cmath>
#include <float.h>
#include "RenderScriptToolkit.h"
namespace  renderscript{

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

    void RGBToXYZ(U8CPU r,U8CPU g,U8CPU b,SkScalar xyz[3]);

    SkColor XYZToColor(U8CPU a,const SkScalar xyz[3]);

    void RGBToHSL(U8CPU r,U8CPU g,U8CPU b,SkScalar hsl[3]);

    SkColor HSLToColor(U8CPU a,const SkScalar hsl[3]);

    void ColorBlenderHSV(RenderScriptToolkit::BlendingMode blendingMode, const SkScalar inHsv[3], SkScalar dstHsv[3]);

    void ColorBlenderRGB(RenderScriptToolkit::BlendingMode blendingMode, SkScalar inRGB[3], const SkScalar dstRGB[3]);

    static int constrain(int32_t amount, int32_t low, int32_t high) {
        return amount < low ? low : (amount > high ? high : amount);
    }

   static float constrain(SkScalar amount, SkScalar low, SkScalar high) {
        return amount < low ? low : (amount > high ? high : amount);
    }

    static float min(float r, float g, float b) { return std::min(r, std::min(g, b)); }
    static float max(float r, float g, float b) { return std::max(r, std::max(g, b)); }

    static float sat(float r, float g, float b) { return max(r,g,b) - min(r,g,b); }
    static float lum(float r, float g, float b) { return r*0.30f + g*0.59f + b*0.11f; }

// The two SetSat() routines in the specs look different, but they're logically equivalent.
// Both map the minimum channel to 0, maximum to s, and scale the middle proportionately.
// The KHR version has done a better job at simplifying its math, so we use it here.
    static void set_sat(float* r, float* g, float* b, float s) {
        float mn = min(*r,*g,*b),
                mx = max(*r,*g,*b);
        auto channel = [=](float c) {
            return mx == mn ? 0
                            : (c - mn) * s / (mx - mn);
        };
        *r = channel(*r);
        *g = channel(*g);
        *b = channel(*b);
    }

    static void clip_color(float* r, float* g, float* b) {
        float l  = lum(*r,*g,*b),
                mn = min(*r,*g,*b),
                mx = max(*r,*g,*b);
        auto clip = [=](float c) {
            if (mn < 0) { c = l + (c - l) * (    l) / (l - mn); }
            if (mx > 1) { c = l + (c - l) * (1 - l) / (mx - l); }
            SkASSERT(-0.0001f <  c);   // This may end up very slightly negative...
            SkASSERT(       c <= 1);
            return c;
        };
        *r = clip(*r);
        *g = clip(*g);
        *b = clip(*b);
    }

    static void set_lum(float* r, float* g, float* b, float l) {
        float diff = l - lum(*r,*g,*b);
        *r += diff;
        *g += diff;
        *b += diff;
        clip_color(r,g,b);
    }


    static void hue(float  dr, float  dg, float  db,
                    float* sr, float* sg, float* sb) {
        // Hue of Src, Saturation and Luminosity of Dst.
        float R = *sr,
                G = *sg,
                B = *sb;
        set_sat(&R,&G,&B, sat(dr,dg,db));
        set_lum(&R,&G,&B, lum(dr,dg,db));
        *sr = R;
        *sg = G;
        *sb = B;
    }

    static void saturation(float  dr, float  dg, float  db,
                           float* sr, float* sg, float* sb) {
        // Saturation of Src, Hue and Luminosity of Dst
        float R = dr,
                G = dg,
                B = db;
        set_sat(&R,&G,&B, sat(*sr,*sg,*sb));
        set_lum(&R,&G,&B, lum( dr, dg, db));  // This may seem redundant, but it is not.
        *sr = R;
        *sg = G;
        *sb = B;
    }

    static void color(float  dr, float  dg, float  db,
                      float* sr, float* sg, float* sb) {
        // Hue and Saturation of Src, Luminosity of Dst.
        float R = *sr,
                G = *sg,
                B = *sb;
        set_lum(&R,&G,&B, lum(dr,dg,db));
        *sr = R;
        *sg = G;
        *sb = B;
    }

    static void luminosity(float  dr, float  dg, float  db,
                           float* sr, float* sg, float* sb) {
        // Luminosity of Src, Hue and Saturation of Dst.
        float R = dr,
                G = dg,
                B = db;
        set_lum(&R,&G,&B, lum(*sr,*sg,*sb));
        *sr = R;
        *sg = G;
        *sb = B;
    }
}
#endif //RENDERSCRIPT_INTRINSICS_REPLACEMENT_TOOLKIT_COLORUTIL_H
