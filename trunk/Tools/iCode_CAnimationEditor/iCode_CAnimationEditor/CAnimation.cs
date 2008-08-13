using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
//7/23/08
namespace iCode_CAnimationEditor
{
    public class CAnimation
    {
        string szname;
        public string szName { get { return szname; } set { szname = value; } }

        List<Frame> m_frames = new List<Frame>();
        public List<Frame> m_Frames { get { return m_frames; } }
        
        float       m_fspeed;
        public float m_fSpeed { get { return m_fspeed; } set { m_fspeed = value; } }
        
        public void AddFrame(Frame toAdd)
        {
            m_frames.Add(toAdd);
        }

        public void RemoveFrame(Frame toRemove)
        {
            m_frames.Remove(toRemove);
        }

        public override string ToString()
        {
            if (szName != null)
                return szName;
            else
                return "Animation";
        }
    }

    public class Frame
    {

        #region Frame Rectangle Information
                int frameX;
                public int FrameX { get { return frameX; } set { frameX = value; } }
            
                int frameY;
                public int FrameY { get { return frameY; } set { frameY = value; } }

                int frameWidth;
                public int FrameWidth { get { return frameWidth; } set { frameWidth = value; } }
                
                int frameHeight;
                public int FrameHeight { get { return frameHeight; } set { frameHeight = value; } }
        #endregion

        #region Collision Rectangle Information
                int collisionX;
                public int CollisionX { get { return collisionX; } set { collisionX = value; } }

                int collisionY;
                public int CollisionY { get { return collisionY; } set { collisionY = value; } }

                int collisionWidth;
                public int CollisionWidth { get { return collisionWidth; } set { collisionWidth = value; } }

                int collisionHeight;
                public int CollisionHeight { get { return collisionHeight; } set { collisionHeight = value; } }
        #endregion

        #region Passive Rectangle Information
        int passiveX;
        public int PassiveX { get { return passiveX; } set { passiveX = value; } }

        int passiveY;
        public int PassiveY { get { return passiveY; } set { passiveY = value; } }

        int passiveWidth;
        public int PassiveWidth { get { return passiveWidth; } set { passiveWidth = value; } }

        int passiveHeight;
        public int PassiveHeight { get { return passiveHeight; } set {passiveHeight = value; } }
        #endregion

        float       fduration;
        public float fDuration { get { return fduration; } set { fduration = value; } }
        
        //Point       ptanchor;
        //public Point ptAnchor { get { return ptanchor; } set { ptanchor = value; } }

        int anchorX;
        public int AnchorX { get { return anchorX; } set { anchorX = value; }}

        int anchorY;
        public int AnchorY { get { return anchorY; } set { anchorY = value;}}

        List<Point> accessoryPoints = new List<Point>();
     
        public List<Point> AccessoryPoint { get { return accessoryPoints; } set { accessoryPoints = value; } }

        string sztriggertype = "None";
        public string TriggerType { get { return sztriggertype; } set { sztriggertype = value; } }

        string szTriggerName = "None";
        public string TriggerName { get { return szTriggerName; } set { szTriggerName = value; } }

        public override string ToString()
        {
            return "Frame";// +nframenumber.ToString();
        }
    }

}
