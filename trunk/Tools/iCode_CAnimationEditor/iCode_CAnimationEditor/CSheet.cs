using System;
using System.Collections.Generic;
using System.Text;
//7/23/08
namespace iCode_CAnimationEditor
{
    public class CSheet
    {
        List<CAnimation> m_Animations = new List<CAnimation>();
        public List<CAnimation> Animations { get { return m_Animations; } set { m_Animations = value; } }
        
        string m_szUnitType = "Infantry";
        public string UnitType { get { return m_szUnitType; } set { m_szUnitType = value; } }

        public void AddAnimation(CAnimation toAdd)
        {
            m_Animations.Add(toAdd);
        }

        public void RemoveAnimation(CAnimation toRemove)
        {
            m_Animations.Remove(toRemove);
        }
    }
}
