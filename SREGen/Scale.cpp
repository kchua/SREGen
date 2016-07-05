#include "Scale.h"

// Constructs a major scale.
Scale Scale::generateMajorScale(string scale)
{
	Scale temp = Scale();
	auto isFlatKeySig = [](string scale)->bool
	{
		return (scale == "f") || (scale.length > 1 && scale.at(1) == 'e');
	};

	vector<string> keyboard = (isFlatKeySig(scale)) ? keyboardFlat : keyboardSharp;
	int kbIndex = keyIndex(scale, keyboard);
	int curr = 0;

	while (curr < majorPattern.size)
	{
		temp.keys[curr] = keyboard[kbIndex];
		if (kbIndex + majorPattern[curr] > keyboard.size)
		{
			kbIndex = kbIndex + majorPattern[curr] - keyboard.size + 1;
		}
		curr++;
	}

	return temp;
}

// Constructs a harmonic minor scale.
Scale Scale::generateMinorScale(string scale)
{
	auto isFlatKeySig = [](string scale)->bool
	{
		return (scale == "f") 
			|| (scale == "c")
			|| (scale == "g")
			|| (scale == "d")
			|| (scale.length > 1 && scale.at(1) == 'e');
	};
	vector<string> keyboard = (isFlatKeySig(scale)) ? keyboardFlat : keyboardSharp;
	int kbIndex = keyIndex(scale, keyboard);
	int curr = 0;

	while (curr < 2)
	{
		if (kbIndex + minorPattern[curr] > keyboard.size)
		{
			kbIndex = kbIndex + minorPattern[curr] - keyboard.size + 1;
		}
		curr++;
	}

	Scale temp = generateMajorScale(keyboard[kbIndex]);
	vector<string> newKeys = vector<string>(7);

	// Shift to minor
	for (int i = 5; i < 7; i++)
	{
		newKeys[i - 5] = temp.keys[i];
	}
	for (int i = 0; i < 5; i++)
	{
		newKeys[i + 2] = temp.keys[i];
	}

	// Modify for harmonic minor
	if (newKeys[7].length == 3)
	{
		newKeys[7] = newKeys[7].substr(0, 1);
	}
	else
	{
		newKeys[7] = newKeys[7].append("is");
	}

	temp.keys = newKeys;
	return temp;
}

string& Scale::operator[](Note note)
{

}

Scale::Scale() : keys(7) {};

/* Finds where a key is on the keyboard. */
int Scale::keyIndex(string scale, const vector<string>& keyboard)
{
	int i = 0;
	while (keyboard[i] != scale)
	{
		i++;
	}
	return i;
}

// The keyboard, with black keys viewed as sharps.
const vector<string> Scale::keyboardSharp = 
	vector<string>{ "c", "cis", "d", "dis", "e", "f", "fis", "g", "gis", "a", "ais", "b" };

// The keyboard, with black keys viewed as flats.
const vector<string> Scale::keyboardFlat =
	vector<string>{ "c", "des", "d", "ees", "e", "f", "ges", "g", "aes", "a", "bes", "b" };

// Major scale pattern
const vector<int> Scale::majorPattern =
	vector<int>{ 2, 2, 1, 2, 2, 2 };

// Harmonic minor scale pattern
const vector<int> Scale::minorPattern =
	vector<int>{ 2, 1, 2, 2, 1, 3 };