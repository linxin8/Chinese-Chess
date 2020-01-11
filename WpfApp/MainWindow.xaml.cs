using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
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
        private Chess lastSelectedChess = null;
        private bool isPlayerTurn=true;
        private LogicMap logicMap=new LogicMap();

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
            //var testMap = new int[10, 9] {
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
//{13,14,15,16,17,16,15,14,13,},
//{00,00,00,00,00,00,00,00,00,},
//{00,12,00,00,00,00,00,12,00,},
//{11,00,11,00,11,00,11,00,11,},
//{00,00,00,00,00,00,00,00,00,},
//{00,00,00,00,00,00,00,00,00,},
//{21,00,21,00,21,00,21,00,21,},
//{00,22,00,00,22,00,00,00,00,},
//{00,00,00,00,00,00,00,00,00,},
//{23,24,25,26,27,26,25,24,23,} };
//            logicMap.LoadMap(testMap);
            //var dllMap = logicMap.GetDLLMap();
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

        private void OnAITurnEnd(Position firstPosition, Position secondPosition)
        { 
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
            this.Dispatcher.BeginInvoke(System.Windows.Threading.DispatcherPriority.Normal,
                (ThreadStart)delegate ()
                {
                    logicMap.MoveChess(firstPosition, secondPosition);
                    isPlayerTurn = true;
                    Board.HighlightedPostion = secondPosition;
                });
        }

        private void OnBorderMouseUp(int x, int y)
        {
            if (!isPlayerTurn)
            {
                return;
            }
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
                        isPlayerTurn = false;
                        logicMap.MoveChess(lastSelectedChess.Position, mousePosition);
                        Board.SelectedPostion = null;
                        Board.HintedPosition = null;
                        Board.HighlightedPostion = mousePosition;
                        lastSelectedChess = null;
                        logicMap.ForceTriggerMapUpdate();  
                        var thr = new Thread(new ThreadStart(OnAITurn));
                        thr.Start();
                        
                    }
                    else
                    {// unselect chess 
                        Board.SelectedPostion = null;
                        Board.HintedPosition = null;
                        lastSelectedChess = null;
                        Board.HighlightedPostion = null;
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
                            Board.HighlightedPostion = null;
                            lastSelectedChess = null;
                        }
                        else
                        {// capture chess 
                            isPlayerTurn = false;
                            logicMap.MoveChess(lastSelectedChess.Position, mousePosition);
                            Board.SelectedPostion = null;
                            Board.HintedPosition = null;
                            Board.HighlightedPostion = mousePosition;
                            lastSelectedChess = null;
                            logicMap.ForceTriggerMapUpdate();
                            Board.UpdateViewCentent(logicMap.ChessMap); 
                            var thr = new Thread(new ThreadStart(OnAITurn));
                            thr.Start(); 

                        }
                    }
                    else
                    {// unselect chess 
                        Board.SelectedPostion = null;
                        Board.HintedPosition = null;
                        Board.HighlightedPostion = null;
                        lastSelectedChess = null;
                    }
                } 
            }
        }
    }
}
