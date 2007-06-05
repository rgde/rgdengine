using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Rgde.Contols
{
    public partial class LayoutEditor : UserControl
    {
        Image m_image = null;
        Rectangle m_visible_rect = new Rectangle();
        float m_scale = 1.0f;

        public LayoutEditor()
        {
            this.DoubleBuffered = true;
            InitializeComponent();
        }

        int old_x = 0;
        int old_y = 0;

        protected override void AdjustFormScrollbars(bool displayScrollbars)
        {
            HorizontalScroll.Value = m_visible_rect.X;
            VerticalScroll.Value = m_visible_rect.Y;
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            if (e.Button == MouseButtons.Right)
            {
                Cursor = Cursors.Hand;
                old_x = e.X;
                old_y = e.Y;
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            Cursor = Cursors.Arrow;
            old_x = 0;
            old_y = 0;
        }

        void ClampRect()
        {
            m_visible_rect.X = m_visible_rect.X < 0 ? 0 : m_visible_rect.X;
            m_visible_rect.Y = m_visible_rect.Y < 0 ? 0 : m_visible_rect.Y;

            int max_x = m_visible_rect.X + m_visible_rect.Width;
            m_visible_rect.X = max_x > m_image.Width ? m_image.Width - m_visible_rect.Width : m_visible_rect.X;

            int max_y = m_visible_rect.Y + m_visible_rect.Height;
            m_visible_rect.Y = max_y > m_image.Height ? m_image.Height - m_visible_rect.Height : m_visible_rect.Y;
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                int delta_x = e.X - old_x;
                int delta_y = e.Y - old_y;

                old_x = e.X;
                old_y = e.Y;

                m_visible_rect.X -= (int)(delta_x*m_scale);
                m_visible_rect.Y -= (int)(delta_y*m_scale);

                ClampRect();
                
                UpdateScroolBarsVisibility();
                Invalidate();
            }
            
            base.OnMouseMove(e);
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);

            m_scale += e.Delta / 2000.0f;
            m_scale = m_scale < 0.05f ? 0.05f : m_scale; //min scale
            //m_scale = m_scale > 1.0f ? 1.0f : m_scale;

            m_visible_rect.Height = (int)(m_scale*ClientRectangle.Height);
            m_visible_rect.Width = (int)(m_scale*ClientRectangle.Width);

            if (m_visible_rect.Height > m_image.Height || m_visible_rect.Width > m_image.Width)
            {
                if (m_visible_rect.Height > m_visible_rect.Width)
                {
                    m_visible_rect.Height = m_image.Height;
                    m_scale = (1.0f * m_visible_rect.Height) / ClientRectangle.Height;
                    m_visible_rect.Width = (int)(m_scale * ClientRectangle.Width);
                }
                else
                {
                    m_visible_rect.Width = m_image.Width;
                    m_scale = (1.0f * m_visible_rect.Width) / ClientRectangle.Width;
                    m_visible_rect.Height = (int)(m_scale * ClientRectangle.Height);
                }
            }

            ClampRect();

            UpdateScroolBarsVisibility();
            Invalidate();
        }

        protected override void OnScroll(ScrollEventArgs se)
        {
            //base.OnScroll(se);

            bool horiz = se.ScrollOrientation == ScrollOrientation.HorizontalScroll;

            int cur_value = horiz ? m_visible_rect.X : m_visible_rect.Y;

            switch(se.Type)
            {
                case ScrollEventType.SmallDecrement:
                case ScrollEventType.SmallIncrement:
                case ScrollEventType.LargeDecrement:
                case ScrollEventType.LargeIncrement:
                    cur_value += se.NewValue;
                    break;

                case ScrollEventType.ThumbPosition:
                case ScrollEventType.ThumbTrack:
                    cur_value = se.NewValue;
                    break;

                case ScrollEventType.First:
                    cur_value = 0;
                    break;

                case ScrollEventType.Last:
                    cur_value = horiz ? m_image.Width - m_visible_rect.Width : m_image.Height - m_visible_rect.Height;
                    break;
            }

            if (horiz)
            {
                m_visible_rect.X = cur_value;
            }
            else
            {
                m_visible_rect.Y = cur_value;
            }

            ClampRect();
            HorizontalScroll.Value = m_visible_rect.X;
            VerticalScroll.Value = m_visible_rect.Y;
            Invalidate();
        }

        public float Scale
        {
            get { return m_scale; }
            set 
            {
                //OnScaleChanged(m_scale);
                m_scale = value; 
            }
        }

        public Image Image
        {
            get { return m_image; }
            set 
            { 
                m_image = value;
                m_visible_rect = ClientRectangle;
                UpdateScroolBarsVisibility();
            }
        }

        private void UpdateScroolBarsVisibility()
        {
            if (m_image == null)
            {
                this.VerticalScroll.Visible = false;
                this.HorizontalScroll.Visible = false;
            }
            else
            {
                if (m_visible_rect.Width < m_image.Width)
                {
                    HorizontalScroll.Visible = true;
                    HorizontalScroll.Minimum = 0;
                    
                    HorizontalScroll.LargeChange = m_visible_rect.Width;
                    HorizontalScroll.Maximum = m_image.Width;
                    HorizontalScroll.SmallChange = 1;
                    HorizontalScroll.Value = m_visible_rect.Left;
                }
                else
                    HorizontalScroll.Visible = false;

                if (m_visible_rect.Height < m_image.Height)
                {
                    VerticalScroll.Visible = true;
                    VerticalScroll.Minimum = 0;
                    VerticalScroll.SmallChange = 1;
                    
                    VerticalScroll.LargeChange = m_visible_rect.Height;
                    VerticalScroll.Maximum = m_image.Height;
                    VerticalScroll.Value = m_visible_rect.Top;
                }
                else
                    VerticalScroll.Visible = false;
            }
        }


        protected override void OnPaint(PaintEventArgs e)
        {
            //ClampRect();
            HorizontalScroll.Value = m_visible_rect.X;
            VerticalScroll.Value = m_visible_rect.Y;

            //base.OnPaint(e);
            e.Graphics.DrawRectangle(Pens.Red, ClientRectangle);
            if (m_image != null)
            {
                e.Graphics.InterpolationMode = InterpolationMode.NearestNeighbor;//.
                e.Graphics.DrawImage(m_image, ClientRectangle, m_visible_rect, GraphicsUnit.Pixel);
            }
        }
    }
}