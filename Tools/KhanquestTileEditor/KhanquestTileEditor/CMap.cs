///<summary>
///File: "CMap.cs"
///Author: Sebastian Lewicki
///Purpose: Class that stores Map information
///</summary>
using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace KhanquestTileEditor
{
    //public enum TileType { PLAIN, MOUNTAIN, FOREST, SHALLOW_WATER, DEEP_WATER }

    public struct tTile
    {
        public string m_szTileID;
        public Point m_ptPos;
        public bool m_bPlayerSpawn;
        public bool m_bEnemySpawn;
        public bool m_bCollision;
    };

    public class CMap
    {
        ManagedDirect3D D3D = ManagedDirect3D.Instance;
        ManagedTextureManager TM = ManagedTextureManager.Instance;

        Point m_ptWorldPosition = new Point(0, 0);
        int m_nImageID;
        List<CLayer> m_lLayer = new List<CLayer>();
        int m_nCurrentLayer = 0;
        List<Rectangle> m_rTiles = new List<Rectangle>();
        bool m_bOnce = true;

        public Point WorldPosition
        {
            get { return m_ptWorldPosition; }
            set { m_ptWorldPosition = value; }
        }

        public int WorldPositionX
        {
            get { return m_ptWorldPosition.X; }
            set { m_ptWorldPosition.X = value; }
        }

        public int WorldPositionY
        {
            get { return m_ptWorldPosition.Y; }
            set { m_ptWorldPosition.Y = value; }
        }

        public int ImageID
        {
            get { return m_nImageID; }
            set { m_nImageID = value; }
        }

        public List<CLayer> Layer
        {
            get { return m_lLayer; }
            set { m_lLayer = value; }
        }

        public int CurrentLayer
        {
            get { return m_nCurrentLayer; }
            set { m_nCurrentLayer = value; }
        }

        public List<Rectangle> TileRects
        {
            get { return m_rTiles; }
            set { m_rTiles = value; }
        }

        public bool Once
        {
            get { return m_bOnce; }
            set
            {
                m_bOnce = value;
                m_rTiles.Clear();
            }
        }

        public CMap()
        {
            NewLayer(new Size(100, 100));
            m_lLayer[m_nCurrentLayer].Visible = true;

            for (int x = 0; x < m_lLayer[m_nCurrentLayer].MapSize.Width; x++)
            {
                for (int y = 0; y < m_lLayer[m_nCurrentLayer].MapSize.Height; y++)
                {
                    m_lLayer[m_nCurrentLayer].Tiles[x, y].m_szTileID = "Plains";
                }
            }
        }

        public void NewLayer(Size sMapSize)
        {
            m_lLayer.Add(new CLayer(sMapSize.Width, sMapSize.Height));
        }

        public void ImageLoad(string szFileName)
        {
            m_nImageID = TM.LoadTexture(szFileName, Color.FromArgb(255, 255, 0, 255).ToArgb());
        }

        public void ImageRelease()
        {
            TM.ReleaseTexture(m_nImageID);
        }

        public void RenderMap(int nScreenWidth, int nScreenHeight)
        {
            D3D.LineBegin();

            Render(nScreenWidth, nScreenHeight);

            D3D.LineEnd();
        }

        void Render(int nScreenWidth, int nScreenHeight)
        {
            m_rTiles = new List<Rectangle>();
            Point ptStartpoint = new Point(m_lLayer[0].TileSize.Width / 2, nScreenHeight / 2);

            for (int nLayer = 0; nLayer < m_lLayer.Count; nLayer++)
            {
                for (int Col = 0; Col < m_lLayer[nLayer].MapSize.Height; Col++)
                {
                    for (int Row = 0; Row < m_lLayer[nLayer].MapSize.Width; Row++)
                    {
                        Rectangle rTile = new Rectangle(m_lLayer[nLayer].Tiles[Row, Col].m_ptPos.X * m_lLayer[nLayer].TileSize.Width, m_lLayer[nLayer].Tiles[Row, Col].m_ptPos.Y * m_lLayer[nLayer].TileSize.Height, m_lLayer[nLayer].TileSize.Width, m_lLayer[nLayer].TileSize.Height);

                        Rectangle rRect = new Rectangle(((Row * (m_lLayer[nLayer].TileSize.Width / 2)) + (Col * m_lLayer[nLayer].TileSize.Width / 2) + ptStartpoint.X) - m_ptWorldPosition.X, ((Row * -(m_lLayer[nLayer].TileSize.Height / 2)) + (Col * m_lLayer[nLayer].TileSize.Height / 2) + ptStartpoint.Y) - m_ptWorldPosition.Y, m_lLayer[nLayer].TileSize.Width, m_lLayer[nLayer].TileSize.Height);

                        m_rTiles.Add(rRect);

                        if (m_lLayer[nLayer].Visible == true)
                        {
                            if (m_lLayer[m_nCurrentLayer].Tiles[Row, Col].m_bEnemySpawn == false && m_lLayer[m_nCurrentLayer].Tiles[Row, Col].m_bPlayerSpawn == false && m_lLayer[m_nCurrentLayer].Tiles[Row, Col].m_bCollision == false)
                                TM.Draw(m_nImageID, ((Row * (m_lLayer[nLayer].TileSize.Width / 2)) + (Col * m_lLayer[nLayer].TileSize.Width / 2) + ptStartpoint.X) - m_ptWorldPosition.X, ((Row * -(m_lLayer[nLayer].TileSize.Height / 2)) + (Col * m_lLayer[nLayer].TileSize.Height / 2) + ptStartpoint.Y) - m_ptWorldPosition.Y, 1, 1, rTile, 0, 0, 0, Color.White.ToArgb());
                            else if (m_lLayer[m_nCurrentLayer].Tiles[Row, Col].m_bCollision == true)
                                TM.Draw(m_nImageID, ((Row * (m_lLayer[nLayer].TileSize.Width / 2)) + (Col * m_lLayer[nLayer].TileSize.Width / 2) + ptStartpoint.X) - m_ptWorldPosition.X, ((Row * -(m_lLayer[nLayer].TileSize.Height / 2)) + (Col * m_lLayer[nLayer].TileSize.Height / 2) + ptStartpoint.Y) - m_ptWorldPosition.Y, 1, 1, rTile, 0, 0, 0, Color.DeepPink.ToArgb());
                            else if (m_lLayer[m_nCurrentLayer].Tiles[Row, Col].m_bPlayerSpawn == true)
                                TM.Draw(m_nImageID, ((Row * (m_lLayer[nLayer].TileSize.Width / 2)) + (Col * m_lLayer[nLayer].TileSize.Width / 2) + ptStartpoint.X) - m_ptWorldPosition.X, ((Row * -(m_lLayer[nLayer].TileSize.Height / 2)) + (Col * m_lLayer[nLayer].TileSize.Height / 2) + ptStartpoint.Y) - m_ptWorldPosition.Y, 1, 1, rTile, 0, 0, 0, Color.Red.ToArgb());
                            else if (m_lLayer[m_nCurrentLayer].Tiles[Row, Col].m_bEnemySpawn == true)
                                TM.Draw(m_nImageID, ((Row * (m_lLayer[nLayer].TileSize.Width / 2)) + (Col * m_lLayer[nLayer].TileSize.Width / 2) + ptStartpoint.X) - m_ptWorldPosition.X, ((Row * -(m_lLayer[nLayer].TileSize.Height / 2)) + (Col * m_lLayer[nLayer].TileSize.Height / 2) + ptStartpoint.Y) - m_ptWorldPosition.Y, 1, 1, rTile, 0, 0, 0, Color.Blue.ToArgb());
                        }
                    }
                }
            }

            m_bOnce = false;
        }
    }
}
