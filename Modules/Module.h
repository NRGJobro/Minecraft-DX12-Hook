#pragma once

class Module {
public:
	Module(std::string category = "null", std::string name = "null", std::string tooltip = "null", uintptr_t keybind = 0x0, bool enabled = false) {
		this->category = category;
		this->name = name;
		this->tooltip = tooltip;
		this->keybind = keybind;
		this->enabled = enabled;
	};

public:
	std::string category;
	std::string name;
	std::string tooltip;
	uintptr_t keybind;
	bool enabled;

public:

	virtual void OnImGuiRender() {
	}
	virtual void OnEnable() {
	}
	virtual void OnDisable() {
	}

	double _pow(double a, double b) {
		double c = 1;
		for (int i = 0; i < b; i++)
			c *= a;
		return c;
	}

	double _fact(double x) {
		double ret = 1;
		for (int i = 1; i <= x; i++)
			ret *= i;
		return ret;
	}

	float sin(float x) {
		float res = 0, pow = x, fact = 1;
		for (int i = 0; i < 5; i++) {
			res += pow / fact;
			pow *= -1 * x * x;
			fact *= (2 * (i + 1)) * (2 * (i + 1) + 1);
		}
		return res;
	}

	double cos(double x) {
		double y = 1;
		double s = -1;
		for (int i = 2; i <= 100; i += 2) {
			y += s * (_pow(x, i) / _fact(i));
			s *= -1;
		}
		return y;
	}
};