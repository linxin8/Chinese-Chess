using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessGame
{
    public struct Position
    {
        public int x;
        public int y;
        static public Position operator +(Position left, Position right)
        {
            return new Position { x = left.x + right.x, y = left.y + right.y };
        }
        static public Position operator -(Position left, Position right)
        {
            return new Position { x = left.x - right.x, y = left.y - right.y };
        }
    }
    public class Rule
    { 
        public enum ChessType
        {
            King,
            Guard,
            Elephant,
            Rook,
            Knight,
            Cannon,
            Pawn
        }
        public enum Country 
        {
            Black,
            Red
        }  
        public const int MapRow = 10;
        public const int MapColomn = 9;
        public static bool IsLegalPosition(int x, int y) => x >= 0 && y < MapColomn && y >= 0 && y < MapRow;
        public static bool IsLegalPosition(Position position) => IsLegalPosition(position.y, position.y);
    } 
}