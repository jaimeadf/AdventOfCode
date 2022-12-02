#include <iostream>
#include <fstream>

enum Shape {
    Rock,
    Paper,
    Scissors
};

enum RoundResult {
    PlayerA,
    Draw,
    PlayerB
};

class Round {
private:
    Shape m_PlayerA;
    Shape m_PlayerB;

public:
    Round(Shape playerA, Shape playerB) : m_PlayerA(playerA), m_PlayerB(playerB) {

    }

    int CalculatePlayerAScore() {
        return CalculateMoveScore(m_PlayerA) + CalculateResultScore(RoundResult::PlayerA);
    }

    int CalculatePlayerBScore() {
        return CalculateMoveScore(m_PlayerB) + CalculateResultScore(RoundResult::PlayerB);
    }

    RoundResult CheckResult() {
        if (m_PlayerA == m_PlayerB) {
            return RoundResult::Draw;
        }

        if (m_PlayerA == GetWinnerShapeOf(m_PlayerB)) {
            return RoundResult::PlayerA;
        }

        return RoundResult::PlayerB;
    }
    
    static Shape GetWinnerShapeOf(Shape shape) {
        return (Shape)((shape + 1) % 3);
    }

    static Shape GetLoserShapeOf(Shape shape) {
        return (Shape)((shape + 3 - 1) % 3);
    }

private:
    int CalculateMoveScore(Shape shape) {
        return shape + 1;
    }

    int CalculateResultScore(RoundResult expectedWinner) {
        RoundResult result = CheckResult();

        if (result == RoundResult::Draw) {
            return 3;
        }

        if (result == expectedWinner) {
            return 6;
        }

        return 0;
    }
};

Shape SolveShapeToPlay(Shape opponent, RoundResult result) {
    if (result == RoundResult::Draw) {
        return opponent;
    }

    if (result == RoundResult::PlayerA) {
        return Round::GetLoserShapeOf(opponent);
    }

    return Round::GetWinnerShapeOf(opponent);
}

Shape DecodePlayerAShape(char character) {
    return (Shape)(character - 'A');
}

Shape DecodePlayerBShape(char character) {
    return (Shape)(character - 'X');
}

RoundResult DecodeResult(char character) {
    return (RoundResult)(character - 'X');
}

void PartOne() {
    std::ifstream inputFile("input.txt");

    std::string line;
    int totalScore = 0;

    while (std::getline(inputFile, line)) {
        char characterA = line[0];
        char characterB = line[2];

        Round round(DecodePlayerAShape(characterA), DecodePlayerBShape(characterB));

        totalScore += round.CalculatePlayerBScore();
    }

    std::cout << "Part One: " << totalScore << std::endl;
}

void PartTwo() {
    std::ifstream inputFile("input.txt");

    std::string line;
    int totalScore = 0;

    while (std::getline(inputFile, line)) {
        char characterA = line[0];
        char characterResult = line[2];

        RoundResult result = DecodeResult(characterResult);

        Shape playerA = DecodePlayerAShape(characterA);
        Shape playerB = SolveShapeToPlay(playerA, result);        

        Round round(playerA, playerB);

        totalScore += round.CalculatePlayerBScore();
    }

    std::cout << "Part Two: " << totalScore << std::endl;
}

int main() {
    PartOne();
    PartTwo();
}