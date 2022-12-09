#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>

struct Vector2
{
    int X;
    int Y;

    Vector2(int x = 0, int y = 0) : X(x), Y(y)
    {

    }

    bool operator==(const Vector2& other) const
    {
        return X == other.X && Y == other.Y;
    }

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(X + other.X, Y + other.Y);
    }

    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(X - other.X, Y - other.Y);
    }

    struct HashFunction
    {
        size_t operator()(const Vector2& vector) const
        {
            size_t xHash = std::hash<int>()(vector.X);
            size_t yHash = std::hash<int>()(vector.Y) << 1;

            return xHash ^ yHash;
        }
    };
};

class Rope
{
private:
    std::vector<Vector2> m_Knots;

public:
    Rope(size_t length) : m_Knots(length)
    {

    }

    void MoveRight()
    {
        Move({ 1, 0 });
    }

    void MoveLeft()
    {
        Move({ -1, 0 });
    }

    void MoveUp()
    {
        Move({ 0, 1 });
    }

    void MoveDown()
    {
        Move({ 0, -1 });
    }

    const Vector2& Head() const
    {
        return m_Knots.front();
    }

    const Vector2& Tail() const
    {
        return m_Knots.back();
    }

    friend std::ostream& operator<<(std::ostream& os, const Rope& rope);

private:
    void Move(const Vector2& direction)
    {
        m_Knots[0] = m_Knots[0] + direction;

        for (size_t i = 1; i < m_Knots.size(); i++)
        {
            const Vector2& knotA = m_Knots[i - 1];
            const Vector2& knotB = m_Knots[i];

            if (IsKnotsAdjacent(knotA, knotB))
            {
                break;
            }

            m_Knots[i] = GetCloserAdjacentPosition(knotA, knotB);
        }
    }

    bool IsKnotsAdjacent(const Vector2& knotA, const Vector2& knotB) const
    {
        return knotB.X >= knotA.X - 1 && knotB.X <= knotA.X + 1
            && knotB.Y >= knotA.Y - 1 && knotB.Y <= knotA.Y + 1;
    }

    Vector2 GetCloserAdjacentPosition(const Vector2& knotA, const Vector2& knotB)
    {
        Vector2 positions[] = {
            // Row
            { knotA.X, knotA.Y + 1 },
            { knotA.X, knotA.Y - 1 },

            // Column
            { knotA.X + 1, knotA.Y },
            { knotA.X - 1, knotA.Y },

            // Diagonals
            { knotA.X - 1, knotA.Y + 1 },
            { knotA.X + 1, knotA.Y + 1 },
            { knotA.X - 1, knotA.Y -1 },
            { knotA.X + 1, knotA.Y - 1 }
        };

        for (Vector2& position : positions)
        {
            if (std::abs(knotB.X - position.X) <= 1 && std::abs(knotB.Y - position.Y) <= 1)
            {
                return position;
            }
        }

        return { 0, 0 };
    }
};

int main()
{
    std::ifstream inputFile("input.txt");
    std::string line;

    Rope ropeOne(2);
    Rope ropeTwo(10);

    std::unordered_set<Vector2, Vector2::HashFunction> ropeOneVisitedPositions;
    std::unordered_set<Vector2, Vector2::HashFunction> ropeTwoVisitedPositions;

    while (std::getline(inputFile, line))
    {
        char direction = line[0];
        int amount = std::atoi(line.c_str() + 2);

        for (int count = 0; count < amount; count++)
        {
            switch (direction)
            {
                case 'R':
                    ropeOne.MoveRight();
                    ropeTwo.MoveRight();
                    break;
                case 'L':
                    ropeOne.MoveLeft();
                    ropeTwo.MoveLeft();
                    break;
                case 'U':
                    ropeOne.MoveUp();
                    ropeTwo.MoveUp();
                    break;
                case 'D':
                    ropeOne.MoveDown();
                    ropeTwo.MoveDown();
                    break;
            }

            ropeOneVisitedPositions.insert(ropeOne.Tail());
            ropeTwoVisitedPositions.insert(ropeTwo.Tail());
        }
    }

    std::cout << "Part One: " << ropeOneVisitedPositions.size() << std::endl;
    std::cout << "Part Two: " << ropeTwoVisitedPositions.size() << std::endl;
}