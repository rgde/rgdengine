using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Rgde.Contols
{
    public delegate void TextureRegionEventDelegate(UI.TextureRegion reg);
    
    public partial class LayoutEditor : UserControl
    {
        public delegate void LayoutEditorEventDelegate(object sender);

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

        private AppState m_state = new AppState();
        private Image m_image = null;
        private Rectangle m_visible_rect = new Rectangle();
        private float m_scale = 1.0f;
        private int old_x = 0;
        private int old_y = 0;

        private List<UI.TextureRegion> regions = new List<UI.TextureRegion>();
        private List<UI.TextureRegion> hovered_regions = new List<UI.TextureRegion>();
        private List<UI.TextureRegion> selected_regions = new List<UI.TextureRegion>();

        public LayoutEditor()
        {
            this.BorderStyle = BorderStyle.FixedSingle;
            this.DoubleBuffered = true;
            InitializeComponent();

            this.Load += new EventHandler(LayoutEditor_Load);
        }

        void LayoutEditor_Load(object sender, EventArgs e)
        {
            UI.TextureRegion r1 = new UI.TextureRegion();
            r1.Rectangle = new Rectangle(10, 10, 30, 30);
            r1.Name = "Button.Bkg";


            UI.TextureRegion r2 = new UI.TextureRegion();
            r2.Rectangle = new Rectangle(50, 10, 30, 30);
            r2.Name = "Button.Pressed";


            UI.TextureRegion r3 = new UI.TextureRegion();
            r3.Rectangle = new Rectangle(50, 50, 80, 30);
            r3.Name = "Button.Hover";

            AddTextureRegion(r1);
            AddTextureRegion(r2);
            AddTextureRegion(r3);
        }

        #region Public Properties

        public UI.TextureRegion SelectedRegion
        {
            get
            {
                return selected_regions.Count == 0 ?
                           null
                           :
                           selected_regions[0];
            }
        }

        public UI.TextureRegion HoveredRegion
        {
            get
            {
                return hovered_regions.Count == 0 ?
                           null
                           :
                           hovered_regions[0];
            }
        }

        public  List<UI.TextureRegion> Regions
        {
            get { return regions; }
            set { regions = value; }
        }

        public float Scale
        {
            get { return m_scale; }
            set
            {
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

        #endregion

        public event TextureRegionEventDelegate OnAddRegion;
        public event TextureRegionEventDelegate OnDeleteRegion;
        public event TextureRegionEventDelegate OnSelectRegion;
        public event LayoutEditorEventDelegate OnSelectionChange;

        public void DeleteTextureRegion(UI.TextureRegion reg)
        {
            if (null == reg)
                return;

            regions.Remove(reg);
            ClearSelection();
            if (OnDeleteRegion != null)
                OnDeleteRegion(reg);
        }

        public void AddTextureRegion(UI.TextureRegion reg)
        {
            if (null == reg)
                return;

            regions.Add(reg);

            if (OnAddRegion != null)
                OnAddRegion(reg);
        }

        public void SelectRegion(UI.TextureRegion reg)
        {
            if (null == reg)
                return;

            ClearSelection();
            AddToSelection(reg);
        }

        public void AddToSelection(UI.TextureRegion reg)
        {
            if (null == reg)
                return;

            selected_regions.Add(reg);

            if (null != OnSelectRegion)
                OnSelectRegion(reg);
        }

        public void ClearSelection()
        {
            selected_regions.Clear();

            if (null != OnSelectionChange)
                OnSelectionChange(this);
        }

        public void AddTextureRegion(UI.TextureRegion reg, bool select)
        {
            AddTextureRegion(reg);
            if (select)
            {
                SelectRegion(reg);                
            }
        }

        #region Parent control override
        protected override void OnMouseMove(MouseEventArgs e)
        {
            int dx = (int)((e.X - old_x) / m_scale);
            int dy = (int)((e.Y - old_y) / m_scale);

            int delta_x = e.X - old_x;
            int delta_y = e.Y - old_y;

            old_x = e.X;
            old_y = e.Y;

            bool need_to_redraw = hovered_regions.Count > 0;
            hovered_regions.Clear();

            if (e.Button == MouseButtons.Right && null != m_image)
            {
                m_visible_rect.X -= (int)(delta_x / m_scale);
                m_visible_rect.Y -= (int)(delta_y / m_scale);

                ClampRect();

                UpdateScroolBarsVisibility();
                need_to_redraw = true;
            }

            base.OnMouseMove(e);

            int mouse_x = e.X;
            int mouse_y = e.Y;

            float x = (float)m_visible_rect.X;
            float y = (float)m_visible_rect.Y;

            if (e.Button == MouseButtons.None)
            {
                foreach (UI.TextureRegion r in regions)
                {
                    if (r.GetRect(x, y, m_scale).Contains(new Point(mouse_x, mouse_y)))
                    {
                        hovered_regions.Add(r);
                    }
                }

                if (hovered_regions.Count > 0)
                    need_to_redraw = true;

                SetCursor(mouse_x, mouse_y);
            }

            if (m_state.action != AppState.Action.None)
            {
                if (m_state.action == AppState.Action.Resizing)
                {
                    switch (m_state.rect_part)
                    {
                        case RectParts.LeftTopSizer:
                            UpdateRegion(dx, dy, -dx, -dy);
                            need_to_redraw = true;
                            break;
                        case RectParts.RightTopSizer:
                            UpdateRegion(0, dy, dx, -dy);
                            need_to_redraw = true;
                            break;
                        case RectParts.RightDownSizer:
                            UpdateRegion(0, 0, dx, dy);
                            need_to_redraw = true;
                            break;
                        case RectParts.LeftDownSizer:
                            UpdateRegion(dx, 0, -dx, dy);
                            need_to_redraw = true;
                            break;
                    }
                }
                else if (m_state.action == AppState.Action.Moving)
                {
                    UpdateRegion(dx, dy, 0, 0);
                    need_to_redraw = true;
                }
            }

            if (need_to_redraw)
                Invalidate();
        }

        bool control_key = false;
        bool shift_key = false;

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);

            if (e.KeyCode == Keys.Delete && selected_regions.Count > 0)
            {
                hovered_regions.Clear();

                List<UI.TextureRegion> old_selected_regions = new List<UI.TextureRegion>(selected_regions);

                foreach (UI.TextureRegion reg in old_selected_regions)
                {
                    DeleteTextureRegion(reg);
                }

                Invalidate();

                return;
            }

            control_key = e.Control;
            shift_key = e.Shift;
        }

        protected override void OnKeyUp(KeyEventArgs e)
        {
            base.OnKeyUp(e);

            if (control_key)
                control_key = e.Control;

            if (shift_key)
                shift_key = e.Shift;
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);

            bool need_to_redraw = selected_regions.Count > 0;

            int mouse_x = e.X;
            int mouse_y = e.Y;

            float x = (float)m_visible_rect.X;
            float y = (float)m_visible_rect.Y;

            if (e.Button == MouseButtons.Right)
            {
                Cursor = Cursors.Hand;
                old_x = e.X;
                old_y = e.Y;
            }
            else if (e.Button == MouseButtons.Left)
            {
                if (shift_key)
                {
                    foreach (UI.TextureRegion r in regions)
                    {
                        if (r.GetRect(x, y, m_scale).Contains(new Point(mouse_x, mouse_y)))
                        {
                            UI.TextureRegion r1 = new UI.TextureRegion(r);
                            r1.Name += "Copy";
                            AddTextureRegion(r1, true);
                            m_state.action = AppState.Action.Moving;
                            m_state.rect_part = RectParts.None;
                            return;
                        }
                    }
                }

                if (control_key)
                {
                    UI.TextureRegion r1 = new UI.TextureRegion();
                    r1.Rectangle = new Rectangle(mouse_x, mouse_y, 4, 4);
                    r1.Name = "New Region";
                    AddTextureRegion(r1, true);
                    m_state.action = AppState.Action.Resizing;
                    m_state.rect_part = RectParts.RightDownSizer;
                    return;
                }

                foreach (UI.TextureRegion r in selected_regions)
                {
                    need_to_redraw = true;

                    RectParts p = TestMouseHover(r, e.X, e.Y);

                    m_state.rect_part = p;

                    switch (p)
                    {
                        case RectParts.Body:
                            m_state.action = AppState.Action.Moving;
                            return;
                        case RectParts.LeftDownSizer:
                        case RectParts.LeftTopSizer:
                        case RectParts.RightDownSizer:
                        case RectParts.RightTopSizer:
                            m_state.action = AppState.Action.Resizing;
                            return;
                    }
                }

                selected_regions.Clear();

                foreach (UI.TextureRegion r in hovered_regions)
                {
                    AddToSelection(r);
                    need_to_redraw = true;

                    RectParts p = TestMouseHover(r, e.X, e.Y);

                    m_state.rect_part = p;

                    switch (p)
                    {
                        case RectParts.Body:
                            m_state.action = AppState.Action.Moving;
                            return;
                        case RectParts.LeftDownSizer:
                        case RectParts.LeftTopSizer:
                        case RectParts.RightDownSizer:
                        case RectParts.RightTopSizer:
                            m_state.action = AppState.Action.Resizing;
                            return;
                    }
                }
            }

            if (need_to_redraw)
                Invalidate();
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            Cursor = Cursors.Arrow;
            old_x = 0;
            old_y = 0;
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);

            if (null == m_image)
                return;

            m_scale += e.Delta / 2000.0f;
            m_scale = m_scale < 0.05f ? 0.05f : m_scale; //min scale

            RecalcVisibleRect();

            if (m_visible_rect.Height > m_image.Height || m_visible_rect.Width > m_image.Width)
            {
                if (m_visible_rect.Height > m_visible_rect.Width)
                {
                    m_visible_rect.Height = m_image.Height;
                    m_scale = (1.0f * m_visible_rect.Height) / ClientRectangle.Height;
                }
                else
                {
                    m_visible_rect.Width = m_image.Width;
                    m_scale = (1.0f * m_visible_rect.Width) / ClientRectangle.Width;
                }
                RecalcVisibleRect();
            }

            ClampRect();

            UpdateScroolBarsVisibility();
            Invalidate();
        }

        protected override void OnScroll(ScrollEventArgs se)
        {
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

        protected override void AdjustFormScrollbars(bool displayScrollbars)
        {
            try
            {
                HorizontalScroll.Value = m_visible_rect.X;
                VerticalScroll.Value = m_visible_rect.Y;
            }
            catch { }
        }
       
        protected override void OnPaint(PaintEventArgs e)
        {
            //m_visible_rect.Width = (int)(ClientRectangle.Width);//*m_scale);
            //m_visible_rect.Height = (int)(ClientRectangle.Height);//*m_scale);
            RecalcVisibleRect();
            
            HorizontalScroll.Value = m_visible_rect.X;
            VerticalScroll.Value = m_visible_rect.Y;

            if (m_image != null)
            {
                e.Graphics.InterpolationMode = InterpolationMode.NearestNeighbor;//.
                e.Graphics.DrawImage(m_image, ClientRectangle, m_visible_rect, GraphicsUnit.Pixel);
            }

            DrawBoxes(e.Graphics);
        }
        #endregion

        #region Private methods
        private void SetCursor(int x, int y)
        {
            try
            {
                float ox = (float)m_visible_rect.X;
                float oy = (float)m_visible_rect.Y;

                foreach (UI.TextureRegion r in selected_regions)
                {
                    Rectangle rect = r.GetRect(ox, oy, m_scale);
                    Rectangle[] rects = UI.TextureRegion.GetSelectionRectangles(rect);

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
                    else if (rect.Contains(mouse_pos))
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

        private void DrawBoxes(System.Drawing.Graphics g)
        {
            float x = (float)m_visible_rect.X;
            float y = (float)m_visible_rect.Y;

            foreach (UI.TextureRegion r in regions)
            {
                r.Draw(g, Scale, x, y, UI.TextureRegion.DrawMode.Normal);
            }

            foreach (UI.TextureRegion r in hovered_regions)
            {
                r.Draw(g, Scale, x, y, UI.TextureRegion.DrawMode.Hovered);
            }

            foreach (UI.TextureRegion r in selected_regions)
            {
                r.Draw(g, Scale, x, y, UI.TextureRegion.DrawMode.Selected);
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

        private void UpdateRegion(int dx, int dy, int dwidth, int dheight)
        {
            foreach (UI.TextureRegion r in selected_regions)
            {
                if (r == null) continue;

                if (MouseButtons == MouseButtons.Left)
                {
                    Rectangle rect = r.Rectangle;
                    rect.X += (int)(dx);
                    rect.Y += (int)(dy);
                    rect.Width += (int)(dwidth);
                    rect.Height += (int)(dheight);
                    
                    rect.Width = rect.Width > 1 ? rect.Width : 1;
                    rect.Height = rect.Height > 1 ? rect.Height : 1;

                    r.Rectangle = rect;
                }
            }
        }

        private RectParts TestMouseHover(UI.TextureRegion r, int x, int y)
        {

            float ox = (float)m_visible_rect.X;
            float oy = (float)m_visible_rect.Y;

            Rectangle rect = r.GetRect(ox, oy, m_scale);
            Rectangle[] rects = UI.TextureRegion.GetSelectionRectangles(rect);

            Point mouse_pos = new Point(x, y);

            if (rects[0].Contains(mouse_pos))
            {
                return RectParts.LeftTopSizer;
            }
            else if (rects[1].Contains(mouse_pos))
            {
                return RectParts.RightTopSizer;
            }
            else if (rects[2].Contains(mouse_pos))
            {
                return RectParts.RightDownSizer;
            }
            else if (rects[3].Contains(mouse_pos))
            {
                return RectParts.LeftDownSizer;
            }
            else if (rect.Contains(mouse_pos))
            {
                return RectParts.Body;
            }
            else
                return RectParts.None;
        }

        private void ClampRect()
        {
            m_visible_rect.X = m_visible_rect.X < 0 ? 0 : m_visible_rect.X;
            m_visible_rect.Y = m_visible_rect.Y < 0 ? 0 : m_visible_rect.Y;

            m_visible_rect.Width = m_visible_rect.Width > m_image.Width ? m_image.Width : m_visible_rect.Width;
            m_visible_rect.Height = m_visible_rect.Height > m_image.Height ? m_image.Height : m_visible_rect.Height;

            int max_x = m_visible_rect.X + m_visible_rect.Width;
            m_visible_rect.X = max_x > m_image.Width ? m_image.Width - m_visible_rect.Width : m_visible_rect.X;

            int max_y = m_visible_rect.Y + m_visible_rect.Height;
            m_visible_rect.Y = max_y > m_image.Height ? m_image.Height - m_visible_rect.Height : m_visible_rect.Y;
        }

        private void RecalcVisibleRect()
        {
            m_visible_rect.Height = (int)(ClientRectangle.Height / m_scale);
            m_visible_rect.Width = (int)(ClientRectangle.Width / m_scale);
        }
        #endregion
    }
}