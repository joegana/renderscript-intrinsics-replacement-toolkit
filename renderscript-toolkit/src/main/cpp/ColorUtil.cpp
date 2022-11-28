//
// Created by admin on 2022/11/28.
//

#include "ColorUtil.h"

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
    SkScalar hx = (hsv[0] < 0 || hsv[0] >= SkIntToScalar(360)) ? 0 : hsv[0]/60;
    SkScalar w = SkScalarFloorToScalar(hx);
    SkScalar f = hx - w;

    unsigned p = SkScalarRoundToInt((SK_Scalar1 - s) * v * 255);
    unsigned q = SkScalarRoundToInt((SK_Scalar1 - (s * f)) * v * 255);
    unsigned t = SkScalarRoundToInt((SK_Scalar1 - (s * (SK_Scalar1 - f))) * v * 255);

    unsigned r, g, b;

    SkASSERT((unsigned)(w) < 6);
    switch ((unsigned)(w)) {
        case 0: r = v_byte;  g = t;      b = p; break;
        case 1: r = q;       g = v_byte; b = p; break;
        case 2: r = p;       g = v_byte; b = t; break;
        case 3: r = p;       g = q;      b = v_byte; break;
        case 4: r = t;       g = p;      b = v_byte; break;
        default: r = v_byte; g = p;      b = q; break;
    }
    return SkColorSetARGB(a, r, g, b);
}
