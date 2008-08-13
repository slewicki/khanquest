using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using SGD;
using System.IO;
using System.Xml.Serialization;
//7/23/08
namespace iCode_CAnimationEditor
{
    public partial class Form1 : Form
    {
        
        CSheet Sheet = new CSheet();
        
        int FrameRect;

        int playcounter = 0;

        public Form1()
        {
            InitializeComponent();

            FrameRect = 1;
            FrameRectbutton.BackColor = Color.FromArgb(255, 255, 128, 0);
        }

        public CSheet CSheet
        {
            get
            {
                //CSheet Sheet = new CSheet();

                return Sheet;
            }
            set
            {
                while (AnimationlistBox.SelectedItem != null)
                {
                    Sheet.AddAnimation((CAnimation)AnimationlistBox.SelectedItem);
                    AnimationlistBox.SelectedIndex++;
                }

            }
        }

#region Adds
        private void AddAnimationbutton_Click(object sender, EventArgs e)
        {
            CAnimation toAdd = new CAnimation();
            CSheet.Animations.Add(toAdd);
            
            if (AnimationNametextBox.Text == "")
                AnimationNametextBox.Text = "Animation";
            toAdd.szName = AnimationNametextBox.Text;

            AnimationlistBox.Items.Add(toAdd);
            AnimationlistBox.SelectedItem = toAdd;
            AnimationNametextBox.Text = "";
        }
        
        private void AccessoryAddbutton_Click(object sender, EventArgs e)
        {
            if (FramelistBox.SelectedItem == null)
                return;
            Point toAdd = new Point(); 
            AccessorylistBox.Items.Add(toAdd);
            AccessorylistBox.SelectedItem = toAdd;
            Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint.Add(toAdd);

        }

        private void AddFramebutton_Click(object sender, EventArgs e)
        {
            if (AnimationlistBox.SelectedItem != null)
            {
                Frame toAdd = new Frame();
                CSheet.Animations[AnimationlistBox.SelectedIndex].AddFrame(toAdd);     
                FramelistBox.Items.Add(toAdd);
                FramelistBox.SelectedItem = toAdd;
                FrameClear();
                AccessorylistBox.Items.Clear();
            }
        }
#endregion

#region Removes
        private void RemoveAnimationbutton_Click(object sender, EventArgs e)
        {
            if (AnimationlistBox.SelectedItem == null)
                return;
            AnimationlistBox.SelectedIndex--;
            CSheet.Animations.RemoveAt(AnimationlistBox.SelectedIndex + 1);
            AnimationlistBox.Items.RemoveAt(AnimationlistBox.SelectedIndex + 1);
            if (AnimationlistBox.Items == null)
                FramelistBox.Items.Clear();
        }

        private void RemoveFramebutton_Click(object sender, EventArgs e)
        {
            if (FramelistBox.SelectedItem == null)
                return;
            FramelistBox.SelectedIndex--;
            CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.RemoveAt(FramelistBox.SelectedIndex+1);
            FramelistBox.Items.RemoveAt(FramelistBox.SelectedIndex+1);
            if (FramelistBox.Items == null)
                AccessorylistBox.Items.Clear();
        }

        private void RemoveAccessorybutton_Click(object sender, EventArgs e)
        {
            if (AccessorylistBox.SelectedItem == null || FramelistBox.SelectedItem == null)
                return;
            AccessorylistBox.SelectedIndex--;
            CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint.RemoveAt(AccessorylistBox.SelectedIndex + 1);
            AccessorylistBox.Items.RemoveAt(AccessorylistBox.SelectedIndex + 1);
        }
#endregion

#region Inserts
        private void InsertAnimationbutton_Click(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 )
                return;
            CAnimation toInsert = new CAnimation();
            AnimationlistBox.Items.Insert(AnimationlistBox.SelectedIndex + 1,toInsert);
            Sheet.Animations.Insert(AnimationlistBox.SelectedIndex + 1, toInsert);
            AnimationlistBox.SelectedItem = toInsert;
            if (AnimationNametextBox.Text == "")
                AnimationNametextBox.Text = "Animation";
            toInsert.szName = AnimationNametextBox.Text;
        }
       
        private void InsertAccessorybutton_Click(object sender, EventArgs e)
        {
            if (FramelistBox.SelectedItem == null)
                return;
            Point toInsert = new Point();
            AccessorylistBox.Items.Insert(AccessorylistBox.SelectedIndex + 1, toInsert);
            Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint.Insert(AccessorylistBox.SelectedIndex + 1, toInsert);
            AccessorylistBox.SelectedItem = toInsert;
        }
       
        private void InstertFramebutton_Click(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
                return;
            Frame toInsert = new Frame();
            FramelistBox.Items.Insert(FramelistBox.SelectedIndex + 1, toInsert);
            Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Insert(FramelistBox.SelectedIndex + 1, toInsert);
            FramelistBox.SelectedItem = toInsert;
        }
#endregion

#region FrameInfo

        public void FrameClear()
        {
            WidthnumericUpDown.Value = 0;
            HeightnumericUpDown.Value = 0;
            XPosnumercUpDown.Value = 0;
            YPosnumericUpDown.Value = 0;
            TriggercomboBox.Text = "None";
            TriggerNametextBox.Text = " ";
            DurrationnumericUpDown.Value = 0;
            XnumericUpDownaccesory.Value = 0;
            YnumericUpDownaccessory.Value = 0;
        }

        private void XPosnumercUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
            {
                XPosnumercUpDown.Value = 0;
                return;
            }

            if (FrameRect == 1)
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameX = (int)XPosnumercUpDown.Value;
                sheetDisplayWindow1.FrameX = (int)XPosnumercUpDown.Value;    
            }
            else if (FrameRect == 2)
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionX = (int)XPosnumercUpDown.Value;
                sheetDisplayWindow1.CollisionX = (int)XPosnumercUpDown.Value;
            }
            else
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveX = (int)XPosnumercUpDown.Value;
                sheetDisplayWindow1.PassiveX = (int)XPosnumercUpDown.Value;
            }
            
            previewWindow1.FrameX = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameX;

            sheetDisplayWindow1.Invalidate();
            previewWindow1.Invalidate();
        }

        private void YPosnumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
            {
                YPosnumericUpDown.Value = 0;
                return;
            }

            if (FrameRect == 1)
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameY = (int)YPosnumericUpDown.Value;
                sheetDisplayWindow1.FrameY = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameY;
            }
            else if (FrameRect == 2)
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionY = (int)YPosnumericUpDown.Value;
                sheetDisplayWindow1.CollisionY = (int)YPosnumericUpDown.Value;
            }
            else
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveY = (int)YPosnumericUpDown.Value;
                sheetDisplayWindow1.PassiveY = (int)YPosnumericUpDown.Value;
            }
            
            previewWindow1.FrameY = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameY;

            previewWindow1.Invalidate();
            sheetDisplayWindow1.Invalidate();
        }

        private void WidthnumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
            {
                WidthnumericUpDown.Value = 0;
                return;
            }

            if (FrameRect == 1)
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameWidth = (int)WidthnumericUpDown.Value;
                sheetDisplayWindow1.FrameRectWidth = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameWidth;
            }
            else if (FrameRect == 2)
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionWidth = (int)WidthnumericUpDown.Value;
                sheetDisplayWindow1.CollisionRectWidth = (int)WidthnumericUpDown.Value;
            }
            else
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveWidth = (int)WidthnumericUpDown.Value;
                sheetDisplayWindow1.PassiveWidth = (int)WidthnumericUpDown.Value;
            }
            
            previewWindow1.FrameWidth = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameWidth;

            previewWindow1.Invalidate();
            sheetDisplayWindow1.Invalidate();
        }

        private void HeightnumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
            {
                HeightnumericUpDown.Value = 0;
                return;
            }

            if (FrameRect == 1)
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameHeight = (int)HeightnumericUpDown.Value;
                sheetDisplayWindow1.FrameRectHeight = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameHeight;                
            }
            else if (FrameRect == 2)
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionHeight = (int)HeightnumericUpDown.Value;
                sheetDisplayWindow1.CollisionRectHeight = (int)HeightnumericUpDown.Value;
            }
            else
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveHeight = (int)HeightnumericUpDown.Value;
                sheetDisplayWindow1.PassiveHeight = (int)HeightnumericUpDown.Value;
            }
            
            previewWindow1.FrameHeight = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameHeight;

            previewWindow1.Invalidate();
            sheetDisplayWindow1.Invalidate();
        }

        private void AptXnumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
            {
                AptXnumericUpDown.Value = 0;
                return;
            }

            CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AnchorX = (int)AptXnumericUpDown.Value;
            sheetDisplayWindow1.AptX = (int)AptXnumericUpDown.Value;
            previewWindow1.AptX = (int)AptXnumericUpDown.Value;
            previewWindow1.Invalidate();
            sheetDisplayWindow1.Invalidate();
        }

        private void AptYnumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
            {
                AptYnumericUpDown.Value = 0;
                return;
            }

            CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AnchorY = (int)AptYnumericUpDown.Value;
            sheetDisplayWindow1.AptY = (int)AptYnumericUpDown.Value;
            previewWindow1.AptY = (int)AptYnumericUpDown.Value;
            previewWindow1.Invalidate();
            sheetDisplayWindow1.Invalidate();
        }

        private void DurrationnumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
            {
                DurrationnumericUpDown.Value = 0;
                return;
            }

            CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].fDuration = (float)DurrationnumericUpDown.Value;
        }

        private void TriggercomboBox_Leave(object sender, EventArgs e)
        {
            if (FramelistBox.SelectedItem != null)
                Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].TriggerType = TriggercomboBox.Text;
        }

        private void TriggerNametextBox_Leave(object sender, EventArgs e)
        {
            if (FramelistBox.SelectedItem != null)
                Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].TriggerName = TriggerNametextBox.Text;
            else
                TriggerNametextBox.Text = " ";
        }

        private void XnumericUpDownaccesory_ValueChanged(object sender, EventArgs e)
        {
            if (AccessorylistBox.SelectedItem == null)
            {
                XnumericUpDownaccesory.Value = 0;
                return;
            }
            if (Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint.Count <= 0)
            {
                XnumericUpDownaccesory.Value = 0;
                return;
            }
            Point temporary = new Point();
            temporary.X = (int)XnumericUpDownaccesory.Value;
            temporary.Y = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint[AccessorylistBox.SelectedIndex].Y; 
            CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint[AccessorylistBox.SelectedIndex] = temporary;
            sheetDisplayWindow1.AccessoryX = (int)XnumericUpDownaccesory.Value;
            previewWindow1.Invalidate();
            sheetDisplayWindow1.Invalidate();
        }

        private void YnumericUpDownaccessory_ValueChanged(object sender, EventArgs e)
        {
            if (AccessorylistBox.SelectedItem == null)
            {
                YnumericUpDownaccessory.Value = 0;
                return;
            }
            if (Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint.Count <= 0)
            {
                YnumericUpDownaccessory.Value = 0;
                return;
            }
            Point temporary = new Point();
            temporary.X = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint[AccessorylistBox.SelectedIndex].X;
            temporary.Y = (int)YnumericUpDownaccessory.Value;
            CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint[AccessorylistBox.SelectedIndex] = temporary;
            sheetDisplayWindow1.AccessoryY = (int)YnumericUpDownaccessory.Value;
            previewWindow1.Invalidate();
            sheetDisplayWindow1.Invalidate();
            
        }

#endregion

#region SelectionChange
        private void AnimationlistBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || AnimationlistBox.SelectedItem == null)
                return;
            
            timer1.Stop();
            FramelistBox.Items.Clear();
            for (int i = 0; i < ((CAnimation)AnimationlistBox.SelectedItem).m_Frames.Count; ++i)
            {
                FramelistBox.Items.Add(Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[i]);
            }
            if (FramelistBox.Items.Count > 0)
                FramelistBox.SelectedIndex = 0;
            AnimationNametextBox.Text = ((CAnimation)Sheet.Animations[AnimationlistBox.SelectedIndex]).szName;
            AnimationlistBox.Update();
            AccessorylistBox.Items.Clear();
        }

        private void FramelistBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (AnimationlistBox.SelectedItem == null)
                return;
            if (Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0 || FramelistBox.SelectedItem == null)
                return;

            #region FrameRect
            sheetDisplayWindow1.FrameX = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameX;
            sheetDisplayWindow1.FrameY = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameY;
            sheetDisplayWindow1.FrameRectWidth = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameWidth;
            sheetDisplayWindow1.FrameRectHeight = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameHeight;
            #endregion
            #region CollRect
            sheetDisplayWindow1.CollisionX = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionX;
            sheetDisplayWindow1.CollisionY = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionY;
            sheetDisplayWindow1.CollisionRectWidth = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionWidth;
            sheetDisplayWindow1.CollisionRectHeight = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionHeight;
            #endregion
            #region PassiveRect
            sheetDisplayWindow1.PassiveX = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveX;
            sheetDisplayWindow1.PassiveY = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveY;
            sheetDisplayWindow1.PassiveWidth = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveWidth;
            sheetDisplayWindow1.PassiveHeight = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveHeight;
            #endregion

            if (FrameRect == 1)
            {
                XPosnumercUpDown.Value = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameX;
                YPosnumericUpDown.Value = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameY;
                WidthnumericUpDown.Value = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameWidth;
                HeightnumericUpDown.Value = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameHeight;
                
            }
            else if (FrameRect == 2)
            {
                XPosnumercUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionX;
                YPosnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionY;
                WidthnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionWidth;
                HeightnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionHeight;
            }
            else
            {
                XPosnumercUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveX;
                YPosnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveY;
                WidthnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveWidth;
                HeightnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveHeight;
            }
           
            DurrationnumericUpDown.Value = (decimal)Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].fDuration;
            TriggerNametextBox.Text = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].TriggerName;
            TriggercomboBox.Text = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].TriggerType;
            AptYnumericUpDown.Value = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AnchorY;
            AptXnumericUpDown.Value = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AnchorX;

            AccessorylistBox.Items.Clear();
            for (int i = 0; i < ((Frame)FramelistBox.SelectedItem).AccessoryPoint.Count; ++i)
            {
                AccessorylistBox.Items.Add(Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint[i]);
            }
            //if (Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint.Count > 0)
            //{
            //    AccessorylistBox.SelectedIndex = 0;
            //    XnumericUpDownaccesory.Value = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint[AccessorylistBox.SelectedIndex].X;
            //    YnumericUpDownaccessory.Value = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint[AccessorylistBox.SelectedIndex].Y;
            //}
            SpeednumericUpDown.Value = (decimal)Sheet.Animations[AnimationlistBox.SelectedIndex].m_fSpeed;

            sheetDisplayWindow1.Invalidate();
            previewWindow1.Invalidate();
        }
#endregion

        private void FrameRectbutton_Click(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
                return;

            if (FrameRect != 1)
            {
                FrameRect = 1;

                FrameRectbutton.BackColor = Color.FromArgb(255, 255, 128, 0);
                CollisionRectbutton.BackColor = Control.DefaultBackColor;
                PassiveRectbutton.BackColor = Control.DefaultBackColor;

                XPosnumercUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameX;
                YPosnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameY;
                WidthnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameWidth;
                HeightnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].FrameHeight;
            }
        }

        private void PassiveRectbutton_Click(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
                return;

            if (FrameRect != 3)
            {
                FrameRect = 3;

                PassiveRectbutton.BackColor = Color.FromArgb(255, 255, 128, 0);
                FrameRectbutton.BackColor = Control.DefaultBackColor;
                CollisionRectbutton.BackColor = Control.DefaultBackColor;

                XPosnumercUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveX;
                YPosnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveY;
                WidthnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveWidth;
                HeightnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].PassiveHeight;
            }
        }

        private void CollisionRectbutton_Click(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
                return;

            if (FrameRect != 2)
            {
                FrameRect = 2;

                CollisionRectbutton.BackColor = Color.FromArgb(255, 255, 128, 0);
                FrameRectbutton.BackColor = Control.DefaultBackColor;
                PassiveRectbutton.BackColor = Control.DefaultBackColor;

                XPosnumercUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionX;
                YPosnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionY;
                WidthnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionWidth;
                HeightnumericUpDown.Value = CSheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].CollisionHeight;
            }
        }

        private void AnimationNametextBox_Leave(object sender, EventArgs e)
        {
            if (AnimationlistBox.SelectedItem != null)
            {
                CSheet.Animations[AnimationlistBox.SelectedIndex].szName = AnimationNametextBox.Text;
            }
            else
                AnimationNametextBox.Text = "Default";
            if (AnimationNametextBox.Text == "")
            {
                AnimationNametextBox.Text = "Default";
            }
        }

#region Preview

        private void Playbutton_Click(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
                return;
            timer1.Enabled = true;
            timer1.Interval = 1;
            timer1.Start();
            FrameRect = 1;
            FrameRectbutton.BackColor = Color.FromArgb(255, 255, 128, 0);
            PassiveRectbutton.BackColor = Control.DefaultBackColor;
            CollisionRectbutton.BackColor = Control.DefaultBackColor;
            CollisionRectbutton.Enabled = false;
            PassiveRectbutton.Enabled = false;
            sheetDisplayWindow1.Enabled = false;
            XPosnumercUpDown.Enabled = false;
            YPosnumericUpDown.Enabled = false;
            WidthnumericUpDown.Enabled = false;
            HeightnumericUpDown.Enabled = false;
            AptXnumericUpDown.Enabled = false;
            AptYnumericUpDown.Enabled = false;

            XnumericUpDownaccesory.Enabled = false;
            YnumericUpDownaccessory.Enabled = false;
            #region Add
            AddFramebutton.Enabled = false;
            AddAnimationbutton.Enabled = false;
            AccessoryAddbutton.Enabled = false;
            #endregion
            #region Insert
            InsertAccessorybutton.Enabled = false;
            InsertAnimationbutton.Enabled = false;
            InstertFramebutton.Enabled = false;
            #endregion
            #region Remove
            RemoveAccessorybutton.Enabled = false;
            RemoveAnimationbutton.Enabled = false;
            RemoveFramebutton.Enabled = false;
            #endregion
            TriggercomboBox.Enabled = false;
            TriggerNametextBox.Enabled = false;
            DurrationnumericUpDown.Enabled = false;
            AnimationlistBox.Enabled = false;

        }
       
        private void Stopbutton_Click(object sender, EventArgs e)
        {
            timer1.Stop();
            CollisionRectbutton.Enabled = true;
            PassiveRectbutton.Enabled = true;
            sheetDisplayWindow1.Enabled = true;
            XPosnumercUpDown.Enabled = true;
            YPosnumericUpDown.Enabled = true;
            WidthnumericUpDown.Enabled = true;
            HeightnumericUpDown.Enabled = true;
            AptXnumericUpDown.Enabled = true;
            AptYnumericUpDown.Enabled = true;

            XnumericUpDownaccesory.Enabled = true;
            YnumericUpDownaccessory.Enabled = true;
            #region Add
            AddFramebutton.Enabled = true;
            AddAnimationbutton.Enabled = true;
            AccessoryAddbutton.Enabled = true;
            #endregion
            #region Insert
            InsertAccessorybutton.Enabled = true;
            InsertAnimationbutton.Enabled = true;
            InstertFramebutton.Enabled = true;
            #endregion
            #region Remove
            RemoveAccessorybutton.Enabled = true;
            RemoveAnimationbutton.Enabled = true;
            RemoveFramebutton.Enabled = true;
            #endregion
            TriggercomboBox.Enabled = true;
            TriggerNametextBox.Enabled = true;
            DurrationnumericUpDown.Enabled = true;
            AnimationlistBox.Enabled = true;
        }

        private void Prevbutton_Click(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
                return;

            if (0 >= FramelistBox.SelectedIndex)
                FramelistBox.SelectedIndex = FramelistBox.Items.Count-1;
            else
                FramelistBox.SelectedIndex--;
        }

        private void Nextbutton_Click(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
                return;

            if (FramelistBox.Items.Count - 1 <= FramelistBox.SelectedIndex)
                FramelistBox.SelectedIndex = 0;
            else
                FramelistBox.SelectedIndex++;
        }

        private void Resetbutton_Click(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
                return;
            FramelistBox.SelectedIndex = 0;
        }
#endregion
       
#region MenuBar
        private void newToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            timer1.Stop();
            Sheet.Animations.Clear();
            FrameClear();
            FramelistBox.Items.Clear();
            AnimationlistBox.Items.Clear();
            AccessorylistBox.Items.Clear();
            AnimationNametextBox.Text = "";
            SpeednumericUpDown.Value = 0;
            UnitTypetextBox.Text = "Infantry";
            AptXnumericUpDown.Value = AptYnumericUpDown.Value = 0;
            sheetDisplayWindow1.FrameX = sheetDisplayWindow1.FrameY = sheetDisplayWindow1.FrameRectWidth = sheetDisplayWindow1.FrameRectHeight = 0;
            sheetDisplayWindow1.CollisionX = sheetDisplayWindow1.CollisionY = sheetDisplayWindow1.CollisionRectWidth = sheetDisplayWindow1.CollisionRectHeight = 0;
            sheetDisplayWindow1.PassiveX = sheetDisplayWindow1.PassiveY = sheetDisplayWindow1.PassiveWidth = sheetDisplayWindow1.PassiveHeight = 0;
            sheetDisplayWindow1.SetBitmap(null);
            previewWindow1.FrameX = previewWindow1.FrameY = 0;
            sheetDisplayWindow1.SetBitmap(null);
            sheetDisplayWindow1.Invalidate();
            previewWindow1.SetBitmap(null);
            previewWindow1.Invalidate();
        }

        private void exitToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            this.Close();
        }

        private void importImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Stopbutton_Click(sender, e);
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "png File|*.png";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                string szFileName;
                szFileName = dlg.FileName;

                sheetDisplayWindow1.SetBitmap(szFileName);

                previewWindow1.SetBitmap(szFileName);

                sheetDisplayWindow1.Invalidate();
                previewWindow1.Invalidate();
            }
        }

        private void saveBinToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                    SaveFileDialog dlg = new SaveFileDialog();
                    dlg.DefaultExt = "dat";
                    dlg.Filter = "Binary|*.dat";
                    if (DialogResult.OK == dlg.ShowDialog())
                    {
                        FileInfo fi = new FileInfo(dlg.FileName);
                        //prefixing the filename
                        if (!fi.Name.StartsWith("KQ_"))
                        {
                            string filename = fi.DirectoryName + "//" + "KQ_" + fi.Name;
                            dlg.FileName = filename;
                        }
                        
                        FileStream fs = File.Create(dlg.FileName);
                        BinaryWriter bw = new BinaryWriter(fs);
                        //write out the length of the unit type string
                        bw.Write((Int32)(Sheet.UnitType.Length));
                        //write out the unit type string
                        bw.Write(Sheet.UnitType.ToCharArray());
                        //find the number of animations in the sheet
                        bw.Write((Int16)Sheet.Animations.Count);
                        for (int i = 0; i < Sheet.Animations.Count; i++)
                        {
                            //write out the length of the name
                            bw.Write((Int32)Sheet.Animations[i].szName.Length);
                            //write out the name as a char array
                            bw.Write(Sheet.Animations[i].szName.ToCharArray());
                            //write out the speed to play the animation at
                            bw.Write((double)Sheet.Animations[i].m_fSpeed);
                            //find the number of frames in current animation
                            bw.Write((Int16)Sheet.Animations[i].m_Frames.Count);
                            for (int j = 0; j < Sheet.Animations[i].m_Frames.Count; j++)
                            {
                                #region RenderRect
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].FrameX);
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].FrameY);
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].FrameWidth);
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].FrameHeight);
                                #endregion

                                #region CollisionRect
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].CollisionX);
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].CollisionY);
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].CollisionWidth);
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].CollisionHeight);
                                #endregion

                                #region PassiveRect
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].PassiveX);
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].PassiveY);
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].PassiveWidth);
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].PassiveHeight);
                                #endregion

                                #region Anchor Points
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].AnchorX);
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].AnchorY);
                                #endregion

                                //write out the current frames lifespan
                                bw.Write((double)Sheet.Animations[i].m_Frames[j].fDuration);
                                
                                #region Trigger Information
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].TriggerName.Length);
                                bw.Write(Sheet.Animations[i].m_Frames[j].TriggerName.ToCharArray());
                                bw.Write((Int32)Sheet.Animations[i].m_Frames[j].TriggerType.Length);
                                bw.Write(Sheet.Animations[i].m_Frames[j].TriggerType.ToCharArray());
                                #endregion

                                //find the ammmount of accessory points in the current frame
                                bw.Write((Int16)Sheet.Animations[i].m_Frames[j].AccessoryPoint.Count);
                                for (int k = 0; k < Sheet.Animations[i].m_Frames[j].AccessoryPoint.Count; k++)
                                {
                                    //write out the location of the current accessory point
                                    bw.Write((Int32)Sheet.Animations[i].m_Frames[j].AccessoryPoint[k].X);
                                    bw.Write((Int32)Sheet.Animations[i].m_Frames[j].AccessoryPoint[k].Y);
                                }
                            }
                        }
                        bw.Close();
                        fs.Close();
                    }
            }
            catch (Exception de)
            {
                MessageBox.Show("Error saving DAT file! Exception: " + de.GetType().Name, "Error");
            }
        }

        private void saveXMLToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                XmlSerializer serializer = new XmlSerializer(typeof(CSheet));
                SaveFileDialog dlg = new SaveFileDialog();
                dlg.DefaultExt = "xml";
                dlg.Filter = "XML|*.xml";
 
                if (DialogResult.OK == dlg.ShowDialog())
                {
                    //prefix the filename
                    FileInfo fi = new FileInfo(dlg.FileName);
                    if (!fi.Name.StartsWith("KQ_"))
                    {
                        string filename = fi.DirectoryName + "//" + "KQ_" + fi.Name;
                        dlg.FileName = filename;
                    }

                    StreamWriter writer = new StreamWriter(dlg.FileName);
                    //serialize the data
                    serializer.Serialize(writer, Sheet);

                    writer.Close();
                }
            }
            catch (Exception de)
            {
                MessageBox.Show("Error saving XML file! Exception: "+de.GetType().Name, "Error");
            }
        }

        private void openBINToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                OpenFileDialog dlg = new OpenFileDialog();
                dlg.DefaultExt = "dat";
                dlg.Filter = "Binary|*.dat";
                if (DialogResult.OK == dlg.ShowDialog())
                {
                    #region Clear
                    FrameClear();
                    FramelistBox.Items.Clear();
                    AnimationlistBox.Items.Clear();
                    AccessorylistBox.Items.Clear();
                    AnimationNametextBox.Text = " ";
                    SpeednumericUpDown.Value = 0;
                    #endregion

                    FileStream fs = new FileStream(dlg.FileName, FileMode.Open);
                    BinaryReader br = new BinaryReader(fs);
                    //read in the unit type string's length
                    int length = br.ReadInt32();
                    //set the sheet's unit type to the files' unit type 
                    Sheet.UnitType = new string(br.ReadChars(length));
                    UnitTypetextBox.Text = Sheet.UnitType;
                    //find the number of animation
                    int count = br.ReadInt16();
                    for (int i = 0; i < count; i++)
                    {
                        CAnimation animation = new CAnimation();
                        //read in how long the name is
                        length = br.ReadInt32();
                        //read in the name and set it
                        animation.szName = new string(br.ReadChars(length));
                        //set the animations speed
                        animation.m_fSpeed = (float)br.ReadDouble();
                        //find out how many frames are in the current animation
                        int framecount = br.ReadInt16();
                        for (int j = 0; j < framecount; j++)
                        {
                            Frame toAdd = new Frame();
                            #region RenderRect
                                toAdd.FrameX = br.ReadInt32();
                                toAdd.FrameY = br.ReadInt32();
                                toAdd.FrameWidth = br.ReadInt32();
                                toAdd.FrameHeight = br.ReadInt32();
                            #endregion
                            #region CollisionRect
                                toAdd.CollisionX = br.ReadInt32();
                                toAdd.CollisionY = br.ReadInt32();
                                toAdd.CollisionWidth = br.ReadInt32();
                                toAdd.CollisionHeight = br.ReadInt32();
                            #endregion
                            #region PassiveRect
                                toAdd.PassiveX = br.ReadInt32();
                                toAdd.PassiveY = br.ReadInt32();
                                toAdd.PassiveWidth = br.ReadInt32();
                                toAdd.PassiveHeight = br.ReadInt32();
                            #endregion
                            toAdd.AnchorX = br.ReadInt32();
                            toAdd.AnchorY = br.ReadInt32();
                            toAdd.fDuration = (float)br.ReadDouble();
                            //reead in the length of the trigger name
                            int triggernamelen = br.ReadInt32();
                            //set the trigger name
                            toAdd.TriggerName = new string(br.ReadChars(triggernamelen));
                            //read in the length of the trigger type
                            int triggertyplen = br.ReadInt32();
                            //set the trigger type
                            toAdd.TriggerType = new string(br.ReadChars(triggertyplen));
                            //find the number of accessories in a frame
                            int accessorycount = br.ReadInt16();
                            for (int k = 0; k < accessorycount; k++)
                            {
                                //set the accessory's data
                                Point accessory = new Point();
                                accessory.X = br.ReadInt32();
                                accessory.Y = br.ReadInt32();
                                //add the accessory to the frame
                                toAdd.AccessoryPoint.Add(accessory);
                            }
                            animation.AddFrame(toAdd);
                        }
                        Sheet.AddAnimation(animation);
                        AnimationlistBox.Items.Add(animation);
                    }
                    br.Close();
                    fs.Close();
                    AnimationlistBox.SelectedIndex = 0;
                }            
            }
            catch(Exception de)
            {
                MessageBox.Show("Error opening BIN file! Exception: " + de.GetType().Name, "Open Error");
            }
        }

        private void openXMLToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                OpenFileDialog dlg = new OpenFileDialog();
                dlg.DefaultExt = "xml";
                dlg.Filter = "XML|*.xml";
                if (DialogResult.OK == dlg.ShowDialog())
                {
                    #region Clear
                    FrameClear();
                    FramelistBox.Items.Clear();
                    AnimationlistBox.Items.Clear();
                    AccessorylistBox.Items.Clear();
                    AnimationNametextBox.Text = " ";
                    SpeednumericUpDown.Value = 0;
                    #endregion

                    FileStream fs = new FileStream(dlg.FileName, FileMode.Open);

                    XmlSerializer serializer = new XmlSerializer(typeof(CSheet));
                    Sheet = (CSheet)serializer.Deserialize(fs);

                    fs.Close();

                    AnimationlistBox.Items.Clear();
                    FramelistBox.Items.Clear();

                    for (int i = 0; i < Sheet.Animations.Count; i++)
                        AnimationlistBox.Items.Add(Sheet.Animations[i]);

                    UnitTypetextBox.Text = Sheet.UnitType;

                    if(AnimationlistBox.Items.Count > 0)
                        AnimationlistBox.SelectedIndex = 0;
                }
            }
            catch (Exception de)
            {
                MessageBox.Show("Error opening XML file! Exception: " + de.GetType().Name, "Open Error");
            }
        }
#endregion

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <=0)
                return;
            
            if (((Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].fDuration)*10)/(float)SpeednumericUpDown.Value < playcounter)
            {
                if (FramelistBox.Items.Count - 1 <= FramelistBox.SelectedIndex)
                {
                    FramelistBox.SelectedIndex = 0;
                    playcounter = 0;
                }
                else
                {
                    FramelistBox.SelectedIndex++;
                    playcounter = 0;    
                }
            }
            else
                playcounter += timer1.Interval;
        }
      
#region Mouse Input
        private void sheetDisplayWindow1_MouseDown(object sender, MouseEventArgs e)
        {
            if (Sheet.Animations.Count <= 0 || Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames.Count <= 0)
                return;
            if (e.Button == MouseButtons.Left)
            {
                WidthnumericUpDown.Value = 0;
                HeightnumericUpDown.Value = 0;

                XPosnumercUpDown.Value = e.Location.X - sheetDisplayWindow1.Offset.X;
                YPosnumericUpDown.Value = e.Location.Y - sheetDisplayWindow1.Offset.Y;
            }
            sheetDisplayWindow1.Invalidate();
        }

        private void sheetDisplayWindow1_MouseUp(object sender, MouseEventArgs e)
        {
            if (FramelistBox.SelectedItem == null)
                return;
            if (sheetDisplayWindow1.GetBitmap() == null)
                return;
            if (e.Button == MouseButtons.Left)
            {
                if (e.Location.X - sheetDisplayWindow1.Offset.X < XPosnumercUpDown.Value)
                {
                    int switchvalues = (int)XPosnumercUpDown.Value;

                    if (e.Location.X + sheetDisplayWindow1.Offset.X > 0)
                    {

                        XPosnumercUpDown.Value = e.Location.X - sheetDisplayWindow1.Offset.X;
                        WidthnumericUpDown.Value = switchvalues - XPosnumercUpDown.Value;
                    }
                    else
                    {
                        if (e.Location.X - sheetDisplayWindow1.Offset.X < 0)
                            XPosnumercUpDown.Value = 0;
                        else
                            XPosnumercUpDown.Value = e.Location.X - sheetDisplayWindow1.Offset.X;
                        WidthnumericUpDown.Value = switchvalues - XPosnumercUpDown.Value;
                    }
                }
                else
                {
                    if (e.Location.X - sheetDisplayWindow1.Offset.X > sheetDisplayWindow1.GetBitmap().Width)
                    {
                        WidthnumericUpDown.Value = sheetDisplayWindow1.GetBitmap().Width - XPosnumercUpDown.Value - 1;
                    }
                    else
                        WidthnumericUpDown.Value = e.Location.X - XPosnumercUpDown.Value - sheetDisplayWindow1.Offset.X;
                }
                if (e.Location.Y - sheetDisplayWindow1.Offset.Y < YPosnumericUpDown.Value)
                {
                    int switchvalues = (int)YPosnumericUpDown.Value;

                    if (e.Location.Y + sheetDisplayWindow1.Offset.Y > 0)
                    {

                        YPosnumericUpDown.Value = e.Location.Y - sheetDisplayWindow1.Offset.Y;
                        HeightnumericUpDown.Value = switchvalues - YPosnumericUpDown.Value;
                    }
                    else
                    {
                        if (e.Location.Y - sheetDisplayWindow1.Offset.Y < 0)
                            YPosnumericUpDown.Value = 0;
                        else
                            YPosnumericUpDown.Value = e.Location.Y - sheetDisplayWindow1.Offset.Y;
                        HeightnumericUpDown.Value = switchvalues - YPosnumericUpDown.Value;
                    }
                }
                else
                {
                    if (e.Location.Y - sheetDisplayWindow1.Offset.Y > sheetDisplayWindow1.GetBitmap().Height)
                    {
                        HeightnumericUpDown.Value = sheetDisplayWindow1.GetBitmap().Height - YPosnumericUpDown.Value - 1;
                    }
                    else
                        HeightnumericUpDown.Value = e.Location.Y - YPosnumericUpDown.Value - sheetDisplayWindow1.Offset.Y;
                }
            }
            sheetDisplayWindow1.Invalidate();
        }
        #endregion

        private void SpeednumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (AnimationlistBox.SelectedItem != null)
                Sheet.Animations[AnimationlistBox.SelectedIndex].m_fSpeed = (float)SpeednumericUpDown.Value;
            else
                SpeednumericUpDown.Value = 0;
        }

        private void AccessorylistBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (FramelistBox.SelectedItem == null)
                return;
            if (Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint.Count <= 0 || AccessorylistBox.SelectedItem == null)
                return;

            XnumericUpDownaccesory.Value = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint[AccessorylistBox.SelectedIndex].X;
            YnumericUpDownaccessory.Value = Sheet.Animations[AnimationlistBox.SelectedIndex].m_Frames[FramelistBox.SelectedIndex].AccessoryPoint[AccessorylistBox.SelectedIndex].Y;
        }

        private void fileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Stopbutton_Click(sender,e);
        }

        private void UnitTypetextBox_Leave(object sender, EventArgs e)
        {
            Sheet.UnitType = UnitTypetextBox.Text;            
        }
    }
}