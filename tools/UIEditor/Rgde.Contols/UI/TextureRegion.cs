using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace Rgde.Contols.UI
{
    public class TextureRegion
    {
        public enum RectParts
        {
            Body,
            LeftTopSizer,
            RightTopSizer,
            RightDownSizer,
            LeftDownSizer,
            None
        };

        public TextureRegion()
        {

        }

        public TextureRegion(TextureRegion src)
        {
            group_name = src.group_name;
            name = src.name;
            visible = true;
            tag = null;
            rect = src.rect;
        }

        string group_name = "Root";
        string name;
        Rectangle rect;
        bool visible = true;
        Object tag = null;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public string GroupName
        {
            get { return group_name; }
            set { group_name = value; }
        }

        public bool Visible
        {
            get { return visible; }
            set { visible = value; }
        }

        public Object Tag
        {
            get {return tag;}
            set {tag = value;}
        }

        public Rectangle Rectangle
        {
            get { return rect; }
            set { rect = value; }
        }

        public Rectangle GetRect(float scale)
        {
            return GetRect(0, 0, scale);
        }

        public Rectangle GetRect(float x, float y, float scale)
        {
            int pos_x = (int)((rect.X - x) * scale);
            int pos_y = (int)((rect.Y - y) * scale);
            int width = (int)(rect.Width * scale);
            int height = (int)(rect.Height * scale);
            Rectangle frect = new Rectangle(pos_x, pos_y, width, height);
            return frect;
        }

        public enum DrawMode
        {
            Normal,
            Hovered,
            Selected
        };

        public void Draw(System.Drawing.Graphics g, float scale, float x, float y, DrawMode mode)
        {
            Rectangle frect = GetRect(x, y, scale);

            g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

            int pen_width = mode == DrawMode.Hovered || mode == DrawMode.Selected ? 3 : 1;

            if (mode == DrawMode.Selected)
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

        public void Draw(System.Drawing.Graphics g, float scale, DrawMode mode)
        {
            Draw(g, scale, 0, 0, mode);
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

            const int half_size = 5;
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
    }
}
