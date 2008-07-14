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

            layout_editor.MouseEnter += new EventHandler(pictureBox1_MouseEnter);

            layout_editor.OnAddRegion += new TextureRegionEventDelegate(layout_editor_OnAddRegion);
            layout_editor.OnDeleteRegion += new TextureRegionEventDelegate(layout_editor_OnDeleteRegion);
            layout_editor.OnSelectRegion += new TextureRegionEventDelegate(layout_editor_OnSelectRegion);
            layout_editor.OnSelectionChange += new LayoutEditor.LayoutEditorEventDelegate(layout_editor_OnSelectionChange);

            //CreateTreeNode("Root");
        }

        void layout_editor_OnSelectionChange(object sender)
        {
            treeView1.SelectedNode = null;
        }

        void layout_editor_OnSelectRegion(TextureRegion reg)
        {
            treeView1.SelectedNode = reg.Tag as TreeNode;
        }

        void layout_editor_OnDeleteRegion(TextureRegion reg)
        {
            if (reg.Tag != null)
            {
                TreeNode node = reg.Tag as TreeNode;
                node.Remove();
            }
        }

        void layout_editor_OnAddRegion(TextureRegion reg)
        {
            CreateTreeNode(reg.Name, reg);
        }

        void pictureBox1_MouseEnter(object sender, EventArgs e)
        {
            layout_editor.Focus();            
        }

        private void openToolStripButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            if (DialogResult.OK == ofd.ShowDialog(this))
            {
                layout_editor.Image = new Bitmap(ofd.FileName);
                layout_editor.Invalidate();
            }
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

        TreeNode CreateTreeNode(string name, TextureRegion reg)
        {
            TreeNode node = new TreeNode(name);
            node.Tag = reg;
            reg.Tag = node;

            node.ImageIndex = 2;
            node.SelectedImageIndex = 2;

            node.Checked = true;
            if (treeView1.SelectedNode != null)
            {
                if (treeView1.SelectedNode.Tag == null)
                {
                    treeView1.SelectedNode.Nodes.Add(node);
                }
                else if (treeView1.SelectedNode.Parent != null) 
                {
                    treeView1.SelectedNode.Parent.Nodes.Add(node);
                }
                else
                    treeView1.Nodes.Add(node);
            }
            else
            {
                treeView1.Nodes.Add(node);
            }

            //treeView1.SelectedNode = node;

            return node;
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

        private void treeView1_AfterCheck(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Tag != null)
            {
                TextureRegion reg = e.Node.Tag as TextureRegion;
                reg.Visible = e.Node.Checked;
                layout_editor.Invalidate();
            }
        }
    }
}