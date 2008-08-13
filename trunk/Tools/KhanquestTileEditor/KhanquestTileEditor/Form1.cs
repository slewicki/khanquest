using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace KhanquestTileEditor
{
    public partial class Form1 : Form
    {
        ManagedDirect3D D3D = ManagedDirect3D.Instance;
        ManagedTextureManager TM = ManagedTextureManager.Instance;

        //Map m_Map = new Map();
        //Tile m_Tile = new Tile();
        Point m_ptSelectedPoint = Point.Empty;
        int m_nMousePosX, m_nMousePosY;
        //string szFileName;

        //public Tile Tile
        //{
        //    get { return m_Tile; }
        //    set { m_Tile = value; }
        //}

        public Form1()
        {
            InitializeComponent();
            D3D.InitManagedDirect3D(map1, map1.Width, map1.Height, true, false);
            TM.InitManagedTextureManager(D3D.Device, D3D.Sprite);
            map1.mMap.ImageLoad("../../Resources/KQ_Load.bmp");
            cmbLayer.SelectedIndex = 0;
            cmbTrigger.SelectedIndex = 0;
            rdoTile.Checked = true;
        }

        private void numTileHeight_ValueChanged(object sender, EventArgs e)
        {
            tile1.TileSize = new Size((int)numTileWidth.Value, (int)numTileHeight.Value);
            map1.mMap.Layer[map1.mMap.CurrentLayer].TileSize = new Size((int)numTileWidth.Value, (int)numTileHeight.Value);

            if ((int)numTileHeight.Value > 0 && (int)numTileWidth.Value > 0)
            {
                numTileGridHeight.Enabled = true;
                numTileGridWidth.Enabled = true;
            }
        }

        private void numTileWidth_ValueChanged(object sender, EventArgs e)
        {
            tile1.TileSize = new Size((int)numTileWidth.Value, (int)numTileHeight.Value);
            map1.mMap.Layer[map1.mMap.CurrentLayer].TileSize = new Size((int)numTileWidth.Value, (int)numTileHeight.Value);

            if ((int)numTileHeight.Value > 0 && (int)numTileWidth.Value > 0)
            {
                numTileGridHeight.Enabled = true;
                numTileGridWidth.Enabled = true;
            }
        }

        private void numTileGridHeight_ValueChanged(object sender, EventArgs e)
        {
            tile1.TileSetSize = new Size((int)numTileGridWidth.Value, (int)numTileGridHeight.Value);
        }

        private void numTileGridWidth_ValueChanged(object sender, EventArgs e)
        {
            tile1.TileSetSize = new Size((int)numTileGridWidth.Value, (int)numTileGridHeight.Value);
        }

        private void btnLayer_Click(object sender, EventArgs e)
        {
            Size sNewSize = new Size((int)numMapWidth.Value, (int)numMapHeight.Value);
            //m_Map.mMap.NewLayer(sNewSize);
            //cmbLayer.Items.Add(m_Map.mMap.Layer.Count);
            map1.mMap.NewLayer(sNewSize);
            map1.mMap.Layer[map1.mMap.CurrentLayer + 1].TileSize = new Size((int)numTileWidth.Value, (int)numTileHeight.Value);
            cmbLayer.Items.Add(map1.mMap.Layer.Count);
            cmbLayer.SelectedIndex = cmbLayer.SelectedIndex + 1;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            D3D.DeviceBegin();
            D3D.SpriteBegin();
            D3D.Clear(255, 255, 255);

            //Render here
            map1.mMap.RenderMap(map1.Width, map1.Height);
            D3D.DrawText(m_nMousePosX.ToString() + ", " + m_nMousePosY.ToString(), 5, 5, 0, 0, 0);

            D3D.SpriteEnd();
            D3D.DeviceEnd();

            D3D.Present();
        }

        private void importImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                FileInfo fi = new FileInfo(dlg.FileName);
                map1.mMap.ImageRelease();
                tile1.TileImage =  new Bitmap(dlg.FileName);
                tile1.ImageFile = fi.Name;
                map1.mMap.ImageLoad(dlg.FileName);
            }
        }

        private void numMapHeight_ValueChanged(object sender, EventArgs e)
        {
            map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize = new Size((int)numMapWidth.Value, (int)numMapHeight.Value);
            map1.mMap.Once = true;
            this.Refresh();

            for (int x = 0; x < (int)numMapWidth.Value; x++)
            {
                for (int y = 0; y < (int)numMapHeight.Value; y++)
                {
                    if (map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[x, y].m_szTileID == null)
                        map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[x, y].m_szTileID = "Plains";
                }
            }
        }

        private void numMapWidth_ValueChanged(object sender, EventArgs e)
        {
            map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize = new Size((int)numMapWidth.Value, (int)numMapHeight.Value);
            map1.mMap.Once = true;
            this.Refresh();

            for (int x = 0; x < (int)numMapWidth.Value; x++)
            {
                for (int y = 0; y < (int)numMapHeight.Value; y++)
                {
                    if (map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[x, y].m_szTileID == null)
                        map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[x, y].m_szTileID = "Plains";
                }
            }
        }

        private Point IsoMouse(int x, int y)
        {
            Point ptNewPoint = Point.Empty;

            ptNewPoint.Y = ((int)numTileWidth.Value * y + (int)numTileHeight.Value * x) / ((int)numTileHeight.Value * (int)numTileWidth.Value) - 8;
            float fTempX = (6 - (((int)numTileWidth.Value * y - (float)numTileHeight.Value * x) / ((int)numTileHeight.Value * (int)numTileWidth.Value)));
            ptNewPoint.X = (int)fTempX;

            if (fTempX > (ptNewPoint.X + .5))
                ptNewPoint.X += 1;

            if(ptNewPoint.Y < map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Height && ptNewPoint.X < map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Width && ptNewPoint.Y >= 0 && ptNewPoint.X >= 0)
            {
                m_nMousePosX = ptNewPoint.X;
                m_nMousePosY = ptNewPoint.Y;
            }

            return new Point(m_nMousePosX, m_nMousePosY);
        }

        private void map1_MouseClick(object sender, MouseEventArgs e)
        {
            Point ptTilePos = new Point(e.X + map1.mMap.WorldPosition.X, e.Y + map1.mMap.WorldPosition.Y);
            //int nTileHeight = ptTilePos.Y;
            //int nTileWidth = ptTilePos.X;
            Point ptPrevTile = Point.Empty;

            //for (int Col = 0; Col < map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Height; Col++)
            //{
            //    for (int Row = 0; Row < map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Width; Row++)
            //    {
            //        if (map1.mMap.TileRects[(Col * map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Height) + Row].IntersectsWith(new Rectangle(new Point(e.X - 16 + map1.mMap.WorldPosition.X, e.Y + map1.mMap.WorldPosition.Y), new Size(5, 1))))
            //        {
            //            nTileWidth = Row;
            //            nTileHeight = Col;
            //        }
            //    }
            //}

            ptTilePos = IsoMouse(ptTilePos.X, ptTilePos.Y);

            int nTileHeight = ptTilePos.Y;
            int nTileWidth = ptTilePos.X;

            if (e.X > -1 && nTileHeight < map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Height && nTileWidth < map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Width && rdoTile.Checked == true)
            {
                if (chkCollision.Checked == false && chkTrigger.Checked == false)
                {
                    ptPrevTile = map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_ptPos;
                    map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_ptPos = m_ptSelectedPoint;
                }

                //map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_szTileID = "Plains";

                if(chkCollision.Checked == true)
                    map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_bCollision = !map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_bCollision;

                if(chkTrigger.Checked == true)
                    map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_szTileID = cmbTrigger.SelectedItem.ToString();
            }

            else if (e.X > -1 && rdoPlayerSpawn.Checked == true)
            {
                chkCollision.Checked = false;
                chkTrigger.Checked = false;

                if (e.Button == MouseButtons.Left)
                {
                    if (map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_bEnemySpawn == true)
                        map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_bEnemySpawn = false;

                    map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_bPlayerSpawn = !map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_bPlayerSpawn;
                }
            }

            else if (e.X > -1 && rdoEnemySpawn.Checked == true)
            {
                chkCollision.Checked = false;
                chkTrigger.Checked = false;

                if (e.Button == MouseButtons.Left)
                    map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_bPlayerSpawn = false;
                map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_bEnemySpawn = !map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_bEnemySpawn;
            }

            if (e.Button == MouseButtons.Right)
                AreaFill(nTileWidth, nTileHeight, ptPrevTile, m_ptSelectedPoint, 0);
        }

        private void AreaFill(int nPosX, int nPosY, Point ptPrevTile, Point ptNewTile, int Counter)
        {
            if (Counter > 900)
                return;

            if (ptPrevTile == ptNewTile)
                return;

            map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nPosX, nPosY].m_ptPos = ptNewTile;
            if (nPosX - 1 >= 0 && map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nPosX - 1, nPosY].m_ptPos == ptPrevTile)
            {
                AreaFill(nPosX - 1, nPosY, ptPrevTile, ptNewTile, Counter + 1);
            }
            if (nPosY - 1 >= 0 && map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nPosX, nPosY - 1].m_ptPos == ptPrevTile)
            {
                AreaFill(nPosX, nPosY - 1, ptPrevTile, ptNewTile, Counter + 1);
            }
            if (nPosX + 1 < map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Width && map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nPosX + 1, nPosY].m_ptPos == ptPrevTile)
            {
                AreaFill(nPosX + 1, nPosY, ptPrevTile, ptNewTile, Counter + 1);
            }
            if (nPosY + 1 < map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Height && map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nPosX, nPosY + 1].m_ptPos == ptPrevTile)
            {
                AreaFill(nPosX, nPosY + 1, ptPrevTile, ptNewTile, Counter + 1);
            }

            return;
        }

        private void map1_MouseMove(object sender, MouseEventArgs e)
        {
            m_nMousePosX = e.X;
            m_nMousePosY = e.Y;

            if (e.Button == MouseButtons.Left)
            {
                Point ptTilePos = new Point(e.X + map1.mMap.WorldPosition.X, e.Y + map1.mMap.WorldPosition.Y);
                //int nTileHeight = 0;
                //int nTileWidth = 0;

                //for (int Col = 0; Col < map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Height; Col++)
                //{
                //    for (int Row = 0; Row < map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Width; Row++)
                //    {
                //        if (map1.mMap.TileRects[(Col * map1.mMap.Layer[map1.mMap.CurrentLayer].MapSize.Height) + Row].IntersectsWith(new Rectangle(new Point(e.X + map1.mMap.WorldPosition.X, e.Y + map1.mMap.WorldPosition.Y), new Size(1, 1))))
                //        {
                //            nTileWidth = Row;
                //            nTileHeight = Col;
                //        }
                //    }
                //}

                ptTilePos = IsoMouse(ptTilePos.X, ptTilePos.Y);

                int nTileHeight = ptTilePos.Y;
                int nTileWidth = ptTilePos.X;

                if (e.X > -1 && rdoTile.Checked == true)
                {
                    map1.mMap.Layer[map1.mMap.CurrentLayer].Tiles[nTileWidth, nTileHeight].m_ptPos = m_ptSelectedPoint;
                }
            }
        }

        private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            map1.mMap.WorldPositionY -= (e.OldValue - e.NewValue) * 20;
            map1.Refresh();
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            map1.mMap.WorldPositionX -= (e.OldValue - e.NewValue) * 20;
            map1.Refresh();
        }

        private void tile1_MouseClick(object sender, MouseEventArgs e)
        {
            if (numTileGridWidth.Value > 1 && numTileGridHeight.Value > 1)
            {
                int nTileHeight = (e.Y + Math.Abs(tile1.AutoScrollPosition.Y)) / tile1.TileSize.Height;
                int nTileWidth = (e.X + Math.Abs(tile1.AutoScrollPosition.X)) / tile1.TileSize.Width;

                m_ptSelectedPoint = new Point(nTileWidth, nTileHeight);
            }
        }

        private void rdoTile_CheckedChanged(object sender, EventArgs e)
        {
            if(rdoTile.Checked == true)
            {
                chkCollision.Enabled = true;
                chkTrigger.Enabled = true;
            }
            else
            {
                chkCollision.Enabled = false;
                chkTrigger.Enabled = false;
            }
        }

        private void cmbLayer_SelectedIndexChanged(object sender, EventArgs e)
        {
            map1.mMap.CurrentLayer = cmbLayer.SelectedIndex;

            for (int nLayer = 0; nLayer < map1.mMap.Layer.Count; nLayer++)
            {
                if (nLayer <= map1.mMap.CurrentLayer)
                    map1.mMap.Layer[nLayer].Visible = true;
                else
                    map1.mMap.Layer[nLayer].Visible = false;
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "Level files|*.level|All Files|*.*";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                FileInfo fi = new FileInfo(dlg.FileName);
                if (!fi.Name.StartsWith("KQ_"))
                {
                    string szFilename = fi.DirectoryName + "\\" + "KQ_" + fi.Name;
                    dlg.FileName = szFilename;
                }

                BinaryWriter bw = new BinaryWriter(File.Open(dlg.FileName, FileMode.Create, FileAccess.Write));
                bw.Write(tile1.ImageFile.Length);
                bw.Write(tile1.ImageFile.ToCharArray());
                bw.Write(map1.mMap.Layer.Count);

                bw.Write(map1.mMap.Layer[0].MapSize.Width);
                bw.Write(map1.mMap.Layer[0].MapSize.Height);
                bw.Write(map1.mMap.Layer[0].TileSize.Width);
                bw.Write(map1.mMap.Layer[0].TileSize.Height);

                for (int nLayer = 0; nLayer < map1.mMap.Layer.Count; nLayer++)
                {
                    for (int x = 0; x < map1.mMap.Layer[0].MapSize.Height; x++)
                    {
                        for (int y = 0; y < map1.mMap.Layer[0].MapSize.Width; y++)
                        {
                            bw.Write(map1.mMap.Layer[nLayer].Tiles[x, y].m_szTileID.Length);
                            bw.Write(map1.mMap.Layer[nLayer].Tiles[x, y].m_szTileID.ToCharArray());
                            bw.Write(map1.mMap.Layer[nLayer].Tiles[x, y].m_ptPos.X);
                            bw.Write(map1.mMap.Layer[nLayer].Tiles[x, y].m_ptPos.Y);
                            bw.Write(map1.mMap.Layer[nLayer].Tiles[x, y].m_bEnemySpawn);
                            bw.Write(map1.mMap.Layer[nLayer].Tiles[x, y].m_bPlayerSpawn);
                            bw.Write(map1.mMap.Layer[nLayer].Tiles[x, y].m_bCollision);
                        }
                    }
                }

                bw.Flush();
                bw.Close();
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            timer1.Enabled = false;
            cmbLayer.Items.Clear();
            cmbLayer.Items.Add(1);
            map1.mMap.Layer.Clear();

            map1.mMap.NewLayer(new Size(10, 10));
            map1.mMap.Layer[0].Visible = true;

            numMapHeight.Value = 10;
            numMapWidth.Value = 10;

            hScrollBar1.Value = 0;
            vScrollBar1.Value = 1000;

            cmbTrigger.SelectedIndex = 0;
            chkCollision.Checked = false;
            chkTrigger.Checked = false;
            rdoTile.Checked = true;

            timer1.Enabled = true;
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Level files|*.level|All Files|*.*";

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                cmbLayer.Items.Clear();
                map1.mMap.Layer.Clear();

                BinaryReader br = new BinaryReader(File.Open(dlg.FileName, FileMode.Open, FileAccess.Read));

                map1.mMap.Layer = new List<CLayer>();
                int nStringSize = br.ReadInt32();
                string szFileName = new string(br.ReadChars(nStringSize));
                int nTempLayer = br.ReadInt32();

                map1.mMap.Layer.Add(new CLayer(br.ReadInt32(), br.ReadInt32()));
                map1.mMap.Layer[0].TileSize = new Size(br.ReadInt32(), br.ReadInt32());

                for (int nLayer = 0; nLayer < nTempLayer; nLayer++)
                {
                    cmbLayer.Items.Add(nLayer + 1);

                    if (nLayer > 0)
                        map1.mMap.Layer.Add(new CLayer(map1.mMap.Layer[0].MapSize.Width, map1.mMap.Layer[0].MapSize.Height));

                    for (int x = 0; x < map1.mMap.Layer[0].MapSize.Height; x++)
                    {
                        for (int y = 0; y < map1.mMap.Layer[0].MapSize.Width; y++)
                        {
                            nStringSize = br.ReadInt32();
                            string szType = new string(br.ReadChars(nStringSize));
                            map1.mMap.Layer[nLayer].Tiles[x, y].m_szTileID = szType;
                            map1.mMap.Layer[nLayer].Tiles[x, y].m_ptPos = new Point(br.ReadInt32(), br.ReadInt32());
                            map1.mMap.Layer[nLayer].Tiles[x, y].m_bEnemySpawn = br.ReadBoolean();
                            map1.mMap.Layer[nLayer].Tiles[x, y].m_bPlayerSpawn = br.ReadBoolean();
                            map1.mMap.Layer[nLayer].Tiles[x, y].m_bCollision = br.ReadBoolean();
                        }
                    }
                }
                br.Close();
            }

            cmbLayer.SelectedIndex = 0;
            numMapHeight.Value = map1.mMap.Layer[0].MapSize.Height;
            numTileWidth.Value = map1.mMap.Layer[0].TileSize.Width;
            numMapWidth.Value = map1.mMap.Layer[0].MapSize.Width;
            numTileHeight.Value = map1.mMap.Layer[0].TileSize.Height;
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void btnTrigger_Click(object sender, EventArgs e)
        {
            cmbTrigger.Items.Add(txtTrigger.Text);
        }
    }
}