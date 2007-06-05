using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Rgde.Contols.UI
{
    public class TextureRegion
    {
        public string group_name;
        public string name;
        public Rectangle rect;
        public bool visible = true;
        public Object tag = null;
        public bool selected = false;

        public bool IsSeleced() { return selected; }

        public Rectangle GetRect(float scale)
        {
            int pos_x = (int)(rect.X * scale);
            int pos_y = (int)(rect.Y * scale);
            int width = (int)(rect.Width * scale);
            int height = (int)(rect.Height * scale);
            Rectangle frect = new Rectangle(pos_x, pos_y, width, height);
            return frect;
        }

        public void Draw(System.Drawing.Graphics g, float scale, bool hovered)
        {
            Rectangle frect = GetRect(scale);

            g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

            int pen_width = hovered ? 3 : 1;

            if (selected)
            {
                g.DrawRectangle(new Pen(Color.Red, pen_width), frect);

                Rectangle[] rects = GetSelectionRectangles(frect);
                g.FillRectangles(Brushes.Aqua, rects);
            }
            else
            {
                g.DrawRectangle(new Pen(Color.RoyalBlue, pen_width), frect);
            }
        }

        public bool IsMouseOver()
        {
            int x = Cursor.Position.X;
            return false;
        }

        public static Rectangle[] GetSelectionRectangles(Rectangle r)
        {
            int pos_x = r.X;
            int pos_y = r.Y;
            int width = r.Width;
            int height = r.Height;

            const int half_size = 6;
            const int full_size = half_size * 2;

            Rectangle r1 = new Rectangle(pos_x - half_size, pos_y - half_size, full_size, full_size);
            Rectangle r2 = new Rectangle(pos_x - half_size + width, pos_y - half_size, full_size, full_size);
            Rectangle r3 = new Rectangle(pos_x - half_size + width, pos_y + height - half_size, full_size, full_size);
            Rectangle r4 = new Rectangle(pos_x - half_size, pos_y + height - half_size, full_size, full_size);

            Rectangle[] rectangles = new Rectangle[4];
            rectangles[0] = r1;
            rectangles[1] = r2;
            rectangles[2] = r3;
            rectangles[3] = r4;
            return rectangles;
        }


        public override string ToString()
        {
            return name;
        }

        public string GroupName
        {
            get { return group_name; }
            set { group_name = value; }
        }

        public Rectangle Rectangle
        {
            get { return rect; }
            set { rect = value; }
        }

        public string Name
        {
            get { return name; }
            set 
            { 
                name = value;
                
                if (tag != null)
                {
                    TreeNode owner = tag as TreeNode;
                    owner.Text = value;
                }
            }
        }
    }
}
