#include "Event.h"
#include <unordered_map>
#include <map>
#include <fstream>
#include <stdlib.h>
#include <iomanip>

//main player's statistics

#ifndef PLAYER_
#define PLAYER_

class Player
{
private:
	int totalWin=0;
	int totalLoss=0;
	int totalDraw=0;
public:
	std::vector<match> matchHistory;
	std::map<std::string, opponent> opponentRecord;
	std::map<std::string, eventFormat> formatRecord;
	//std::unordered_map<std::string, opponent> opponentRecord;
	//std::unordered_map<std::string, eventFormat> formatRecord;
	void setTotalWin(int win) { totalWin = win; }
	void setTotalLoss(int loss) { totalLoss = loss; }
	void setTotalDraw(int draw) { totalDraw = draw; }
	void incrementWin() { totalWin++; }
	void incrementLoss() { totalLoss++; }
	void incrementDraw() { totalDraw++; }
	int getWins() { return totalWin; }
	int getLosses() { return totalLoss; }
	int getDraws() { return totalDraw; }


	//void readData(std::ifstream in);
	void readData();
	void calculateStats();

};

//struct match
//{
//	std::string date;
//	std::string eventName;
//	std::string store;
//	std::string format;
//	std::string location;
//
//	std::string outcome;		//win loss draw
//	std::string opponent;		//opponent's name
//};


//dirty infile
//void Player::readData(std::ifstream infile)
void Player::readData()
{
	std::string tempFile;
	std::cout << "Enter file name to read" << std::endl;
	std::getline(std::cin, tempFile);
	std::ifstream infile(tempFile.c_str());
	match baseMatch;
	std::string temp;
	std::string currentline;
	std::getline(infile, temp);

	while (temp != "Hasbro")			//while not eof
	{
		std::size_t findFormat = temp.find("Format:");
		while (findFormat == std::string::npos)		//while "Format" is not found
		{
			std::getline(infile, temp);			//get the next line until it leads with "Format:"
			findFormat = temp.find("Format:");
			if (temp == "Hasbro")
				break;
		}
		if (temp == "Hasbro")
			break;
		temp.erase(temp.begin(), temp.begin() + 8);	//remove "Format: "
		baseMatch.format = temp;
		std::getline(infile, temp);
		temp.erase(temp.begin(), temp.begin() + 10);	//remove Location: "
		baseMatch.location = temp;
		std::getline(infile, temp);		//clear Place:
		std::getline(infile, temp);		//clear sanctioning number
		std::getline(infile, temp);		//clear Match History:
		int matchNum = 1;
		//match baseMatch = tempMatch;
		std::getline(infile, temp);		//read first match
		std::size_t findPlanes = temp.find("Planeswalker Points Earned:");		//find planeswalker points earned
		std::size_t findTeammates = temp.find("Teammates:");
		while (findPlanes == std::string::npos && findTeammates == std::string::npos)	//while in match history, need to keep all of the above information
		{
			match tempMatch = baseMatch;		//create a new match to add
			
			int numCheck = (int)temp[0] - 48;		
			if (numCheck != matchNum)
				std::getline(infile, temp);		//if there was an opponent or if first match of event, then get the next line with match outcome
			std::size_t findWin = temp.find("Win");
			if (findWin != std::string::npos)	//if "Win" found	
			{
				incrementWin();
				tempMatch.outcome = "Win";
			}
			std::size_t findBye = temp.find("Bye");
			if (findBye != std::string::npos)
			{
				incrementWin();
				tempMatch.outcome = "Win";
			}
			std::size_t findLoss = temp.find("Loss");
			if (findLoss != std::string::npos)
			{
				incrementLoss();
				tempMatch.outcome = "Loss";
			}
			std::size_t findDraw = temp.find("Draw");
			if (findDraw != std::string::npos)
			{
				incrementDraw();
				tempMatch.outcome = "Draw";
			}
			matchNum++;
			std::getline(infile, temp);		//gets either opponent name, next match number, or Planeswalker Points
			findPlanes = temp.find("Planeswalker Points Earned:");
			findTeammates = temp.find("Teammates:");
			if (findPlanes == std::string::npos && findTeammates == std::string::npos)// if Planeswalker Points Earned is not found AND findteammates is not found
			{
				int num = (int)temp[0] - 48;		//change first char of line to number
				if (num != matchNum)		//if there is an opponent or it doesn't go onto the next match
				{
					tempMatch.opponent = temp;
				}
				else
					tempMatch.opponent = "Unknown opponent";
			}
			else
			{

				tempMatch.opponent = "Unknown opponent";
			}
			/////////////////// TO DO /////////////////////
			//add match to opponent dictionary
			std::map<std::string, opponent>::iterator opIt = opponentRecord.find(tempMatch.opponent);
			if (opIt == opponentRecord.end())			//if opponent of that name is not found, add them
			{
				opponent tempOp;
				tempOp.opponentName = tempMatch.opponent;
				if (tempMatch.outcome == "Win")
					tempOp.wins++;
				if (tempMatch.outcome == "Loss")
					tempOp.losses++;
				if (tempMatch.outcome == "Draw")
					tempOp.draws++;
				opponentRecord.emplace(tempOp.opponentName, tempOp);
			}
			else				//if opponent is found, update win/loss/draw
			{
				if (tempMatch.outcome == "Win")
					opIt->second.wins++;
				if (tempMatch.outcome == "Loss")
					opIt->second.losses++;
				if (tempMatch.outcome == "Draw")
					opIt->second.draws++;
			}
			//add match to format dictionary
			std::map<std::string, eventFormat>::iterator eventIt = formatRecord.find(tempMatch.format);
			if (eventIt == formatRecord.end())			//if format not found
			{
				eventFormat tempEv;
				tempEv.formatType = tempMatch.format;
				if (tempMatch.outcome == "Win")
					tempEv.wins++;
				if (tempMatch.outcome == "Loss")
					tempEv.losses++;
				if (tempMatch.outcome == "Draw")
					tempEv.draws++;
				formatRecord.emplace(tempEv.formatType, tempEv);
			}
			else
			{
				if (tempMatch.outcome == "Win")
					eventIt->second.wins++;
				if (tempMatch.outcome == "Loss")
					eventIt->second.losses++;
				if (tempMatch.outcome == "Draw")
					eventIt->second.draws++;
			}
			matchHistory.push_back(tempMatch);		//add match to matchHistory
			if (tempMatch.opponent == "Unknown opponent")
			{
				findPlanes = temp.find("Planeswalker Points Earned:");
			}
			else
			{
				std::getline(infile, temp);		//read the next line
				findPlanes = temp.find("Planeswalker Points Earned:");
				findTeammates = temp.find ("Teammates:");
			}
		}//end while not planeswalker points
	}//end of file
}
void Player::calculateStats()
{
	//overall W/L
	std::ofstream outfile;
	std::string filename;
	std::cout << "Enter an output file name" << std::endl;
	std::getline(std::cin, filename);
	outfile.open(filename.c_str());
	float WLRatio;
	WLRatio = (float)((float)getWins() / ((float)getLosses()+(float)getWins()));
	if (getWins() + getLosses() == 0)
	{
		std::cout << "No wins or losses on record" << std::endl;
		std::cout << "Total Draws: " << getDraws() << std::endl << std::endl;
		outfile << "No wins or losses on record" << std::endl;
		outfile << "Total Draws: " << getDraws() << std::endl << std::endl;
	}
	else
	{
		std::cout << "============= Overall win ratio (wins/losses): " <<std::setprecision(5) << std::fixed <<  (float)WLRatio << " =============" <<  std::endl;
		std::cout << "Total Wins: " << getWins() << std::endl;
		std::cout << "Total Losses: " << getLosses() << std::endl;
		std::cout << "Total Draws: " << getDraws() << std::endl << std::endl;
		std::cout << "============= Wins by opponent =============" << std::endl << std::endl;
		outfile << "============= Overall win ratio (wins/losses): " << std::setprecision(5) << std::fixed << (float)WLRatio << " =============" << std::endl;
		outfile << "Total Wins: " << getWins() << std::endl;
		outfile << "Total Losses: " << getLosses() << std::endl;
		outfile << "Total Draws: " << getDraws() << std::endl << std::endl;
		outfile << "============= Wins by opponent =============" << std::endl << std::endl;
		for (auto it = opponentRecord.cbegin(); it != opponentRecord.cend(); it++)
		{
			std::cout << "Opponent name: " << it->first << std::endl;
			outfile << "Opponent name: " << it->first << std::endl;
			if (it->second.wins + it->second.losses == 0)
			{
				std::cout << "All matches resulted in a draw with this opponent" << std::endl;
				std::cout << "Draws against this opponent: " << it->second.draws << std::endl << std::endl;
				outfile << "All matches resulted in a draw with this opponent" << std::endl;
				outfile << "Draws against this opponent: " << it->second.draws << std::endl << std::endl;
			}
			else
			{
				WLRatio = (float)((float)it->second.wins / ((float)it->second.losses + (float)it->second.wins));
				std::cout << "Win % against this opponent: " << WLRatio << std::endl;
				std::cout << "Wins against this opponent: " << it->second.wins << std::endl;
				std::cout << "Losses against this opponent: " << it->second.losses << std::endl;
				std::cout << "Draws against this opponent: " << it->second.draws << std::endl << std::endl;
				outfile << "Win % against this opponent: " << WLRatio << std::endl;
				outfile << "Wins against this opponent: " << it->second.wins << std::endl;
				outfile << "Losses against this opponent: " << it->second.losses << std::endl;
				outfile << "Draws against this opponent: " << it->second.draws << std::endl << std::endl;
			}
		}
		std::cout << std::endl;
		std::cout << "============= Wins by format =============" << std::endl;
		outfile << std::endl;
		outfile << "============= Wins by format =============" << std::endl;
		for (auto it = formatRecord.cbegin(); it != formatRecord.cend(); it++)
		{
			std::cout << "Format name: " << it->first << std::endl;
			outfile << "Format name: " << it->first << std::endl;
			WLRatio = (float)((float)it->second.wins / ((float)it->second.losses + (float)it->second.wins));
			std::cout << "Win % in this format: " << WLRatio << std::endl;
			std::cout << "Wins in this format: " << it->second.wins << std::endl;
			std::cout << "Losses in this format: " << it->second.losses << std::endl;
			std::cout << "Draws in this format: " << it->second.draws << std::endl << std::endl;
			outfile << "Win % in this format: " << WLRatio << std::endl;
			outfile << "Wins in this format: " << it->second.wins << std::endl;
			outfile << "Losses in this format: " << it->second.losses << std::endl;
			outfile << "Draws in this format: " << it->second.draws << std::endl << std::endl;
		}
	}
}





#endif
