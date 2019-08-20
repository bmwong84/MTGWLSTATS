
#include "Player.h"


int main()
{
	std::vector <Player> playerList;
	char addAnother = 'y';
	do
	{
	Player tempPlayer;
	//std::string tempFile;
	//std::cout << "Enter file name to read" << std::endl;
	//std::getline(std::cin, tempFile);
	//std::ifstream instream(tempFile.c_str());
	//instream.open(tempFile.c_str());
	//tempPlayer.readData(instream);				//deleted function????
	tempPlayer.readData();
	tempPlayer.calculateStats();
	playerList.push_back(tempPlayer);
	std::cout << "Add another player?" << std::endl;
	std::cin >> addAnother;
	std::cin.ignore();
	} while (addAnother == 'y' || addAnother == 'Y');

}