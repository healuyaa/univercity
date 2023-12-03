namespace lab4_6
{
    partial class MainFrom
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
            menuStrip1 = new MenuStrip();
            create_newToolStripMenuItem = new ToolStripMenuItem();
            reductToolStripMenuItem = new ToolStripMenuItem();
            listView1 = new ListView();
            menuStrip1.SuspendLayout();
            SuspendLayout();
            // 
            // menuStrip1
            // 
            menuStrip1.Items.AddRange(new ToolStripItem[] { create_newToolStripMenuItem, reductToolStripMenuItem });
            menuStrip1.Location = new Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Size = new Size(414, 24);
            menuStrip1.TabIndex = 0;
            menuStrip1.Text = "menuStrip1";
            // 
            // create_newToolStripMenuItem
            // 
            create_newToolStripMenuItem.Name = "create_newToolStripMenuItem";
            create_newToolStripMenuItem.Size = new Size(78, 20);
            create_newToolStripMenuItem.Text = "create_new";
            create_newToolStripMenuItem.Click += create_newToolStripMenuItem_Click;
            // 
            // reductToolStripMenuItem
            // 
            reductToolStripMenuItem.Name = "reductToolStripMenuItem";
            reductToolStripMenuItem.Size = new Size(53, 20);
            reductToolStripMenuItem.Text = "reduct";
            reductToolStripMenuItem.Click += reductToolStripMenuItem_Click;
            // 
            // listView1
            // 
            listView1.GridLines = true;
            listView1.Location = new Point(12, 27);
            listView1.Name = "listView1";
            listView1.Size = new Size(385, 353);
            listView1.TabIndex = 2;
            listView1.UseCompatibleStateImageBehavior = false;
            listView1.View = View.Details;
            listView1.SelectedIndexChanged += listView1_SelectedIndexChanged;
            // 
            // MainFrom
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(414, 450);
            Controls.Add(listView1);
            Controls.Add(menuStrip1);
            MainMenuStrip = menuStrip1;
            Name = "MainFrom";
            Text = "MainFrom";
            Load += MainFrom_Load;
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private MenuStrip menuStrip1;
        private ToolStripMenuItem reductToolStripMenuItem;
        private ToolStripMenuItem create_newToolStripMenuItem;
        private ListView listView1;
    }
}