//
// Created by admin on 2022/11/28.
//

#include "ColorUtil.h"

namespace renderscript {
    void RGBToHSV(U8CPU r, U8CPU g, U8CPU b, SkScalar hsv[3]) {
        SkASSERT(hsv);

        unsigned min = std::min(r, std::min(g, b));
        unsigned max = std::max(r, std::max(g, b));
        unsigned delta = max - min;

        SkScalar v = ByteToScalar(max);
        SkASSERT(v >= 0 && v <= SK_Scalar1);

        if (0 == delta) { // we're a shade of gray
            hsv[0] = 0;
            hsv[1] = 0;
            hsv[2] = v;
            return;
        }

        SkScalar s = ByteDivToScalar(delta, max);
        SkASSERT(s >= 0 && s <= SK_Scalar1);

        SkScalar h;
        if (r == max) {
            h = ByteDivToScalar(g - b, delta);
        } else if (g == max) {
            h = SkIntToScalar(2) + ByteDivToScalar(b - r, delta);
        } else { // b == max
            h = SkIntToScalar(4) + ByteDivToScalar(r - g, delta);
        }

        h *= 60;
        if (h < 0) {
            h += SkIntToScalar(360);
        }
        SkASSERT(h >= 0 && h < SkIntToScalar(360));

        hsv[0] = h;
        hsv[1] = s;
        hsv[2] = v;
    }

    SkColor HSVToColor(U8CPU a, const SkScalar hsv[3]) {
        SkASSERT(hsv);

        SkScalar s = SkTPin(hsv[1], 0.0f, 1.0f);
        SkScalar v = SkTPin(hsv[2], 0.0f, 1.0f);

        U8CPU v_byte = SkScalarRoundToInt(v * 255);

        if (SkScalarNearlyZero(s)) { // shade of gray
            return SkColorSetARGB(a, v_byte, v_byte, v_byte);
        }
        SkScalar hx = (hsv[0] < 0 || hsv[0] >= SkIntToScalar(360)) ? 0 : hsv[0] / 60;
        SkScalar w = SkScalarFloorToScalar(hx);
        SkScalar f = hx - w;

        unsigned p = SkScalarRoundToInt((SK_Scalar1 - s) * v * 255);
        unsigned q = SkScalarRoundToInt((SK_Scalar1 - (s * f)) * v * 255);
        unsigned t = SkScalarRoundToInt((SK_Scalar1 - (s * (SK_Scalar1 - f))) * v * 255);

        unsigned r, g, b;

        SkASSERT((unsigned) (w) < 6);
        switch ((unsigned) (w)) {
            case 0:
                r = v_byte;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = v_byte;
                b = p;
                break;
            case 2:
                r = p;
                g = v_byte;
                b = t;
                break;
            case 3:
                r = p;
                g = q;
                b = v_byte;
                break;
            case 4:
                r = t;
                g = p;
                b = v_byte;
                break;
            default:
                r = v_byte;
                g = p;
                b = q;
                break;
        }
        return SkColorSetARGB(a, r, g, b);
    }

  void RGBToXYZ(U8CPU r, U8CPU g, U8CPU b, SkScalar xyz[3]){
        SkASSERT(xyz);
        SkScalar sr = r / 255.0f ;
        sr = sr < 0.04045 ? sr / 12.92f : pow((sr + 0.055f) / 1.055f, 2.4);
        SkScalar sg = g / 255.0f;
        sg = sg < 0.04045f ? sg / 12.92f : pow((sg + 0.055f) / 1.055f, 2.4);
        SkScalar sb = b / 255.0f ;
        sb = sb < 0.04045f ? sb / 12.92f : pow((sb + 0.055f) / 1.055f, 2.4);

        xyz[0] = 100 * (sr * 0.4124 + sg * 0.3576 + sb * 0.1805);
        xyz[1] = 100 * (sr * 0.2126 + sg * 0.7152 + sb * 0.0722);
        xyz[2] = 100 * (sr * 0.0193 + sg * 0.1192 + sb * 0.9505);
 }


SkColor XYZToColor(U8CPU a,const SkScalar xyz[3]){
    SkASSERT(xyz);
    SkScalar x = xyz[0],y = xyz[1],z = xyz[2] ;
    SkScalar r = (x * 3.2406f + y * -1.5372f + z * -0.4986f) / 100;
    SkScalar g = (x * -0.9689f + y * 1.8758 + z * 0.0415f) / 100;
    SkScalar b = (x * 0.0557f + y * -0.2040f + z * 1.0570f) / 100;

    r = r > 0.0031308 ? 1.055 * std::pow(r, 1 / 2.4) - 0.055 : 12.92 * r;
    g = g > 0.0031308 ? 1.055 * std::pow(g, 1 / 2.4) - 0.055 : 12.92 * g;
    b = b > 0.0031308 ? 1.055 * std::pow(b, 1 / 2.4) - 0.055 : 12.92 * b;

    return SkColorSetARGB(a,
                    constrain((int) std::round(r * 255), 0, 255),
                    constrain((int) std::round(g * 255), 0, 255),
                    constrain((int) std::round(b * 255), 0, 255));
}

    void RGBToHSL(U8CPU r,U8CPU g,U8CPU b,SkScalar hsl[3]){
        SkASSERT(hsl);
        SkScalar rf = r / 255.0f;
        SkScalar gf = g / 255.0f;
        SkScalar bf = b / 255.0f;

        SkScalar max = std::max(rf, std::max(gf, bf));
        float min = std::min(rf, std::min(gf, bf));
        float deltaMaxMin = max - min;

        SkScalar h,s ;
        SkScalar l = (max + min) /2.0f ;

        if(max == min){
            h = s = 0.0f ;
        }else{
            if(max == rf){
                h = SkIntToScalar(SkScalarRoundToInt((gf - bf) / deltaMaxMin) % 6) ;
            }else if(max == gf){
                h = ((bf - rf) / deltaMaxMin) + 2.0f;
            }else{
                h = ((rf - gf) / deltaMaxMin) + 4.0f;
            }
            s = deltaMaxMin / (1.0f - std::abs(2.0f * l - 1.0f));
        }

        h = SkIntToScalar(SkScalarRoundToInt(h * 60) % 360);
        if (h < 0) {
            h += 360.0f;
        }

        hsl[0] = constrain(h, 0.0f, 360.0f);
        hsl[1] = constrain(s, 0.0f, 1.0f);
        hsl[2] = constrain(l, 0.0f, 1.0f);
    }

    SkColor HSLToColor(U8CPU a,const SkScalar hsl[3]){
         SkScalar h = hsl[0];
         SkScalar s = hsl[1];
         SkScalar l = hsl[2];

         SkScalar c = (1.0f - std::abs(2 * l - 1.0f)) * s;
         SkScalar m = l - 0.5f * c;
         SkScalar x = c * (1.0f - std::abs((SkScalarRoundToInt(h / 60.0f) % 2) - 1.0f));

         int hueSegment = SkScalarRoundToInt(h / 60);

         int r = 0, g = 0, b = 0;

        switch (hueSegment) {
            case 0:
                r = std::round(255 * (c + m));
                g = std::round(255 * (x + m));
                b = std::round(255 * m);
                break;
            case 1:
                r = std::round(255 * (x + m));
                g = std::round(255 * (c + m));
                b = std::round(255 * m);
                break;
            case 2:
                r = std::round(255 * m);
                g = std::round(255 * (c + m));
                b = std::round(255 * (x + m));
                break;
            case 3:
                r = std::round(255 * m);
                g = std::round(255 * (x + m));
                b = std::round(255 * (c + m));
                break;
            case 4:
                r = std::round(255 * (x + m));
                g = std::round(255 * m);
                b = std::round(255 * (c + m));
                break;
            case 5:
            case 6:
                r = std::round(255 * (c + m));
                g = std::round(255 * m);
                b = std::round(255 * (x + m));
                break;
        }

        r = constrain(r, 0, 255);
        g = constrain(g, 0, 255);
        b = constrain(b, 0, 255);

        return SkColorSetARGB(a,r,g,b);
    }

    void ColorBlender(RenderScriptToolkit::BlendingMode blendingMode, const SkScalar inHsl[3],
                      SkScalar dstHsl[3]) {
        SkASSERT(inHsv);
        SkASSERT(dstHsv);

        switch (blendingMode) {
            case RenderScriptToolkit::BlendingMode::HUE: {
                dstHsl[0] = inHsl[0];
            }
                break;
            case RenderScriptToolkit::BlendingMode::SATURATION: {
                dstHsl[1] = inHsl[1];
            }
                break;
            case RenderScriptToolkit::BlendingMode::COLOR: {
                dstHsl[0] = inHsl[0];
                dstHsl[1] = inHsl[1];
            }
                break;
            case RenderScriptToolkit::BlendingMode::LUMINOSITY: {
                dstHsl[2] = inHsl[2];
            }
                break;

        }
    }
}



