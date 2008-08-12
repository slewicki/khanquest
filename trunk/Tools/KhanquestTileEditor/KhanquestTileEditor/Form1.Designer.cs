namespace KhanquestTileEditor
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            KhanquestTileEditor.CMap cMap5 = new KhanquestTileEditor.CMap();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tile1 = new KhanquestTileEditor.Tile();
            this.map1 = new KhanquestTileEditor.Map();
            this.vScrollBar1 = new System.Windows.Forms.VScrollBar();
            this.hScrollBar1 = new System.Windows.Forms.HScrollBar();
            this.lblTileHeight = new System.Windows.Forms.Label();
            this.lblTileWidth = new System.Windows.Forms.Label();
            this.numTileHeight = new System.Windows.Forms.NumericUpDown();
            this.numTileWidth = new System.Windows.Forms.NumericUpDown();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lblTileGridHeight = new System.Windows.Forms.Label();
            this.lblTileGridWidth = new System.Windows.Forms.Label();
            this.lblMapHeight = new System.Windows.Forms.Label();
            this.lblMapWidth = new System.Windows.Forms.Label();
            this.numMapWidth = new System.Windows.Forms.NumericUpDown();
            this.numMapHeight = new System.Windows.Forms.NumericUpDown();
            this.numTileGridHeight = new System.Windows.Forms.NumericUpDown();
            this.numTileGridWidth = new System.Windows.Forms.NumericUpDown();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.lblTrigger = new System.Windows.Forms.Label();
            this.txtTrigger = new System.Windows.Forms.TextBox();
            this.btnTrigger = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.lblTriggers = new System.Windows.Forms.Label();
            this.cmbTrigger = new System.Windows.Forms.ComboBox();
            this.rdoTile = new System.Windows.Forms.RadioButton();
            this.rdoPlayerSpawn = new System.Windows.Forms.RadioButton();
            this.rdoEnemySpawn = new System.Windows.Forms.RadioButton();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.chkCollision = new System.Windows.Forms.CheckBox();
            this.chkTrigger = new System.Windows.Forms.CheckBox();
            this.lblLayer = new System.Windows.Forms.Label();
            this.cmbLayer = new System.Windows.Forms.ComboBox();
            this.btnLayer = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.menuStrip1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.map1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numTileHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTileWidth)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numMapWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMapHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTileGridHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTileGridWidth)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1150, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.toolStripSeparator,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newToolStripMenuItem.Image")));
            this.newToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.newToolStripMenuItem.Size = new System.Drawing.Size(140, 22);
            this.newToolStripMenuItem.Text = "&New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripMenuItem.Image")));
            this.openToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openToolStripMenuItem.Size = new System.Drawing.Size(140, 22);
            this.openToolStripMenuItem.Text = "&Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // toolStripSeparator
            // 
            this.toolStripSeparator.Name = "toolStripSeparator";
            this.toolStripSeparator.Size = new System.Drawing.Size(137, 6);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("saveToolStripMenuItem.Image")));
            this.saveToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(140, 22);
            this.saveToolStripMenuItem.Text = "&Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(140, 22);
            this.saveAsToolStripMenuItem.Text = "Save &As";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(137, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(140, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.importImageToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.toolsToolStripMenuItem.Text = "&Tools";
            // 
            // importImageToolStripMenuItem
            // 
            this.importImageToolStripMenuItem.Name = "importImageToolStripMenuItem";
            this.importImageToolStripMenuItem.Size = new System.Drawing.Size(139, 22);
            this.importImageToolStripMenuItem.Text = "Import Image";
            this.importImageToolStripMenuItem.Click += new System.EventHandler(this.importImageToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(115, 22);
            this.aboutToolStripMenuItem.Text = "&About...";
            // 
            // splitContainer1
            // 
            this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.splitContainer1.Location = new System.Drawing.Point(13, 28);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tile1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.map1);
            this.splitContainer1.Size = new System.Drawing.Size(769, 437);
            this.splitContainer1.SplitterDistance = 256;
            this.splitContainer1.TabIndex = 1;
            // 
            // tile1
            // 
            this.tile1.AutoScroll = true;
            this.tile1.AutoScrollMinSize = new System.Drawing.Size(200, 60);
            this.tile1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tile1.ImageFile = null;
            this.tile1.Location = new System.Drawing.Point(0, 0);
            this.tile1.Name = "tile1";
            this.tile1.Size = new System.Drawing.Size(252, 433);
            this.tile1.TabIndex = 0;
            this.tile1.Text = "tile1";
            this.tile1.TileImage = ((System.Drawing.Image)(resources.GetObject("tile1.TileImage")));
            this.tile1.TileSetSize = new System.Drawing.Size(0, 0);
            this.tile1.TileSize = new System.Drawing.Size(0, 0);
            this.tile1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.tile1_MouseClick);
            // 
            // map1
            // 
            this.map1.Controls.Add(this.vScrollBar1);
            this.map1.Controls.Add(this.hScrollBar1);
            this.map1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.map1.Location = new System.Drawing.Point(0, 0);
            cMap5.CurrentLayer = 0;
            cMap5.ImageID = 0;
            cMap5.Once = true;
            cMap5.TileRects = null;
            cMap5.WorldPosition = new System.Drawing.Point(0, 0);
            cMap5.WorldPositionX = 0;
            cMap5.WorldPositionY = 0;
            this.map1.mMap = cMap5;
            this.map1.Name = "map1";
            this.map1.Size = new System.Drawing.Size(505, 433);
            this.map1.TabIndex = 0;
            this.map1.Text = "map1";
            this.map1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.map1_MouseMove);
            this.map1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.map1_MouseClick);
            // 
            // vScrollBar1
            // 
            this.vScrollBar1.Dock = System.Windows.Forms.DockStyle.Right;
            this.vScrollBar1.Location = new System.Drawing.Point(488, 0);
            this.vScrollBar1.Maximum = 2000;
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.Size = new System.Drawing.Size(17, 416);
            this.vScrollBar1.TabIndex = 1;
            this.vScrollBar1.Value = 1000;
            this.vScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vScrollBar1_Scroll);
            // 
            // hScrollBar1
            // 
            this.hScrollBar1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.hScrollBar1.Location = new System.Drawing.Point(0, 416);
            this.hScrollBar1.Maximum = 2000;
            this.hScrollBar1.Name = "hScrollBar1";
            this.hScrollBar1.Size = new System.Drawing.Size(505, 17);
            this.hScrollBar1.TabIndex = 0;
            this.hScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.hScrollBar1_Scroll);
            // 
            // lblTileHeight
            // 
            this.lblTileHeight.AutoSize = true;
            this.lblTileHeight.Location = new System.Drawing.Point(6, 16);
            this.lblTileHeight.Name = "lblTileHeight";
            this.lblTileHeight.Size = new System.Drawing.Size(38, 13);
            this.lblTileHeight.TabIndex = 2;
            this.lblTileHeight.Text = "Height";
            // 
            // lblTileWidth
            // 
            this.lblTileWidth.AutoSize = true;
            this.lblTileWidth.Location = new System.Drawing.Point(6, 42);
            this.lblTileWidth.Name = "lblTileWidth";
            this.lblTileWidth.Size = new System.Drawing.Size(35, 13);
            this.lblTileWidth.TabIndex = 3;
            this.lblTileWidth.Text = "Width";
            // 
            // numTileHeight
            // 
            this.numTileHeight.Location = new System.Drawing.Point(50, 14);
            this.numTileHeight.Maximum = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.numTileHeight.Name = "numTileHeight";
            this.numTileHeight.Size = new System.Drawing.Size(55, 20);
            this.numTileHeight.TabIndex = 4;
            this.numTileHeight.ValueChanged += new System.EventHandler(this.numTileHeight_ValueChanged);
            // 
            // numTileWidth
            // 
            this.numTileWidth.Location = new System.Drawing.Point(50, 40);
            this.numTileWidth.Maximum = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.numTileWidth.Name = "numTileWidth";
            this.numTileWidth.Size = new System.Drawing.Size(55, 20);
            this.numTileWidth.TabIndex = 5;
            this.numTileWidth.ValueChanged += new System.EventHandler(this.numTileWidth_ValueChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.AutoSize = true;
            this.groupBox1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.groupBox1.Controls.Add(this.lblTileHeight);
            this.groupBox1.Controls.Add(this.numTileWidth);
            this.groupBox1.Controls.Add(this.lblTileWidth);
            this.groupBox1.Controls.Add(this.numTileHeight);
            this.groupBox1.Location = new System.Drawing.Point(788, 28);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(111, 79);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Tile";
            // 
            // lblTileGridHeight
            // 
            this.lblTileGridHeight.AutoSize = true;
            this.lblTileGridHeight.Location = new System.Drawing.Point(6, 16);
            this.lblTileGridHeight.Name = "lblTileGridHeight";
            this.lblTileGridHeight.Size = new System.Drawing.Size(38, 13);
            this.lblTileGridHeight.TabIndex = 7;
            this.lblTileGridHeight.Text = "Height";
            // 
            // lblTileGridWidth
            // 
            this.lblTileGridWidth.AutoSize = true;
            this.lblTileGridWidth.Location = new System.Drawing.Point(6, 42);
            this.lblTileGridWidth.Name = "lblTileGridWidth";
            this.lblTileGridWidth.Size = new System.Drawing.Size(35, 13);
            this.lblTileGridWidth.TabIndex = 8;
            this.lblTileGridWidth.Text = "Width";
            // 
            // lblMapHeight
            // 
            this.lblMapHeight.AutoSize = true;
            this.lblMapHeight.Location = new System.Drawing.Point(6, 16);
            this.lblMapHeight.Name = "lblMapHeight";
            this.lblMapHeight.Size = new System.Drawing.Size(38, 13);
            this.lblMapHeight.TabIndex = 9;
            this.lblMapHeight.Text = "Height";
            // 
            // lblMapWidth
            // 
            this.lblMapWidth.AutoSize = true;
            this.lblMapWidth.Location = new System.Drawing.Point(6, 42);
            this.lblMapWidth.Name = "lblMapWidth";
            this.lblMapWidth.Size = new System.Drawing.Size(35, 13);
            this.lblMapWidth.TabIndex = 10;
            this.lblMapWidth.Text = "Width";
            // 
            // numMapWidth
            // 
            this.numMapWidth.Location = new System.Drawing.Point(50, 40);
            this.numMapWidth.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numMapWidth.Name = "numMapWidth";
            this.numMapWidth.Size = new System.Drawing.Size(55, 20);
            this.numMapWidth.TabIndex = 11;
            this.numMapWidth.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numMapWidth.ValueChanged += new System.EventHandler(this.numMapWidth_ValueChanged);
            // 
            // numMapHeight
            // 
            this.numMapHeight.Location = new System.Drawing.Point(50, 14);
            this.numMapHeight.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numMapHeight.Name = "numMapHeight";
            this.numMapHeight.Size = new System.Drawing.Size(55, 20);
            this.numMapHeight.TabIndex = 12;
            this.numMapHeight.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numMapHeight.ValueChanged += new System.EventHandler(this.numMapHeight_ValueChanged);
            // 
            // numTileGridHeight
            // 
            this.numTileGridHeight.Enabled = false;
            this.numTileGridHeight.Location = new System.Drawing.Point(50, 14);
            this.numTileGridHeight.Maximum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numTileGridHeight.Name = "numTileGridHeight";
            this.numTileGridHeight.Size = new System.Drawing.Size(55, 20);
            this.numTileGridHeight.TabIndex = 13;
            this.numTileGridHeight.ValueChanged += new System.EventHandler(this.numTileGridHeight_ValueChanged);
            // 
            // numTileGridWidth
            // 
            this.numTileGridWidth.Enabled = false;
            this.numTileGridWidth.Location = new System.Drawing.Point(50, 40);
            this.numTileGridWidth.Maximum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numTileGridWidth.Name = "numTileGridWidth";
            this.numTileGridWidth.Size = new System.Drawing.Size(55, 20);
            this.numTileGridWidth.TabIndex = 14;
            this.numTileGridWidth.ValueChanged += new System.EventHandler(this.numTileGridWidth_ValueChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.AutoSize = true;
            this.groupBox2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.groupBox2.Controls.Add(this.lblTileGridHeight);
            this.groupBox2.Controls.Add(this.numTileGridWidth);
            this.groupBox2.Controls.Add(this.lblTileGridWidth);
            this.groupBox2.Controls.Add(this.numTileGridHeight);
            this.groupBox2.Location = new System.Drawing.Point(905, 28);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(111, 79);
            this.groupBox2.TabIndex = 15;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Tile Grid";
            // 
            // groupBox3
            // 
            this.groupBox3.AutoSize = true;
            this.groupBox3.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.groupBox3.Controls.Add(this.lblMapHeight);
            this.groupBox3.Controls.Add(this.lblMapWidth);
            this.groupBox3.Controls.Add(this.numMapHeight);
            this.groupBox3.Controls.Add(this.numMapWidth);
            this.groupBox3.Location = new System.Drawing.Point(1022, 28);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(111, 79);
            this.groupBox3.TabIndex = 16;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Map";
            // 
            // lblTrigger
            // 
            this.lblTrigger.AutoSize = true;
            this.lblTrigger.Location = new System.Drawing.Point(6, 16);
            this.lblTrigger.Name = "lblTrigger";
            this.lblTrigger.Size = new System.Drawing.Size(40, 13);
            this.lblTrigger.TabIndex = 17;
            this.lblTrigger.Text = "Trigger";
            // 
            // txtTrigger
            // 
            this.txtTrigger.Location = new System.Drawing.Point(52, 13);
            this.txtTrigger.Name = "txtTrigger";
            this.txtTrigger.Size = new System.Drawing.Size(100, 20);
            this.txtTrigger.TabIndex = 18;
            // 
            // btnTrigger
            // 
            this.btnTrigger.Location = new System.Drawing.Point(50, 49);
            this.btnTrigger.Name = "btnTrigger";
            this.btnTrigger.Size = new System.Drawing.Size(75, 23);
            this.btnTrigger.TabIndex = 19;
            this.btnTrigger.Text = "Add Trigger";
            this.btnTrigger.UseVisualStyleBackColor = true;
            this.btnTrigger.Click += new System.EventHandler(this.btnTrigger_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.AutoSize = true;
            this.groupBox4.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.groupBox4.Controls.Add(this.lblTriggers);
            this.groupBox4.Controls.Add(this.cmbTrigger);
            this.groupBox4.Controls.Add(this.lblTrigger);
            this.groupBox4.Controls.Add(this.btnTrigger);
            this.groupBox4.Controls.Add(this.txtTrigger);
            this.groupBox4.Location = new System.Drawing.Point(816, 122);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(300, 91);
            this.groupBox4.TabIndex = 20;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Trigger";
            // 
            // lblTriggers
            // 
            this.lblTriggers.AutoSize = true;
            this.lblTriggers.Location = new System.Drawing.Point(204, 16);
            this.lblTriggers.Name = "lblTriggers";
            this.lblTriggers.Size = new System.Drawing.Size(45, 13);
            this.lblTriggers.TabIndex = 21;
            this.lblTriggers.Text = "Triggers";
            // 
            // cmbTrigger
            // 
            this.cmbTrigger.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbTrigger.FormattingEnabled = true;
            this.cmbTrigger.Items.AddRange(new object[] {
            "Plains",
            "Mountains",
            "Forest",
            "Shallow Water",
            "Deep Water"});
            this.cmbTrigger.Location = new System.Drawing.Point(173, 32);
            this.cmbTrigger.Name = "cmbTrigger";
            this.cmbTrigger.Size = new System.Drawing.Size(121, 21);
            this.cmbTrigger.TabIndex = 20;
            // 
            // rdoTile
            // 
            this.rdoTile.AutoSize = true;
            this.rdoTile.Location = new System.Drawing.Point(6, 19);
            this.rdoTile.Name = "rdoTile";
            this.rdoTile.Size = new System.Drawing.Size(42, 17);
            this.rdoTile.TabIndex = 21;
            this.rdoTile.TabStop = true;
            this.rdoTile.Text = "Tile";
            this.rdoTile.UseVisualStyleBackColor = true;
            this.rdoTile.CheckedChanged += new System.EventHandler(this.rdoTile_CheckedChanged);
            // 
            // rdoPlayerSpawn
            // 
            this.rdoPlayerSpawn.AutoSize = true;
            this.rdoPlayerSpawn.Location = new System.Drawing.Point(105, 42);
            this.rdoPlayerSpawn.Name = "rdoPlayerSpawn";
            this.rdoPlayerSpawn.Size = new System.Drawing.Size(90, 17);
            this.rdoPlayerSpawn.TabIndex = 22;
            this.rdoPlayerSpawn.TabStop = true;
            this.rdoPlayerSpawn.Text = "Player Spawn";
            this.rdoPlayerSpawn.UseVisualStyleBackColor = true;
            // 
            // rdoEnemySpawn
            // 
            this.rdoEnemySpawn.AutoSize = true;
            this.rdoEnemySpawn.Location = new System.Drawing.Point(6, 42);
            this.rdoEnemySpawn.Name = "rdoEnemySpawn";
            this.rdoEnemySpawn.Size = new System.Drawing.Size(93, 17);
            this.rdoEnemySpawn.TabIndex = 23;
            this.rdoEnemySpawn.TabStop = true;
            this.rdoEnemySpawn.Text = "Enemy Spawn";
            this.rdoEnemySpawn.UseVisualStyleBackColor = true;
            // 
            // groupBox5
            // 
            this.groupBox5.AutoSize = true;
            this.groupBox5.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.groupBox5.Controls.Add(this.chkCollision);
            this.groupBox5.Controls.Add(this.chkTrigger);
            this.groupBox5.Controls.Add(this.rdoTile);
            this.groupBox5.Controls.Add(this.rdoPlayerSpawn);
            this.groupBox5.Controls.Add(this.rdoEnemySpawn);
            this.groupBox5.Location = new System.Drawing.Point(788, 219);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(201, 101);
            this.groupBox5.TabIndex = 26;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Mode";
            // 
            // chkCollision
            // 
            this.chkCollision.AutoSize = true;
            this.chkCollision.Enabled = false;
            this.chkCollision.Location = new System.Drawing.Point(9, 65);
            this.chkCollision.Name = "chkCollision";
            this.chkCollision.Size = new System.Drawing.Size(64, 17);
            this.chkCollision.TabIndex = 25;
            this.chkCollision.Text = "Collision";
            this.chkCollision.UseVisualStyleBackColor = true;
            // 
            // chkTrigger
            // 
            this.chkTrigger.AutoSize = true;
            this.chkTrigger.Enabled = false;
            this.chkTrigger.Location = new System.Drawing.Point(105, 65);
            this.chkTrigger.Name = "chkTrigger";
            this.chkTrigger.Size = new System.Drawing.Size(59, 17);
            this.chkTrigger.TabIndex = 24;
            this.chkTrigger.Text = "Trigger";
            this.chkTrigger.UseVisualStyleBackColor = true;
            // 
            // lblLayer
            // 
            this.lblLayer.AutoSize = true;
            this.lblLayer.Location = new System.Drawing.Point(6, 16);
            this.lblLayer.Name = "lblLayer";
            this.lblLayer.Size = new System.Drawing.Size(33, 13);
            this.lblLayer.TabIndex = 27;
            this.lblLayer.Text = "Layer";
            // 
            // cmbLayer
            // 
            this.cmbLayer.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbLayer.FormattingEnabled = true;
            this.cmbLayer.Items.AddRange(new object[] {
            "1"});
            this.cmbLayer.Location = new System.Drawing.Point(45, 13);
            this.cmbLayer.Name = "cmbLayer";
            this.cmbLayer.Size = new System.Drawing.Size(121, 21);
            this.cmbLayer.TabIndex = 28;
            this.cmbLayer.SelectedIndexChanged += new System.EventHandler(this.cmbLayer_SelectedIndexChanged);
            // 
            // btnLayer
            // 
            this.btnLayer.Location = new System.Drawing.Point(61, 53);
            this.btnLayer.Name = "btnLayer";
            this.btnLayer.Size = new System.Drawing.Size(75, 23);
            this.btnLayer.TabIndex = 29;
            this.btnLayer.Text = "Add Layer";
            this.btnLayer.UseVisualStyleBackColor = true;
            this.btnLayer.Click += new System.EventHandler(this.btnLayer_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.AutoSize = true;
            this.groupBox6.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.groupBox6.Controls.Add(this.cmbLayer);
            this.groupBox6.Controls.Add(this.btnLayer);
            this.groupBox6.Controls.Add(this.lblLayer);
            this.groupBox6.Location = new System.Drawing.Point(797, 326);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(172, 95);
            this.groupBox6.TabIndex = 30;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Layer";
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 16;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1150, 477);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox5);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Khanquest: Tile Editor";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.map1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numTileHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTileWidth)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numMapWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMapHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTileGridHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numTileGridWidth)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importImageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Label lblTileHeight;
        private System.Windows.Forms.Label lblTileWidth;
        private System.Windows.Forms.NumericUpDown numTileHeight;
        private System.Windows.Forms.NumericUpDown numTileWidth;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label lblTileGridHeight;
        private System.Windows.Forms.Label lblTileGridWidth;
        private System.Windows.Forms.Label lblMapHeight;
        private System.Windows.Forms.Label lblMapWidth;
        private System.Windows.Forms.NumericUpDown numMapWidth;
        private System.Windows.Forms.NumericUpDown numMapHeight;
        private System.Windows.Forms.NumericUpDown numTileGridHeight;
        private System.Windows.Forms.NumericUpDown numTileGridWidth;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label lblTrigger;
        private System.Windows.Forms.TextBox txtTrigger;
        private System.Windows.Forms.Button btnTrigger;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label lblTriggers;
        private System.Windows.Forms.ComboBox cmbTrigger;
        private System.Windows.Forms.RadioButton rdoTile;
        private System.Windows.Forms.RadioButton rdoPlayerSpawn;
        private System.Windows.Forms.RadioButton rdoEnemySpawn;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label lblLayer;
        private System.Windows.Forms.ComboBox cmbLayer;
        private System.Windows.Forms.Button btnLayer;
        private System.Windows.Forms.GroupBox groupBox6;
        private Tile tile1;
        private System.Windows.Forms.Timer timer1;
        private Map map1;
        private System.Windows.Forms.CheckBox chkCollision;
        private System.Windows.Forms.CheckBox chkTrigger;
        private System.Windows.Forms.VScrollBar vScrollBar1;
        private System.Windows.Forms.HScrollBar hScrollBar1;
    }
}

