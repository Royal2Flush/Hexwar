#include "AdditionalFunctions.hpp"

string toString(float input, int decimals)
{
	string result;
	ostringstream convert;
	convert << fixed << setprecision(decimals) << input;
	result = convert.str();
	return result;
}

string toString(int input)
{
	string result;
	ostringstream convert;
	convert << input;
	result = convert.str();
	return result;
}

int sgn(int x)
{
	if (x != 0)
		return x / abs(x);
	else
		return 0;
}