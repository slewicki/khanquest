/// <summary>
/// File: “Paddle.cpp”
/// Author: Rodney Kite (RK)
/// Date Created	:	7/17/2008
/// Purpose: The main form for the Particle Editor.
/// </summary>

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using Microsoft.DirectX.Direct3D;
using System.IO;
namespace Khanquest
{
    /// <summary>
    /// Particle Editor Form.
    /// </summary>
    public partial class Form1 : Form
    {
        ManagedDirect3D D3D = ManagedDirect3D.Instance;
        ManagedTextureManager TM = ManagedTextureManager.Instance;
        public int ColorKey = Color.White.ToArgb();
        int ImageID = -1;

        /// <summary>
        /// Initialize the Particle Editor Form.
        /// </summary>

        decimal m_PercentXScale = 0;
        decimal m_PercentYScale = 0;

        int m_nCountOut = 1;

        string m_szFileName = string.Empty;
        string m_szTrigger = string.Empty;
        bool m_bCboxChecked = true;
        int m_nOffsetX = 10;
        int M_nOffsetY = 10;

        List<Particle> particles = new List<Particle>();

        Random randNum = new Random();
        Random random = new Random();

        /// <summary>
        /// Initialize the Particle Editor Form.
        /// </summary>
        public Form1()
        {
            InitializeComponent();
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            D3D.InitManagedDirect3D(panel1, panel1.Width, panel1.Height, true, true);
            TM.InitManagedTextureManager(D3D.Device, D3D.Sprite);
        }

        /// <summary>
        /// Particle Class to hold the particle info.
        /// </summary>
        /// <returns>Particle</returns>
        public Particle Part
        {
            get
            {
                Particle Part = new Particle();
                Part.MaxParticles = nudMaxParticles.Value;
                Part.Continuous = cbContinuous.Checked;
                Part.MaxLife = nudMaxLife.Value;
                Part.MinLife = nudMinLife.Value;

                Part.VelX = nudVelX.Value;
                Part.VelY = nudVelY.Value;

                if (cbBlendSource.SelectedItem !=null)
                {
                	Part.SourceBlend = cbBlendSource.SelectedItem.ToString();
                }
                if (cbBlendDest.SelectedItem != null)
                {
                	Part.DestBlend = cbBlendDest.SelectedItem.ToString();
                }

                Part.OffsetX = tbXOffset.Value;
                Part.OffsetY = tbYOffset.Value;

                Part.LocX = randNum.Next( (panel1.Width / 2) + (int)nudStartX.Value, (panel1.Width / 2) + (int)nudStartX.Value);
                Part.LocY = randNum.Next( (panel1.Height / 2) + (int)nudStartY.Value, (panel1.Height / 2) + (int)nudStartY.Value);

                Part.StartAlpha = nudStartAlpha.Value;
                Part.StartRed = nudStartRed.Value;
                Part.StartGreen = nudStartGreen.Value;
                Part.StartBlue = nudStartBlue.Value;

                Part.CurrentAlpha = nudStartAlpha.Value;
                Part.CurrentRed = nudStartRed.Value;
                Part.CurrentGreen = nudStartGreen.Value;
                Part.CurrentBlue = nudStartBlue.Value;

                Part.EndAlpha = nudEndAlpha.Value;
                Part.EndRed = nudEndRed.Value;
                Part.EndGreen = nudEndGreen.Value;
                Part.EndBlue = nudEndBlue.Value;

                Part.StartScaleX = nudStartScaleX.Value;
                Part.StartScaleY = nudStartScaleY.Value;
                Part.CurrentScaleX = nudStartScaleX.Value;
                Part.CurrentScaleY = nudStartScaleY.Value;
                Part.EndScaleX = nudEndScaleX.Value;
                Part.EndScaleY = nudEndScaleY.Value;

                Part.GravityX = nudGravityX.Value;
                Part.GravityY = nudGravityY.Value;

                if (nudMaxLife.Value < nudMinLife.Value)
                    nudMaxLife.Value = nudMinLife.Value;

	           Part.Life = randNum.Next((int)nudMinLife.Value, (int)nudMaxLife.Value);

                if (openFileDialog2.FileName != null)
                {
                    Part.FileName = Path.GetFileName(openFileDialog2.FileName);
                }
                return Part;
            }
        }

        /// <summary>
        /// Exit the Particle Editor
        /// </summary>
        /// <param name="fileName">The name of the file to load</param>
        /// <returns>Null</returns>
        public void LoadImage(string fileName)
        {
            ImageID = TM.LoadTexture(fileName, ColorKey);
        }

        /// <summary>
        /// Exit the Particle Editor
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        /// <summary>
        /// Resets all values to the default
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void newToolStripButton_Click(object sender, EventArgs e)
        {
            nudMaxParticles.Value = 1;
            nudMinLife.Value = 25;
            nudMaxLife.Value = 50;

            tbYOffset.Value = 0;
            tbXOffset.Value = 0;

            nudStartRed.Value = 255;
            nudStartGreen.Value = 0;
            nudStartBlue.Value = 0;
            nudStartAlpha.Value = 255;

            nudEndRed.Value = 150;
            nudEndGreen.Value = 150;
            nudEndBlue.Value = 0;
            nudEndAlpha.Value = 150;

            nudVelX.Value = 0;
            nudVelY.Value = 0;

            nudStartScaleX.Value = .25M;
            nudStartScaleY.Value = .25M;
            nudEndScaleX.Value = .25M;
            nudEndScaleY.Value = .25M;
            nudGravityX.Value = 0;
            nudGravityY.Value = 0;

            cbMaxParticles.Checked = false;
            cbMaxLife.Checked = false;
            cbMinLife.Checked = false;
            cbVelX.Checked = false;
            cbVelY.Checked = false;
            cboxDestBlend.Checked = false;
            cboxSourceBlend.Checked = false;
            cbXOffset.Checked = false;
            cbYOffset.Checked = false;
            cbStartColor.Checked = false;
            cbEndColor.Checked = false;
            cbStartScaleX.Checked = false;
            cbStartScaleY.Checked = false;
            cbEndScaleX.Checked = false;
            cbEndScaleY.Checked = false;
            cboxSourceBlend.Checked = false;
            cboxGravityX.Checked = false;
            cboxGravityY.Checked = false;
            txtBoxTrigger.Text = string.Empty;
            particles.Clear();
            particles.TrimExcess();
            Part.FileName = string.Empty;
            cbContinuous.Checked = true;
           // rbSaveXML.Checked = true;
            Form1.ActiveForm.Text = "Particle Editor (Khanquest)";
        }

        /// <summary>
        /// Save values to an xml or binary file
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void saveToolStripButton_Click(object sender, EventArgs e)
        {
            try
            {

                // Save as XML
                if (rbSaveXML.Checked)
                {
                    SaveFileDialog dlg = new SaveFileDialog();
                    dlg.Filter = "Xml Files|*.xml";
                    dlg.FilterIndex = 1;
                    dlg.DefaultExt = "xml";

                    if (DialogResult.OK == dlg.ShowDialog())
                    {
                        FileInfo fi = new FileInfo(dlg.FileName);
                        if (!fi.Name.StartsWith("KQ_"))
                        {
                            string filename = fi.DirectoryName + "//" + "KQ_" + fi.Name;
                            dlg.FileName = filename;
                        }
                        XmlWriterSettings settings = new XmlWriterSettings();
                        settings.ConformanceLevel = ConformanceLevel.Document;
                        // auto format the xml file;
                        settings.Indent = true;


                        // create in scope so it falls out and closes automatically
                        using (XmlWriter writer = XmlWriter.Create(dlg.FileName, settings))
                        {
                            writer.WriteStartElement("Emitter");
                            writer.WriteAttributeString("Continuous", cbContinuous.Checked.ToString());
                            writer.WriteAttributeString("MaxParticles", Part.MaxParticles.ToString());
                            writer.WriteAttributeString("MaxLife", Part.MaxLife.ToString());
                            writer.WriteAttributeString("MinLife", Part.MinLife.ToString());

                            writer.WriteAttributeString("VelX", Part.VelX.ToString());
                            writer.WriteAttributeString("VelY", Part.VelY.ToString());
                            writer.WriteAttributeString("SourceBlend", Part.SourceBlend.ToString());
                            writer.WriteAttributeString("DestBlend", Part.DestBlend.ToString());
                            writer.WriteAttributeString("OffsetX", Part.OffsetX.ToString());
                            writer.WriteAttributeString("OffsetY", Part.OffsetY.ToString());

                            writer.WriteAttributeString("StartRed", Part.StartRed.ToString());
                            writer.WriteAttributeString("EndRed", Part.EndRed.ToString());
                            writer.WriteAttributeString("StartGreen", Part.StartGreen.ToString());
                            writer.WriteAttributeString("EndGreen", Part.EndGreen.ToString());
                            writer.WriteAttributeString("StartBlue", Part.StartBlue.ToString());
                            writer.WriteAttributeString("EndBlue", Part.EndBlue.ToString());
                            writer.WriteAttributeString("StartAlpha", Part.StartAlpha.ToString());
                            writer.WriteAttributeString("EndAlpah", Part.EndAlpha.ToString());

                            writer.WriteAttributeString("StartScaleX", Part.StartScaleX.ToString());
                            writer.WriteAttributeString("EndScaleX", Part.EndScaleX.ToString());
                            writer.WriteAttributeString("StartScaleY", Part.StartScaleY.ToString());
                            writer.WriteAttributeString("EndScaleY", Part.EndScaleY.ToString());
                            writer.WriteAttributeString("GravityX", Part.GravityX.ToString());
                            writer.WriteAttributeString("GravityY", Part.GravityY.ToString());

                            writer.WriteAttributeString("Trigger", m_szTrigger);
                            writer.WriteAttributeString("FileName", Part.FileName);

                            //Form1.ActiveForm.Text = "Particle Editor (Khanquest) " + openFileDialog1.FileName;

                            writer.WriteEndElement();// Close Element
                        }
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
                        if (Part.Continuous)
                            bw.Write((Int16)1);
                        else
                            bw.Write((Int16)0);

                        bw.Write((int)Part.MaxParticles);
                        bw.Write((int)Part.MaxLife);
                        bw.Write((int)Part.MinLife);

                        bw.Write((int)Part.VelX);
                        bw.Write((int)Part.VelY);

                        bw.Write((Int32) Part.SourceBlend.Length);
                        bw.Write(Part.SourceBlend.ToCharArray());

                        bw.Write((int)Part.DestBlend.Length);
                        bw.Write(Part.DestBlend.ToCharArray());

                        bw.Write((int)Part.OffsetX);
                        bw.Write((int)Part.OffsetY);

                        bw.Write((int)Part.StartRed);
                        bw.Write((int)Part.EndRed);
                        bw.Write((int)Part.StartGreen);
                        bw.Write((int)Part.EndGreen);
                        bw.Write((int)Part.StartBlue);
                        bw.Write((int)Part.EndBlue);
                        bw.Write((int)Part.StartAlpha);
                        bw.Write((int)Part.EndAlpha);

                        bw.Write((double)Part.StartScaleX);
                        bw.Write((double)Part.EndScaleX);
                        bw.Write((double)Part.StartScaleY);
                        bw.Write((double)Part.EndScaleY);
                        bw.Write((double)Part.GravityX);
                        bw.Write((double)Part.GravityY);

                        bw.Write((int)m_szTrigger.Length);
                        bw.Write(m_szTrigger.ToCharArray());

                        bw.Write((int)Part.FileName.Length);
                        bw.Write(Part.FileName.ToCharArray());

                        bw.Close();
                        fs.Close();
                    }
                }
            }

            catch (System.Exception de)
            {
                MessageBox.Show("Error saving file! Exception:  " + de.GetType().Name, "Error");

            }
        }

        /// <summary>
        /// Opens an Xml or binary file and reads in the values
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {

                if (rbSaveXML.Checked)
                {
                    if (DialogResult.OK == openFileDialog1.ShowDialog())
                    {
                        XmlReaderSettings settings = new XmlReaderSettings();
                        settings.ConformanceLevel = ConformanceLevel.Document;
                        settings.IgnoreWhitespace = true;
                        settings.IgnoreComments = true;

                        using (XmlReader reader = XmlReader.Create(openFileDialog1.FileName, settings))
                        {
                            //skip stuff at top and go to data
                            reader.MoveToContent();

                            if ((reader.Name == "Emitter") && (reader.IsStartElement()))
                            {
                                cbContinuous.Checked = Convert.ToBoolean(reader["Continuous"]);
                                nudMaxParticles.Value = Convert.ToDecimal(reader["MaxParticles"]);
                                nudMinLife.Value = Convert.ToDecimal(reader["MinLife"]);
                                nudMaxLife.Value = Convert.ToDecimal(reader["MaxLife"]);

                                nudVelX.Value = Convert.ToDecimal(reader["VelX"]);
                                nudVelY.Value = Convert.ToDecimal(reader["VelY"]);

                                cbBlendSource.SelectedItem = Convert.ToString(reader["SourceBlend"]);
                                cbBlendDest.SelectedItem = Convert.ToString(reader["DestBlend"]);

                                m_nOffsetX = tbXOffset.Value = Convert.ToInt32(reader["OffsetX"]);
                                M_nOffsetY = tbYOffset.Value = Convert.ToInt32(reader["OffsetY"]);

                                nudStartRed.Value = Convert.ToDecimal(reader["StartRed"]);
                                nudEndRed.Value = Convert.ToDecimal(reader["EndRed"]);
                                nudStartGreen.Value = Convert.ToDecimal(reader["StartGreen"]);
                                nudEndGreen.Value = Convert.ToDecimal(reader["EndGreen"]);
                                nudStartBlue.Value = Convert.ToDecimal(reader["StartBlue"]);
                                nudEndBlue.Value = Convert.ToDecimal(reader["EndBlue"]);
                                nudStartAlpha.Value = Convert.ToDecimal(reader["StartAlpha"]);
                                nudEndAlpha.Value = Convert.ToDecimal(reader["EndAlpah"]);

                                nudStartScaleX.Value = Convert.ToDecimal(reader["StartScaleX"]);
                                nudEndScaleX.Value = Convert.ToDecimal(reader["EndScaleX"]);
                                nudStartScaleY.Value = Convert.ToDecimal(reader["StartScaleY"]);
                                nudEndScaleY.Value = Convert.ToDecimal(reader["EndScaleY"]);
                                nudGravityX.Value = Convert.ToDecimal(reader["GravityX"]);
                                nudGravityY.Value = Convert.ToDecimal(reader["GravityY"]);
                                txtBoxTrigger.Text = Convert.ToString(reader["Trigger"]);
                                Part.FileName = m_szFileName = Convert.ToString(reader["FileName"]);
                                //LoadImage(fileName);
                                reader.Close();
                                Refresh();
                            }
                        }
                        if (m_szFileName != "")
                        {
	                        m_szFileName = "../Resource/" + m_szFileName;
	                        LoadImage(m_szFileName);
                        }
                        loadParticles();
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
                        FileStream fs = new FileStream(dlg.FileName, FileMode.Open);
                        BinaryReader br = new BinaryReader(fs);
                        int Continue = br.ReadInt16();
                        if (Continue != 0)
                            cbContinuous.Checked = true;
                        else
                            cbContinuous.Checked = false;


                        nudMaxParticles.Value = br.ReadInt32();
                        nudMaxLife.Value = br.ReadInt32();
                        nudMinLife.Value = br.ReadInt32();

                        nudVelX.Value = br.ReadInt32();
                        nudVelY.Value = br.ReadInt32();

                        int length = br.ReadInt32();
                        cbBlendSource.SelectedItem = new string(br.ReadChars(length));

                        length = br.ReadInt32();
                        cbBlendDest.SelectedItem = new string(br.ReadChars(length));

                        m_nOffsetX = tbXOffset.Value = br.ReadInt32();
                        M_nOffsetY = tbYOffset.Value = br.ReadInt32();

                        nudStartRed.Value = br.ReadInt32();
                        nudEndRed.Value = br.ReadInt32();
                        nudStartGreen.Value = br.ReadInt32();
                        nudEndGreen.Value = br.ReadInt32();
                        nudStartBlue.Value = br.ReadInt32();
                        nudEndBlue.Value = br.ReadInt32();
                        nudStartAlpha.Value = br.ReadInt32();
                        nudEndAlpha.Value = br.ReadInt32();

                        nudStartScaleX.Value = (decimal)br.ReadDouble();
                        nudEndScaleX.Value = (decimal)br.ReadDouble();
                        nudStartScaleY.Value = (decimal)br.ReadDouble();
                        nudEndScaleY.Value = (decimal)br.ReadDouble();
                        nudGravityX.Value = (decimal)br.ReadDouble();
                        nudGravityY.Value = (decimal)br.ReadDouble();

                        length = br.ReadInt32();
                        m_szTrigger = new string(br.ReadChars(length));

                        length = br.ReadInt32();
                        Part.FileName = m_szFileName = new string(br.ReadChars(length));

                        br.Close();
                        fs.Close();
                        if (m_szFileName != "")
                        {

                            m_szFileName = "../Resource/" + m_szFileName;
                            LoadImage(m_szFileName);
                        }
                        loadParticles();
                    }

                }
                //////////////////////////////////////////////////////////////////////////
                //Form1.ActiveForm.Text = "Particle Editor (Khanquest) " + openFileDialog1.FileName;
            }
            // Catch the IOException generated if the 
            // specified part of the file is locked.
            catch (Exception de)
            {
                MessageBox.Show("Error opening file! Exception: " + de.GetType().Name, "Open Error");
            }
                        //    string filename = fi.DirectoryName + "//" + "KQ_" + fi.Name;
        }

        /// <summary>
        /// Paint the panel that displays the particles
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            if (nudMinLife.Value > nudMaxLife.Value)
                nudMaxLife.Value = nudMinLife.Value;
        }

        /// <summary>
        /// Reloads changed particles when values are changed
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void nudStartScaleX_ValueChanged(object sender, EventArgs e)
        {
            loadParticles();
            Refresh();
        }

        /// <summary>
        /// Loads the the particles into a list
        /// </summary>
        /// <returns>Null</returns>
        private void loadParticles()
        {
            particles.Clear();
            particles.TrimExcess();

            m_nCountOut = 1;

            if (particles.Count < nudMaxParticles.Value)
            {
                for (int i = 0; i < nudMaxParticles.Value; ++i)
                {
                    particles.Add(Part);
                }
            }
            if (particles.Count > nudMaxParticles.Value)
            {
                for (int i = particles.Count; i > nudMaxParticles.Value; --i)
                {
                    particles.RemoveAt(i - 1);
                }
            }
        }

        /// <summary>
        /// timer function, updates particle info over time
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void timer1_Tick(object sender, EventArgs e)
        {

            if (!this.DesignMode)
            {
                // If there are particles to draw
                if (particles.Count > 0)
                {
                    for (int i = 0; i < nudMaxParticles.Value; ++i)
                    {
                        // Update location
                        UpdateLoc(i);

                        // Update the scale
                        UpdateScale(i);

                        // Update Color
                        UpdateColor(i);

                        // Update Age
                        UpdateAge(i);
                    }
                }

                // Start to draw the particles
                D3D.DeviceBegin();
                D3D.SpriteBegin();
                D3D.Clear(0, 0, 0);

                D3D.Device.SetRenderState(RenderStates.AlphaBlendEnable, true);


                #region Blend modes
                // Source blend
                if ((string)cbBlendSource.SelectedItem == "One")
                    D3D.Device.RenderState.SourceBlend = Blend.One;
                if ((string)cbBlendSource.SelectedItem == "InvBlendFactor")
                    D3D.Device.RenderState.SourceBlend = Blend.InvBlendFactor;
                if ((string)cbBlendSource.SelectedItem == "BlendFactor")
                    D3D.Device.RenderState.SourceBlend = Blend.BlendFactor;
                if ((string)cbBlendSource.SelectedItem == "BothInvSourceAlpha")
                    D3D.Device.RenderState.SourceBlend = Blend.BothInvSourceAlpha;
                if ((string)cbBlendSource.SelectedItem == "SourceAlphaSat")
                    D3D.Device.RenderState.SourceBlend = Blend.SourceAlphaSat;
                if ((string)cbBlendSource.SelectedItem == "InvDestinationColor")
                    D3D.Device.RenderState.SourceBlend = Blend.InvDestinationColor;
                if ((string)cbBlendSource.SelectedItem == "DestinationColor")
                    D3D.Device.RenderState.SourceBlend = Blend.DestinationColor;
                if ((string)cbBlendSource.SelectedItem == "InvDestinationAlpha")
                    D3D.Device.RenderState.SourceBlend = Blend.InvDestinationAlpha;
                if ((string)cbBlendSource.SelectedItem == "DestinationAlpha")
                    D3D.Device.RenderState.SourceBlend = Blend.DestinationAlpha;
                if ((string)cbBlendSource.SelectedItem == "InvSourceAlpha")
                    D3D.Device.RenderState.SourceBlend = Blend.InvSourceAlpha;
                if ((string)cbBlendSource.SelectedItem == "SourceAlpha")
                    D3D.Device.RenderState.SourceBlend = Blend.SourceAlpha;
                if ((string)cbBlendSource.SelectedItem == "InvSourceColor")
                    D3D.Device.RenderState.SourceBlend = Blend.InvSourceColor;
                if ((string)cbBlendSource.SelectedItem == "SourceColor")
                    D3D.Device.RenderState.SourceBlend = Blend.SourceColor;

                // Dest Blend
                if ((string)cbBlendDest.SelectedItem == "Zero")
                    D3D.Device.RenderState.DestinationBlend = Blend.Zero;
                if ((string)cbBlendDest.SelectedItem == "One")
                    D3D.Device.RenderState.DestinationBlend = Blend.One;
                if ((string)cbBlendDest.SelectedItem == "InvBlendFactor")
                    D3D.Device.RenderState.DestinationBlend = Blend.InvBlendFactor;
                if ((string)cbBlendDest.SelectedItem == "BlendFactor")
                    D3D.Device.RenderState.DestinationBlend = Blend.BlendFactor;
                if ((string)cbBlendDest.SelectedItem == "BothInvSourceAlpha")
                    D3D.Device.RenderState.DestinationBlend = Blend.BothInvSourceAlpha;
                if ((string)cbBlendDest.SelectedItem == "BothSourceAlpha")
                    D3D.Device.RenderState.DestinationBlend = Blend.BothSourceAlpha;
                if ((string)cbBlendDest.SelectedItem == "SourceAlphaSat")
                    D3D.Device.RenderState.DestinationBlend = Blend.SourceAlphaSat;
                if ((string)cbBlendDest.SelectedItem == "InvDestinationColor")
                    D3D.Device.RenderState.DestinationBlend = Blend.InvDestinationColor;
                if ((string)cbBlendDest.SelectedItem == "DestinationColor")
                    D3D.Device.RenderState.DestinationBlend = Blend.DestinationColor;
                if ((string)cbBlendDest.SelectedItem == "InvDestinationAlpha")
                    D3D.Device.RenderState.DestinationBlend = Blend.InvDestinationAlpha;
                if ((string)cbBlendDest.SelectedItem == "DestinationAlpha")
                    D3D.Device.RenderState.DestinationBlend = Blend.DestinationAlpha;
                if ((string)cbBlendDest.SelectedItem == "InvSourceAlpha")
                    D3D.Device.RenderState.DestinationBlend = Blend.InvSourceAlpha;
                if ((string)cbBlendDest.SelectedItem == "SourceAlpha")
                    D3D.Device.RenderState.DestinationBlend = Blend.SourceAlpha;
                if ((string)cbBlendDest.SelectedItem == "InvSourceColor")
                    D3D.Device.RenderState.DestinationBlend = Blend.InvSourceColor;
                if ((string)cbBlendDest.SelectedItem == "SourceColor")
                    D3D.Device.RenderState.DestinationBlend = Blend.SourceColor;
                #endregion

                D3D.Sprite.Flush();

                if (ImageID != -1)
                {
                    if (particles.Count > 0)
                    {
	                    for (int i = 0; i < m_nCountOut; ++i)
	                    {
	                        if (particles[i].Alive)
	                        {
	                            TM.Draw(ImageID, 
                                    (int)particles[i].LocX - (TM.GetTextureWidth(ImageID) / 2),
	                                (int)particles[i].LocY - (TM.GetTextureHeight(ImageID) / 2),
	                                (float)particles[i].CurrentScaleX,
	                                (float)particles[i].CurrentScaleY,
	                                Rectangle.Empty, 0, 0, 0,
	                                Color.FromArgb(
	                                (int)particles[i].CurrentAlpha,
	                                (int)particles[i].CurrentRed,
	                                (int)particles[i].CurrentGreen,
	                                (int)particles[i].CurrentBlue).ToArgb()
	                                );
	
	                        }
	                    }
                    }

                    
                }
                D3D.Sprite.Flush();
                D3D.Device.RenderState.DestinationBlend = Blend.Zero;
                D3D.Device.RenderState.SourceBlend = Blend.Zero;
                D3D.Device.SetRenderState(RenderStates.AlphaBlendEnable, false);

                D3D.SpriteEnd();
                D3D.DeviceEnd();

                D3D.Present();
                //Refresh(); // a combo of Invalidate and Update;
            }
        }

        /// <summary>
        /// Update the particles with time
        /// </summary>
        /// <param name="i">the index of the particle to update</param>
        /// <returns>Null</returns>
        private void UpdateAge(int i)
        {
            particles[i].Age = particles[i].Age + 1;

            if (particles[i].Age > particles[i].Life)
            {
                if (cbContinuous.Checked)
                {
                    particles[i].Age = 0;
                    particles[i].Life = randNum.Next((int)nudMinLife.Value, (int)nudMaxLife.Value);

                    particles[i].LocX = randNum.Next(( (panel1.Width / 2) + (int)nudStartX.Value) - m_nOffsetX, ((panel1.Width / 2) + (int)nudStartX.Value ) + m_nOffsetX );
                    particles[i].LocY = randNum.Next(( (panel1.Height / 2) + (int)nudStartY.Value) - M_nOffsetY, ((panel1.Height / 2) + (int)nudStartY.Value) + M_nOffsetY );

                    particles[i].CurrentAlpha = particles[i].StartAlpha;
                    particles[i].CurrentRed = particles[i].StartRed;
                    particles[i].CurrentGreen = particles[i].StartGreen;
                    particles[i].CurrentBlue = particles[i].StartBlue;
                    particles[i].CurrentScaleX = particles[i].StartScaleX;
                    particles[i].CurrentScaleY = particles[i].StartScaleY;
                }
                else
                {
                    particles[i].Alive = false;
                }
            }
        }

        /// <summary>
        /// Update the particles color
        /// </summary>
        /// <param name="i">the index of the particle to update</param>
        /// <returns>Null</returns>
        private void UpdateColor(int i)
        {
            float percent = (float)particles[i].Age / (float)particles[i].Life;

            particles[i].CurrentAlpha = ColorOperator.Lerp((int)particles[i].StartAlpha, (int)particles[i].EndAlpha, percent);
            particles[i].CurrentRed = ColorOperator.Lerp((int)particles[i].StartRed, (int)particles[i].EndRed, percent);
            particles[i].CurrentGreen = ColorOperator.Lerp((int)particles[i].StartGreen, (int)particles[i].EndGreen, percent);
            particles[i].CurrentBlue = ColorOperator.Lerp((int)particles[i].StartBlue, (int)particles[i].EndBlue, percent);
        }

        /// <summary>
        /// Update the particles scale
        /// </summary>
        /// <param name="i">the index of the particle to update</param>
        /// <returns>Null</returns>
        private void UpdateScale(int i)
        {
            // update ScaleX
            if (particles[i].CurrentScaleX != particles[i].EndScaleX)
            {
                if (particles[i].StartScaleX < particles[i].EndScaleX)
                {
                    m_PercentXScale = (particles[i].EndScaleX - particles[i].StartScaleX) / (decimal)particles[i].Life;
                    particles[i].CurrentScaleX = particles[i].CurrentScaleX + m_PercentXScale;
                }

                if (particles[i].CurrentScaleX > particles[i].EndScaleX)
                {
                    m_PercentXScale = (particles[i].StartScaleX - particles[i].EndScaleX) / particles[i].Life;
                    particles[i].CurrentScaleX = particles[i].CurrentScaleX - m_PercentXScale;
                }
            }
            // update ScaleY
            if (particles[i].CurrentScaleY != particles[i].EndScaleY)
            {
                if (particles[i].StartScaleY < particles[i].EndScaleY)
                {
                    m_PercentYScale = (particles[i].EndScaleY - particles[i].StartScaleY) / particles[i].Life;
                    particles[i].CurrentScaleY = particles[i].CurrentScaleY + m_PercentYScale;
                }

                if (particles[i].CurrentScaleY > particles[i].EndScaleY)
                {
                    m_PercentYScale = (particles[i].StartScaleY - particles[i].EndScaleY) / particles[i].Life;
                    particles[i].CurrentScaleY = particles[i].CurrentScaleY - m_PercentYScale;
                }
            }
        }

        /// <summary>
        /// Update the particles location
        /// </summary>
        /// <param name="i">the index of the particle to update</param>
        /// <returns>Null</returns>
        private void UpdateLoc(int i)
        {
            float percent = (float)particles[i].Age / (float)particles[i].Life;
            particles[i].LocX = particles[i].LocX + (int)nudVelX.Value + ColorOperator.Lerp(0, (int)nudGravityY.Value, percent);
            particles[i].LocY = particles[i].LocY + (int)nudVelY.Value + ColorOperator.Lerp(0, (int)nudGravityX.Value, percent);
        }

        /// <summary>
        /// Update number of particles when Maximum number is changed
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void nudMaxParticles_ValueChanged(object sender, EventArgs e)
        {

        }

        /// <summary>
        /// Randomize the editor settings
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void bRandomize_Click(object sender, EventArgs e)
        {
            loadParticles();

            if (cbMaxParticles.Checked)
                nudMaxParticles.Value = randNum.Next((int)nudMaxParticles.Minimum, (int)nudMaxParticles.Maximum);

            if (cbMaxLife.Checked)
                nudMaxLife.Value = randNum.Next((int)nudMaxLife.Minimum, (int)nudMinLife.Maximum);

            if (cbMinLife.Checked)
                nudMinLife.Value = randNum.Next((int)nudMinLife.Minimum, (int)nudMinLife.Maximum);

            if (cbVelX.Checked)
                nudVelX.Value = randNum.Next((int)nudVelX.Minimum, (int)nudVelX.Maximum);

            if (cbVelY.Checked)
                nudVelY.Value = randNum.Next((int)nudVelX.Minimum, (int)nudVelX.Maximum);

            if (cboxSourceBlend.Checked)
                cbBlendSource.SelectedIndex = randNum.Next(1, 15);

            if (cboxDestBlend.Checked)
                cbBlendDest.SelectedIndex = randNum.Next(1, 16);

            if (cbXOffset.Checked)
            {
                for (int i = 0; i < particles.Count; ++i)
                {
                    m_nOffsetX = tbXOffset.Value = randNum.Next((int)tbXOffset.Minimum, (int)tbXOffset.Maximum);
                }

            }


            if (cbYOffset.Checked)
            {
                for (int i = 0; i < particles.Count; ++i)
                {
                   M_nOffsetY = tbYOffset.Value = randNum.Next(tbYOffset.Minimum, tbYOffset.Maximum);
                }

            }

            if (cbStartColor.Checked)
            {
                nudStartAlpha.Value = randNum.Next(0, 255);
                nudStartRed.Value = randNum.Next(0, 255);
                nudStartGreen.Value = randNum.Next(0, 255);
                nudStartBlue.Value = randNum.Next(0, 255);
            }

            if (cbEndColor.Checked)
            {
                nudEndAlpha.Value = randNum.Next(0, 255);
                nudEndRed.Value = randNum.Next(0, 255);
                nudEndGreen.Value = randNum.Next(0, 255);
                nudEndBlue.Value = randNum.Next(0, 255);
            }

            if (cbStartScaleX.Checked)
                nudStartScaleX.Value = RandomDouble((int)nudStartScaleX.Minimum, nudStartScaleX.Maximum);

            if (cbStartScaleY.Checked)
                nudStartScaleY.Value = RandomDouble(nudStartScaleY.Minimum, nudStartScaleY.Maximum);

            if (cbEndScaleX.Checked)
                nudEndScaleX.Value = RandomDouble(nudEndScaleX.Minimum, nudEndScaleX.Maximum);

            if (cbEndScaleY.Checked)
                nudEndScaleY.Value = RandomDouble(nudEndScaleY.Minimum, nudEndScaleY.Maximum);

            if (cboxGravityX.Checked)
                nudGravityX.Value = randNum.Next((int)nudGravityX.Minimum, (int)nudGravityX.Maximum);

            if (cboxGravityY.Checked)
                nudGravityY.Value = randNum.Next((int)nudGravityY.Minimum, (int)nudGravityY.Maximum);

        }

        /// <summary>
        /// Check all randomize check boxes
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void cbCheckAll_Click(object sender, EventArgs e)
        {
            m_bCboxChecked = !m_bCboxChecked;

            if (m_bCboxChecked)
            {
                cbMaxParticles.Checked = false;

                cbMaxLife.Checked = false;

                cbMinLife.Checked = false;

                cbVelX.Checked = false;

                cbVelY.Checked = false;

                cboxDestBlend.Checked = false;

                cboxSourceBlend.Checked = false;

                cbXOffset.Checked = false;

                cbYOffset.Checked = false;

                cbStartColor.Checked = false;

                cbEndColor.Checked = false;

                cbStartScaleX.Checked = false;

                cbStartScaleY.Checked = false;

                cbEndScaleX.Checked = false;

                cbEndScaleY.Checked = false;

                cboxSourceBlend.Checked = false;

                cboxGravityX.Checked = false;

                cboxGravityY.Checked = false;
            }
            else
            {
                cbMaxParticles.Checked = true;

                cbMaxLife.Checked = true;

                cbMinLife.Checked = true;

                cbVelX.Checked = true;

                cbVelY.Checked = true;

                cboxSourceBlend.Checked = true;

                cboxDestBlend.Checked = true;

                cbXOffset.Checked = true;

                cbYOffset.Checked = true;

                cbStartColor.Checked = true;

                cbEndColor.Checked = true;

                cbStartScaleX.Checked = true;

                cbStartScaleY.Checked = true;

                cbEndScaleX.Checked = true;

                cbEndScaleY.Checked = true;

                cboxSourceBlend.Checked = true;

                cboxGravityX.Checked = true;

                cboxGravityY.Checked = true;
            }

        }

        /// <summary>
        /// Update the offset X value when the track bar is moved
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void tbXOffset_Scroll(object sender, EventArgs e)
        {
            m_nOffsetX = tbXOffset.Value;
        }

        /// <summary>
        /// Update the offset Y value when the track bar is moved
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void tbYOffset_Scroll(object sender, EventArgs e)
        {
            M_nOffsetY = tbYOffset.Value;
            loadParticles();
        }

        /// <summary>
        /// Return a random decimal number between the given minimum and maximum
        /// for the scale X and Y camped at a max of 3
        /// </summary>
        /// <param name="min">The minimum number</param>
        /// <param name="max">The maximum number</param>
        /// <returns>Null</returns>
        private decimal RandomDouble(decimal min, decimal max)
        {
            double result1 = random.Next((int)min, (int)max);
            double result2 = random.Next((int)min, (int)max);
            while (result2 == 0)
                result2 = random.Next((int)min, (int)max);
            decimal number = (decimal)( (result1 / result2) / 10);
            
            if (number > 3)
            {
                number = 3;
            }

            return number;
        }

        /// <summary>
        /// Reload particles if the Continuous check box is checked
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void txtBoxTrigger_Leave(object sender, EventArgs e)
        {
            m_szTrigger = txtBoxTrigger.Text;
            Refresh();
        }

        /// <summary>
        /// Loads a Particle image file.
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void lToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (DialogResult.OK == openFileDialog2.ShowDialog())
            {
                Part.FileName = Path.GetFileName(openFileDialog2.FileName);

                LoadImage(openFileDialog2.FileName);
                loadParticles();
            }

            else
            {
                return;
            }
        }


        /// <summary>
        /// Change the Max life value to Min life if its over the MinLife value
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void nudMaxLife_Leave(object sender, EventArgs e)
        {
            if (nudMaxLife.Value < nudMinLife.Value)
                nudMaxLife.Value = nudMinLife.Value;

            loadParticles();
            Refresh();
        }


        /// <summary>
        /// Timer to keep all the particles from drawing at once at the start
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void timerAddPart_Tick(object sender, EventArgs e)
        {
            if (m_nCountOut < particles.Count)
            {
                m_nCountOut++;
            }
        }

        /// <summary>
        /// Reload Particles when you change after you change the Contentious setting 
        /// </summary>
        /// <param name="sender">The object calling the function</param>
        /// <param name="e">Any event sent to the function</param>
        /// <returns>Null</returns>
        private void buttonReset_Click(object sender, EventArgs e)
        {
            loadParticles();
        }

     }
}