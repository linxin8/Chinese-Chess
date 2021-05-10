using System;

namespace ChessGame
{
    public class LogicMap
    {
        public delegate void MapUpdateHandler(Chess[,] map);
        public event MapUpdateHandler MapUpdate;
        private readonly Chess [,] map  = new Chess[Rule.MapRow, Rule.MapColomn];
        public Chess[,] ChessMap => map;
        public LogicMap()
        {
            LoadOpeningMap();
        }
        public int[,] GetDLLMap()
        {
            var dllMap = new int[10, 9];
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    var chess = map[i, j];
                    if(chess==null)
                    {
                        dllMap[i, j] = 0;
                    }
                    else
                    {
                        var countryValue = chess.Country == Rule.Country.Black ? 10 : 20;
                        var chessValue = 0;
                        switch (chess.Type)
                        {
                            case Rule.ChessType.King:
                                chessValue = 7;
                                break;
                            case Rule.ChessType.Guard:
                                chessValue = 6;
                                break;
                            case Rule.ChessType.Elephant:
                                chessValue = 5;
                                break;
                            case Rule.ChessType.Rook:
                                chessValue = 3;
                                break;
                            case Rule.ChessType.Knight:
                                chessValue = 4;
                                break;
                            case Rule.ChessType.Cannon:
                                chessValue = 2;
                                break;
                            case Rule.ChessType.Pawn:
                                chessValue=1;
                                break;
                            default:
                                throw new NotImplementedException();
                        }
                        dllMap[i, j] = countryValue+chessValue;
                    }
                }
            }
            return dllMap;
        }
        public void LoadOpeningMap()
        { 
            var newMap = new int[10,9]
            {
                 {13,14,15,16,17,16,15,14,13},
                 {00,00,00,00,00,00,00,00,00},
                 {00,12,00,00,00,00,00,12,00},
                 {11,00,11,00,11,00,11,00,11},
                 {00,00,00,00,00,00,00,00,00},
                 {00,00,00,00,00,00,00,00,00},
                 {21,00,21,00,21,00,21,00,21},
                 {00,22,00,00,00,00,00,22,00},
                 {00,00,00,00,00,00,00,00,00},
                 {23,24,25,26,27,26,25,24,23}
            };
            LoadMap(newMap);
        }
        public void LoadMap(int[,] newMap)
        {
            for(int i=0;i<10;i++)
            {
                for(int j=0;j<9;j++)
                {
                    var country = newMap[i, j] / 10 == 1 ? Rule.Country.Black : Rule.Country.Red;
                    switch (newMap[i, j] % 10)
                    {
                        case 0:
                            SetChess(j, i,null); 
                            break;
                        case 1:
                            SetChess(j, i, Rule.ChessType.Pawn, country);
                            break;
                        case 2:
                            SetChess(j, i, Rule.ChessType.Cannon, country);
                            break;
                        case 3:
                            SetChess(j, i, Rule.ChessType.Rook, country);
                            break;
                        case 4:
                            SetChess(j, i, Rule.ChessType.Knight, country);
                            break;
                        case 5:
                            SetChess(j, i, Rule.ChessType.Elephant, country);
                            break;
                        case 6:
                            SetChess(j, i, Rule.ChessType.Guard, country);
                            break;
                        case 7: 
                            SetChess(j, i, Rule.ChessType.King, country);
                            break;
                        default:
                            throw new NotImplementedException();
                    }
                }
            }
        }
        public void ForceTriggerMapUpdate() => MapUpdate?.Invoke(map);
        public void SetChess(int x, int y, Rule.ChessType chessType,Rule.Country country)
        {
            switch (chessType) 
            {
                case Rule.ChessType.King:
                    map[y, x] = new KingChess(this, x, y, country);
                    break;
                case Rule.ChessType.Guard:
                    map[y, x] = new GuardChess(this, x, y, country);
                    break;
                case Rule.ChessType.Elephant:
                    map[y, x] = new ElephantChess(this, x, y, country);
                    break;
                case Rule.ChessType.Rook:
                    map[y, x] = new RookChess(this, x, y, country);
                    break;
                case Rule.ChessType.Knight:
                    map[y, x] = new KnightChess(this, x, y, country);
                    break;
                case Rule.ChessType.Cannon:
                    map[y, x] = new CannonChess(this, x, y, country);
                    break; 
                case Rule.ChessType.Pawn:
                    map[y, x] = new PawnChess(this, x, y, country);
                    break;
                default:
                    throw new NotSupportedException();
            }
            MapUpdate?.Invoke(map);
        }
        public void SetChess(int x, int y, Chess chess)
        {
            map[y, x] = chess;
            MapUpdate?.Invoke(map);
        }
        public void MoveChess(int x, int y, int desX,int desY)
        {
            map[desY, desX] = map[y, x];
            map[y, x].Y = desY;
            map[y, x].X = desX;
            map[y, x] = null;
            MapUpdate?.Invoke(map);
        }
        public bool HasChess(int x, int y) => map[y, x] != null;
        public bool HasChess(Position position) => HasChess(position.x, position.y);
        public void MoveChess(Position org, Position des) => MoveChess(org.x, org.y, des.x, des.y);
        public Chess GetChess(int x, int y) => map[y, x];
        public Chess GetChess(Position position) => GetChess(position.x, position.y);
       
    }
}
