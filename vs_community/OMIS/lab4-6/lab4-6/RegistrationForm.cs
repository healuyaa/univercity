using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace lab4_6
{
    public partial class RegistrationForm : Form
    {
        private Dictionary<string, string> users;

        public RegistrationForm()
        {
            InitializeComponent();
            users = LoadUsers();
        }

        private Dictionary<string, string> LoadUsers()
        {
            Dictionary<string, string> loadedUsers = new Dictionary<string, string>();

            if (File.Exists("C:\\project\\vs_community\\OMIS\\lab4-6\\users.txt"))
            {
                string[] lines = File.ReadAllLines("C:\\project\\vs_community\\OMIS\\lab4-6\\users.txt");
                foreach (var line in lines)
                {
                    string[] parts = line.Split(':');
                    if (parts.Length == 2)
                    {
                        loadedUsers[parts[0]] = parts[1];
                    }
                }
            }

            return loadedUsers;
        }

        private void SaveUsers()
        {
            File.WriteAllLines("users.txt", users.Select(user => $"{user.Key}:{user.Value}"));
        }

        private void RegistrationForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            SaveUsers();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string username = textBoxUsername.Text;
            string password = textBoxPassword.Text;

            if (string.IsNullOrWhiteSpace(username) || string.IsNullOrWhiteSpace(password))
            {
                MessageBox.Show("Введите имя пользователя и пароль", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (users.ContainsKey(username))
            {
                MessageBox.Show("Пользователь с таким именем уже существует", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // Просто сохраняем пароль в чистом виде (в реальном приложении это не безопасно!)
            users.Add(username, password);

            MessageBox.Show("Регистрация успешна!", "Успех", MessageBoxButtons.OK, MessageBoxIcon.Information);
            MainFrom mainFrom = new MainFrom();
            mainFrom.ShowDialog();
            Close();
        }

        private void RegistrationForm_Load(object sender, EventArgs e)
        {

        }
    }
}
