using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.IO;
using System.Xml;
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

            propertyGrid.Validated += new EventHandler(propertyGrid_Validated);
        }

        void propertyGrid_Validated(object sender, EventArgs e)
        {
            if (propertyGrid.SelectedObject != null)
            {
                TextureRegion reg = propertyGrid.SelectedObject as TextureRegion;

                if (reg == null)
                    return;

                TreeNode node = reg.Tag as TreeNode;

                if (node == null)
                    return;

                if (node.Text != reg.Name)
                {
                    node.Text = reg.Name;
                    treeView1.SelectedNode = node;
                }

                layout_editor.Invalidate();                
            }
        }

        void layout_editor_OnSelectionChange(object sender)
        {
            treeView1.SelectedNode = null;
        }

        void layout_editor_OnSelectRegion(TextureRegion reg)
        {
            treeView1.SelectedNode = reg.Tag as TreeNode;
            propertyGrid.SelectedObject = reg;
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

        string imageset_name = "";
        string imageset_filename = "";
        string imagefile = "";
        int NativeHorzRes = 512;
        int NativeVertRes = 512;
        bool AutoScaled = true;

        private void openToolStripButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Imageset Files|*.imageset";            

            if (DialogResult.OK == ofd.ShowDialog(this))
            {
                layout_editor.Clear();
                treeView1.Nodes.Clear();
                imageset_filename = ofd.FileName;

                this.Text = ofd.FileName;

                using(FileStream fs = new FileStream(ofd.FileName, FileMode.Open))
                {
                    System.Xml.XmlDocument doc = new System.Xml.XmlDocument();

                    doc.Load(fs);                    
                    XmlNode imageset_node = doc["Imageset"];

                    //<Imageset Name="Abilities" Imagefile="Abilities.tga" 
                    imageset_name = imageset_node.Attributes["Name"].Value;
                    imagefile = imageset_node.Attributes["Imagefile"].Value;

                    imagefile = System.IO.Path.GetDirectoryName(ofd.FileName) + "\\" + imagefile;
                    imagefile = imagefile.ToLower();

                    layout_editor.Image = DevIL.DevIL.LoadBitmap(imagefile);

                    if (null == layout_editor.Image && imagefile.Contains(".tga"))
                    {
                        string imagefile_copy = imagefile.Replace(".tga", ".dds");
                        
                        Image image = DevIL.DevIL.LoadBitmap(imagefile);
                        
                        if (null != image)
                        {
                            imagefile = imagefile_copy;
                            layout_editor.Image = image;
                        }
                    }

                    if (null != layout_editor.Image)
                    {
                        NativeHorzRes = layout_editor.Image.Width;
                        NativeVertRes = layout_editor.Image.Height;
                    }

                    //NativeHorzRes="1024" NativeVertRes="1024"
                    if (null != imageset_node.Attributes["NativeHorzRes"])
                        NativeHorzRes = System.Convert.ToInt32(imageset_node.Attributes["NativeHorzRes"].Value);

                    if (null != imageset_node.Attributes["NativeHorzRes"])
                        NativeVertRes = System.Convert.ToInt32(imageset_node.Attributes["NativeVertRes"].Value);

                    // AutoScaled="true">
                    AutoScaled = true;

                    if (null != imageset_node.Attributes["AutoScaled"])
                        AutoScaled = System.Convert.ToBoolean(imageset_node.Attributes["AutoScaled"].Value);

                    foreach (XmlNode node in imageset_node.ChildNodes)
                    {
                        //XmlElement 
                        XmlElement el = node as XmlElement;
                        if (null == el)
                            continue;

                        TextureRegion t = new TextureRegion();

                        //<Image Name="TestAbility_2" YPos="0" XPos="40" Width="40" Height="40" />
                        t.Name = node.Attributes["Name"].Value;
                        int x = System.Convert.ToInt32(node.Attributes["XPos"].Value);
                        int y = System.Convert.ToInt32(node.Attributes["YPos"].Value);
                        int w = System.Convert.ToInt32(node.Attributes["Width"].Value);
                        int h = System.Convert.ToInt32(node.Attributes["Height"].Value);

                        Rectangle r = new Rectangle(x, y, w, h);
                        t.Rectangle = r;

                        layout_editor.AddTextureRegion(t, false);
                    }
                    
                    layout_editor.Invalidate();
                }
            }
        }

        private void saveToolStripButton_Click(object sender, EventArgs e)
        {
            if (imageset_filename.Length == 0)
            {
                SaveFileDialog sfd = new SaveFileDialog();
                sfd.Filter = "Imageset Files|*.imageset";
                if (DialogResult.OK == sfd.ShowDialog(this))
                {
                    imageset_filename = sfd.FileName;
                }
                else
                    return;
            }

            System.Xml.XmlDocument doc = new System.Xml.XmlDocument();
            XmlElement imageset_el = doc.CreateElement("Imageset");
            imageset_el.SetAttribute("Name", imageset_name);
            imageset_el.SetAttribute("Imagefile", imagefile);

            imageset_el.SetAttribute("NativeHorzRes", NativeHorzRes.ToString());
            imageset_el.SetAttribute("NativeVertRes", NativeVertRes.ToString());
            imageset_el.SetAttribute("AutoScaled", AutoScaled.ToString());

            doc.AppendChild(imageset_el);

            foreach (TextureRegion reg in layout_editor.Regions)
            {
                XmlElement el = doc.CreateElement("Image");
                el.SetAttribute("Name", reg.Name);

                el.SetAttribute("XPos", reg.Rectangle.X.ToString());
                el.SetAttribute("YPos", reg.Rectangle.Y.ToString());
                el.SetAttribute("Width", reg.Rectangle.Width.ToString());
                el.SetAttribute("Height", reg.Rectangle.Height.ToString());
                imageset_el.AppendChild(el);
            }

            doc.Normalize();
            doc.Save(imageset_filename);
        }

        private void addRootNodeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CreateTreeNode("Root");
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            treeView1.Nodes.Clear();
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
            TextureRegion tr = e.Node.Tag as TextureRegion;
            if (layout_editor.SelectedRegion != tr)
            {
                layout_editor.ClearSelection();
                layout_editor.AddToSelection(tr);
                layout_editor.Invalidate();
            }

            e.Node.NodeFont = new Font(e.Node.TreeView.Font, FontStyle.Underline | FontStyle.Bold);
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

        private void treeView1_AfterCheck(object sender, TreeViewEventArgs e)
        {
            if (e.Node.Tag != null)
            {
                TextureRegion reg = e.Node.Tag as TextureRegion;
                reg.Visible = e.Node.Checked;
                layout_editor.Invalidate();
            }
        }

        private void newToolStripButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Image Files|*.dds; *.tga";

            if (DialogResult.OK == ofd.ShowDialog(this))
            {
                imageset_name = "";
                imageset_filename = "";
                
                AutoScaled = true;

                layout_editor.Regions.Clear();
                treeView1.Nodes.Clear();

                imagefile = ofd.FileName;
                layout_editor.Image = DevIL.DevIL.LoadBitmap(ofd.FileName);

                NativeHorzRes = layout_editor.Image.Width;
                NativeVertRes = layout_editor.Image.Height;
            }
        }

        void ResizeSelectedRegion(int dw, int dh, bool is_shift_pressed)
        {
            TextureRegion reg = layout_editor.SelectedRegion;

            if (reg == null)
                return;

            if (is_shift_pressed)
            {
                dw *= 10;
                dh *= 10;
            }

            Rectangle rect = reg.Rectangle;
            rect.Width += dw;
            rect.Height += dh;

            reg.Rectangle = rect;

            layout_editor.Invalidate();
            propertyGrid.SelectedObject = reg;
        }

        void MoveSelectedRegion(int dx, int dy, bool is_shift_pressed)
        {
            TextureRegion reg = layout_editor.SelectedRegion;

            if (reg == null)
                return;

            if (is_shift_pressed)
            {
                dx *= 10;
                dy *= 10;
            }

            Rectangle rect = reg.Rectangle;
            rect.X += dx;
            rect.Y += dy;

            reg.Rectangle = rect;

            layout_editor.Invalidate();
            propertyGrid.SelectedObject = reg;
        }

        private void MainForm_KeyUp(object sender, KeyEventArgs e)
        {
            if(e.Control)
            {
                switch(e.KeyCode)
                {
                    case Keys.O:
                        openToolStripButton_Click(null, new EventArgs());
                        break;
                    case Keys.S:
                        saveToolStripButton_Click(null, new EventArgs());
                        break;
                    case Keys.Left:
                        ResizeSelectedRegion(-1, 0, e.Shift);
                        break;
                    case Keys.Right:
                        ResizeSelectedRegion(1, 0, e.Shift);
                        break;
                    case Keys.Up:
                        ResizeSelectedRegion(0, -1, e.Shift);
                        break;
                    case Keys.Down:
                        ResizeSelectedRegion(0, 1, e.Shift);
                        break;
                }
            }
            else
            {
                switch (e.KeyCode)
                {
                    case Keys.Left:
                        MoveSelectedRegion(-1, 0, e.Shift);
                        break;
                    case Keys.Right:
                        MoveSelectedRegion(1, 0, e.Shift);
                        break;
                    case Keys.Up:
                        MoveSelectedRegion(0, -1, e.Shift);
                        break;
                    case Keys.Down:
                        MoveSelectedRegion(0, 1, e.Shift);
                        break;
                }
            }
        }

        private void treeView1_BeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            if (e.Node.TreeView.SelectedNode != null)
            {
                e.Node.TreeView.SelectedNode.NodeFont = null;
            }
        }
    }
}