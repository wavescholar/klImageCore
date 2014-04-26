/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//       Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ipp;

namespace wavelet
{
   public partial class AboutForm : Form
   {
      public AboutForm()
      {
         InitializeComponent();
         IppLibraryVersion ver = ip.ippiGetLibVersion();
         lblName.Text = ver.Name;
         lblVersion.Text = ver.Version;
      }

      private void button1_Click(object sender, EventArgs e)
      {
         Close();
      }

   }
}