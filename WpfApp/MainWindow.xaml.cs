using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ChessGame
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private LogicMap logicMap = new LogicMap();
        private Chess lastSelectedChess = null;


        public MainWindow()
        {
            InitializeComponent();
            Init();
            DLLTest();
        }

        public void DLLTest()
        {
            //var testMap = new int[10, 9]
            //{
            //     {13,14,15,16,17,16,15,13,00},
            //     {00,00,00,00,00,00,00,00,00},
            //     {00,00,00,00,00,00,14,00,00},
            //     {11,00,11,00,00,00,11,00,11},
            //     {00,00,00,00,00,00,00,00,12},
            //     {00,00,00,00,22,00,00,00,00},
            //     {21,00,21,00,24,00,21,00,21},
            //     {23,00,00,00,00,00,00,22,25},
            //     {00,00,00,00,00,00,00,00,00},
            //     {00,00,25,26,27,26,00,24,23}
            //};
            //var testMap = new int[10, 9]
            //{
            //     {00,00,15,16,17,16,00,14,13},
            //     {00,00,00,00,00,00,00,00,00},
            //     {13,00,00,00,00,00,00,23,15},
            //     {11,00,11,00,14,00,11,00,11},
            //     {00,00,00,00,12,00,00,00,00},
            //     {00,00,00,00,00,00,00,00,22},
            //     {21,00,21,00,00,00,21,00,21},
            //     {00,00,00,00,00,00,24,00,00},
            //     {00,00,00,00,00,00,00,00,00},
            //     {23,24,25,26,27,26,25,00,00}
            //};           
            //var testMap = new int[10, 9]
            //{
            //     {13,14,15,16,17,16,15,14,13},
            //     {00,00,00,00,00,00,00,00,00},
            //     {00,12,00,00,00,00,00,12,00},
            //     {11,00,11,00,11,00,11,00,11},
            //     {00,00,00,00,00,00,00,00,00},
            //     {00,00,00,00,00,00,00,00,00},
            //     {21,00,21,00,21,00,21,00,21},
            //     {00,22,00,00,00,00,00,22,00},
            //     {00,00,00,00,00,00,00,00,00},
            //     {23,24,25,26,27,26,25,24,23}
            //};
            //var testMap = new int[10, 9]
            //{
            //     {13,14,15,16,17,16,15,14,13},
            //     {00,00,00,00,00,00,00,00,00},
            //     {00,12,00,00,00,00,00,12,00},
            //     {11,00,11,00,11,00,11,00,11},
            //     {00,00,00,00,00,00,00,00,00},
            //     {00,00,00,00,00,00,21,00,00},
            //     {21,00,21,00,21,00,00,00,21},
            //     {00,22,00,00,00,00,00,22,00},
            //     {00,00,00,00,00,00,00,00,00},
            //     {23,24,25,26,27,26,25,24,23}
            //};
            var testMap = new int[10, 9] {
//{00,00,00,16,17,16,15,00,00,}, 
//{14,00,13,00,00,00,00,00,00,}, 
//{00,00,00,00,15,00,14,00,13,}, 
//{11,00,00,00,11,24,23,00,11,}, 
//{00,00,00,00,00,00,00,00,00,}, 
//{00,00,00,00,00,00,00,00,00,}, 
//{12,00,00,00,21,00,21,00,21,}, 
//{00,00,22,00,22,00,24,00,00,}, 
//{00,00,00,00,00,00,00,12,00,}, 
//{00,23,25,26,27,26,25,00,00,},
{00,13,15,16,17,16,00,00,00,},
{00,00,00,00,00,00,00,00,00,},
{00,00,00,00,15,00,00,23,00,},
{11,12,00,00,11,00,00,13,11,},
{00,12,11,00,00,00,00,00,00,},
{21,00,00,00,00,24,00,00,00,},
{00,00,21,00,21,00,00,00,21,},
{24,00,22,00,22,00,00,00,00,},
{00,00,00,00,00,00,00,00,00,},
{00,00,25,26,27,26,25,00,00,},
            };
            //logicMap.LoadMap(testMap);
            //            var dllMap = logicMap.GetDLLMap();
            //DecisionTreeDll.GetDecision(testMap, out DecisionTreeDllDecision decision);
            //var version = DecisionTreeDll.GetVersion();
            //var name = DecisionTreeDll.GetName(); 
            //Console.WriteLine($"{decision.fromX},{decision.fromY}->{decision.desX},{decision.desY}");
            //var positions = DecisionTreeDll.GetAccessiblePosition(logicMap.GetDLLMap(), 0, 0);
            //Console.WriteLine($"{positions.Count}");
        }

        public void Init()
        { 
            logicMap.MapUpdate += Board.UpdateViewCentent;
            logicMap.ForceTriggerMapUpdate();
            Board.BoardMouseUp += OnBorderMouseUp; 
        }

        private void OnAITurn()
        {
            var dllMap = logicMap.GetDLLMap();
            DecisionTreeDll.GetDecision(dllMap, out DecisionTreeDllDecision decision);
            var firstPosition = new Position
            {
                x = decision.fromX,
                y = decision.fromY,
            };
            var secondPosition = new Position
            {
                x = decision.desX,
                y = decision.desY,
            };
            logicMap.MoveChess(firstPosition, secondPosition);
        }

        private void OnBorderMouseUp(int x, int y)
        {
            var chess = logicMap.GetChess(x, y);
            var mousePosition = new Position { x = x, y = y };
            if (lastSelectedChess == null)
            {
                if (chess == null)
                {// no thing to do 
                }
                else
                {// select chess
                    Board.SelectedPostion = chess.Position;
                    //Board.HintedPosition = chess.GetAccessiblePosition();
                    Board.HintedPosition = DecisionTreeDll.GetAccessiblePosition(logicMap.GetDLLMap(), chess.Position.x, chess.Position.y);
                    lastSelectedChess = chess;  
                }
            }
            else
            {
                if (chess == null)
                {
                    //var positions = lastSelectedChess.GetAccessiblePosition();
                    var positions = DecisionTreeDll.GetAccessiblePosition(logicMap.GetDLLMap(), lastSelectedChess.Position.x, lastSelectedChess.Position.y);
                    if (positions.Contains(mousePosition))
                    {// move chess 
                        logicMap.MoveChess(lastSelectedChess.Position, mousePosition);
                        Board.SelectedPostion = null;
                        Board.HintedPosition = null;
                        lastSelectedChess = null;
                        OnAITurn();
                    }
                    else
                    {// unselect chess 
                        Board.SelectedPostion = null;
                        Board.HintedPosition = null;
                        lastSelectedChess = null;
                    }
                }
                else
                {
                    //var positions = lastSelectedChess.GetAccessiblePosition();
                    var positions = DecisionTreeDll.GetAccessiblePosition(logicMap.GetDLLMap(), lastSelectedChess.Position.x, lastSelectedChess.Position.y);
                    if (positions.Contains(mousePosition))
                    { 
                        if (lastSelectedChess.Country == chess.Country)
                        {// unselect chess 
                            Board.SelectedPostion = null;
                            Board.HintedPosition = null;
                            lastSelectedChess = null;
                        }
                        else
                        {// capture chess 
                            logicMap.MoveChess(lastSelectedChess.Position, mousePosition);
                            Board.SelectedPostion = null;
                            Board.HintedPosition = null;
                            lastSelectedChess = null;
                            OnAITurn();
                        }
                    }
                    else
                    {// unselect chess 
                        Board.SelectedPostion = null;
                        Board.HintedPosition = null;
                        lastSelectedChess = null;
                    }
                } 
            }
        }
    }
}
