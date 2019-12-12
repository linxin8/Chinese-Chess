using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace ChessGame
{
    /// <summary>
    /// ChessBoard.xaml 的交互逻辑
    /// </summary>
    public partial class ChessBoard : UserControl
    {
        public delegate void BoardMouseUpEventHandler(int x, int y);
        public event BoardMouseUpEventHandler BoardMouseUp;
        private Button[,] textButton = new Button[10, 9];
        private Position? selectedPostion;
        private Position? highlightedPostion;
        private Brush orignalBrush = Brushes.LightGray;
        public Position? SelectedPostion
        { get { return selectedPostion; }
            set
            { 
                if (selectedPostion != null)
                {
                    ResetBackgroundColor(selectedPostion.Value);
                }
                selectedPostion = value;
                if (selectedPostion != null)
                {
                    SetBackgroundColor(selectedPostion.Value, Brushes.LightGreen);
                }
            }
        }
        public Position? HighlightedPostion
        {
            get { return highlightedPostion; }
            set
            {
                if (highlightedPostion != null)
                {
                    ResetBackgroundColor(highlightedPostion.Value);
                }
                highlightedPostion = value;
                if (highlightedPostion != null)
                {
                    SetBackgroundColor(highlightedPostion.Value, Brushes.LightGreen);
                }
            }
        }
        private List<Position> hintedPosition;
        public List<Position> HintedPosition
        {
            get { return hintedPosition; }
            set
            {
                if (hintedPosition != null)
                {
                    foreach (var p in hintedPosition)
                    {
                        ResetBackgroundColor(p);
                    }
                }
                hintedPosition = value;
                if (hintedPosition != null)
                {
                    foreach (var p in hintedPosition)
                    {
                        SetBackgroundColor(p, Brushes.LightSkyBlue);
                    }
                }
            }
        }
        public ChessBoard()
        {
            InitializeComponent();
            Init();
        }

        public void Init()
        {
            BoardGrid.Rows = 10;
            BoardGrid.Columns = 9;
            for (int i = 0; i < BoardGrid.Rows; i++)
            {
                for (int j = 0; j < BoardGrid.Columns; j++)
                {
                    var button = textButton[i, j] = new Button
                    {
                        BorderBrush = Brushes.DarkGray,
                        Background = orignalBrush,
                    }; 
                    button.Click += new RoutedEventHandler((object o, RoutedEventArgs args) =>
                     {
                         var index = BoardGrid.Children.IndexOf(button);
                         BoardMouseUp?.Invoke(index % BoardGrid.Columns, index / BoardGrid.Columns);
                     });
                    button.MouseUp += new MouseButtonEventHandler((object o, MouseButtonEventArgs args) =>
                    {
                        var index = BoardGrid.Children.IndexOf(button);
                        BoardMouseUp?.Invoke(index % BoardGrid.Columns, index / BoardGrid.Columns);
                    });
                    button.MouseEnter += new MouseEventHandler((object o, MouseEventArgs args) =>
                     {
                         button.Opacity = 0.5;
                     });
                    button.MouseLeave += new MouseEventHandler((object o, MouseEventArgs args) =>
                    {
                        button.Opacity = 1;
                    });
                    BoardGrid.Children.Add(button);
                }
            }
        }
        public void UpdateViewCentent(Chess[,] map)
        {
            for (int i = 0; i < Rule.MapRow; i++)
            {
                for (int j = 0; j < Rule.MapColomn; j++)
                {
                    var chess = map[i, j];
                    var text = chess == null ? "" : chess.ChineseName;
                    //if (textLabel[i, j].Content.ToString() != text)
                    //{
                    textButton[i, j].Content = text;
                    textButton[i, j].BorderBrush = Brushes.Gray;
                    textButton[i, j].FontSize = 30;
                    //}
                    if (chess != null)
                    {
                        if (chess.Country == Rule.Country.Black)
                        {
                            textButton[i, j].Foreground = Brushes.Black;
                        }
                        else
                        {
                            textButton[i, j].Foreground = Brushes.OrangeRed;
                        }
                    }

                }
            }
        }
        public void SetBackgroundColor(List<Position> positions, Brush brush)
        {
            foreach (var p in positions)
            {
                SetBackgroundColor(p, brush);
            }
        }
        private void SetBackgroundColor(Position position, Brush brush)
        {
            //textLabel[position.y, position.x].Background = brush;
            ((Button)BoardGrid.Children[position.y * 9 + position.x]).Background = brush;
        }

        private void ResetBackgroundColor(Position position) => SetBackgroundColor(position, orignalBrush);

        private void ResetBackgroundColor(List<Position> positions)
        {
            foreach (var p in positions)
            {
                ResetBackgroundColor(p);
            }
        }
         
    }
}

