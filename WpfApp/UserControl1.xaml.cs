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
    /// UserControl1.xaml 的交互逻辑
    /// </summary>
    public partial class UserControl1 : UserControl
    {
        public UserControl1()
        {
            InitializeComponent();
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    struct DecisionTreeDllDecision
    { 
        public int fromX;
        public int fromY;
        public int desX;
        public int desY;
    }
     
    class DecisionTreeDll
    {
        [StructLayout(LayoutKind.Sequential)]
        struct PositionList
        { 
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
            public int[] x;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
            public int[] y;
            public int length;
        };  
        [DllImport(@"Decisiontree.dll",EntryPoint ="getVersion", CallingConvention = CallingConvention.Cdecl)]
        public static extern  int GetVersion();
        [DllImport(@"Decisiontree.dll", EntryPoint = "getName", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetName();
        [DllImport(@"Decisiontree.dll", EntryPoint = "getDecision", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetDecision([MarshalAs(UnmanagedType.LPArray,SizeConst =90)] int[,] map,out DecisionTreeDllDecision decision);
        [DllImport(@"Decisiontree.dll", EntryPoint = "getAccessiblePosition", CallingConvention = CallingConvention.Cdecl)]
        private static extern void _GetAccessiblePosition([MarshalAs(UnmanagedType.LPArray, SizeConst = 90)] int[,] map, int x, int y, out PositionList positionList);
        public static List<Position> GetAccessiblePosition(int[,] map, int x, int y)
        {
            _GetAccessiblePosition(map, x, y, out PositionList positionList);
            List<Position> positions = new List<Position>(); 
            for(int i=0;i<positionList.length;i++)
            {
                positions.Add(new Position { x = positionList.x[i], y = positionList.y[i] });
            }
            return positions;
        }

    }
}
