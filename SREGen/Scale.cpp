#include "Scale.h"

// Constructs a major scale.
Scale Scale::generateMajorScale(string scale)
{
	Scale temp = Scale(scale);
	auto isFlatKeySig = [](string scale)->bool
	{
		return (scale == "f") || (scale.length() > 1 && scale.at(1) == 'e');
	};

	vector<string> keyboard = (isFlatKeySig(scale)) ? keyboardFlat : keyboardSharp;

	int kbIndex = keyIndex(scale, keyboard);
	int curr = 0;

	while (curr < majorPattern.size())
	{
		if (temp.keys[curr].at(0) == keyboard[kbIndex].at(0))
		{
			temp.keys[curr] = keyboard[kbIndex];
		}
		else if ((temp.keys[curr].at(0) < keyboard[kbIndex].at(0))
			|| (temp.keys[curr].at(0) == 'g' && keyboard[kbIndex].at(0) == 'a'))
		{
			temp.keys[curr].append("is");
		}
		else
		{
			temp.keys[curr].append("es");
		}

		if (kbIndex + majorPattern[curr] >= keyboard.size())
		{
			kbIndex = kbIndex + majorPattern[curr] - keyboard.size();
		}
		else 
		{
			kbIndex = kbIndex + majorPattern[curr];
		}
		curr++;
	}
	if (temp.keys[curr].at(0) == keyboard[kbIndex].at(0))
	{
		temp.keys[curr] = keyboard[kbIndex];
	}
	else if ((temp.keys[curr].at(0) < keyboard[kbIndex].at(0))
		|| (temp.keys[curr].at(0) == 'g' && keyboard[kbIndex].at(0) == 'a'))
	{
		temp.keys[curr].append("is");
	}
	else
	{
		temp.keys[curr].append("es");
	}

	return temp;
}

// Constructs a natural minor scale.
Scale Scale::generateMinorScale(string scale)
{
	auto isFlatKeySig = [](string scale)->bool
	{
		return (scale == "f") 
			|| (scale == "c")
			|| (scale == "g")
			|| (scale == "d")
			|| (scale.length() > 1 && scale.at(1) == 'e');
	};
	vector<string> keyboard = (isFlatKeySig(scale)) ? keyboardFlat : keyboardSharp;
	int kbIndex = keyIndex(scale, keyboard);
	int curr = 0;

	while (curr < 2)
	{
		if (kbIndex + minorPattern[curr] >= keyboard.size())
		{
			kbIndex = kbIndex + minorPattern[curr] - keyboard.size();
		}
		else
		{
			kbIndex = kbIndex + majorPattern[curr];
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

	temp.keys = newKeys;
	return temp;
}

string Scale::operator[](Note note)
{
	string letter = keys[note.getScaleNum()];

	switch (note.getAccidental())
	{
	case -1:
		if (letter.length() > 1 && letter.at(1) == 'i')
		{
			letter = letter.substr(0, letter.length() - 2);
		}
		else
		{
			letter.append("es");
		}
		break;

	case 1:
		if (letter.length() > 1 && letter.at(1) == 'e')
		{
			letter = letter.substr(0, letter.length() - 2);
		}
		else
		{
			letter.append("is");
		}
		break;
	}

	int offset = (keys[0] >= "fis" || keys[0] <= "bes") ? -1 : 0;

	int octaveChange = 0;
	if ((keys[0] < "c" && letter > "c") || (letter >= "c" && letter < keys[0])) {
		octaveChange = 1;
	}
	int octave = note.getOctave() + offset + octaveChange;

	while (octave != 3)
	{
		if (octave > 3)
		{
			octave--;
			letter.append("'");
		}
		else
		{
			octave++;
			letter.append(",");
		}
	}

	letter.append(to_string(note.getDuration()));

	return letter;
}

Scale::Scale(string scale) 
	: keys(7) 
{
	char curr = scale.at(0);
	keys[0] = string(1, curr);
	for (int i = 1; i < 7; i++) 
	{
		curr++;
		if (curr == 'h') 
		{
			curr = 'a';
		}
		keys[i] = string(1, curr);
	}
}

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

// Natural minor scale pattern
const vector<int> Scale::minorPattern =
	vector<int>{ 2, 1, 2, 2, 1, 2 };