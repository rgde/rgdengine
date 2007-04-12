using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace UIEditor
{
    public partial class Form1 : Form
    {
        public class TextureRegion
        {
            public string name;
            public Rectangle rect;

            public override string ToString()
            {
                return name;
            }
        };

        enum RectParts
        {
            Body,
            LeftTopSizer,
            RightTopSizer,
            RightDownSizer,
            LeftDownSizer,
            None
        };

        class AppState
        {
            public enum Action
            {
                None,
                Moving,
                Resizing
            };

            public Action action = Action.None;
            public RectParts rect_part = RectParts.None;
        };

        AppState m_state = new AppState();

        Bitmap bitmap = null;

        public Form1()
        {
            InitializeComponent();
            listBox1.Items.Clear();

            TextureRegion r1 = new TextureRegion();
            r1.rect = new Rectangle(10, 10, 30, 30);
            r1.name = "Button.Bkg";
            listBox1.Items.Add(r1);            

            TextureRegion r2 = new TextureRegion();
            r2.rect = new Rectangle(50, 10, 30, 30);
            r2.name = "Button.Pressed";
            listBox1.Items.Add(r2);

            TextureRegion r3 = new TextureRegion();
            r3.rect = new Rectangle(50, 50, 80, 30);
            r3.name = "Button.Hover";
            listBox1.Items.Add(r3);

            this.DoubleBuffered = true;

            pictureBox1.MouseWheel += new MouseEventHandler(pictureBox1_MouseWheel);
            pictureBox1.MouseEnter += new EventHandler(pictureBox1_MouseEnter);
        }

        void pictureBox1_MouseEnter(object sender, EventArgs e)
        {
            pictureBox1.Focus();            
        }

        float scale = 1.0f;

        void pictureBox1_MouseWheel(object sender, MouseEventArgs e)
        {
            scale += e.Delta / 10000.0f;
            pictureBox1.Invalidate();
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                if (listBox1.SelectedItem != null)
                {
                    TextureRegion r = (TextureRegion)listBox1.SelectedItem;
                    regionName.Text = r.name;
                }
                else
                {
                    regionName.Text = "";
                }
            }
            catch
            {
                regionName.Text = "";
            }

            pictureBox1.Invalidate();
        }


        private void openToolStripButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            if (DialogResult.OK == ofd.ShowDialog(this))
            {
                bitmap = new Bitmap(ofd.FileName);
                pictureBox1.Invalidate();
            }
        }

        int mouse_x = 0;
        int mouse_y = 0;
        int old_hovered_item = -1;

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            //e.Graphics

            if (bitmap != null)
                e.Graphics.DrawImage(bitmap, new RectangleF(0, 0, bitmap.Width * scale, bitmap.Height * scale));

            DrawBoxes(e.Graphics);

            //e.Graphics = gr;
            //e.Graphics.DrawRectangle(new Pen(Color.Green, 1), mouse_x, mouse_y, 10,10);
        }

        Rectangle GetBoxRect(int i)
        {
            TextureRegion r = (TextureRegion)listBox1.Items[i];
            int pos_x = (int)(r.rect.X * scale);
            int pos_y = (int)(r.rect.Y * scale);
            int width = (int)(r.rect.Width * scale);
            int height = (int)(r.rect.Height * scale);
            Rectangle frect = new Rectangle(pos_x, pos_y, width, height);
            return frect;
        }

        bool IsInsideRect(Rectangle r, int x, int y)
        {
            return r.Contains(new Point(x, y));            
        }

        bool IsMouseInside(Rectangle r)
        {
            return IsInsideRect(r, mouse_x, mouse_y);
        }



        bool IsMouseOverSelectedBox()
        {
            try
            {
                SetCursor(mouse_x, mouse_y);
            }
            catch
            {
            }

            return false;
        }

        Rectangle[] GetSelectionRectangles(Rectangle r)
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

        private void DrawBoxes(System.Drawing.Graphics g)
        {
            for (int i = 0; i < listBox1.Items.Count; ++i)
            {
                try
                {
                    TextureRegion r = (TextureRegion)listBox1.Items[i];
                    int pos_x = (int)(r.rect.X * scale);
                    int pos_y = (int)(r.rect.Y * scale);
                    int width = (int)(r.rect.Width * scale);
                    int height = (int)(r.rect.Height * scale);
                    Rectangle frect = new Rectangle(pos_x, pos_y, width, height);

                    g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                    g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

                    int pen_width = (old_hovered_item == i) ? 3 : 1;

                    if (i == listBox1.SelectedIndex)
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
                catch
                {

                }
            }
        }

        void UpdateSelection(int dx, int dy, int dwidth, int dheight)
        {
            TextureRegion r = listBox1.SelectedItem as TextureRegion;
            if (MouseButtons == MouseButtons.Left)
            {
                r.rect.X += dx;
                r.rect.Y += dy;
                r.rect.Width += dwidth;
                r.rect.Height += dheight;
            }

            pictureBox1.Invalidate();
        }

        void SetCursor(int x, int y)
        {
            try
            {
                if (listBox1.SelectedItem != null)
                {
                    TextureRegion r = (TextureRegion)listBox1.SelectedItem;

                    Rectangle[] rects = GetSelectionRectangles(r.rect);

                    Point mouse_pos = new Point(x, y);

                    if (rects[0].Contains(mouse_pos))
                    {
                        Cursor.Current = Cursors.SizeNWSE;
                    }
                    else if (rects[1].Contains(mouse_pos))
                    {
                        Cursor.Current = Cursors.SizeNESW;                        
                    }
                    else if (rects[2].Contains(mouse_pos))
                    {
                        Cursor.Current = Cursors.SizeNWSE;
                    }
                    else if (rects[3].Contains(mouse_pos))
                    {
                        Cursor.Current = Cursors.SizeNESW;
                    }
                    else if (r.rect.Contains(mouse_pos))
                    {
                        Cursor.Current = Cursors.SizeAll;
                    }
                    else
                        Cursor.Current = Cursors.Default;
                }
            }
            catch
            {
            }
        }


        RectParts TestMouseHover(Rectangle r, int x, int y)
        {
            Rectangle[] rects = GetSelectionRectangles(r);
            Point mouse_pos = new Point(x, y);

            if (rects[0].Contains(mouse_pos))
                return RectParts.LeftTopSizer;

            if (rects[1].Contains(mouse_pos))
                return RectParts.RightTopSizer;

            if (rects[2].Contains(mouse_pos))
                return RectParts.RightDownSizer;

            if (rects[3].Contains(mouse_pos))
                return RectParts.LeftDownSizer;

            if (r.Contains(mouse_pos))
                return RectParts.Body;

            return RectParts.None;
        }

        bool IsOverSelectedRect(int x, int y)
        {
            if (listBox1.SelectedItem != null)
            {
                TextureRegion r = (TextureRegion)listBox1.SelectedItem;

                if (RectParts.None != TestMouseHover(r.rect, mouse_x, mouse_y))
                    return true;
            }
            return false;
        }                

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (listBox1.SelectedItem != null)
            {
                TextureRegion r = (TextureRegion)listBox1.SelectedItem;
                RectParts p = TestMouseHover(r.rect, mouse_x, mouse_y);

                m_state.rect_part = p;

                switch (p)
                {
                    case RectParts.Body:
                        m_state.action = AppState.Action.Moving;
                        return;
                        break;
                    case RectParts.LeftDownSizer:
                    case RectParts.LeftTopSizer:
                    case RectParts.RightDownSizer:
                    case RectParts.RightTopSizer:
                        m_state.action = AppState.Action.Resizing;
                        return;
                        break;
                }
            }

            if (old_hovered_item != -1)
            {
                if (listBox1.SelectedIndex != old_hovered_item)
                {
                    listBox1.SelectedIndex = old_hovered_item;
                    pictureBox1.Invalidate();
                    return;
                }
            }
        }


        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            int dx = e.X - mouse_x;
            int dy = e.Y - mouse_y;

            mouse_x = e.X;
            mouse_y = e.Y;

            if (m_state.action != AppState.Action.None)
            {
                if (m_state.action == AppState.Action.Resizing)
                {
                    switch(m_state.rect_part)
                    {
                        case RectParts.LeftTopSizer:
                            UpdateSelection(dx, dy, -dx, -dy);
                            break;
                        case RectParts.RightTopSizer:
                            UpdateSelection(0, dy, dx, -dy);
                            break;
                        case RectParts.RightDownSizer:
                            UpdateSelection(0, 0, dx, dy);
                            break;
                        case RectParts.LeftDownSizer:
                            UpdateSelection(dx, 0, -dx, dy);
                            break;
                    }
                }
                else if (m_state.action == AppState.Action.Moving)
                {
                    UpdateSelection(dx, dy, 0, 0);
                }
                return;
            }

            IsMouseOverSelectedBox();

            for (int i = 0; i < listBox1.Items.Count; ++i)
            {
                try
                {
                    TextureRegion r = (TextureRegion)listBox1.Items[i];
                    int pos_x = (int)(r.rect.X * scale);
                    int pos_y = (int)(r.rect.Y * scale);
                    int width = (int)(r.rect.Width * scale);
                    int height = (int)(r.rect.Height * scale);

                    if (r.rect.Contains(new Point(mouse_x, mouse_y)))
                    {
                        if (old_hovered_item != i)
                        {
                            old_hovered_item = i;
                            pictureBox1.Invalidate();

                        }
                        return;
                    }
                }
                catch
                {

                }
            }

            if (old_hovered_item != -1)
            {
                old_hovered_item = -1;
                pictureBox1.Invalidate();
            }
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            m_state.action = AppState.Action.None;
        }

        private void pictureBox1_MouseLeave(object sender, EventArgs e)
        {
            m_state.action = AppState.Action.None;
        }
    }
}