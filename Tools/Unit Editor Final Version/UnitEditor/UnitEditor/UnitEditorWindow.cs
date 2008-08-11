/// <summary>
/// File: “UnitEditorWindow.cs”
/// Author: Sean Hamstra (SH)
/// Purpose: This file contains all of the functionality of the Unit Editor form.
/// All of the editor's work is taken care of in this file.
/// </summary>

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml.Serialization;
using System.IO;

namespace UnitEditor
{
    public partial class UnitEditorWindow : Form
    {
        /// <summary>
        /// Constructor of UnitEditorWindow
        /// </summary>
        public UnitEditorWindow()
        {
            // Let VS initialize this stuff
            InitializeComponent();
            
        }
        bool m_bUpdating;
        /// <summary>
        /// Gets and Sets a CUnit to be used by the editor window
        /// </summary>
        /// <returns>A CUnit.</returns>
        public CUnit CUnit
        {
            get
            {
                CUnit unit = new CUnit();
                unit.HP =           (int)this.numericUpDownHP.Value;
                unit.Attack =       (int)this.numericUpDownAttack.Value;
                unit.Range =        (int)this.numericUpDownRange.Value;
                unit.AttackSpeed =  (float)this.numericUpDownAttSpeed.Value;
                unit.Movement = (float)this.numericUpDownMovement.Value;
                unit.Cost = (int)this.numericUpDownCost.Value;
                
                unit.HPMax = (int)numericUpDownHPMax.Value;
                unit.HPMin = (int)numericUpDownHPMin.Value;
                
                unit.AttackMax = (int)numericUpDownAttackMax.Value;
                unit.AttackMin = (int)numericUpDownAttackMin.Value;
                
                unit.RangeMax = (int)numericUpDownRangeMax.Value;
                unit.RangeMin = (int)numericUpDownRangeMin.Value;

                unit.CostMax = (int)numericUpDownCostMax.Value;
                unit.CostMin = (int)numericUpDownCostMin.Value;

                unit.AttackSpeedMax = (float)numericUpDownAttSpdMax.Value;
                unit.AttackSpeedMin = (float)numericUpDownAttSpdMin.Value;
                unit.MovementSpeedMax = (float)numericUpDownMoveMax.Value;
                unit.MovementSpeedMin = (float)numericUpDownMoveMin.Value;
                
                return unit;
            }   
            set
            {
                m_bUpdating = true;
                this.numericUpDownHP.Value = value.HP;
                this.numericUpDownAttack.Value = value.Attack; 
                this.numericUpDownRange.Value = value.Range;
                this.numericUpDownAttSpeed.Value = (decimal)value.AttackSpeed;
                this.numericUpDownMovement.Value = (decimal)value.Movement;
                this.numericUpDownCost.Value = value.Cost;

                numericUpDownAttackMax.Value = value.AttackMax;
                numericUpDownAttackMin.Value = value.AttackMin;

                numericUpDownHPMax.Value = value.HPMax;
                numericUpDownHPMin.Value = value.HPMin;

                numericUpDownCostMax.Value = value.CostMax;
                numericUpDownCostMin.Value = value.CostMin;

                numericUpDownRangeMax.Value = value.RangeMax;

                numericUpDownRangeMin.Value = value.RangeMin;

                numericUpDownAttSpdMax.Value = (decimal)value.AttackSpeedMax;
                numericUpDownAttSpdMin.Value = (decimal)value.AttackSpeedMin;
                

                numericUpDownMoveMax.Value = (decimal)value.MovementSpeedMax;
                numericUpDownMoveMin.Value = (decimal)value.MovementSpeedMin;
                m_bUpdating = false;
               

            }
        }

        /// <summary>
        /// When clicked the editor will close.
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// When a new unit in the list is selected, 
        /// this function sets the form information to the selected unit.
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void listBoxUnits_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listBoxUnits.SelectedItem != null)
            {
                CUnit unit = (CUnit)listBoxUnits.SelectedItem;
                this.CUnit = unit;
            }
        }

        /// <summary>
        /// When "Save As" is clicked in the File Menu a Save As dialog will open for 
        /// saving in the selected radio button file format(XML or Binary).
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                // Write in XML
                if (radioButtonXML.Checked)
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(List<CUnit>));
                    SaveFileDialog dlg = new SaveFileDialog();
                    dlg.DefaultExt = "xml";
                    dlg.Filter = "XML|*.xml";

                    List<CUnit> units = new List<CUnit>();
                    for (int i = 0; i < listBoxUnits.Items.Count; i++)
                    {
                        units.Add((CUnit)listBoxUnits.Items[i]);
                    }

                    if (DialogResult.OK == dlg.ShowDialog())
                    {
                        FileInfo fi = new FileInfo(dlg.FileName);
                        if (!fi.Name.StartsWith("KQ_"))
                        {
                            string filename = fi.DirectoryName + "//" + "KQ_" + fi.Name;
                            dlg.FileName = filename;
                        }
                        StreamWriter writer = new StreamWriter(dlg.FileName);
                        serializer.Serialize(writer, units);
                        writer.Close();
                    }
                }

                // Write in Binary
                else
                {
                    SaveFileDialog dlg = new SaveFileDialog();
                    dlg.DefaultExt = "dat";
                    dlg.Filter = "Binary|*.dat";
                    if (DialogResult.OK == dlg.ShowDialog())
                    {
                        FileInfo fi = new FileInfo(dlg.FileName);
                        if (!fi.Name.StartsWith("KQ_"))
                        {
                            string filename = fi.DirectoryName + "//" + "KQ_" + fi.Name;
                            dlg.FileName = filename;
                        } 
                        FileStream fs = File.Create(dlg.FileName);
                        BinaryWriter bw = new BinaryWriter(fs);
                        bw.Write((Int16)listBoxUnits.Items.Count);
                        for (int i = 0; i < listBoxUnits.Items.Count; i++)
                        {
                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).Type);
                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).HP);
                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).Attack);
                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).Range);
                            bw.Write((double)((CUnit)listBoxUnits.Items[i]).AttackSpeed);
                            bw.Write((double)((CUnit)listBoxUnits.Items[i]).Movement);
                            // New Stuff
                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).Cost);

                            // New Randomize stuff
                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).HPMin);
                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).HPMax);

                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).AttackMin);
                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).AttackMax);

                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).RangeMin);
                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).RangeMax);

                            bw.Write((double)((CUnit)listBoxUnits.Items[i]).AttackSpeedMin);
                            bw.Write((double)((CUnit)listBoxUnits.Items[i]).AttackSpeedMax);

                            bw.Write((double)((CUnit)listBoxUnits.Items[i]).MovementSpeedMin);
                            bw.Write((double)((CUnit)listBoxUnits.Items[i]).MovementSpeedMax);

                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).CostMin);
                            bw.Write((Int32)((CUnit)listBoxUnits.Items[i]).CostMax);

                            
                        }
                        bw.Close();
                        fs.Close();
                    }
                }
            }

            // Catch the Exception generated if the 
            // we have problems saving the file
            catch (Exception de)
            {
                // Give the user a little info and prevent a crash!
                MessageBox.Show("Error saving file! Exception: "+de.GetType().Name, "Error");
            }

        }

        /// <summary>
        /// When "Open" is clicked in the File Menu an Open dialog will launch 
        /// for opening of the selected radio button file format (XML or Binary).
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                // Open XML File
                if (radioButtonXML.Checked)
                {
                    OpenFileDialog dlg = new OpenFileDialog();
                    dlg.DefaultExt = "xml";
                    dlg.Filter = "XML|*.xml";
                    if (DialogResult.OK == dlg.ShowDialog())
                    {
                        FileStream fs = new FileStream(dlg.FileName, FileMode.Open);

                        List<CUnit> units = new List<CUnit>();
                        XmlSerializer serializer = new XmlSerializer(typeof(List<CUnit>));

                        units = (List<CUnit>)serializer.Deserialize(fs);
                        fs.Close();
                        listBoxUnits.Items.Clear();
                        foreach (CUnit unit in units)
                        {
                            listBoxUnits.Items.Add(unit);

                        }
                    }
                }
                // Open Binary File
                else
                {
                    OpenFileDialog dlg = new OpenFileDialog();
                    dlg.DefaultExt = "dat";
                    dlg.Filter = "Binary|*.dat";
                    if (DialogResult.OK == dlg.ShowDialog())
                    {
                        listBoxUnits.Items.Clear();
                        FileStream fs = new FileStream(dlg.FileName, FileMode.Open);

                        BinaryReader br = new BinaryReader(fs);
                        int count = br.ReadInt16();
                        for (int i = 0; i < count; i++)
                        {
                            CUnit unit = new CUnit();
                            unit.Type = (eUnitType)br.ReadInt32();
                            unit.HP = (int)br.ReadInt32();
                            unit.Attack = (int)br.ReadInt32();
                            unit.Range = (int)br.ReadInt32();
                            unit.AttackSpeed = (float)br.ReadDouble();
                            unit.Movement = (float)br.ReadDouble();
                            // New stuff
                            unit.Cost = (int)br.ReadInt32();

                            // New Randomize Stuff
                            unit.HPMin = (int)br.ReadInt32();
                            unit.HPMax = (int)br.ReadInt32();

                            unit.AttackMin = (int)br.ReadInt32();
                            unit.AttackMax = (int)br.ReadInt32();

                            unit.RangeMin = (int)br.ReadInt32();
                            unit.RangeMax = (int)br.ReadInt32();

                            unit.AttackSpeedMin = (float)br.ReadDouble();
                            unit.AttackSpeedMax = (float)br.ReadDouble();

                            unit.MovementSpeedMin = (float)br.ReadDouble();
                            unit.MovementSpeedMax = (float)br.ReadDouble();

                            unit.CostMin = (int)br.ReadInt32();
                            unit.CostMax = (int)br.ReadInt32();


                            listBoxUnits.Items.Add(unit);
                        }
                        br.Close();
                        fs.Close();
                    }
                }
                listBoxUnits.SelectedIndex = 0;
            }
          
            // Catch the Exception generated if the 
            // we have problems reading the file
            catch(Exception de)
            {
                // Give the user a little info and prevent a crash!
                MessageBox.Show("Error opening file! Exception: "+ de.GetType().Name, "Open Error");
                
                // Reset all values to make sure no garabe was read in
                Reset();
            }
        }

        /// <summary>
        /// When this numericUpDown is changed the selected unit's HP stat will change.
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void numericUpDownHP_ValueChanged(object sender, EventArgs e)
        {
            ((CUnit)listBoxUnits.SelectedItem).HP = this.CUnit.HP;
        }

        /// <summary>
        /// Loads initial editor set-up when the application is launched.
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void UnitEditorWindow_Load(object sender, EventArgs e)
        {
            Reset();
        }

        /// <summary>
        /// When this numericUpDown is changed the selected unit's Attack stat will change.
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void numericUpDownAttack_ValueChanged(object sender, EventArgs e)
        {
            ((CUnit)listBoxUnits.SelectedItem).Attack = this.CUnit.Attack;
        }

        /// <summary>
        /// When this numericUpDown is changed the selected unit's Range stat will change.
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void numericUpDownRange_ValueChanged(object sender, EventArgs e)
        {
            ((CUnit)listBoxUnits.SelectedItem).Range = this.CUnit.Range;
        }

        /// <summary>
        /// When this numericUpDown is changed the selected unit's Attack Speed stat will change.
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void numericUpDownAttSpeed_ValueChanged(object sender, EventArgs e)
        {
            ((CUnit)listBoxUnits.SelectedItem).AttackSpeed = this.CUnit.AttackSpeed;
        }

        /// <summary>
        /// When this numericUpDown is changed the selected unit's Movement stat will change.
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void numericUpDownMovement_ValueChanged(object sender, EventArgs e)
        {
            ((CUnit)listBoxUnits.SelectedItem).Movement = this.CUnit.Movement;
        }

        /// <summary>
        /// When "New" is selected in the File Menu all unit info will be reset.
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Reset();
        }

        /// <summary>
        /// Resets all unit info stats to the minimum values.
        /// </summary>
        /// <returns>void</returns>
        private void Reset()
        {
            listBoxUnits.Items.Clear();

            CUnit infantry = new CUnit(65, 80, 11, 1, 1.0f, 1.0f, eUnitType.INFANTRY);
            infantry.HPMin = 50;
            infantry.HPMax = 110;
            infantry.AttackMin = 6;
            infantry.AttackMax = 16;
            infantry.RangeMin = 1;
            infantry.RangeMax = 1;
            infantry.CostMin = 50;
            infantry.CostMax = 100;
            infantry.AttackSpeedMin = .5f;
            infantry.AttackSpeedMax = 1.5f;
            infantry.MovementSpeedMin = .5f;
            infantry.MovementSpeedMax = 1.5f;
            CUnit cavalry = new CUnit(150, 150, 10, 1, 1.0f, 2.0f, eUnitType.CAVALRY);
            cavalry.HPMin = 100;
            cavalry.HPMax = 200;
            cavalry.AttackMin = 5;
            cavalry.AttackMax = 15;
            cavalry.RangeMin = 1;
            cavalry.RangeMax = 1;
            cavalry.CostMin = 100;
            cavalry.CostMax = 200;
            cavalry.AttackSpeedMin = .5f;
            cavalry.AttackSpeedMax = 1.5f;
            cavalry.MovementSpeedMin = 1.0f;
            cavalry.MovementSpeedMax = 3.0f;
            CUnit cavalryArcher = new CUnit(150, 90, 8, 6, 1.5f, 2.0f, eUnitType.CAVALRY_ARCHER);
            cavalryArcher.HPMin = 60;
            cavalryArcher.HPMax = 120;
            cavalryArcher.AttackMin = 5;
            cavalryArcher.AttackMax = 11;
            cavalryArcher.RangeMin = 2;
            cavalryArcher.RangeMax = 10;
            cavalryArcher.CostMin = 100;
            cavalryArcher.CostMax = 200;
            cavalryArcher.AttackSpeedMin = 1.0f;
            cavalryArcher.AttackSpeedMax = 2.0f;
            cavalryArcher.MovementSpeedMin = 1.0f;
            cavalryArcher.MovementSpeedMax = 3.0f;
            CUnit axmen = new CUnit(65, 70, 12, 1, 1.0f, 1.0f, eUnitType.AXMEN);
            axmen.HPMin = 40;
            axmen.HPMax = 100;
            axmen.AttackMin = 6;
            axmen.AttackMax = 18;
            axmen.RangeMin = 1;
            axmen.RangeMax = 1;
            axmen.CostMin = 50;
            axmen.CostMax = 100;
            axmen.AttackSpeedMin = .5f;
            axmen.AttackSpeedMax = 1.5f;
            axmen.MovementSpeedMin = .5f;
            axmen.MovementSpeedMax = 1.5f;
            CUnit archer = new CUnit(50, 50, 8, 6, 1.5f, .75f, eUnitType.ARCHER);
            archer.HPMin = 20;
            archer.HPMax = 80;
            archer.AttackMin = 5;
            archer.AttackMax = 11;
            archer.RangeMin = 2;
            archer.RangeMax = 10;
            archer.CostMin = 25;
            archer.CostMax = 75;
            archer.AttackSpeedMin = 1.0f;
            archer.AttackSpeedMax = 2.0f;
            archer.MovementSpeedMin = .5f;
            archer.MovementSpeedMax = 1.5f;
            CUnit warElephant = new CUnit(300, 300, 15, 1, 1.75f, .5f, eUnitType.WAR_ELEPHANT);
            warElephant.HPMin = 100;
            warElephant.HPMax = 500;
            warElephant.AttackMin = 10;
            warElephant.AttackMax = 20;
            warElephant.RangeMin = 1;
            warElephant.RangeMax = 1;
            warElephant.CostMin = 100;
            warElephant.CostMax = 500;
            warElephant.AttackSpeedMin = 1.0f;
            warElephant.AttackSpeedMax = 3.0f;
            warElephant.MovementSpeedMin = .25f;
            warElephant.MovementSpeedMax = 1.0f;

            listBoxUnits.Items.Add(infantry);
            listBoxUnits.Items.Add(cavalry);
            listBoxUnits.Items.Add(cavalryArcher);
            listBoxUnits.Items.Add(axmen);
            listBoxUnits.Items.Add(archer);
            listBoxUnits.Items.Add(warElephant);
            listBoxUnits.SelectedIndex = 0;
        }

        /// <summary>
        /// When the "Randomize Unit" button is pushed it randomizes 
        /// all of the selected unit's stats.
        /// </summary>
        /// <param name="sender">The sender of the event</param>
        /// <param name="e">Event data sent with the event</param>
        /// <returns>void</returns>
        private void buttonRandomize_Click(object sender, EventArgs e)
        {
            RandomizeSelected();
        }

        private void RandomizeSelected()
        {
            Random rand = new Random();

            numericUpDownAttSpeed.Value = RandomDecimal((double)numericUpDownAttSpdMin.Value, (double)numericUpDownAttSpdMax.Value);

            numericUpDownMovement.Value = RandomDecimal((double)numericUpDownMoveMin.Value, (double)numericUpDownMoveMax.Value);

            numericUpDownHP.Value = rand.Next((int)numericUpDownHPMin.Value, (int)numericUpDownHPMax.Value + 1);

            numericUpDownAttack.Value = rand.Next((int)numericUpDownAttackMin.Value, (int)numericUpDownAttackMax.Value + 1);

            numericUpDownRange.Value = rand.Next((int)numericUpDownRangeMin.Value, (int)numericUpDownRangeMax.Value + 1);

            numericUpDownCost.Value = rand.Next((int)numericUpDownCostMin.Value, (int)numericUpDownCostMax.Value + 1);
        }

        private decimal RandomDecimal(double dMin, double dMax)
        {
            Random rand = new Random();
            if (dMin == dMax)
                return (decimal)dMin;
            return (decimal)((dMax-dMin) * rand.NextDouble() + dMin);
        }

        private void numericUpDownHPMin_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownHPMin.Value > numericUpDownHPMax.Value)
                numericUpDownHPMax.Value = numericUpDownHPMin.Value;
              
            ((CUnit)listBoxUnits.SelectedItem).HPMin = this.CUnit.HPMin;

        }

        private void numericUpDownHPMax_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && numericUpDownHPMin.Value > numericUpDownHPMax.Value)
                numericUpDownHPMin.Value = numericUpDownHPMax.Value;
            ((CUnit)listBoxUnits.SelectedItem).HPMax = this.CUnit.HPMax;

        }

        private void numericUpDownAttackMin_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && (numericUpDownAttackMin.Value > numericUpDownAttackMax.Value))
                numericUpDownAttackMax.Value = numericUpDownAttackMin.Value;
            ((CUnit)listBoxUnits.SelectedItem).AttackMin = this.CUnit.AttackMin;

        }

        private void numericUpDownAttackMax_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && numericUpDownAttackMin.Value > numericUpDownAttackMax.Value)
                numericUpDownAttackMin.Value = numericUpDownAttackMax.Value;
            ((CUnit)listBoxUnits.SelectedItem).AttackMax = this.CUnit.AttackMax;

        }

        private void numericUpDownRangeMin_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && numericUpDownRangeMin.Value > numericUpDownRangeMax.Value)
                numericUpDownRangeMax.Value = numericUpDownRangeMin.Value;
            ((CUnit)listBoxUnits.SelectedItem).RangeMin = this.CUnit.RangeMin;
        }

        private void numericUpDownRangeMax_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && numericUpDownRangeMin.Value > numericUpDownRangeMax.Value)
                numericUpDownRangeMin.Value = numericUpDownRangeMax.Value;
            ((CUnit)listBoxUnits.SelectedItem).RangeMax = this.CUnit.RangeMax;
        }

        private void numericUpDownAttSpdMin_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && numericUpDownAttSpdMin.Value > numericUpDownAttSpdMax.Value)
                numericUpDownAttSpdMax.Value = numericUpDownAttSpdMin.Value;
            ((CUnit)listBoxUnits.SelectedItem).AttackSpeedMin = this.CUnit.AttackSpeedMin;
        }

        private void numericUpDownAttSpdMax_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && numericUpDownAttSpdMin.Value > numericUpDownAttSpdMax.Value)
                numericUpDownAttSpdMin.Value = numericUpDownAttSpdMax.Value;
            ((CUnit)listBoxUnits.SelectedItem).AttackSpeedMax = this.CUnit.AttackSpeedMax;
        }

        private void numericUpDownMoveMin_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && numericUpDownMoveMin.Value > numericUpDownMoveMax.Value)
                numericUpDownMoveMax.Value = numericUpDownMoveMin.Value;
            ((CUnit)listBoxUnits.SelectedItem).MovementSpeedMin = this.CUnit.MovementSpeedMin;
        }

        private void numericUpDownMoveMax_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && numericUpDownMoveMin.Value > numericUpDownMoveMax.Value)
                numericUpDownMoveMin.Value = numericUpDownMoveMax.Value;
            ((CUnit)listBoxUnits.SelectedItem).MovementSpeedMax = this.CUnit.MovementSpeedMax;
        }

        private void numericUpDownCostMin_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && numericUpDownCostMin.Value > numericUpDownCostMax.Value)
                numericUpDownCostMax.Value = numericUpDownCostMin.Value;
            ((CUnit)listBoxUnits.SelectedItem).CostMin = this.CUnit.CostMin;
        }

        private void numericUpDownCostMax_ValueChanged(object sender, EventArgs e)
        {
            if (!m_bUpdating && numericUpDownCostMin.Value > numericUpDownCostMax.Value)
                numericUpDownCostMin.Value = numericUpDownCostMax.Value;
            ((CUnit)listBoxUnits.SelectedItem).CostMax = this.CUnit.CostMax;

        }

        private void buttonRandomizeAll_Click(object sender, EventArgs e)
        {
            int nCurIndex = listBoxUnits.SelectedIndex;
            for (int i = 0; i < listBoxUnits.Items.Count; i++)
            {
                listBoxUnits.SelectedIndex = i;
                RandomizeSelected();
            }
            listBoxUnits.SelectedIndex = nCurIndex;
        }

        private void numericUpDownCost_ValueChanged(object sender, EventArgs e)
        {
            ((CUnit)listBoxUnits.SelectedItem).Cost = this.CUnit.Cost;
        }

    }
}