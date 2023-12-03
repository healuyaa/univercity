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
    public partial class MainFrom : Form
    {
        private string surname;
        private string phone;

        public MainFrom()
        {
            this.surname = "none";
            this.phone = "none";
            InitializeComponent();
        }
        private void MainFrom_Load(object sender, EventArgs e)
        {
            listView1.Columns.Add("Surname", 180);
            listView1.Columns.Add("Phone", 180);
        }

        private void DisplayDataInListView()
        {
            ListViewItem item_ = new ListViewItem(this.surname);
            item_.SubItems.Add(this.phone);
            listView1.Items.Add(item_);
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        }

        private void reductToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void create_newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            create_menu cm1 = new create_menu();
            cm1.ShowDialog();
            string temps = cm1.send_strings();
            this.surname = temps.Split(':')[0];
            this.phone = temps.Split(':')[1];
            DisplayDataInListView();
        }
    }
}
