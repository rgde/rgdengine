using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.ComponentModel;

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
        RectangleF rect;
        bool visible = true;
        Object tag = null;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }
        
        [Browsable(false)]
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

        [Browsable(false)]
        public Object Tag
        {
            get {return tag;}
            set {tag = value;}
        }

        public RectangleF Rectangle
        {
            get { return rect; }
            set { rect = value; }
        }

        public enum DrawMode
        {
            Normal,
            Hovered,
            Selected
        };

        private void DrawRectangle(System.Drawing.Graphics g, Pen pen)
        {
            g.DrawRectangle(pen, rect.X, rect.Y, rect.Width, rect.Height);
        }

        public void Draw(System.Drawing.Graphics g, DrawMode mode, Image image)
        {
            if (!Visible)
                return;         

            RectangleF frect = rect;

            int pen_width = mode == DrawMode.Hovered ? 2 : 1;

            if (mode == DrawMode.Selected)
            {
                g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceOver;
                g.FillRectangle(new SolidBrush(Color.FromArgb(60, Color.Red)), frect);

                g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                DrawRectangle(g, new Pen(Color.Red, pen_width));
            }
            else
            {
                Color color = Color.RoyalBlue;
                DrawRectangle(g, new Pen(color, pen_width));
            }            
        }

        public void Draw(System.Drawing.Graphics g, float scale, DrawMode mode, Image image)
        {
            Draw(g, mode, image);
        }

        public static RectangleF[] GetSelectionRectangles(RectangleF r)
        {
            float pos_x = r.X;
            float pos_y = r.Y;
            float width = r.Width;
            float height = r.Height;

            const float half_size = 5.0f;
            const float full_size = half_size * 2;

            RectangleF r1 = new RectangleF(pos_x - half_size, pos_y - half_size, full_size, full_size);
            RectangleF r2 = new RectangleF(pos_x - half_size + width, pos_y - half_size, full_size, full_size);
            RectangleF r3 = new RectangleF(pos_x - half_size + width, pos_y + height - half_size, full_size, full_size);
            RectangleF r4 = new RectangleF(pos_x - half_size, pos_y + height - half_size, full_size, full_size);

            RectangleF[] rectangles = new RectangleF[4];
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
