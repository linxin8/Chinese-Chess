using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessGame
{
    public abstract class Chess
    {
        protected readonly LogicMap logicMap;
        public abstract Rule.ChessType Type { get; }
        public Rule.Country Country { get; }
        public abstract string ChineseName { get; }
        public abstract List<Position> GetAccessiblePosition();
        public Position Position
        {
            get => new Position { x = X, y = Y };
            set { X = value.x; Y = value.y; }
        }
        public int X { get; set; }
        public int Y { get; set; }
        public Chess(LogicMap logicMap, int x, int y, Rule.Country country)
        {
            Country = country;
            this.logicMap = logicMap;
            X = x;
            Y = y;
        }
        public Chess(LogicMap logicMap, Position position, Rule.Country country) : this(logicMap, position.x, position.y, country)
        {
        } 
    }

    public class KingChess : Chess
    {
        public override Rule.ChessType Type => Rule.ChessType.King;
        public override string ChineseName => "将";
        private readonly List<Position> relativeMoveList = new List<Position>();
        public override List<Position> GetAccessiblePosition()
        {
            var list = new List<Position>();
            foreach (var v in relativeMoveList)
            {
                var des = v + Position;
                var minX = 3;
                var maxX = 5;
                int minY = Country == Rule.Country.Black ? 0 : 7;
                int maxY = minY + 2;
                if (minX <= des.x && des.x <= maxX && minY <= des.y && des.y <= maxY)
                {
                    var chess = logicMap.GetChess(des);
                    if (chess == null || chess.Country != Country)
                    {
                        list.Add(des);
                    } 
                }
            }
            return list;
        }
        public KingChess(LogicMap logicMap, int x, int y, Rule.Country country) : base(logicMap, x, y, country)
        { 
            relativeMoveList.Add(new Position { x = 1, y = 0 });
            relativeMoveList.Add(new Position { x = -1, y = 0 });
            relativeMoveList.Add(new Position { x = 0, y = 1 });
            relativeMoveList.Add(new Position { x = 0, y = -1 });

        }
        public KingChess(LogicMap logicMap, Position position, Rule.Country country) : this(logicMap, position.x, position.y, country)
        {
        }
    }
    
    public class GuardChess:Chess
    {
        public override Rule.ChessType Type => Rule.ChessType.Guard;
        public override string ChineseName => "士";
        private readonly List<Position> relativeMoveList = new List<Position>();
        public override List<Position> GetAccessiblePosition()
        {
            var list = new List<Position>();
            foreach (var v in relativeMoveList)
            {
                var des = v + Position;
                var minX = 3;
                var maxX = 5;
                int minY = Country == Rule.Country.Black ? 0 : 7;
                int maxY = minY + 2;
                if (minX <= des.x && des.x <= maxX && minY <= des.y && des.y <= maxY)
                {
                    var chess = logicMap.GetChess(des);
                    if (chess == null||chess.Country!=Country)
                    {
                        list.Add(des); 
                    }
                }
            }
            return list;
        }
        public GuardChess(LogicMap logicMap, int x, int y, Rule.Country country) : base(logicMap, x, y, country)
        {
            relativeMoveList.Add(new Position { x = 1, y = 1 });
            relativeMoveList.Add(new Position { x = -1, y = -1 });
            relativeMoveList.Add(new Position { x = 1, y = -1 });
            relativeMoveList.Add(new Position { x = -1, y = 1 });
        }
        public GuardChess(LogicMap logicMap, Position position, Rule.Country country) : this(logicMap, position.x, position.y, country)
        {
        }
    }

    public class ElephantChess : Chess
    {
        public override Rule.ChessType Type => Rule.ChessType.Elephant;
        public override string ChineseName => "象";
        private readonly List<Position> relativeMoveList = new List<Position>();
        private Position Hinder(Position p)
        {
            return new Position
            {
                x = p.x > 0 ? 1 : -1,
                y = p.y > 0 ? 1 : -1
            };
        }
        public override List<Position> GetAccessiblePosition()
        {
            var list = new List<Position>();
            foreach (var v in relativeMoveList)
            {
                var des = v + Position;
                var minX = 0;
                var maxX = 8;
                int minY = Country == Rule.Country.Black ? 0 : 5;
                int maxY = minY + 4;
                if (minX <= des.x && des.x <= maxX && minY <= des.y && des.y <= maxY)
                {
                    if (logicMap.GetChess(Position + Hinder(v)) != null)
                    {
                        continue;
                    }
                    var chess = logicMap.GetChess(des);
                    if (chess == null || chess.Country != Country)
                    {
                        list.Add(des);
                    }
                }
            }
            return list;
        }
        public ElephantChess(LogicMap logicMap, int x, int y, Rule.Country country) : base(logicMap, x, y, country)
        {
            relativeMoveList.Add(new Position { x = 2, y = 2 });
            relativeMoveList.Add(new Position { x = -2, y = -2 });
            relativeMoveList.Add(new Position { x = 2, y = -2 });
            relativeMoveList.Add(new Position { x = -2, y = 2 });
        }
        public ElephantChess(LogicMap logicMap, Position position, Rule.Country country) : this(logicMap, position.x, position.y, country)
        {
        }
    }
    public class RookChess : Chess
    {
        public override Rule.ChessType Type => Rule.ChessType.Rook;
        public override string ChineseName => "车";
        public override List<Position> GetAccessiblePosition()
        {
            void commonFunction(int xOff,int yOff, List<Position> positions)
            { 
                for(int xTemp = X + xOff, yTemp = Y + yOff; 0 <= xTemp && xTemp < 9 && 0 <= yTemp && yTemp < 10;xTemp+=xOff,yTemp+=yOff)
                {
                    var des = new Position { x = xTemp, y = yTemp };
                    var chess = logicMap.GetChess(des);
                    if (chess == null)
                    {
                        positions.Add(des);
                    }
                    else
                    {
                        var country = logicMap.GetChess(des).Country;
                        if (country != Country)
                        {
                            positions.Add(des);
                        }
                        return;
                    } 
                } 
            }
            var list = new List<Position>();
            commonFunction(-1, 0, list);
            commonFunction(1, 0, list);
            commonFunction(0, 1, list);
            commonFunction(0, -1, list);
            return list;
        }
        public RookChess(LogicMap logicMap, int x, int y, Rule.Country country) : base(logicMap, x, y, country)
        { 

        }
        public RookChess(LogicMap logicMap, Position position, Rule.Country country) : this(logicMap, position.x, position.y, country)
        {
        } 
    }
    public class CannonChess : Chess
    {
        public override Rule.ChessType Type => Rule.ChessType.Cannon;
        public override string ChineseName => "炮";
        public override List<Position> GetAccessiblePosition()
        {
            void commonFunction(int xOff, int yOff, List<Position> positions)
            {
                for (int xTemp = X + xOff, yTemp = Y + yOff; 0 <= xTemp && xTemp < 9 && 0 <= yTemp && yTemp < 10; xTemp += xOff, yTemp += yOff)
                {
                    var des = new Position { x = xTemp, y = yTemp };
                    var chess = logicMap.GetChess(des);
                    if (chess == null)
                    {
                        positions.Add(des);
                    }
                    else
                    {
                        for (xTemp += xOff, yTemp += yOff; 0 <= xTemp && xTemp < 9 && 0 <= yTemp && yTemp < 10; xTemp += xOff, yTemp += yOff)
                        {
                            var subDes = new Position { x = xTemp, y = yTemp };
                            var subChess = logicMap.GetChess(subDes);
                            if (subChess != null)
                            {
                                if (subChess.Country != Country)
                                {
                                    positions.Add(subDes);
                                }
                                return;
                            }
                        }
                    }
                }
            } 
            var list = new List<Position>();
            commonFunction(-1, 0, list);
            commonFunction(1, 0, list);
            commonFunction(0, -1, list);
            commonFunction(0, 1, list);
            return list;
        }
        public CannonChess(LogicMap logicMap, int x, int y, Rule.Country country) : base(logicMap, x, y, country)
        {
        }
        public CannonChess(LogicMap logicMap, Position position, Rule.Country country) : this(logicMap, position.x, position.y, country)
        {
        }
    }
    public class KnightChess : Chess
    {
        public override Rule.ChessType Type => Rule.ChessType.Knight;
        public override string ChineseName => "马";
        private readonly List<Position> relativeMoveList = new List<Position>();
        public override List<Position> GetAccessiblePosition()
        {
            Position Hinder(Position p)
            {
                var result = new Position(); 
                if(p.x==2)
                {
                    result.x = 1;
                }
                else if(p.x==-2)
                {
                    result.x = -1;
                }
                else
                {
                    result.x = 0;
                }
                if (p.y == 2)
                {
                    result.y = 1;
                }
                else if (p.y == -2)
                {
                    result.y = -1;
                }
                else
                {
                    result.y = 0;
                }
                return result;
            }
            var list = new List<Position>();
            foreach (var v in relativeMoveList)
            {
                var des = v + Position;
                if (0 <= des.x && des.x <= 8 && 0 <= des.y && des.y <= 9)
                {
                    if (logicMap.GetChess(Position + Hinder(v)) != null)
                    {
                        continue;
                    }
                    var chess = logicMap.GetChess(des);
                    if (chess == null || chess.Country != Country)
                    {
                        list.Add(des);
                    }
                }
            }
            return list;
        }
        public KnightChess(LogicMap logicMap, int x, int y, Rule.Country country) : base(logicMap, x, y, country)
        {
            relativeMoveList.Add(new Position { x = 2, y = 1 });
            relativeMoveList.Add(new Position { x = -2, y = 1 });
            relativeMoveList.Add(new Position { x = 2, y = -1 });
            relativeMoveList.Add(new Position { x = -2, y = -1 });
            relativeMoveList.Add(new Position { x = 1, y = 2 });
            relativeMoveList.Add(new Position { x = -1, y = 2 });
            relativeMoveList.Add(new Position { x = 1, y = -2 });
            relativeMoveList.Add(new Position { x = -1, y = -2 });
        }
        public KnightChess(LogicMap logicMap, Position position, Rule.Country country) : this(logicMap, position.x, position.y, country)
        {
        }
    }
    public class PawnChess : Chess
    {
        public override Rule.ChessType Type => Rule.ChessType.Pawn;
        public override string ChineseName => "兵";
        public override List<Position> GetAccessiblePosition()
        {
            var list = new List<Position>();
            var relativeMoveList = new List<Position>();
            if (Country == Rule.Country.Black)
            {
                if (Y >= 5)
                {
                    relativeMoveList.Add(new Position { x = 1, y = 0 });
                    relativeMoveList.Add(new Position { x = -1, y = 0 });
                }
                relativeMoveList.Add(new Position { x = 0, y = 1 });
            }
            else
            {
                if (Y <= 4)
                {
                    relativeMoveList.Add(new Position { x = 1, y = 0 });
                    relativeMoveList.Add(new Position { x = -1, y = 0 });
                }
                relativeMoveList.Add(new Position { x = 0, y = -1 });
            }
            foreach (var v in relativeMoveList)
            {
                var des = v + Position;
                if (0 <= des.x && des.x <= 8 && 0 <= des.y && des.y <= 9)
                {
                    var chess = logicMap.GetChess(des);
                    if (chess == null || chess.Country != Country)
                    {
                        list.Add(des);
                    }
                    list.Add(des);
                }
            }
            return list;
        }
        public PawnChess(LogicMap logicMap, int x, int y, Rule.Country country) : base(logicMap, x, y, country)
        {
        }
        public PawnChess(LogicMap logicMap, Position position, Rule.Country country) : this(logicMap, position.x, position.y, country)
        {
        }
    }
}
