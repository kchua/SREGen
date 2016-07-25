#ifndef NOTE_H
#define NOTE_H

/* A note forms the basic building block of the musical 
   chromosome. */
class Note {
public:
	Note(int scaleNum, int octave, int duration, int accidental = 0);
	Note(int scaleNum, int accidental = 0);

	Note& operator++();
	Note& operator--();
	Note operator++(int);
	Note operator--(int);
	bool operator==(Note other);

	void octaveUp();
	void octaveDown();
	void setOctave(int octave);
	int getOctave();

	int getScaleNum();
	int getAccidental();
	int getDuration();

private:
	int scaleNum, octave, accidental, duration;
};

#endif