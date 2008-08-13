using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using SGD;
//7/23/08
namespace iCode_CAnimationEditor
{
    public partial class SheetDisplayWindow : ScrollableControl
    {
        Bitmap m_bmp;
        public void SetBitmap(string szFilename)
        {
            if (szFilename != null)
                m_bmp = new Bitmap(szFilename);
            else
                m_bmp = null;
        }
        public Bitmap GetBitmap()
        {
            return m_bmp;
        }


        #region FrameRectangle
        int frameX;
        int frameY;
        int frameWidth;
        int frameHeight;
        public int FrameX { get { return frameX; } set { frameX = value; } }
        public int FrameY { get { return frameY; } set { frameY = value; } }
        public int FrameRectWidth { get { return frameWidth; } set { frameWidth = value; } }
        public int FrameRectHeight { get { return frameHeight; } set { frameHeight = value; } }
        #endregion

        #region CollisionRectangle
        int collisionX;
        int collisionY;
        int collisionWidth;
        int collisionHeight;
        public int CollisionX { get { return collisionX; } set { collisionX = value; } }
        public int CollisionY { get { return collisionY; } set { collisionY = value; } }
        public int CollisionRectWidth { get { return collisionWidth; } set { collisionWidth = value; } }
        public int CollisionRectHeight { get { return collisionHeight; } set { collisionHeight = value; } }
        #endregion

        #region PassiveRectangle
        Rectangle passiveRect;
        public int PassiveX { get { return passiveRect.X; } set { passiveRect.X = value; } }
        public int PassiveY { get { return passiveRect.Y; } set { passiveRect.Y = value; } }
        public int PassiveWidth { get { return passiveRect.Width; } set { passiveRect.Width = value; } }
        public int PassiveHeight { get { return passiveRect.Height; } set { passiveRect.Height = value; } }
        #endregion

        Point AnchorPoint;
        public int AptX { get { return AnchorPoint.X; } set { AnchorPoint.X = value; } }
        public int AptY { get { return AnchorPoint.Y; } set { AnchorPoint.Y = value; } }

        Point AccessoryPoint;
        public int AccessoryX { get { return AccessoryPoint.X; } set { AccessoryPoint.X = value; } }
        public int AccessoryY { get { return AccessoryPoint.Y; } set { AccessoryPoint.Y = value; } }
        
        Point offset;
        public Point Offset { get { return offset; } }

        public SheetDisplayWindow()
        {
            InitializeComponent();
            AutoScroll = true;
                     
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            
            // TODO: Add custom paint code here
            //Draws the Loaded image
            if (m_bmp != null)
            {
                AutoScrollMinSize = m_bmp.Size;
                offset = AutoScrollPosition;   
                
                pe.Graphics.DrawImage(m_bmp,             offset.X,              offset.Y,              m_bmp.Width,    m_bmp.Height);
                pe.Graphics.DrawRectangle(Pens.Red,      frameX + offset.X,     frameY + offset.Y,     frameWidth,     frameHeight);
                pe.Graphics.DrawRectangle(Pens.Orange,   offset.X + collisionX, offset.Y + collisionY, collisionWidth, collisionHeight);
                pe.Graphics.DrawRectangle(Pens.DarkCyan, offset.X + PassiveX,   offset.Y + PassiveY,   PassiveWidth,   PassiveHeight);

                pe.Graphics.FillEllipse(Brushes.HotPink, FrameX + AptX + offset.X, FrameY + AptY + offset.Y,4, 4);
                pe.Graphics.FillEllipse(Brushes.Crimson, FrameX + AccessoryPoint.X + offset.X,
                                        FrameY + AccessoryPoint.Y + offset.Y, 4, 4);
            }
            
            
            // Calling the base class OnPaint
            base.OnPaint(pe);
        } 
    }
}
