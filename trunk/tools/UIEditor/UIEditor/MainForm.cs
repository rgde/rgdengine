using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Rgde.Contols;
using Rgde.Contols.UI;

namespace UIEditor
{
    public partial class MainForm : Form
    {

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
        LayoutEditor layout_editor = null;

        public MainForm()
        {
            InitializeComponent();

            this.DoubleBuffered = true;

            layout_editor = new LayoutEditor();
            
            splitContainer1.Panel2.Controls.Add(layout_editor);
            layout_editor.Dock = DockStyle.Fill;

            layout_editor.MouseWheel += new MouseEventHandler(pictureBox1_MouseWheel);
            layout_editor.MouseEnter += new EventHandler(pictureBox1_MouseEnter);

            layout_editor.MouseLeave += new EventHandler(pictureBox1_MouseLeave);       
        }

        void pictureBox1_MouseEnter(object sender, EventArgs e)
        {
            layout_editor.Focus();            
        }

        float scale = 1.0f;

        void pictureBox1_MouseWheel(object sender, MouseEventArgs e)
        {
            scale += e.Delta / 10000.0f;
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            //foreach (TextureRegion reg in listBox1.Items)
            //{
            //    reg.selected = false;
            //}

            //try
            //{
            //    if (listBox1.SelectedItem != null)
            //    {
            //        TextureRegion r = (TextureRegion)listBox1.SelectedItem;
            //        regionName.Text = r.name;
            //        r.selected = true;
            //    }
            //    else
            //    {
            //        regionName.Text = "";
            //    }
            //}
            //catch
            //{
            //    regionName.Text = "";
            //}

            //pictureBox1.Invalidate();
        }


        private void openToolStripButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            if (DialogResult.OK == ofd.ShowDialog(this))
            {
                bitmap = new Bitmap(ofd.FileName);
                layout_editor.Image = bitmap;
                //pictureBox1.Invalidate();
                layout_editor.Invalidate();
            }
        }

        int mouse_x = 0;
        int mouse_y = 0;
        int old_hovered_item = -1;

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            if (bitmap != null)
                e.Graphics.DrawImage(bitmap, new RectangleF(0, 0, bitmap.Width * scale, bitmap.Height * scale));
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

        bool IsMouseHovered(TextureRegion r)
        {
            int x = (int)(mouse_x);
            int y = (int)(mouse_y);
            return RectParts.None != TestMouseHover(r.GetRect(0,0,scale), x, y);
        }

        void SetCursor(int x, int y)
        {
            //try
            //{
            //    if (listBox1.SelectedItem != null)
            //    {
            //        TextureRegion r = GetSelectedItem();

            //        Rectangle[] rects = TextureRegion.GetSelectionRectangles(r.GetRect(0,0,scale));

            //        Point mouse_pos = new Point(x, y);

            //        if (rects[0].Contains(mouse_pos))
            //        {
            //            Cursor.Current = Cursors.SizeNWSE;
            //        }
            //        else if (rects[1].Contains(mouse_pos))
            //        {
            //            Cursor.Current = Cursors.SizeNESW;                        
            //        }
            //        else if (rects[2].Contains(mouse_pos))
            //        {
            //            Cursor.Current = Cursors.SizeNWSE;
            //        }
            //        else if (rects[3].Contains(mouse_pos))
            //        {
            //            Cursor.Current = Cursors.SizeNESW;
            //        }
            //        else if (r.rect.Contains(mouse_pos))
            //        {
            //            Cursor.Current = Cursors.SizeAll;
            //        }
            //        else
            //            Cursor.Current = Cursors.Default;
            //    }
            //}
            //catch
            //{
            //}
        }


        RectParts TestMouseHover(Rectangle r, int x, int y)
        {
            Rectangle[] rects = TextureRegion.GetSelectionRectangles(r);
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
            //TextureRegion r = GetSelectedItem();

            //if (r != null)
            //{
            //    if (RectParts.None != TestMouseHover(r.GetRect(0,0,scale), mouse_x, mouse_y))
            //        return true;
            //}
            return false;
        }             
   


        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            TextureRegion r = null;// GetSelectedItem();

            if (r != null)
            {                
                RectParts p = TestMouseHover(r.GetRect(0,0,scale), mouse_x, mouse_y);

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


        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            int dx = (int)((e.X - mouse_x)/scale);
            int dy = (int)((e.Y - mouse_y)/scale);

            mouse_x = e.X;
            mouse_y = e.Y;

            //if (m_state.action != AppState.Action.None)
            //{
            //    if (m_state.action == AppState.Action.Resizing)
            //    {
            //        switch(m_state.rect_part)
            //        {
            //            case RectParts.LeftTopSizer:
            //                UpdateSelection(dx, dy, -dx, -dy);
            //                break;
            //            case RectParts.RightTopSizer:
            //                UpdateSelection(0, dy, dx, -dy);
            //                break;
            //            case RectParts.RightDownSizer:
            //                UpdateSelection(0, 0, dx, dy);
            //                break;
            //            case RectParts.LeftDownSizer:
            //                UpdateSelection(dx, 0, -dx, dy);
            //                break;
            //        }
            //    }
            //    else if (m_state.action == AppState.Action.Moving)
            //    {
            //        UpdateSelection(dx, dy, 0, 0);
            //    }
            //    return;
            //}

            IsMouseOverSelectedBox();


        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            m_state.action = AppState.Action.None;
        }

        private void pictureBox1_MouseLeave(object sender, EventArgs e)
        {
            m_state.action = AppState.Action.None;
        }

        private void addRootNodeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateTreeNode("Root");
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            treeView1.Nodes.Clear();
        }

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {
            if(treeView1.Nodes.Count > 0)
            {
                clearToolStripMenuItem.Visible = true;
                addRootNodeToolStripMenuItem.Visible = false;
            }
            else
            {
                clearToolStripMenuItem.Visible = false;
                addRootNodeToolStripMenuItem.Visible = true;
            }

            if (treeView1.SelectedNode != null)
            {
                deleteToolStripMenuItem.Visible = true;

                if (treeView1.SelectedNode.Tag == null)
                {
                    addRectToolStripMenuItem.Visible = true;
                    addFolderToolStripMenuItem.Visible = true;
                }
                else
                {
                    addRectToolStripMenuItem.Visible = false;
                    addFolderToolStripMenuItem.Visible = false;
                }
            }
            else
            {
                addFolderToolStripMenuItem.Visible = false;
                deleteToolStripMenuItem.Visible = false;
                addRectToolStripMenuItem.Visible = false;
                addFolderToolStripMenuItem.Visible = false;
            }
        }

        TreeNode CreateTreeNode(string name)
        {
            TreeNode node = new TreeNode(name);
            node.Checked = true;
            if (treeView1.SelectedNode != null)
            {
                treeView1.SelectedNode.Nodes.Add(node);
                node.ImageIndex = 4;
                node.SelectedImageIndex = 4;
            }
            else 
            {
                node.ImageIndex = 1;
                node.SelectedImageIndex = 1;
                treeView1.Nodes.Add(node);
            }

            treeView1.SelectedNode = node;

            return node;
        }

        TextureRegion CreateTextureRegion(TreeNode owner)
        {
            TextureRegion rec = new TextureRegion();
            owner.Tag = rec;
            rec.Tag = owner;
            //propertyGrid1.SelectedObject = rec;
            owner.ForeColor = Color.Blue;
            rec.Name = owner.Text;
            owner.ImageIndex = 0;
            owner.SelectedImageIndex = 0;
            return rec;
        }

        private void DeleteTreeNode(TreeNode node)
        {
            foreach(TreeNode child in node.Nodes)
            {
                DeleteTreeNode(child);
            }

            node.Nodes.Clear();

            if (node.Tag != null)
            {
                TextureRegion reg = (TextureRegion)node.Tag;
                // remove reg from rendering control
                reg.Tag = null;
                node.Tag = null;
            }

            if(node.Parent != null)
            {
                node.Parent.Nodes.Remove(node);
            }
            else
            {
                node.TreeView.Nodes.Remove(node);
            }
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DeleteTreeNode(treeView1.SelectedNode);
        }

        private void addFolderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateTreeNode("Folder");
        }

        private void addRectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TreeNode node = CreateTreeNode("Rect");
            TextureRegion reg = CreateTextureRegion(node);            
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            //regionName.Text = e.Node.FullPath;
        }

        private void treeView1_AfterExpand(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Parent == null) return;
            e.Node.ImageIndex = 5;
            e.Node.SelectedImageIndex = 5;
        }

        private void treeView1_AfterCollapse(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Parent == null) return;
            e.Node.ImageIndex = 4;
            e.Node.SelectedImageIndex = 4;
        }

        private void treeView1_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            if (e.Node.Tag == null) return;
            TextureRegion reg = (TextureRegion)e.Node.Tag;
            reg.Name = e.Label;
        }

        private void saveToolStripButton_Click(object sender, EventArgs e)
        {
            //TreeViewSerializer out_ = new TreeViewSerializer();
            //out_.SerializeTreeView(treeView1, "test.xml");
        }
    }
}