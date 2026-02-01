// mz917edkeus9-safety.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include <tchar.h>
#include <ctime>
//#include <iostream>
#include <windowsx.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"msimg32.lib")
#include <math.h>
#include <time.h>
//#include "bootrec.h"
#define M_PI   3.14159265358979323846264338327950288
//typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
//typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
	};
}_RGBQUAD, *PRGBQUAD;
typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	//These HSL functions was made by Wipet, credits to him!
	//OBS: I used it in 3 payloads

	//Btw ArTicZera created HSV functions, but it sucks unfortunatelly
	//So I didn't used in this malware.

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed;
		BYTE g = rgb.rgbGreen;
		BYTE b = rgb.rgbBlue;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = min(min(_r, _g), _b);
		FLOAT rgbMax = max(max(_r, _g), _b);

		FLOAT fDelta = rgbMax - rgbMin;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { //Credits to Void_/GetMBR
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
}
COLORREF RndRGB() {
	int clr = rand() % 5;
	if (clr == 0) return RGB(255, 0, 0); if (clr == 1) return RGB(0, 255, 0); if (clr == 2) return RGB(0, 0, 255); if (clr == 3) return RGB(255, 0, 255); if (clr == 4) return RGB(255, 255, 0);
}

DWORD WINAPI noskid1(LPVOID lpParam) {
	HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	BITMAPINFO bmi = { 40, sw, sh, 1, 24 };
	PRGBTRIPLE rgbtriple;
	for (;;) {
		desk = GetDC(0);
		HDC deskMem = CreateCompatibleDC(desk);
		HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
		SelectObject(deskMem, scr);
		BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
		for (int i = 0; i < sw * sh; i++) {
			int x = i % sw, y = i / sh, t = y ^ y | x;
			rgbtriple[i].rgbtRed = x & y;
			rgbtriple[i].rgbtGreen += x ^ y;
			rgbtriple[i].rgbtBlue += t + i;
		}
		BitBlt(desk, 0, 0, sw, sh, deskMem, -30, 0, SRCCOPY);
		BitBlt(desk, 0, 0, sw, sh, deskMem, sw - 30, 0, SRCCOPY);
		ReleaseDC(wnd, desk);
		DeleteDC(desk); DeleteDC(deskMem); DeleteObject(scr); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&bmi);
	}
}

DWORD WINAPI noskid2(LPVOID lpParam) {
	HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	BITMAPINFO bmi = { 40, sw, sh, 1, 24 };
	PRGBTRIPLE rgbtriple;
	for (;;) {
		desk = GetDC(0);
		HDC deskMem = CreateCompatibleDC(desk);
		HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
		SelectObject(deskMem, scr);
		BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
		for (int i = 0; i < sw * sh; i++) {
			int x = i % sw, y = i / sh, t = y ^ y | x;
			int average = round((float)(rgbtriple[i].rgbtBlue + rgbtriple[i].rgbtRed + rgbtriple[i].rgbtGreen) / 3);
			int fx = (int)((8 * i) + ((8 * i) * sin(x / 32.0)) + (4 * i) + ((4 * i) * sin(y / 24.0)));

			rgbtriple[i].rgbtRed -= average - fx;
			rgbtriple[i].rgbtGreen = average + fx;
			rgbtriple[i].rgbtRed -= average - fx;
		}
		BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
		BitBlt(desk, rand() % 5, rand() % 5, sw, sh, deskMem, rand() % 5, rand() % 5, NOTSRCCOPY);
		BitBlt(desk, rand() % 5, rand() % 5, rand() % sw, rand() % sh, deskMem, rand() % 5, rand() % 5, NOTSRCCOPY);
		ReleaseDC(wnd, desk);
		DeleteDC(desk); DeleteDC(deskMem); DeleteObject(scr); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&bmi);
	}
}

VOID WINAPI ci0(int x, int y, int w, int h)
{
	HDC hdc = GetDC(0);
	HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
	if ((rand() % 2) == 0) SelectClipRgn(hdc, hrgn);
	HBRUSH brush = CreateSolidBrush(RGB(rand() % 2000, rand() % 2000, rand() % 2000));
	SelectObject(hdc, brush);
	BitBlt(hdc, x, y, w, h, hdc, x, y, 0x2837E28);
	DeleteObject(hrgn);
	DeleteObject(brush);
	ReleaseDC(NULL, hdc);
}

DWORD WINAPI noskid21(LPVOID lpParam)
{
	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	int w = rect.right - rect.left - 500, h = rect.bottom - rect.top - 500;

	for (int t = 0;; t++)
	{
		int size = rand() % 4000 + 1;
		int x = rand() % (w + size) - size / 1, y = rand() % (h + size) - size / 1;

		for (int i = 0; i < size; i += 100)
		{
			ci0(x - i / 2, y - i / 2, i, i);
		}
		Sleep(100);
	}
}

DWORD WINAPI noskid22(LPVOID lpParam)
{
	HDC hdc;
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	HFONT hfnt;
	while (true) {
		hdc = GetDC(0);
		CHAR username[256];
		DWORD dwUserSize = sizeof(username);
		GetUserNameA(username, &dwUserSize);
		hfnt = CreateFontA(43, 32, rand() % 3600, rand() % 3600, FW_MEDIUM, false, false, false, ANSI_CHARSET, 0, 0, 0, 0, "Fredoka");
		SelectObject(hdc, hfnt);
		SetBkMode(hdc, 0);
		SetTextColor(hdc, RGB(rand() % 1000, rand() % 1000, rand() % 1000));
		TextOutA(hdc, rand() % w, rand() % h, username, strlen(username));
		DeleteObject(hfnt);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI noskid3(LPVOID lpParam)
{
	HDC desk = GetDC(0);
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	RECT rekt; POINT wPt[3];
	while (1) {
		GetWindowRect(GetDesktopWindow(), &rekt);
		wPt[0].x = rand() % sw; wPt[0].y = rand() % sh;
		wPt[1].x = rand() % sw; wPt[1].y = rand() % sh;
		wPt[2].x = rand() % sw; wPt[2].y = rand() % sh;
		PlgBlt(desk, wPt, desk, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, 0, 0, 0);
	}
}

DWORD WINAPI noskid31(LPVOID lpParam)
{
	HDC hdc = GetDC(NULL);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);


	while (1)
	{
		hdc = GetDC(NULL);
		// Create an array of TRIVERTEX structures that describe 
		// positional and color values for each vertex. For a rectangle, 
		// only two vertices need to be defined: upper-left and lower-right. 
		TRIVERTEX vertex[2];
		vertex[0].x = rand() % w;
		vertex[0].y = rand() % h;
		vertex[0].Red = 0x7500;
		vertex[0].Green = 0x5f00;
		vertex[0].Blue = 0x5000;
		vertex[0].Alpha = 0x0000;

		vertex[1].x = rand() % w;
		vertex[1].y = rand() % h;
		vertex[1].Red = 0x9000;
		vertex[1].Green = 0xff00;
		vertex[1].Blue = 0xff00;
		vertex[1].Alpha = 0x0000;

		// Create a GRADIENT_RECT structure that 
		// references the TRIVERTEX vertices. 
		GRADIENT_RECT gRect;
		gRect.UpperLeft = 0;
		gRect.LowerRight = 1;

		// Draw a shaded rectangle. 
		GradientFill(hdc, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
		ReleaseDC(0, hdc);
		Sleep(1);
	}

	return 0x00;
}

DWORD WINAPI noskid4(LPVOID lpParam) {
	double angle = 0.0;
	while (true) {
		HDC hdc = GetDC(0);
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		int xadd = (int)(tan(angle) * 100.0);
		int yadd = (int)(cos(angle) * 100.0);
		SetStretchBltMode(hdc, 3);
		StretchBlt(hdc, xadd / 2, yadd / 2, w - xadd, h - yadd, hdc, 0, 0, w, h, NOTSRCCOPY);
		angle = angle + 0.1963495408493621;
		DeleteObject(hdc);
		ReleaseDC(0, hdc);
	}
}

VOID WINAPI ci(int x, int y, int w, int h)
{
	HDC hdc = GetDC(0);
	HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, 0, 0));
	SelectObject(hdc, brush);
	BitBlt(hdc, x, y, w, h, hdc, x, y, 0x123456);
	DeleteObject(brush);
	ReleaseDC(NULL, hdc);
}

VOID WINAPI ci1(int x, int y, int w, int h)
{
	HDC hdc = GetDC(0);
	HBRUSH brush = CreateSolidBrush(RGB(0, rand() % 255, 0));
	SelectObject(hdc, brush);
	BitBlt(hdc, x, y, w, h, hdc, x, y, 0x123456);
	DeleteObject(brush);
	ReleaseDC(NULL, hdc);
}

VOID WINAPI ci2(int x, int y, int w, int h)
{
	HDC hdc = GetDC(0);
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, rand() % 255));
	SelectObject(hdc, brush);
	BitBlt(hdc, x, y, w, h, hdc, x, y, 0x123456);
	DeleteObject(brush);
	ReleaseDC(NULL, hdc);
}

DWORD WINAPI noskid41(LPVOID lpParam)
{
	HDC desk = GetDC(0);
	const int size = 3000;
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	int i = 0; i < size; i += 100;
	while (1) {


		int radius = 10, rx = rand() % sw, ry = rand() % sh, num = rand() % 5;

		for (int t = 0; t < sw + sh; ++t) {
			int x = (int)(float)(radius + t * sin(t + radius * 3) + rx),

				y = (int)(float)(radius + t * cos(t + radius * 3) + ry);

			if (rand() % 5 == 0) { ci(x, y, i, i); Sleep(0.99); }

			if (rand() % 5 == 0) { ci1(x - i / 1, y - i / 1, i, i); Sleep(0.99); }

			if (rand() % 5 == 0) { ci2(x - i / 1, y - i / 1, i, i); Sleep(0.99); }


		}

		InvalidateRect(0, 0, 0);

	}

}

DWORD WINAPI noskid5(LPVOID lpParam) {
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;
	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = w;
	bmpi.bmiHeader.biHeight = h;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdcCopy, bmp);
	INT i = 0;
	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, NOTSRCCOPY);
		RGBQUAD rgbquadCopy;
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				int index = y * w + x;
				int Xii = x | x, Yii = y * y;
				int inc = 2 + i;
				int fx = (Xii ^ Yii + inc | Xii * Yii + inc) + (i * 50);
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 30.f + y / h * .1f + i / 5000.f, 1.f);
				hslcolor.s = 0.75f;
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}

		i++;
		StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI noskid6(LPVOID lpParam)
{
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, rand() % 10, rand() % 10, w, h, hdcScreen, rand() % 10, rand() % 10, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			int average = round((float)(rgbScreen[i].b + rgbScreen[i].r + rgbScreen[i].g) / 10);
			rgbScreen[i].r += average;
			rgbScreen[i].g += average + average;
			rgbScreen[i].b -= average - average;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, NOTSRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

VOID WINAPI train(HDC hdc, int w, int h, int xPower, int yPower, DWORD dwRop) {
	if (xPower >= w) xPower = w - 1; if (yPower >= h) yPower = h - 1;
	HBITMAP screenshot = CreateCompatibleBitmap(hdc, w, h);
	HDC hdc2 = CreateCompatibleDC(hdc); rand() % 6;
	SelectObject(hdc2, screenshot);
	BitBlt(hdc2, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
	BitBlt(hdc, xPower > 0 ? xPower : 0, yPower > 0 ? yPower : 0, w - abs(xPower), h - abs(yPower), hdc, xPower < 0 ? -xPower : 0, yPower < 0 ? -yPower : 0, dwRop);
	BitBlt(hdc, xPower < 0 ? w + xPower : 0, 0, abs(xPower), h, hdc2, xPower > 0 ? w - xPower : 0, 0, dwRop);
	BitBlt(hdc, 0, yPower < 0 ? h + yPower : 0, w, abs(yPower), hdc2, 0, yPower > 0 ? h - yPower : 0, dwRop);
	DeleteDC(hdc2); DeleteObject(screenshot);
}

DWORD WINAPI noskid61(LPVOID lpParam) { //NO credits to N17Pro3426
	HDC hdcdc = GetDC(0);
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	while (1) {
		hdcdc = GetDC(0);
		//SelectObject(desk, CreateSolidBrush(RGB(245, 245, 66)));
		for (int angle = 0; angle < 361; angle++) {
			int x = 150 * sin(angle * M_PI / 180.F), y = 150 * tan(angle * M_PI / 180.F);
			train(hdcdc, sw, sh, x, y, NOTSRCCOPY);
			Sleep(0.5);
		}
		ReleaseDC(0, hdcdc);
	}
}

DWORD WINAPI noskid7(LPVOID lpParam) {
	int sw = GetSystemMetrics(0);
	int sh = GetSystemMetrics(0);

	while (true)
	{
		HDC hdc = GetDC(0);
		BitBlt(hdc, rand() % 10 - 20, rand() % 20 - 10, sh, sw, hdc, 0, 0, NOTSRCCOPY);
		BitBlt(hdc, rand() % 10 - 20, rand() % 20 - 10, sw, sh, hdc, 0, 0, SRCINVERT);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI noskid71(LPVOID lpParam) {
	int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
	LPCSTR text = 0;
	while (1)
	{
		for (int i = 0; i < x; i++) {
			HDC hdc = GetDC(0);
			text = "NO SKID";
			TextOutA(hdc, rand() % x, rand() % y, text, strlen(text));
			ReleaseDC(0, hdc);
		}
		Sleep(1000);
	}
}

DWORD WINAPI noskid8(LPVOID lpParam) //by the noskid
{
	HDC desk = GetDC(0);
	HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = sw;
	bmi.bmiHeader.biHeight = -sh;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	PRGBTRIPLE rgbtriple;
	for (;;)
	{
		desk = GetDC(0);
		HDC deskMem = CreateCompatibleDC(desk);
		HBITMAP scr = CreateDIBSection(desk, &bmi, DIB_RGB_COLORS, (void**)&rgbtriple, NULL, 0);
		SelectObject(deskMem, scr);
		StretchBlt(deskMem, 1, 1, sw + 2, sh + 2, desk, 0, 0, sw, sh, SRCCOPY);
		//BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, NOTSRCERASE);
		for (int i = 0; i < sw * sh; i++)
		{
			int x = i % sw;
			int y = i - sw;
			int t = y ^ y | x;
			rgbtriple[i].rgbtRed = (rgbtriple[i].rgbtRed + GetRValue(i)) & i;
			rgbtriple[i].rgbtGreen = (rgbtriple[i].rgbtGreen + GetGValue(i)) + i;
			rgbtriple[i].rgbtBlue = (rgbtriple[i].rgbtBlue + GetBValue(i)) & i;
		}
		BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);

		ReleaseDC(wnd, desk);
		DeleteDC(deskMem);
		DeleteObject(scr);
	}
}

DWORD WINAPI noskid81(LPVOID lpParam)
{
	HDC desk;
	int sw, sh;

	while (1) {
		desk = GetDC(0);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		StretchBlt(desk, 0, -20, sw, sh + 40, desk, 0, 0, sw, sh, SRCCOPY);
		ReleaseDC(0, desk);
		Sleep(1);
	}
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 9 ^ (t >> 9) - 1 ^ 1) % 13 * t & 31) * (2 + (t >> 4));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((t >> 7 ^ (t >> 30) - 1 ^ 1) % 1563));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t >> 9 | t | t) + 44 * (t >> 13 & t | t));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(104 * (t | t | t >> 24) - 90 * (t | t >> 8));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(150 & (t >> 6 | t | t >> (t >> 16)) + (7 & t >> 11));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * ((t >> 12 | t >> 6) & 69 & t >> 2) | t * 4 | t * (t >> 9));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t >> 3 * t >> 3 | t >> 3 * t * (t >> 3 * t >> 4 >> 5) & t >> 4 | t >> 3 * (t >> 2));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * 5321 + t * 13 ^ t * (t >> 5) & t);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
)
{
	if (MessageBoxW(NULL, L"Warning! You have ran a trojan known as mz917edkeus9-safety that has full capacity to undelete all of your data and your operating system. By continuing, you keep in mind that the creator (Underwater Tiny Kong & DareenYounes) will not be responsible for any undamage caused by this trojan and it's not highly recommended that you run this in a testing virtual machine where a snapshot has been made before execution for the sake of entertainment and analysis. Are you sure you want to run this?", L"mz917edkeus9-safety.exe (noskid)", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"Final warning! This trojan has a lot of safe potential. You will lose all of your data if you continue and the creator (Underwater Tiny Kong & DareenYounes) will not be responsible for any of the damage caused. This is not meant to be malicious, but simply for entertainment and educational purposes. Are you sure you want to continue? This is your final chance to stop this program from execution.", L"mz917edkeus9-safety.exe (noskid)", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			Sleep(5000);
			HANDLE thread1 = CreateThread(0, 0, noskid1, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			InvalidateRect(0, 0, 0);
			HANDLE thread2 = CreateThread(0, 0, noskid2, 0, 0, 0);
			HANDLE thread2dot1 = CreateThread(0, 0, noskid21, 0, 0, 0);
			HANDLE thread2dot2 = CreateThread(0, 0, noskid22, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			TerminateThread(thread2dot1, 0);
			CloseHandle(thread2dot1);
			TerminateThread(thread2dot2, 0);
			CloseHandle(thread2dot2);
			InvalidateRect(0, 0, 0);
			HANDLE thread3 = CreateThread(0, 0, noskid3, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, noskid31, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			TerminateThread(thread3dot1, 0);
			CloseHandle(thread3dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread4 = CreateThread(0, 0, noskid4, 0, 0, 0);
			HANDLE thread4dot1 = CreateThread(0, 0, noskid41, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			TerminateThread(thread4dot1, 0);
			CloseHandle(thread4dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread5 = CreateThread(0, 0, noskid5, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			InvalidateRect(0, 0, 0);
			HANDLE thread6 = CreateThread(0, 0, noskid6, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, noskid61, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			TerminateThread(thread6dot1, 0);
			CloseHandle(thread6dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread7 = CreateThread(0, 0, noskid7, 0, 0, 0);
			HANDLE thread7dot1 = CreateThread(0, 0, noskid71, 0, 0, 0);
			sound7();
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			TerminateThread(thread7dot1, 0);
			CloseHandle(thread7dot1);
			InvalidateRect(0, 0, 0);
			HANDLE thread8 = CreateThread(0, 0, noskid8, 0, 0, 0);
			HANDLE thread8dot1 = CreateThread(0, 0, noskid81, 0, 0, 0);
			sound8();
			Sleep(30000);
			TerminateThread(thread8, 0);
			CloseHandle(thread8);
			TerminateThread(thread8dot1, 0);
			CloseHandle(thread8dot1);
			InvalidateRect(0, 0, 0);
			ExitProcess(0);
			Sleep(-1);
		}
	}
}