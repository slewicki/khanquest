using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace KhanquestTileEditor
{
    public partial class Map : Panel
    {
        CMap m_Map = new CMap();
        Point m_ptClicked = Point.Empty;
        Tile m_tTile = new Tile();

        public CMap mMap
        {
            get { return m_Map; }
            set { m_Map = value; }
        }

        public Map()
        {
            InitializeComponent();
            DoubleBuffered = true;
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            // TODO: Add custom paint code here

            // Calling the base class OnPaint
            base.OnPaint(pe);
        }
    }
}
