using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FSM_ANT_II
{
    public partial class Form1 : Form
    {
        private int SizeFiguru { get; set; }
        private int WidthSpace { get; set; }
        private int HeightSpace { get; set; }
        private Ant _ant;
        private Image _antImage;
        private Image _leafImage;
        private int _interval;
        public Form1()
        {
            InitializeComponent();
            Init();
            DoubleBuffered = true;
        }

        private void Init()
        {
            SizeFiguru = 20;
            WidthSpace = 20;
            HeightSpace = 20;
            _ant = new Ant(WidthSpace, HeightSpace, Invalidate);
            _antImage = Properties.Resources.ant;
            _leafImage = Properties.Resources.leaf;
            _interval = 50;
            timer1.Interval = _interval;
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            _ant.Mouse = new Point(e.X / SizeFiguru, e.Y / SizeFiguru);
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics graphics = e.Graphics;
            Image thumb = _antImage.GetThumbnailImage(SizeFiguru, SizeFiguru, null, IntPtr.Zero);
            thumb = RotateImage((Bitmap)thumb, _ant.Rotation);
            graphics.DrawImage(thumb, _ant.Position.X * SizeFiguru, _ant.Position.Y * SizeFiguru);

            //Image thumb1 = _leafImage.GetThumbnailImage(SizeFiguru, SizeFiguru, null, IntPtr.Zero);
            //thumb1 = RotateImage((Bitmap)thumb1, _ant.Rotation);
            //graphics.DrawImage(thumb1, _ant.Position.X * SizeFiguru, _ant.Position.Y * SizeFiguru);

            graphics.FillEllipse(Brushes.GreenYellow, _ant.Leaf.X * SizeFiguru, _ant.Leaf.Y * SizeFiguru, SizeFiguru, SizeFiguru);
            graphics.FillEllipse(Brushes.Brown, _ant.Home.X * SizeFiguru, _ant.Home.Y * SizeFiguru, SizeFiguru, SizeFiguru);
            graphics.DrawRectangle(Pens.Black, 1, 1, WidthSpace * SizeFiguru, HeightSpace * SizeFiguru);

            ShowInfo();
        }

        private void ShowInfo()
        {
            //var msg =
            //            $"Position {_ant.Position.X} {_ant.Position.Y}\n" +
            //            $"Mouse    {_ant.Mouse.X} {_ant.Mouse.Y} \n" +
            //            $"Leaf Green {_ant.Leaf.X} {_ant.Leaf.Y} \n" +
            //            $"Home Brown {_ant.Home.X} {_ant.Home.Y} \n" +
            //            $"Velocity  {_ant.Velocity.X} {_ant.Velocity.Y} \n" +
            //            $"Rotation  {_ant.Rotation}\n";
                        //+$"StateName {_ant.GetStateName()}";
            //label1.Text = msg;
        }
        private Bitmap RotateImage(Bitmap bmp, float angle)
        {
            Bitmap rotatedImage = new Bitmap(bmp.Width, bmp.Height);
            using (Graphics g = Graphics.FromImage(rotatedImage))
            {
                // Set the rotation point to the center in the matrix
                g.TranslateTransform(bmp.Width / 2, bmp.Height / 2);
                // Rotate
                g.RotateTransform(angle);
                // Restore rotation point in the matrix
                g.TranslateTransform(-bmp.Width / 2, -bmp.Height / 2);
                // Draw the image on the bitmap
                g.DrawImage(bmp, new Point(0, 0));
            }
            return rotatedImage;
        }

        private void Timer1_Tick(object sender, EventArgs e)
        {
            _ant.Update();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            timer1.Interval += _interval;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (timer1.Interval > _interval)
                timer1.Interval -= _interval;
            else if (timer1.Interval > 5)
                timer1.Interval -= 5;
        }
    }
}