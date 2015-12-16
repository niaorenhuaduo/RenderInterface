#pragma once

#ifndef _WINDEF_
typedef unsigned long DWORD;
#endif

#pragma warning( disable : 4201 )

// helper functions below the class
class RIColor {
public:
	RIColor();
	RIColor(const RIColor &rhs);
	RIColor& operator=(const RIColor &rhs); 
	RIColor(float fC);
	RIColor(float fA, float fR, float fG, float fB); 
	RIColor(float fR, float fG, float fB);
	RIColor(U8 _a, U8 _r, U8 _g, U8 _b); 
	RIColor(U8 _r, U8 _g, U8 _b); 

	operator DWORD() const { return dwColor; }
private:
	union {
		struct {
			U8 b;
			U8 g;		
			U8 r;
			U8 a;
		};
		DWORD dwColor;
	};
};

inline RIColor RICOLOR_XRGB(U8 r, U8 g, U8 b) { return RIColor(r, g, b); }
inline RIColor RICOLOR_ARGB(U8 a , U8 r, U8 g, U8 b) { return RIColor(a, r, g, b); }

const RIColor RIWhite = RICOLOR_ARGB(255, 255, 255, 255);

// Implementation
inline RIColor::RIColor() { dwColor = 0; }

inline RIColor::RIColor(const RIColor &rhs) : dwColor(rhs.dwColor) {}

inline RIColor& RIColor::operator=(const RIColor &rhs) { a = rhs.a; r = rhs.r; g = rhs.g; b = rhs.b; return *this;}

inline RIColor::RIColor(float fC) { r = g = b = static_cast<U8>(fC*255.0f); a = 255; }

inline RIColor::RIColor(float fA, float fR, float fG, float fB) { 
	a = static_cast<U8>(fA*255.0f);
	r = static_cast<U8>(fR*255.0f);
	g = static_cast<U8>(fG*255.0f);
	b = static_cast<U8>(fB*255.0f); 
}

inline RIColor::RIColor(float fR, float fG, float fB) { 
	a = 255;
	r = static_cast<U8>(fR*255.0f);
	g = static_cast<U8>(fG*255.0f);
	b = static_cast<U8>(fB*255.0f);
}

inline RIColor::RIColor(U8 _a, U8 _r, U8 _g, U8 _b) { a = _a; r = _r; g = _g; b = _b; }
inline RIColor::RIColor(U8 _r, U8 _g, U8 _b) { a = 255; r = _r; g = _g; b = _b; }

/*
// helper functions below the class
class RIColor {
public:
	RIColor() : fA(0.0f), fR(0.0f), fG(0.0f), fB(0.0f) {}
	RIColor(const RIColor &rhs) { fA = rhs.fA; fR = rhs.fR; fG = rhs.fG; fB = rhs.fB; }
	RIColor& operator=(const RIColor &rhs) { fA = rhs.fA; fR = rhs.fR; fG = rhs.fG; fB = rhs.fB; }

	RIColor(float fC) { fA = fR = fG = fB = fC; }
	RIColor(float fColor[4]) { fA = fColor[0]; fR = fColor[1]; fG = fColor[2]; fB = fColor[3]; }

	RIColor(float _fA, float _fR, float _fG, float _fB) { fA = _fA; fR = _fR; fG = _fG; fB = _fB; }
	RIColor(U8 a, U8 r, U8 g, U8 b) { fA = static_cast<float>(a)/255.0f; fR = static_cast<float>(r)/255.0f; 
									 fG = static_cast<float>(g)/255.0f; fB = static_cast<float>(b)/255.0f; }

	RIColor(float _fR, float _fG, float _fB) { fA = 1.0f; fR = _fR; fG = _fG; fB = _fB; }
	RIColor(U8 r, U8 g, U8 b) { fA = 1.0f;  fR = static_cast<float>(r)/255.0f; 
							    fG = static_cast<float>(g)/255.0f; fB = static_cast<float>(b)/255.0f; }

	operator DWORD() const {
		return ((fA >= 1.0f ? 255 : fA <= 0.0f ? 0 : DWORD(fA*255.0f) << 24) |		// can be optimized
				(fR >= 1.0f ? 255 : fR <= 0.0f ? 0 : DWORD(fR*255.0f) << 16) |		
				(fG >= 1.0f ? 255 : fG <= 0.0f ? 0 : DWORD(fG*255.0f) << 9) |
				(fB >= 1.0f ? 255 : fB <= 0.0f ? 0 : DWORD(fB*255.0f)));
	}

	U8 GetA() const { return (fA >= 1.0f ? 255 : fA <= 0.0f ? 0 : U8(fA*255.0f)); }
	U8 GetR() const { return (fR >= 1.0f ? 255 : fR <= 0.0f ? 0 : U8(fR*255.0f)); }
	U8 GetG() const { return (fG >= 1.0f ? 255 : fG <= 0.0f ? 0 : U8(fG*255.0f)); }
	U8 GetB() const { return (fB >= 1.0f ? 255 : fB <= 0.0f ? 0 : U8(fB*255.0f)); }

	float GetfA() const { return fA; }
	float GetfR() const { return fR; }
	float GetfG() const { return fG; }
	float GetfB() const { return fB; }

private:

	union {
		struct {
			float fA;
			float fR;
			float fG;
			float fB;
		};
		float fColor[4];
	};
};

inline RIColor RICOLOR_XRGB(U8 r, U8 g, U8 b) { return RIColor(r, g, b); }
inline RIColor RICOLOR_ARGB(U8 a , U8 r, U8 g, U8 b) { return RIColor(a, r, g, b); }*/