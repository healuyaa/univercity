using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab4_6
{
    public partial class create_menu : Form
    {

        public string surname = "none";
        public string phone = "none";
        public create_menu()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.surname = textBox1.Text;
            this.phone = textBox2.Text;
            send_strings();
            Close();
        }

        public string send_strings()
        {
            string end_text = this.surname + ":" + this.phone;
            return end_text;
        }
    }
}
