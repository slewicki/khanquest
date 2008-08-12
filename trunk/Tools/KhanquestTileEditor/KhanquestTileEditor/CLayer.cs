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
    public class CLayer
    {
        tTile[,] m_tTiles = new tTile[10, 10];
        Size m_sMapSize = new Size(10, 10);
        Size m_sTileSize = new Size(0, 0);
        bool m_bVisible = false;

        public CLayer(int nMapWidth, int nMapHeight)
        {
            m_tTiles = new tTile[nMapHeight, nMapWidth];
            m_sMapSize = new Size(nMapWidth, nMapHeight);

            for (int x = 0; x < nMapHeight; x++)
            {
                for (int y = 0; y < nMapWidth; y++)
                {
                    m_tTiles[x, y].m_ptPos.X = 0;
                    m_tTiles[x, y].m_ptPos.Y = 0;
                    m_tTiles[x, y].m_szTileID = "Plains";
                }
            }
        }

        public tTile[,] Tiles
        {
            get { return m_tTiles; }
            set { m_tTiles = value; }
        }

        public Size MapSize
        {
            get { return m_sMapSize; }
            set
            {
                if (MapSize != value)
                {
                    tTile[,] tempTiles = new tTile[value.Width, value.Height];

                    for (int x = 0; x < m_tTiles.GetLength(0) && x < tempTiles.GetLength(0); x++)
                    {
                        for (int y = 0; y < m_tTiles.GetLength(1) && y < tempTiles.GetLength(1); y++)
                        {
                            tempTiles[x, y] = m_tTiles[x, y];
                        }
                    }

                    m_tTiles = tempTiles;
                }

                m_sMapSize = value;
            }
        }

        public Size TileSize
        {
            get { return m_sTileSize; }
            set { m_sTileSize = value; }
        }

        public bool Visible
        {
            get { return m_bVisible; }
            set { m_bVisible = value; }
        }
    }
}
