using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace ImageGrid
{
    public partial class CellGrid : UserControl
    {
        float cell_size = 1024.0f;

        const float max_scale = 0.1f;
        const float min_scale = 10.0f;

        float current_scale = 0.025f;

        const int max_bitmaps = 4;

        Bitmap thumbnail = new Bitmap(2048, 2048, System.Drawing.Imaging.PixelFormat.Format32bppPArgb);

        PointF view_pos = new PointF(1000.0f, 1000.0f);

        Size grid_dimentions = new Size(16, 16);

        Image[] images = null;

        public Size GridDimentions
        {
            get { return grid_dimentions; }
            set 
            {
                if (grid_dimentions != value)
                {
                    grid_dimentions = value;

                    if (images != null)
                        lock (images.SyncRoot)
                        {
                            images = null;
                        }

                    images = new Image[grid_dimentions.Height * grid_dimentions.Width];
                }
            }
        }

        public Image GetImage(int x, int y)
        {
            try
            {
                lock(images.SyncRoot)
                {
                    int index = x + y * GridDimentions.Width;
                    return images[index];
                }
            }
            catch
            {
                return null;
            }
        }

        public void SetImage(int x, int y, Image image)
        {
            try
            {
                lock(images.SyncRoot)
                {
                    int index = x + y * GridDimentions.Width;
                    images[index] = image;
                }
            }
            catch {}
        }

        float CellSize
        {
            get { return cell_size; }
            set { cell_size = value; }
        }

        public CellGrid()
        {
            InitializeComponent();
            this.DoubleBuffered = true;
        }

        bool mouse_captured = false;
        Point old_mouse_pos = new Point(0, 0);

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);

            if (e.Button == MouseButtons.Left)
            {
                mouse_captured = true;
                old_mouse_pos = e.Location;
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);

            if (e.Button == MouseButtons.Left)
            {
                mouse_captured = false;
            }
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (mouse_captured)
            {
                int mdx = e.Location.X - old_mouse_pos.X;
                int mdy = e.Location.Y - old_mouse_pos.Y;
                old_mouse_pos = e.Location;

                view_pos.X += mdx / current_scale;
                view_pos.Y += mdy / current_scale;
              
                Invalidate();
            }
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);

            float delta_scale = current_scale * 0.1f * e.Delta / 120.0f;

            float dp_scale = 1.0f / (current_scale + delta_scale) - 1.0f / current_scale;

            current_scale += delta_scale;

            float dx = e.Location.X * dp_scale;
            float dy = e.Location.Y * dp_scale;

            view_pos.X += dx;
            view_pos.Y += dy;

            Invalidate();
        }

        protected override void  OnMouseLeave(EventArgs e)
        {
 	        base.OnMouseLeave(e);
            mouse_captured = false;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics g = e.Graphics;

            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Low;
            g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
            g.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighSpeed;
            g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;
            
            g.FillRectangle(System.Drawing.Brushes.Beige, this.ClientRectangle);
            g.DrawRectangle(System.Drawing.Pens.DarkKhaki, this.ClientRectangle);

            DrawGrid(g);
        }

        void DrawGrid(Graphics g)
        {
            g.ScaleTransform(current_scale, current_scale);
            g.TranslateTransform(view_pos.X, view_pos.Y);

            g.FillRectangle(System.Drawing.Brushes.DarkOrange, 
                0.0f, 
                0.0f,
                cell_size * grid_dimentions.Width,
                cell_size * grid_dimentions.Height);

            for (int x = 0; x < grid_dimentions.Width; ++x)
            {
                for (int y = 0; y < grid_dimentions.Height; ++y)
                {
                    DrawCell(g, x * cell_size, y * cell_size, cell_size, x + y * grid_dimentions.Width);
                }
            }

            g.TranslateTransform(0, 0);
            g.ScaleTransform(1.0f, 1.0f);
        }

        void DrawCell(Graphics g, float x, float y, float size, int image_index)
        {
            Image image = null;

            try
            {
                if (images != null)
                    lock (images.SyncRoot)
                    {
                        image = images[image_index];
                    }                    
            }
            catch{}
            

            if (image != null)
            {
                try
                {
                    g.DrawImage(image, x, y, size, size);
                }
                catch {}
            }
            else
            {
                g.FillRectangle(System.Drawing.Brushes.DarkGray, x, y, size, size);
            }

            g.DrawRectangle(System.Drawing.Pens.Gray, x, y, size, size);
        }
    }
}
