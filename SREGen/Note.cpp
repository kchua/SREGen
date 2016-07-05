#include "Note.h"

/* Note class member initialization*/
Note::Note(int scaleNum, int octave, int accidental)
	: scaleNum(scaleNum), octave(octave), accidental(accidental) {}

/* Moves the note up the scale. */
Note Note::operator++(int)
{
	if (scaleNum < 6)
	{
		scaleNum++;
	}
	else
	{
		scaleNum = 0;
		octave++;
	}
}

/* Moves the note down the scale. */
Note Note::operator--(int)
{
	if (scaleNum > 0)
	{
		scaleNum--;
	}
	else
	{
		scaleNum = 6;
		octave--;
	}
}
