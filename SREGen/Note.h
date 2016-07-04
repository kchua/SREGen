#ifndef NOTE_H
#define NOTE_H

/* A note forms the basic building block of the musical 
   chromosome. */
class Note
{
public:
	Note(int scaleNum, int octave, int accidental = 0);
	Note operator++(int);
	Note operator--(int);
private:
	int scaleNum, octave, accidental;
};

#endif