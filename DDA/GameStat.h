#ifndef _GAMESTAT_H_
#define _GAMESTAT_H_

#define MAX_INT 1000000

class PlayerStat
{
	long choisesSum, choisesMin, choisesMax;
	long winner;
public :
	const PlayerStat operator+(const PlayerStat &other)
	{
		PlayerStat newPlayerStat;
		newPlayerStat.choisesSum = choisesSum + other.choisesSum;
		newPlayerStat.choisesMin = choisesMin + other.choisesMin;
		newPlayerStat.choisesMax = choisesMax + other.choisesMax;
		newPlayerStat.winner = winner + other.winner;
		return newPlayerStat;
	}
	void SetWinner() {
		winner = 1;
	}
	void UpdateChoises(int choises)
	{
		choisesSum += choises;
		if(choises < choisesMin)
			choisesMin = choises;
		if(choises > choisesMax)
			choisesMax = choises;
	}
	void Reset() {
		choisesSum = 0;
		choisesMin = MAX_INT;
		choisesMax = 0;
		winner = 0;
	}

	long Winner() { return winner; };
};

class GameStat
{
private :
	long numberPlayers;
	// counts all plie number - environmental ai too
	long long turnNumber;
	// doesnt count environmental ai turn
	long long turnNumberReal;

	long long leaderSwitches;
	long long sumScoreDifference;
	long long endScoreDifference;

	PlayerStat * playerStat;

	void CopyToMe(const GameStat &origin);
public:
	GameStat();
	GameStat(int _numberPlayers);
	GameStat(const GameStat &origin);
	~GameStat(void);
	GameStat& GameStat::operator=(const GameStat &origin);
	const GameStat operator+(const GameStat &other);

	void Reset();
	void AddTurnNumber() { turnNumber++; }
	void AddTurnNumberReal() { turnNumberReal++; }
	void AddLeaderSwitch() { leaderSwitches++; };
	void AddScoreDifference(int difference) { sumScoreDifference += difference; };
	void SetEndScoreDifference(int difference) { endScoreDifference = difference; };
	void SetWinner(int playerID) { playerStat[playerID].SetWinner(); };
	void UpdatePlayerChoises(int playerID, int playerChoises) { playerStat[playerID].UpdateChoises(playerChoises); };

	long long TurnNumber() { return turnNumber; };
	long long TurnNumberReal() { return turnNumberReal; };
	long long LeaderSwitches() { return leaderSwitches; };
	long long SumScoreDifference() { return sumScoreDifference;};
	long long EndScoreDifference() { return endScoreDifference;};
	long PlayerWinner(int playerID) { return playerStat[playerID].Winner(); };
};

#endif