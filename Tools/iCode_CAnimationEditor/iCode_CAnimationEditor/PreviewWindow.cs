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
    public partial class PreviewWindow : Control
    {
        Bitmap m_bmp;
        public void SetBitmap(string szFilename)
        {
            if (szFilename != null)
                m_bmp = new Bitmap(szFilename);
            else
                m_bmp = null;
        }

        #region FrameRectangle
            Rectangle frameRect;
            public int FrameX { get { return frameRect.X; } set { frameRect.X = value; } }
            public int FrameY { get { return frameRect.Y; } set { frameRect.Y = value; } }
            public int FrameWidth { get { return frameRect.Width; } set { frameRect.Width = value; } } 
            public int FrameHeight { get { return frameRect.Height; } set { frameRect.Height = value;} }
        #endregion

        Point AnchorPoint;
        public int AptX { get { return AnchorPoint.X; } set { AnchorPoint.X = value; } }
        public int AptY { get { return AnchorPoint.Y; } set { AnchorPoint.Y = value; } }

        public PreviewWindow()
        {
            InitializeComponent();
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            if (m_bmp != null)
            {
                Graphics formGraphics = this.CreateGraphics();
                GraphicsUnit unit = GraphicsUnit.Pixel;
                
                pe.Graphics.DrawImage(m_bmp, new Rectangle((this.Width / 2) - AptX, (this.Height / 2) - AptY, this.FrameWidth, this.FrameHeight), frameRect, unit);

                pe.Graphics.FillEllipse(Brushes.Red, new Rectangle((this.Width / 2), (this.Height / 2), 4, 4));
            
            }
            // Calling the base class OnPaint
            base.OnPaint(pe);
        }
    }
}
