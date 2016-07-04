#include "Note.h"

Note::Note(int scaleNum, int octave, int accidental)
	: scaleNum(scaleNum), octave(octave), accidental(accidental)
{

}

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
