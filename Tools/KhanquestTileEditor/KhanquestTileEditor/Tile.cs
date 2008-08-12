using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace KhanquestTileEditor
{
    public partial class Tile : ScrollableControl
    {
        Point[,] ptTileFocus = new Point[1, 1];
        Size m_sTileSetSize = new Size(0, 0);
        Image m_bmpTileImage = new Bitmap(Properties.Resources.KQ_Load);
        string m_szImage;
        Size m_sTileSize = new Size(0, 0);

        public Size TileSetSize
        {
            get { return m_sTileSetSize; }
            set
            {
                m_sTileSetSize = value;
                if (m_sTileSize.Width * m_sTileSetSize.Width > m_sTileSize.Width || m_sTileSize.Height * m_sTileSetSize.Height > m_sTileSize.Height)
                {
                    this.AutoScrollMinSize = new Size(m_sTileSetSize.Width * m_sTileSize.Width, m_sTileSetSize.Height * m_sTileSize.Height);

                    if (m_sTileSize.Width * m_sTileSetSize.Width < m_bmpTileImage.Width || m_sTileSize.Height * m_sTileSetSize.Height > m_bmpTileImage.Height)
                        this.AutoScrollMinSize = new Size(m_sTileSize.Width * m_sTileSetSize.Width, m_sTileSize.Height * m_sTileSetSize.Height);
                    else if (m_sTileSize.Width * m_sTileSetSize.Width > m_sTileSize.Width)
                        this.AutoScrollMinSize = new Size(m_sTileSize.Width * m_sTileSetSize.Width, m_bmpTileImage.Height);
                    else
                        this.AutoScrollMinSize = new Size(m_sTileSize.Width, m_sTileSize.Height * m_sTileSetSize.Height);
                }
                else
                    this.AutoScrollMinSize = m_bmpTileImage.Size;

                Invalidate();
            }
        }

        public Image TileImage
        {
            get { return m_bmpTileImage; }
            set
            {
                m_bmpTileImage = value;
                this.AutoScrollMinSize = m_bmpTileImage.Size;
                Invalidate();
            }
        }

        public string ImageFile
        {
            get { return m_szImage; }
            set { m_szImage = value; }
        }

        public Size TileSize
        {
            get { return m_sTileSize; }
            set
            {
                m_sTileSize = value;
                this.AutoScrollMinSize = m_bmpTileImage.Size;
                Invalidate();
            }
        }

        public Point[,] TileFocus
        {
            get { return ptTileFocus; }
            set { ptTileFocus = value; }
        }

        public Tile()
        {
            InitializeComponent();
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            // TODO: Add custom paint code here
            Rectangle rRect = new Rectangle();
            rRect.Size = m_sTileSize;
            Pen ThickBlack = new Pen(Color.Black, 2.0f);
            Pen Focus = new Pen(Color.Red, 1.0f);

            pe.Graphics.DrawImage(m_bmpTileImage, this.AutoScrollPosition);

            for (int y = 0; y < m_sTileSetSize.Height; y++)
            {
                for (int x = 0; x < m_sTileSetSize.Width; x++)
                {
                    rRect.X = m_sTileSize.Width * x;
                    rRect.Y = m_sTileSize.Height * y;
                    rRect.Size = m_sTileSize;
                    rRect.Offset(this.AutoScrollPosition);
                    pe.Graphics.DrawRectangle(ThickBlack, rRect);
                }
            }


            for (int y = 0; y < m_sTileSetSize.Height; y++)
            {
                for (int x = 0; x < m_sTileSetSize.Width; x++)
                {
                    rRect.X = m_sTileSize.Width * x;
                    rRect.Y = m_sTileSize.Height * y;
                    rRect.Size = m_sTileSize;
                    rRect.Offset(this.AutoScrollPosition);

                    if (ptTileFocus[0, 0].X / m_sTileSize.Width == x && m_sTileSize.Width != 0 && ptTileFocus[0, 0].Y / m_sTileSize.Height == y && m_sTileSize.Height != 0)
                        pe.Graphics.DrawRectangle(Focus, rRect);
                }
            }
            // Calling the base class OnPaint
            base.OnPaint(pe);
        }

        public void Clear()
        {
            m_bmpTileImage = Properties.Resources.KQ_Load;
            m_sTileSetSize = new Size(0, 0);
            m_sTileSize = new Size(0, 0);
            Invalidate();
        }

        protected override void OnMouseClick(MouseEventArgs e)
        {
            Point ptTilePos = new Point(e.X + Math.Abs(this.AutoScrollPosition.X), e.Y + Math.Abs(this.AutoScrollPosition.Y));
            Point ptClicked = Point.Empty;

            if (m_sTileSize.Width > 0 && m_sTileSize.Height > 0)
            {
                ptClicked.X = ptTilePos.X / m_sTileSize.Width;
                ptClicked.Y = ptTilePos.Y / m_sTileSize.Height;
            }

            for (int x = 0; x < m_sTileSetSize.Width; x++)
            {
                for (int y = 0; y < m_sTileSetSize.Height; y++)
                {
                    if (ptClicked.X == x && ptClicked.Y == y)
                    {
                        tTile Tile = new tTile();
                        Tile.m_ptPos = new Point(ptClicked.X * m_sTileSize.Width, ptClicked.Y * m_sTileSize.Height);
                        ptTileFocus = new Point[1, 1];
                        ptTileFocus[0, 0] = Tile.m_ptPos;
                    }
                }
            }

            Invalidate();

            base.OnMouseClick(e);
        }
    }
}
