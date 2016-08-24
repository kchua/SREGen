#ifndef SREGEN_H
#define SREGEN_H

class SREGen {
public:
	static void generate(int argc, char* argv[]);
private:
	static void generateScale(int argc, char* argv[]);
	static void generateProgression(int argc, char* argv[]);
	static void generateBassLine(int argc, char* argv[]);
	static void generateTwoPartHarmony(int argc, char* argv[]);
};

#endif