using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FSM_ANT_II
{
    public class Ant
    {
        private int _x;
        private int _y;
        private readonly int Distanse = 1;
        private readonly int MouseThreatRadius = 5;
        private readonly int MaxVelocity = 1;


        public Point Position { get; private set; }
        public Point Velocity { get; private set; }
        public FSM Brain { get; private set; }
        public Point Leaf { get; private set; }
        public Point Mouse { get; set; }
        public Point Home { get; private set; }
        public float Rotation { get; private set; }




        public Action Paint;



        public Ant(int x, int y, Action paint)
        {
            var rand = new Random();
            Position = new Point(rand.Next(x), rand.Next(y));
            Velocity = new Point(-x, -y);
            Brain = new FSM();
            Leaf = new Point(rand.Next(x), rand.Next(y));
            Home = new Point(rand.Next(x), rand.Next(y));
            Mouse = new Point(-x, -y);
            _x = x;
            _y = y;
            Paint = paint;
            Paint.Invoke();
            Brain.setState(findLeaf);
        }

        private int Distance(Point p1, Point p2)
        {
            double dx = p1.X - p2.X;
            double dy = p1.Y - p2.Y;
            int res = Convert.ToInt32(Math.Sqrt(dx * dx + dy * dy));
            return res;
        }

        public void findLeaf()
        {
            Velocity = new Point(Leaf.X - Position.X, Leaf.Y - Position.Y);

            // Муравей тольо что подобрал листок
            if (Distance(Leaf, Position) <= Distanse)
            {
                var r = new Random();
                Leaf = new Point(r.Next(_x), r.Next(_y));
                Brain.setState(goHome);
            }
            if (Distance(Mouse, Position) < MouseThreatRadius)
            {
                Brain.setState(runAway);
            }
        }

        public void goHome() {
            Velocity = new Point(Home.X - Position.X, Home.Y - Position.Y);

            if (Distance(Home, Position) <= Distanse)
            {
                // Муравей уже дома.Пора искать новый лист.
                Brain.setState(findLeaf);
            }
            if (Distance(Mouse, Position) < MouseThreatRadius)
            {
                Brain.setState(runAway);
            }
        }
        public void runAway() 
        {
            // Перемещаем муравья подальше от курсора
            Velocity = new Point(Position.X - Mouse.X, Position.Y - Mouse.Y);
            if (Distance(Mouse, Position) > MouseThreatRadius)
            {
                Brain.setState(findLeaf);
            }

        }

        public void Update()
        {
            Brain.update();
            Paint.Invoke(); 
            // Примените вектор скорости к положению, сделав движение муравья.
            MoveBasedOnVelocity();
        }

        private void MoveBasedOnVelocity()
        {
            // Truncate
            double scalar;
            scalar = MaxVelocity / LengthVector(Velocity);
            scalar = scalar < 1.0 ? scalar : 1.0;

            // ScaleBy
            var x = Convert.ToInt32(Velocity.X * scalar);
            var y = Convert.ToInt32(Velocity.Y * scalar);
            Velocity = new Point(x, y);

            // Vector Add
            Position = new Point(Position.X + Velocity.X, Position.Y + Velocity.Y);

            Rotation = (float)(90 + (180 * GetAngle(Velocity)) / Math.PI);
        }

        private double GetAngle(Point p) => Math.Atan2(p.Y, p.X);

        private double LengthVector(Point p) => Math.Sqrt(p.X * p.X + p.Y * p.Y);


    }
}
