////////////////////////////////////////////////////////////////////////////////////////
// ADCTest Copyright and License
// ADCTest is in the public domain and is licensed under a 3-Clause BSD license
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <cmath>
#include <iostream>
#include <map>
#include <wx/wx.h>
#include "Utils\MathUtilities.h"

enum WindowType {
    RectangularWindow = 0,
    BartlettWindow,
    HammingWindow,
    HanningWindow,
    BlackmanWindow,
    GaussianWindow,
    ParzenWindow,
	Kaiser7Window
};

template <typename T>
class Window
{
public:
    /**
     * Construct a windower of the given type.
     */
    Window(WindowType type, size_t size) : m_type(type), m_size(size) { build(); }
    Window(const Window &w) : m_type(w.m_type), m_size(w.m_size) { build(); }
    Window &operator=(const Window &w) {
	if (&w == this) return *this;
	m_type = w.m_type;
	m_size = w.m_size;
	build();
	return *this;
    }
    virtual ~Window() { delete m_cache; }

    void cut(T *src) const { cut(src, src); }
    void cut(const T *src, T *dst) const {
	for (size_t i = 0; i < m_size; ++i) dst[i] = src[i] * m_cache[i];
    }

    WindowType getType() const { return m_type; }
    size_t getSize() const { return m_size; }

protected:
    WindowType m_type;
    size_t m_size;
    T *m_cache;

    void build();
};

template <typename T>
void Window<T>::build()
{
    size_t n = m_size;
    T *mult = new T[n];
    size_t i;
    for (i = 0; i < n; ++i) mult[i] = 1.0;

    switch (m_type) {

		case RectangularWindow:
		for (i = 0; i < n; ++i) {
			mult[i] = mult[i] * 0.5;
		}
		break;

		case BartlettWindow:
		for (i = 0; i < n/2; ++i) {
			mult[i] = mult[i] * (i / T(n/2));
			mult[i + n/2] = mult[i + n/2] * (1.0 - (i / T(n/2)));
		}
		break;

		case HammingWindow:
		for (i = 0; i < n; ++i) {
			mult[i] = mult[i] * (0.54 - 0.46 * cos(2 * M_PI * i / n));
		}
		break;

		case HanningWindow:
		for (i = 0; i < n; ++i) {
			mult[i] = mult[i] * (0.50 - 0.50 * cos(2 * M_PI * i / n));
		}
		break;

		case BlackmanWindow:
		for (i = 0; i < n; ++i) {
			mult[i] = mult[i] * (0.42 - 0.50 * cos(2 * M_PI * i / n) + 0.08 * cos(4 * M_PI * i / n));
		}
		break;

		case GaussianWindow:
		for (i = 0; i < n; ++i) {
			mult[i] = mult[i] * exp((-1.0 / (n*n)) * ((T(2*i) - n) * (T(2*i) - n)));
		}
		break;

		case ParzenWindow:
		for (i = 0; i < n; ++i) {
			mult[i] = mult[i] * (1.0 - fabs((T(2*i) - n) / T(n + 1)));
		}
		break;

		case Kaiser7Window:
		{
			double beta = M_PI*7;
			double bb = MathUtilities::bessel_i0(beta);
			for (i = 0; i < n; ++i)
			{
				mult[i] = mult[i]* T(MathUtilities::bessel_i0(beta*sqrt(4.0*i*(n - 1 - i)) / (n - 1)) / bb);
			}
		}
		break;
    }

    m_cache = mult;
}

#endif
