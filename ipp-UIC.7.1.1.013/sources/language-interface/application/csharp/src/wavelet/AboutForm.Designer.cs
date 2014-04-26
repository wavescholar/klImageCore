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
   partial class AboutForm
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
         this.btnOK = new System.Windows.Forms.Button();
         this.panel1 = new System.Windows.Forms.Panel();
         this.lblName = new System.Windows.Forms.Label();
         this.lblVersion = new System.Windows.Forms.Label();
         this.label1 = new System.Windows.Forms.Label();
         this.panel1.SuspendLayout();
         this.SuspendLayout();
         // 
         // btnOK
         // 
         this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
         this.btnOK.Location = new System.Drawing.Point(118, 86);
         this.btnOK.Name = "btnOK";
         this.btnOK.Size = new System.Drawing.Size(75, 29);
         this.btnOK.TabIndex = 0;
         this.btnOK.Text = "OK";
         this.btnOK.UseVisualStyleBackColor = true;
         this.btnOK.Click += new System.EventHandler(this.button1_Click);
         // 
         // panel1
         // 
         this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
         this.panel1.Controls.Add(this.label1);
         this.panel1.Controls.Add(this.lblVersion);
         this.panel1.Controls.Add(this.lblName);
         this.panel1.Location = new System.Drawing.Point(12, 12);
         this.panel1.Name = "panel1";
         this.panel1.Size = new System.Drawing.Size(293, 60);
         this.panel1.TabIndex = 1;
         // 
         // lblName
         // 
         this.lblName.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
         this.lblName.Location = new System.Drawing.Point(6, 27);
         this.lblName.Name = "lblName";
         this.lblName.Size = new System.Drawing.Size(135, 20);
         this.lblName.TabIndex = 0;
         this.lblName.Text = "Name";
         this.lblName.TextAlign = System.Drawing.ContentAlignment.TopRight;
         // 
         // lblVersion
         // 
         this.lblVersion.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
         this.lblVersion.Location = new System.Drawing.Point(147, 27);
         this.lblVersion.Name = "lblVersion";
         this.lblVersion.Size = new System.Drawing.Size(139, 20);
         this.lblVersion.TabIndex = 1;
         this.lblVersion.Text = "Version";
         // 
         // label1
         // 
         this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                     | System.Windows.Forms.AnchorStyles.Right)));
         this.label1.AutoSize = true;
         this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
         this.label1.Location = new System.Drawing.Point(3, 0);
         this.label1.Name = "label1";
         this.label1.Size = new System.Drawing.Size(284, 16);
         this.label1.TabIndex = 2;
         this.label1.Text = "Intel® Integrated Performance Primitives";
         // 
         // AboutForm
         // 
         this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
         this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
         this.ClientSize = new System.Drawing.Size(315, 126);
         this.Controls.Add(this.panel1);
         this.Controls.Add(this.btnOK);
         this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
         this.ImeMode = System.Windows.Forms.ImeMode.Close;
         this.MaximizeBox = false;
         this.MinimizeBox = false;
         this.Name = "AboutForm";
         this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
         this.Text = "About";
         this.panel1.ResumeLayout(false);
         this.panel1.PerformLayout();
         this.ResumeLayout(false);

      }

      #endregion

      private System.Windows.Forms.Button btnOK;
      private System.Windows.Forms.Panel panel1;
      private System.Windows.Forms.Label lblName;
      private System.Windows.Forms.Label lblVersion;
      private System.Windows.Forms.Label label1;
   }
}