/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/
namespace wavelet
{
   partial class MainForm
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
         System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
         this.MainMenu = new System.Windows.Forms.MenuStrip();
         this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
         this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
         this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
         this.eToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
         this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
         this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
         this.panel1 = new System.Windows.Forms.Panel();
         this.label1 = new System.Windows.Forms.Label();
         this.imgSrc = new System.Windows.Forms.PictureBox();
         this.panel2 = new System.Windows.Forms.Panel();
         this.label2 = new System.Windows.Forms.Label();
         this.imgDst = new System.Windows.Forms.PictureBox();
         this.panel3 = new System.Windows.Forms.Panel();
         this.cbD = new System.Windows.Forms.CheckBox();
         this.cbV = new System.Windows.Forms.CheckBox();
         this.cbH = new System.Windows.Forms.CheckBox();
         this.cbA = new System.Windows.Forms.CheckBox();
         this.label7 = new System.Windows.Forms.Label();
         this.label6 = new System.Windows.Forms.Label();
         this.label5 = new System.Windows.Forms.Label();
         this.label4 = new System.Windows.Forms.Label();
         this.imgD = new System.Windows.Forms.PictureBox();
         this.imgV = new System.Windows.Forms.PictureBox();
         this.imgH = new System.Windows.Forms.PictureBox();
         this.imgA = new System.Windows.Forms.PictureBox();
         this.label3 = new System.Windows.Forms.Label();
         this.toolTip = new System.Windows.Forms.ToolTip(this.components);
         this.lblCR = new System.Windows.Forms.Label();
         this.lblCG = new System.Windows.Forms.Label();
         this.lblCB = new System.Windows.Forms.Label();
         this.lblL1B = new System.Windows.Forms.Label();
         this.lblL1G = new System.Windows.Forms.Label();
         this.lblL1R = new System.Windows.Forms.Label();
         this.lblL2B = new System.Windows.Forms.Label();
         this.lblL2G = new System.Windows.Forms.Label();
         this.lblL2R = new System.Windows.Forms.Label();
         this.lblRL2R = new System.Windows.Forms.Label();
         this.lblRCB = new System.Windows.Forms.Label();
         this.lblRL2G = new System.Windows.Forms.Label();
         this.lblRCR = new System.Windows.Forms.Label();
         this.lblRL2B = new System.Windows.Forms.Label();
         this.lblRCG = new System.Windows.Forms.Label();
         this.lblRL1R = new System.Windows.Forms.Label();
         this.lblRL1B = new System.Windows.Forms.Label();
         this.lblRL1G = new System.Windows.Forms.Label();
         this.cbxType = new System.Windows.Forms.ComboBox();
         this.groupBox1 = new System.Windows.Forms.GroupBox();
         this.groupBox2 = new System.Windows.Forms.GroupBox();
         this.label8 = new System.Windows.Forms.Label();
         this.MainMenu.SuspendLayout();
         this.panel1.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.imgSrc)).BeginInit();
         this.panel2.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.imgDst)).BeginInit();
         this.panel3.SuspendLayout();
         ((System.ComponentModel.ISupportInitialize)(this.imgD)).BeginInit();
         ((System.ComponentModel.ISupportInitialize)(this.imgV)).BeginInit();
         ((System.ComponentModel.ISupportInitialize)(this.imgH)).BeginInit();
         ((System.ComponentModel.ISupportInitialize)(this.imgA)).BeginInit();
         this.groupBox1.SuspendLayout();
         this.groupBox2.SuspendLayout();
         this.SuspendLayout();
         // 
         // MainMenu
         // 
         this.MainMenu.BackColor = System.Drawing.SystemColors.Menu;
         this.MainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.aboutToolStripMenuItem});
         this.MainMenu.Location = new System.Drawing.Point(0, 0);
         this.MainMenu.Name = "MainMenu";
         this.MainMenu.Size = new System.Drawing.Size(479, 24);
         this.MainMenu.TabIndex = 0;
         this.MainMenu.Text = "MainMenu";
         // 
         // fileToolStripMenuItem
         // 
         this.fileToolStripMenuItem.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
         this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.toolStripMenuItem1,
            this.eToolStripMenuItem});
         this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
         this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
         this.fileToolStripMenuItem.Text = "&File";
         // 
         // openToolStripMenuItem
         // 
         this.openToolStripMenuItem.Name = "openToolStripMenuItem";
         this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
         this.openToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
         this.openToolStripMenuItem.Text = "&Open...";
         this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
         // 
         // toolStripMenuItem1
         // 
         this.toolStripMenuItem1.Name = "toolStripMenuItem1";
         this.toolStripMenuItem1.Size = new System.Drawing.Size(149, 6);
         // 
         // eToolStripMenuItem
         // 
         this.eToolStripMenuItem.Name = "eToolStripMenuItem";
         this.eToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
         this.eToolStripMenuItem.Text = "E&xit";
         this.eToolStripMenuItem.Click += new System.EventHandler(this.eToolStripMenuItem_Click);
         // 
         // aboutToolStripMenuItem
         // 
         this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
         this.aboutToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
         this.aboutToolStripMenuItem.Text = "About";
         this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
         // 
         // openFileDialog
         // 
         this.openFileDialog.Filter = "JPEG files (*.jpg)|*.jpg";
         this.openFileDialog.Title = "Open File";
         // 
         // panel1
         // 
         this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
         this.panel1.Controls.Add(this.label1);
         this.panel1.Controls.Add(this.imgSrc);
         this.panel1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
         this.panel1.Location = new System.Drawing.Point(12, 155);
         this.panel1.Name = "panel1";
         this.panel1.Size = new System.Drawing.Size(160, 124);
         this.panel1.TabIndex = 1;
         // 
         // label1
         // 
         this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                     | System.Windows.Forms.AnchorStyles.Right)));
         this.label1.AutoSize = true;
         this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
         this.label1.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
         this.label1.Location = new System.Drawing.Point(53, 0);
         this.label1.Name = "label1";
         this.label1.Size = new System.Drawing.Size(50, 13);
         this.label1.TabIndex = 1;
         this.label1.Text = "Original";
         this.label1.TextAlign = System.Drawing.ContentAlignment.TopCenter;
         // 
         // imgSrc
         // 
         this.imgSrc.Location = new System.Drawing.Point(3, 16);
         this.imgSrc.Name = "imgSrc";
         this.imgSrc.Size = new System.Drawing.Size(150, 100);
         this.imgSrc.TabIndex = 0;
         this.imgSrc.TabStop = false;
         this.toolTip.SetToolTip(this.imgSrc, "The original source image");
         // 
         // panel2
         // 
         this.panel2.AccessibleRole = System.Windows.Forms.AccessibleRole.None;
         this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
         this.panel2.Controls.Add(this.label2);
         this.panel2.Controls.Add(this.imgDst);
         this.panel2.ImeMode = System.Windows.Forms.ImeMode.NoControl;
         this.panel2.Location = new System.Drawing.Point(308, 155);
         this.panel2.Name = "panel2";
         this.panel2.Size = new System.Drawing.Size(160, 124);
         this.panel2.TabIndex = 2;
         // 
         // label2
         // 
         this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                     | System.Windows.Forms.AnchorStyles.Right)));
         this.label2.AutoSize = true;
         this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
         this.label2.ImageAlign = System.Drawing.ContentAlignment.TopCenter;
         this.label2.Location = new System.Drawing.Point(33, 0);
         this.label2.Name = "label2";
         this.label2.Size = new System.Drawing.Size(90, 13);
         this.label2.TabIndex = 1;
         this.label2.Text = "Reconstructed";
         this.label2.TextAlign = System.Drawing.ContentAlignment.TopCenter;
         // 
         // imgDst
         // 
         this.imgDst.Location = new System.Drawing.Point(3, 16);
         this.imgDst.Name = "imgDst";
         this.imgDst.Size = new System.Drawing.Size(150, 100);
         this.imgDst.TabIndex = 0;
         this.imgDst.TabStop = false;
         this.toolTip.SetToolTip(this.imgDst, "The reconstructed image");
         // 
         // panel3
         // 
         this.panel3.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
         this.panel3.Controls.Add(this.cbD);
         this.panel3.Controls.Add(this.cbV);
         this.panel3.Controls.Add(this.cbH);
         this.panel3.Controls.Add(this.cbA);
         this.panel3.Controls.Add(this.label7);
         this.panel3.Controls.Add(this.label6);
         this.panel3.Controls.Add(this.label5);
         this.panel3.Controls.Add(this.label4);
         this.panel3.Controls.Add(this.imgD);
         this.panel3.Controls.Add(this.imgV);
         this.panel3.Controls.Add(this.imgH);
         this.panel3.Controls.Add(this.imgA);
         this.panel3.Controls.Add(this.label3);
         this.panel3.Location = new System.Drawing.Point(178, 40);
         this.panel3.Name = "panel3";
         this.panel3.Size = new System.Drawing.Size(124, 324);
         this.panel3.TabIndex = 3;
         // 
         // cbD
         // 
         this.cbD.AutoSize = true;
         this.cbD.Checked = true;
         this.cbD.CheckState = System.Windows.Forms.CheckState.Checked;
         this.cbD.Location = new System.Drawing.Point(93, 287);
         this.cbD.Name = "cbD";
         this.cbD.Size = new System.Drawing.Size(15, 14);
         this.cbD.TabIndex = 11;
         this.toolTip.SetToolTip(this.cbD, "To use or not the ‘diagonal detail’ image under reconstruction");
         this.cbD.UseVisualStyleBackColor = true;
         this.cbD.CheckedChanged += new System.EventHandler(this.cb_CheckedChanged);
         // 
         // cbV
         // 
         this.cbV.AutoSize = true;
         this.cbV.Checked = true;
         this.cbV.CheckState = System.Windows.Forms.CheckState.Checked;
         this.cbV.Location = new System.Drawing.Point(93, 218);
         this.cbV.Name = "cbV";
         this.cbV.Size = new System.Drawing.Size(15, 14);
         this.cbV.TabIndex = 10;
         this.toolTip.SetToolTip(this.cbV, "To use or not the ‘vertical detail’ image under reconstruction");
         this.cbV.UseVisualStyleBackColor = true;
         this.cbV.CheckedChanged += new System.EventHandler(this.cb_CheckedChanged);
         // 
         // cbH
         // 
         this.cbH.AutoSize = true;
         this.cbH.Checked = true;
         this.cbH.CheckState = System.Windows.Forms.CheckState.Checked;
         this.cbH.Location = new System.Drawing.Point(93, 149);
         this.cbH.Name = "cbH";
         this.cbH.Size = new System.Drawing.Size(15, 14);
         this.cbH.TabIndex = 9;
         this.toolTip.SetToolTip(this.cbH, "To use or not the ‘horizontal detail’ image under reconstruction");
         this.cbH.UseVisualStyleBackColor = true;
         this.cbH.CheckedChanged += new System.EventHandler(this.cb_CheckedChanged);
         // 
         // cbA
         // 
         this.cbA.AutoSize = true;
         this.cbA.Checked = true;
         this.cbA.CheckState = System.Windows.Forms.CheckState.Checked;
         this.cbA.Location = new System.Drawing.Point(93, 80);
         this.cbA.Name = "cbA";
         this.cbA.Size = new System.Drawing.Size(15, 14);
         this.cbA.TabIndex = 4;
         this.toolTip.SetToolTip(this.cbA, "To use or not the ‘approximation’ image under reconstruction");
         this.cbA.UseVisualStyleBackColor = true;
         this.cbA.CheckedChanged += new System.EventHandler(this.cb_CheckedChanged);
         // 
         // label7
         // 
         this.label7.AutoSize = true;
         this.label7.Location = new System.Drawing.Point(25, 235);
         this.label7.Name = "label7";
         this.label7.Size = new System.Drawing.Size(49, 13);
         this.label7.TabIndex = 8;
         this.label7.Text = "Diagonal";
         // 
         // label6
         // 
         this.label6.AutoSize = true;
         this.label6.Location = new System.Drawing.Point(28, 166);
         this.label6.Name = "label6";
         this.label6.Size = new System.Drawing.Size(42, 13);
         this.label6.TabIndex = 7;
         this.label6.Text = "Vertical";
         // 
         // label5
         // 
         this.label5.AutoSize = true;
         this.label5.Location = new System.Drawing.Point(22, 97);
         this.label5.Name = "label5";
         this.label5.Size = new System.Drawing.Size(54, 13);
         this.label5.TabIndex = 6;
         this.label5.Text = "Horizontal";
         // 
         // label4
         // 
         this.label4.AutoSize = true;
         this.label4.Location = new System.Drawing.Point(14, 28);
         this.label4.Name = "label4";
         this.label4.Size = new System.Drawing.Size(73, 13);
         this.label4.TabIndex = 5;
         this.label4.Text = "Approximation";
         // 
         // imgD
         // 
         this.imgD.Location = new System.Drawing.Point(12, 251);
         this.imgD.Name = "imgD";
         this.imgD.Size = new System.Drawing.Size(75, 50);
         this.imgD.TabIndex = 4;
         this.imgD.TabStop = false;
         this.toolTip.SetToolTip(this.imgD, "The ‘diagonal detail’ image is obtained by\r\nvertical and horizontal highpass filt" +
                 "ering");
         // 
         // imgV
         // 
         this.imgV.Location = new System.Drawing.Point(12, 182);
         this.imgV.Name = "imgV";
         this.imgV.Size = new System.Drawing.Size(75, 50);
         this.imgV.TabIndex = 3;
         this.imgV.TabStop = false;
         this.toolTip.SetToolTip(this.imgV, "The ‘vertical detail’ image is obtained by\r\nvertical lowpass and horizontal highp" +
                 "ass filtering");
         // 
         // imgH
         // 
         this.imgH.Location = new System.Drawing.Point(12, 113);
         this.imgH.Name = "imgH";
         this.imgH.Size = new System.Drawing.Size(75, 50);
         this.imgH.TabIndex = 2;
         this.imgH.TabStop = false;
         this.toolTip.SetToolTip(this.imgH, "The ‘horizontal detail’ image is obtained by\r\nvertical highpass and horizontal lo" +
                 "wpass filtering");
         // 
         // imgA
         // 
         this.imgA.Cursor = System.Windows.Forms.Cursors.Default;
         this.imgA.Location = new System.Drawing.Point(12, 44);
         this.imgA.Name = "imgA";
         this.imgA.Size = new System.Drawing.Size(75, 50);
         this.imgA.TabIndex = 1;
         this.imgA.TabStop = false;
         this.toolTip.SetToolTip(this.imgA, "The ‘approximation’ image is obtained by\r\nvertical and horizontal lowpass filteri" +
                 "ng");
         // 
         // label3
         // 
         this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                     | System.Windows.Forms.AnchorStyles.Right)));
         this.label3.AutoSize = true;
         this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
         this.label3.Location = new System.Drawing.Point(22, 0);
         this.label3.Name = "label3";
         this.label3.Size = new System.Drawing.Size(80, 13);
         this.label3.TabIndex = 0;
         this.label3.Text = "Decomposed";
         // 
         // lblCR
         // 
         this.lblCR.ForeColor = System.Drawing.Color.Red;
         this.lblCR.Location = new System.Drawing.Point(118, 25);
         this.lblCR.Name = "lblCR";
         this.lblCR.Size = new System.Drawing.Size(50, 13);
         this.lblCR.TabIndex = 10;
         this.lblCR.Text = "CR";
         this.lblCR.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblCR, "C-norm of original and reconstructed images");
         // 
         // lblCG
         // 
         this.lblCG.ForeColor = System.Drawing.Color.Green;
         this.lblCG.Location = new System.Drawing.Point(62, 25);
         this.lblCG.Name = "lblCG";
         this.lblCG.Size = new System.Drawing.Size(50, 13);
         this.lblCG.TabIndex = 11;
         this.lblCG.Text = "CG";
         this.lblCG.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblCG, "C-norm of original and reconstructed images");
         // 
         // lblCB
         // 
         this.lblCB.ForeColor = System.Drawing.Color.Blue;
         this.lblCB.Location = new System.Drawing.Point(6, 25);
         this.lblCB.Name = "lblCB";
         this.lblCB.Size = new System.Drawing.Size(50, 13);
         this.lblCB.TabIndex = 12;
         this.lblCB.Text = "CB";
         this.lblCB.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblCB, "C-norm of original and reconstructed images");
         // 
         // lblL1B
         // 
         this.lblL1B.ForeColor = System.Drawing.Color.Blue;
         this.lblL1B.Location = new System.Drawing.Point(6, 45);
         this.lblL1B.Name = "lblL1B";
         this.lblL1B.Size = new System.Drawing.Size(50, 13);
         this.lblL1B.TabIndex = 15;
         this.lblL1B.Text = "1234567";
         this.lblL1B.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblL1B, "L1-norm of original and reconstructed images");
         // 
         // lblL1G
         // 
         this.lblL1G.ForeColor = System.Drawing.Color.Green;
         this.lblL1G.Location = new System.Drawing.Point(62, 45);
         this.lblL1G.Name = "lblL1G";
         this.lblL1G.Size = new System.Drawing.Size(50, 13);
         this.lblL1G.TabIndex = 16;
         this.lblL1G.Text = "1234567";
         this.lblL1G.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblL1G, "L1-norm of original and reconstructed images");
         // 
         // lblL1R
         // 
         this.lblL1R.ForeColor = System.Drawing.Color.Red;
         this.lblL1R.Location = new System.Drawing.Point(118, 45);
         this.lblL1R.Name = "lblL1R";
         this.lblL1R.Size = new System.Drawing.Size(50, 13);
         this.lblL1R.TabIndex = 17;
         this.lblL1R.Text = "1234567";
         this.lblL1R.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblL1R, "L1-norm of original and reconstructed images");
         // 
         // lblL2B
         // 
         this.lblL2B.ForeColor = System.Drawing.Color.Blue;
         this.lblL2B.Location = new System.Drawing.Point(6, 65);
         this.lblL2B.Name = "lblL2B";
         this.lblL2B.Size = new System.Drawing.Size(50, 13);
         this.lblL2B.TabIndex = 18;
         this.lblL2B.Text = "1234567";
         this.lblL2B.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblL2B, "L2-norm of original and reconstructed images");
         // 
         // lblL2G
         // 
         this.lblL2G.ForeColor = System.Drawing.Color.Green;
         this.lblL2G.Location = new System.Drawing.Point(62, 65);
         this.lblL2G.Name = "lblL2G";
         this.lblL2G.Size = new System.Drawing.Size(50, 13);
         this.lblL2G.TabIndex = 19;
         this.lblL2G.Text = "1234567";
         this.lblL2G.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblL2G, "L2-norm of original and reconstructed images");
         // 
         // lblL2R
         // 
         this.lblL2R.ForeColor = System.Drawing.Color.Red;
         this.lblL2R.Location = new System.Drawing.Point(118, 65);
         this.lblL2R.Name = "lblL2R";
         this.lblL2R.Size = new System.Drawing.Size(50, 13);
         this.lblL2R.TabIndex = 20;
         this.lblL2R.Text = "1234567";
         this.lblL2R.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblL2R, "L2-norm of original and reconstructed images");
         // 
         // lblRL2R
         // 
         this.lblRL2R.ForeColor = System.Drawing.Color.Red;
         this.lblRL2R.Location = new System.Drawing.Point(87, 65);
         this.lblRL2R.Name = "lblRL2R";
         this.lblRL2R.Size = new System.Drawing.Size(32, 13);
         this.lblRL2R.TabIndex = 20;
         this.lblRL2R.Text = "1.23";
         this.lblRL2R.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblRL2R, "Relative error for L2-norm of original and reconstructed images");
         // 
         // lblRCB
         // 
         this.lblRCB.ForeColor = System.Drawing.Color.Blue;
         this.lblRCB.Location = new System.Drawing.Point(11, 25);
         this.lblRCB.Name = "lblRCB";
         this.lblRCB.Size = new System.Drawing.Size(32, 13);
         this.lblRCB.TabIndex = 12;
         this.lblRCB.Text = "1.23";
         this.lblRCB.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblRCB, "Relative error for C-norm of original and reconstructed images");
         // 
         // lblRL2G
         // 
         this.lblRL2G.ForeColor = System.Drawing.Color.Green;
         this.lblRL2G.Location = new System.Drawing.Point(49, 65);
         this.lblRL2G.Name = "lblRL2G";
         this.lblRL2G.Size = new System.Drawing.Size(32, 13);
         this.lblRL2G.TabIndex = 19;
         this.lblRL2G.Text = "1.23";
         this.lblRL2G.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblRL2G, "Relative error for L2-norm of original and reconstructed images");
         // 
         // lblRCR
         // 
         this.lblRCR.ForeColor = System.Drawing.Color.Red;
         this.lblRCR.Location = new System.Drawing.Point(87, 25);
         this.lblRCR.Name = "lblRCR";
         this.lblRCR.Size = new System.Drawing.Size(32, 13);
         this.lblRCR.TabIndex = 10;
         this.lblRCR.Text = "1.23";
         this.lblRCR.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblRCR, "Relative error for C-norm of original and reconstructed images");
         // 
         // lblRL2B
         // 
         this.lblRL2B.ForeColor = System.Drawing.Color.Blue;
         this.lblRL2B.Location = new System.Drawing.Point(11, 65);
         this.lblRL2B.Name = "lblRL2B";
         this.lblRL2B.Size = new System.Drawing.Size(32, 13);
         this.lblRL2B.TabIndex = 18;
         this.lblRL2B.Text = "1.23";
         this.lblRL2B.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblRL2B, "Relative error for L2-norm of original and reconstructed images");
         // 
         // lblRCG
         // 
         this.lblRCG.ForeColor = System.Drawing.Color.Green;
         this.lblRCG.Location = new System.Drawing.Point(49, 25);
         this.lblRCG.Name = "lblRCG";
         this.lblRCG.Size = new System.Drawing.Size(32, 13);
         this.lblRCG.TabIndex = 11;
         this.lblRCG.Text = "1.23";
         this.lblRCG.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblRCG, "Relative error for C-norm of original and reconstructed images");
         // 
         // lblRL1R
         // 
         this.lblRL1R.ForeColor = System.Drawing.Color.Red;
         this.lblRL1R.Location = new System.Drawing.Point(87, 45);
         this.lblRL1R.Name = "lblRL1R";
         this.lblRL1R.Size = new System.Drawing.Size(32, 13);
         this.lblRL1R.TabIndex = 17;
         this.lblRL1R.Text = "1.23";
         this.lblRL1R.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblRL1R, "Relative error for L1-norm of original and reconstructed images");
         // 
         // lblRL1B
         // 
         this.lblRL1B.ForeColor = System.Drawing.Color.Blue;
         this.lblRL1B.Location = new System.Drawing.Point(11, 45);
         this.lblRL1B.Name = "lblRL1B";
         this.lblRL1B.Size = new System.Drawing.Size(32, 13);
         this.lblRL1B.TabIndex = 15;
         this.lblRL1B.Text = "1.23";
         this.lblRL1B.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblRL1B, "Relative error for L1-norm of original and reconstructed images");
         // 
         // lblRL1G
         // 
         this.lblRL1G.ForeColor = System.Drawing.Color.Green;
         this.lblRL1G.Location = new System.Drawing.Point(49, 45);
         this.lblRL1G.Name = "lblRL1G";
         this.lblRL1G.Size = new System.Drawing.Size(32, 13);
         this.lblRL1G.TabIndex = 16;
         this.lblRL1G.Text = "1.23";
         this.lblRL1G.TextAlign = System.Drawing.ContentAlignment.TopRight;
         this.toolTip.SetToolTip(this.lblRL1G, "Relative error for L1-norm of original and reconstructed images");
         // 
         // cbxType
         // 
         this.cbxType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
         this.cbxType.FormattingEnabled = true;
         this.cbxType.Items.AddRange(new object[] {
            "Haar",
            "LineSpline",
            "QuadSpline",
            "Vaidyanathan"});
         this.cbxType.Location = new System.Drawing.Point(346, 397);
         this.cbxType.Name = "cbxType";
         this.cbxType.Size = new System.Drawing.Size(121, 21);
         this.cbxType.TabIndex = 8;
         this.cbxType.SelectedIndexChanged += new System.EventHandler(this.cbxType_SelectedIndexChanged);
         // 
         // groupBox1
         // 
         this.groupBox1.Controls.Add(this.lblL2R);
         this.groupBox1.Controls.Add(this.lblCB);
         this.groupBox1.Controls.Add(this.lblL2G);
         this.groupBox1.Controls.Add(this.lblCR);
         this.groupBox1.Controls.Add(this.lblL2B);
         this.groupBox1.Controls.Add(this.lblCG);
         this.groupBox1.Controls.Add(this.lblL1R);
         this.groupBox1.Controls.Add(this.lblL1B);
         this.groupBox1.Controls.Add(this.lblL1G);
         this.groupBox1.Location = new System.Drawing.Point(12, 380);
         this.groupBox1.Name = "groupBox1";
         this.groupBox1.Size = new System.Drawing.Size(179, 89);
         this.groupBox1.TabIndex = 9;
         this.groupBox1.TabStop = false;
         this.groupBox1.Text = "Absolute Error";
         // 
         // groupBox2
         // 
         this.groupBox2.Controls.Add(this.lblRL2R);
         this.groupBox2.Controls.Add(this.lblRCB);
         this.groupBox2.Controls.Add(this.lblRL2G);
         this.groupBox2.Controls.Add(this.lblRCR);
         this.groupBox2.Controls.Add(this.lblRL2B);
         this.groupBox2.Controls.Add(this.lblRCG);
         this.groupBox2.Controls.Add(this.lblRL1R);
         this.groupBox2.Controls.Add(this.lblRL1B);
         this.groupBox2.Controls.Add(this.lblRL1G);
         this.groupBox2.Location = new System.Drawing.Point(197, 380);
         this.groupBox2.Name = "groupBox2";
         this.groupBox2.Size = new System.Drawing.Size(133, 89);
         this.groupBox2.TabIndex = 10;
         this.groupBox2.TabStop = false;
         this.groupBox2.Text = "Relative Error";
         // 
         // label8
         // 
         this.label8.AutoSize = true;
         this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
         this.label8.Location = new System.Drawing.Point(343, 380);
         this.label8.Name = "label8";
         this.label8.Size = new System.Drawing.Size(110, 13);
         this.label8.TabIndex = 11;
         this.label8.Text = "Preset filter banks";
         // 
         // MainForm
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.BackColor = System.Drawing.SystemColors.Control;
         this.ClientSize = new System.Drawing.Size(479, 481);
         this.Controls.Add(this.label8);
         this.Controls.Add(this.groupBox2);
         this.Controls.Add(this.groupBox1);
         this.Controls.Add(this.cbxType);
         this.Controls.Add(this.panel3);
         this.Controls.Add(this.panel2);
         this.Controls.Add(this.panel1);
         this.Controls.Add(this.MainMenu);
         this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
         this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
         this.MainMenuStrip = this.MainMenu;
         this.MaximizeBox = false;
         this.Name = "MainForm";
         this.Text = "Wavelet Transform";
         this.MainMenu.ResumeLayout(false);
         this.MainMenu.PerformLayout();
         this.panel1.ResumeLayout(false);
         this.panel1.PerformLayout();
         ((System.ComponentModel.ISupportInitialize)(this.imgSrc)).EndInit();
         this.panel2.ResumeLayout(false);
         this.panel2.PerformLayout();
         ((System.ComponentModel.ISupportInitialize)(this.imgDst)).EndInit();
         this.panel3.ResumeLayout(false);
         this.panel3.PerformLayout();
         ((System.ComponentModel.ISupportInitialize)(this.imgD)).EndInit();
         ((System.ComponentModel.ISupportInitialize)(this.imgV)).EndInit();
         ((System.ComponentModel.ISupportInitialize)(this.imgH)).EndInit();
         ((System.ComponentModel.ISupportInitialize)(this.imgA)).EndInit();
         this.groupBox1.ResumeLayout(false);
         this.groupBox2.ResumeLayout(false);
         this.ResumeLayout(false);
         this.PerformLayout();

      }

      #endregion

      private System.Windows.Forms.MenuStrip MainMenu;
      private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
      private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
      private System.Windows.Forms.ToolStripMenuItem eToolStripMenuItem;
      private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
      private System.Windows.Forms.OpenFileDialog openFileDialog;
      private System.Windows.Forms.Panel panel1;
      private System.Windows.Forms.PictureBox imgSrc;
      private System.Windows.Forms.Label label1;
      private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
      private System.Windows.Forms.Panel panel2;
      private System.Windows.Forms.Label label2;
      private System.Windows.Forms.PictureBox imgDst;
      private System.Windows.Forms.Panel panel3;
      private System.Windows.Forms.Label label3;
      private System.Windows.Forms.PictureBox imgD;
      private System.Windows.Forms.PictureBox imgV;
      private System.Windows.Forms.PictureBox imgH;
      private System.Windows.Forms.PictureBox imgA;
      private System.Windows.Forms.Label label7;
      private System.Windows.Forms.Label label6;
      private System.Windows.Forms.Label label5;
      private System.Windows.Forms.Label label4;
      private System.Windows.Forms.CheckBox cbA;
      private System.Windows.Forms.CheckBox cbH;
      private System.Windows.Forms.CheckBox cbD;
      private System.Windows.Forms.CheckBox cbV;
      private System.Windows.Forms.ToolTip toolTip;
      private System.Windows.Forms.ComboBox cbxType;
      private System.Windows.Forms.GroupBox groupBox1;
      private System.Windows.Forms.Label lblCR;
      private System.Windows.Forms.Label lblCG;
      private System.Windows.Forms.Label lblCB;
      private System.Windows.Forms.Label lblL1B;
      private System.Windows.Forms.Label lblL1G;
      private System.Windows.Forms.Label lblL1R;
      private System.Windows.Forms.Label lblL2B;
      private System.Windows.Forms.Label lblL2G;
      private System.Windows.Forms.Label lblL2R;
      private System.Windows.Forms.GroupBox groupBox2;
      private System.Windows.Forms.Label lblRL2R;
      private System.Windows.Forms.Label lblRCB;
      private System.Windows.Forms.Label lblRL2G;
      private System.Windows.Forms.Label lblRCR;
      private System.Windows.Forms.Label lblRL2B;
      private System.Windows.Forms.Label lblRCG;
      private System.Windows.Forms.Label lblRL1R;
      private System.Windows.Forms.Label lblRL1B;
      private System.Windows.Forms.Label lblRL1G;
      private System.Windows.Forms.Label label8;

   }
}

