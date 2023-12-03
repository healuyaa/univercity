using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;

namespace lab4_6
{
    public partial class reduct : Form
    {
        public string s1 = "";
        public string s2 = "";

        public reduct()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            s1 = textBox1.Text;
            s2 = textBox2.Text;
        }

        public string send_s1_s2()
        {
            return s1 + ":" + s2;
        }
    }
}
