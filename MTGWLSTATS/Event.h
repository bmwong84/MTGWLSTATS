#include <iostream>
#include <string>
#include <vector>

//keeping track of specific event type draft, standard, modern, sealed etc.
struct eventFormat
{
	std::string formatType;
	int wins = 0;
	int losses = 0;
	int draws = 0;
};

//keeping track of specific opponent
struct opponent
{
	std::string opponentName;
	int wins = 0;
	int losses = 0;
	int draws = 0;
};


struct match
{
	std::string date;
	std::string eventName;
	std::string store;
	std::string format;
	std::string location;
	
	std::string outcome;		//win loss draw
	std::string opponent;		//opponent's name
};


