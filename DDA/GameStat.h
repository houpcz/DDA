#ifndef _GAMESTAT_H_
#define _GAMESTAT_H_

#define MAX_INT 1000000

class PlayerStat
{
	long long choisesSum, choisesMin, choisesMax;
	long long winner;
	long long turnNumber;
public :
	const PlayerStat operator+(const PlayerStat &other)
	{
		PlayerStat newPlayerStat;
		newPlayerStat.choisesSum = choisesSum + other.choisesSum;
		newPlayerStat.choisesMin = choisesMin + other.choisesMin;
		newPlayerStat.choisesMax = choisesMax + other.choisesMax;
		newPlayerStat.winner = winner + other.winner;
		newPlayerStat.turnNumber = turnNumber + other.turnNumber;
		return newPlayerStat;
	}
	void SetWinner() {
		winner = 1;
	}
	void AddTurn()
	{
		turnNumber++;
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
		turnNumber = 0;
	}

	long long Winner() { return winner; };
	long long ChoisesMin() { return choisesMin; };
	long long ChoisesMax() { return choisesMax; };
	long long ChoisesSum() { return choisesSum; };
	long long TurnNumber() { return turnNumber; };
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

	void AddPlayerTurnNumber(int playerID) { playerStat[playerID].AddTurn(); };
	void SetWinner(int playerID) { playerStat[playerID].SetWinner(); };
	void UpdatePlayerChoises(int playerID, int playerChoises) { playerStat[playerID].UpdateChoises(playerChoises); };

	long NumberPlayers() { return numberPlayers; };
	long long TurnNumber() { return turnNumber; };
	long long TurnNumberReal() { return turnNumberReal; };
	long long LeaderSwitches() { return leaderSwitches; };
	long long SumScoreDifference() { return sumScoreDifference;};
	long long EndScoreDifference() { return endScoreDifference;};
	long PlayerWinner(int playerID) { return playerStat[playerID].Winner(); };
	long long PlayerChoisesMin(int playerID) { return playerStat[playerID].ChoisesMin(); };
	long long PlayerChoisesMax(int playerID) { return playerStat[playerID].ChoisesMax(); };
	long long PlayerChoisesSum(int playerID) { return playerStat[playerID].ChoisesSum(); };
	long long PlayerTurnNumber(int playerID) { return playerStat[playerID].TurnNumber(); };
};

#endif