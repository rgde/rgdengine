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
        private RectangleF m_visible_rect = new RectangleF();
        private float m_scale = 1.0f;
        private int old_x = 0;
        private int old_y = 0;

        private List<UI.TextureRegion> regions = new List<UI.TextureRegion>();
        private List<UI.TextureRegion> hovered_regions = new List<UI.TextureRegion>();
        private List<UI.TextureRegion> selected_regions = new List<UI.TextureRegion>();

        public class UndoRedoState
        {
            public List<UI.TextureRegion> regions;
            public List<UI.TextureRegion> selected_regions;

            LayoutEditor owner;

            public UndoRedoState(LayoutEditor owner)
            {
                this.owner = owner;

                regions = new List<UI.TextureRegion>(owner.regions);
                selected_regions = new List<UI.TextureRegion>(owner.selected_regions);
            }

            public void Restore()
            {
                owner.regions = regions;
                owner.hovered_regions.Clear();
                owner.selected_regions = selected_regions;
            }

        }

        private List<UndoRedoState> undo_redo_list = new List<UndoRedoState>();

        public void ClearUndoRedo()
        {
            undo_redo_list.Clear();
        }

        private void PushUndo()
        {
            undo_redo_list.Add(new UndoRedoState(this));
            if(undo_redo_list.Count > 15)
            {
                undo_redo_list.RemoveAt(0);
            }
        }

        public void Undo()
        {
            int last = undo_redo_list.Count;
            if (last == 0) return;
            UndoRedoState state = undo_redo_list[last - 1];
            undo_redo_list.RemoveAt(last-1);
            state.Restore();
        }

        public void Clear()
        {
            regions.Clear();
            hovered_regions.Clear();
            selected_regions.Clear();

            ClearSelection();
        }

        public LayoutEditor()
        {
            this.BorderStyle = BorderStyle.FixedSingle;
            this.DoubleBuffered = true;
            InitializeComponent();

            this.Load += new EventHandler(LayoutEditor_Load);
        }

        void LayoutEditor_Load(object sender, EventArgs e)
        {
            //UI.TextureRegion r1 = new UI.TextureRegion();
            //r1.Rectangle = new Rectangle(10, 10, 30, 30);
            //r1.Name = "Button.Bkg";


            //UI.TextureRegion r2 = new UI.TextureRegion();
            //r2.Rectangle = new Rectangle(50, 10, 30, 30);
            //r2.Name = "Button.Pressed";


            //UI.TextureRegion r3 = new UI.TextureRegion();
            //r3.Rectangle = new Rectangle(50, 50, 80, 30);
            //r3.Name = "Button.Hover";

            //AddTextureRegion(r1);
            //AddTextureRegion(r2);
            //AddTextureRegion(r3);

            ClearUndoRedo();
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

            PushUndo();

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

            PushUndo();
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
            base.OnMouseMove(e);

            float fdx = (e.X - old_x) / m_scale;
            float fdy = (e.Y - old_y) / m_scale;

            old_x = e.X;
            old_y = e.Y;

            bool need_to_redraw = hovered_regions.Count > 0;
            hovered_regions.Clear();

            if (e.Button == MouseButtons.Right)
            {
                m_visible_rect.X += fdx;
                m_visible_rect.Y += fdy;

                need_to_redraw = true;
            }

            float local_mouse_x = e.X / m_scale - m_visible_rect.X;
            float local_mouse_y = e.Y / m_scale - m_visible_rect.Y;

            if (e.Button == MouseButtons.None)
            {
                m_state.action = AppState.Action.None;

                foreach (UI.TextureRegion r in regions)
                {
                    //if (r.GetRect(x, y, m_scale).Contains(new Point(mouse_x, mouse_y)))
                    if (r.Rectangle.Contains((int)local_mouse_x, (int)local_mouse_y))                    
                    {
                        hovered_regions.Add(r);
                    }
                }

                if (hovered_regions.Count > 0)
                    need_to_redraw = true;

                SetCursor(local_mouse_x, local_mouse_y);
            }

            if (m_state.action != AppState.Action.None)
            {
                if (m_state.action == AppState.Action.Resizing)
                {
                    switch (m_state.rect_part)
                    {
                        case RectParts.LeftTopSizer:
                            UpdateRegion(fdx, fdy, -fdx, -fdy);
                            need_to_redraw = true;
                            break;
                        case RectParts.RightTopSizer:
                            UpdateRegion(0, fdy, fdx, -fdy);
                            need_to_redraw = true;
                            break;
                        case RectParts.RightDownSizer:
                            UpdateRegion(0, 0, fdx, fdy);
                            need_to_redraw = true;
                            break;
                        case RectParts.LeftDownSizer:
                            UpdateRegion(fdx, 0, -fdx, fdy);
                            need_to_redraw = true;
                            break;
                    }
                }
                else if (m_state.action == AppState.Action.Moving)
                {
                    UpdateRegion(fdx, fdy, 0, 0);
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

            //int mouse_x = e.X;
            //int mouse_y = e.Y;

            //float x = m_visible_rect.X;
            //float y = m_visible_rect.Y;

            float local_mouse_x = e.X / m_scale - m_visible_rect.X;
            float local_mouse_y = e.Y / m_scale - m_visible_rect.Y;

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
                        if (r.Rectangle.Contains((int)local_mouse_x, (int)local_mouse_y))
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
                    r1.Rectangle = new RectangleF(local_mouse_x, local_mouse_y, 4.0f, 4.0f);
                    r1.Name = "New Region";
                    AddTextureRegion(r1, true);
                    m_state.action = AppState.Action.Resizing;
                    m_state.rect_part = RectParts.RightDownSizer;
                    return;
                }

                foreach (UI.TextureRegion r in selected_regions)
                {
                    need_to_redraw = true;

                    RectParts p = TestMouseHover(r, local_mouse_x, local_mouse_y);

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

                    RectParts p = TestMouseHover(r, local_mouse_x, local_mouse_y);

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
            return;
            Cursor = Cursors.Arrow;
            old_x = 0;
            old_y = 0;
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);

            //if (null == m_image)
            //    return;

            float delta_scale = m_scale * 0.1f * e.Delta / 120.0f;

            float dp_scale = 1.0f / (m_scale + delta_scale) - 1.0f / m_scale;

            m_scale += delta_scale;

            float dx = e.Location.X * dp_scale;
            float dy = e.Location.Y * dp_scale;

            m_visible_rect.X += dx;
            m_visible_rect.Y += dy;

            //m_scale += e.Delta / 2000.0f;
            //if( m_scale < 0.05f )
            //{
            //    m_scale = 0.05f; //min scale
            //    RecalcVisibleRect();
            //}

            //RecalcVisibleRect();

            //ClampRect();

            //UpdateScroolBarsVisibility();
            Invalidate();
        }

        //protected override void OnScroll(ScrollEventArgs se)
        //{
        //    return;
        //    //bool horiz = se.ScrollOrientation == ScrollOrientation.HorizontalScroll;

        //    //int cur_value = horiz ? m_visible_rect.X : m_visible_rect.Y;

        //    //switch(se.Type)
        //    //{
        //    //    case ScrollEventType.SmallDecrement:
        //    //    case ScrollEventType.SmallIncrement:
        //    //    case ScrollEventType.LargeDecrement:
        //    //    case ScrollEventType.LargeIncrement:
        //    //        cur_value += se.NewValue;
        //    //        break;

        //    //    case ScrollEventType.ThumbPosition:
        //    //    case ScrollEventType.ThumbTrack:
        //    //        cur_value = se.NewValue;
        //    //        break;

        //    //    case ScrollEventType.First:
        //    //        cur_value = 0;
        //    //        break;

        //    //    case ScrollEventType.Last:
        //    //        cur_value = horiz ? m_image.Width - m_visible_rect.Width : m_image.Height - m_visible_rect.Height;
        //    //        break;
        //    //}

        //    //if (horiz)
        //    //{
        //    //    m_visible_rect.X = cur_value;
        //    //}
        //    //else
        //    //{
        //    //    m_visible_rect.Y = cur_value;
        //    //}

        //    //ClampRect();
        //    //HorizontalScroll.Value = m_visible_rect.X;
        //    //VerticalScroll.Value = m_visible_rect.Y;
        //    //Invalidate();
        //}

        //protected override void AdjustFormScrollbars(bool displayScrollbars)
        //{
        //    try
        //    {
        //        HorizontalScroll.Value = m_visible_rect.X;
        //        VerticalScroll.Value = m_visible_rect.Y;
        //    }
        //    catch { }
        //}
       
        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics g = e.Graphics;

            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
            g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
            g.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
            g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

            g.ScaleTransform(m_scale, m_scale);
            g.TranslateTransform(m_visible_rect.X, m_visible_rect.Y);

            if (m_image != null)
            {
                g.DrawImage(m_image, 0, 0);
            }

            DrawBoxes(g);
        }
        #endregion

        #region Private methods
        private void SetCursor(float x, float y)
        {
            try
            {
                foreach (UI.TextureRegion r in selected_regions)
                {
                    RectangleF rect = r.Rectangle;
                    RectangleF[] rects = UI.TextureRegion.GetSelectionRectangles(rect);

                    PointF mouse_pos = new PointF(x, y);

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
            GraphicsState old_state = g.Save();

            g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

            foreach (UI.TextureRegion r in regions)
            {
                r.Draw(g, UI.TextureRegion.DrawMode.Normal, m_image);
            }

            foreach (UI.TextureRegion r in hovered_regions)
            {
                r.Draw(g, UI.TextureRegion.DrawMode.Hovered, m_image);
            }

            foreach (UI.TextureRegion r in selected_regions)
            {
                r.Draw(g, UI.TextureRegion.DrawMode.Selected, m_image);
            }

            g.Restore(old_state);
        }

        private void UpdateRegion(float dx, float dy, float dwidth, float dheight)
        {
            foreach (UI.TextureRegion r in selected_regions)
            {
                if (r == null) continue;

                if (MouseButtons == MouseButtons.Left)
                {
                    RectangleF rect = r.Rectangle;

                    if (rect.X + dx < rect.X + rect.Width + dwidth)
                    {
                        rect.X = rect.X + dx;
                        rect.Width = rect.Width + dwidth;
                    }

                    if (rect.Y + dy < rect.Y + rect.Height + dheight)
                    {
                        rect.Y = rect.Y + dy;
                        rect.Height = rect.Height + dheight;
                    }

                    r.Rectangle = rect;
                }
            }
        }

        private RectParts TestMouseHover(UI.TextureRegion r, float x, float y)
        {
            RectangleF rect = r.Rectangle;
            RectangleF[] rects = UI.TextureRegion.GetSelectionRectangles(rect);

            PointF mouse_pos = new PointF(x, y);

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

        #endregion

        private void LayoutEditor_MouseLeave(object sender, EventArgs e)
        {
            hovered_regions.Clear();
            Invalidate();
        }
    }
}