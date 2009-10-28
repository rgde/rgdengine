using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Xml;

namespace ImageGrid
{
    public partial class Form1 : Form
    {
        class ThreadStartParams
        {
            public Form1 form;
            public  ArrayList files;
        };

        CellGrid grid = null;

        class FileInfo
        {
            public string file_name;
            public int x, y;
        };

        public Form1()
        {
            InitializeComponent();
            this.DoubleBuffered = true;

            grid = new CellGrid();
            this.Controls.Add(this.grid);
            grid.Location = new Point(10, 10);
            grid.Size = new Size(400, 300);
            grid.Dock = DockStyle.Fill;
            grid.BringToFront();
        }

        void OpenMap()
        {
            using (OpenFileDialog ofd = new OpenFileDialog())
            {
                ofd.Multiselect = true;
                ofd.Filter = "GSS files (*.gss)|*.gss";

                if (ofd.ShowDialog() == DialogResult.OK)
                {
                    string file = ofd.FileName;
                    string dir = System.IO.Path.GetDirectoryName(file);
                    dir += "\\data";
                    string name = System.IO.Directory.GetParent(dir).Name;
                    string desc_file = name + "_map_desc.xml";
                    string desc_file_path = dir + "\\" + desc_file;

                    const string filename_tag = "_color_";
                    const string filename_ext = ".dds";

                    string filename_part = dir + "\\" + name + filename_tag;

                    using (System.IO.FileStream fstream = new System.IO.FileStream(desc_file_path, System.IO.FileMode.Open))
                    {
                        System.Xml.XmlDocument doc = new XmlDocument();
                        {
                            doc.Load(fstream);
                            XmlNode common_node = doc.FirstChild.NextSibling.FirstChild;
                            XmlNode node_size_x = common_node.Attributes.GetNamedItem("map_size_x");
                            XmlNode node_size_z = common_node.Attributes.GetNamedItem("map_size_z");

                            int size_x = System.Convert.ToInt32(node_size_x.Value);
                            int size_z = System.Convert.ToInt32(node_size_z.Value);

                            ThreadStartParams thread_params = new ThreadStartParams();
                            thread_params.form = this;

                            thread_params.files = new ArrayList();
                            grid.GridDimentions = new Size(size_x, size_z);

                            for (int x = 0; x < size_x; x++)
                            {
                                for (int y = 0; y < size_z; y++)
                                {
                                    string image_name = filename_part
                                        + x.ToString()
                                        + "_"
                                    + y.ToString() + filename_ext;

                                    FileInfo file_info = new FileInfo();
                                    file_info.file_name = image_name;
                                    file_info.x = x;
                                    file_info.y = y;
                                    thread_params.files.Add(file_info);
                                }
                            }

                            Thread thread = new Thread(
                                new ParameterizedThreadStart(ImageGrid.Form1.DoImageLoadingWork));
                            thread.Start(thread_params);
                        }
                    }
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenMap();
        }

        void UpdateImage(int x, int y, Bitmap bitmap)
        {
            grid.SetImage(x, y, bitmap);
            grid.Invalidate();
        }

        public delegate void myMethodDelegate(int x, int y, Bitmap bitmap);


        // TODO: сделать механизм завершения потока при закрытии. 
        // проблема актуальна только если закрывать окно до полной загрузки карты
        // и приводит к тому что процесс закрывается не сразу, и может выдавать ошибки.
        public static void DoImageLoadingWork(object obj)
        {
            ThreadStartParams thread_params = (ThreadStartParams)obj;
            Form1 form = thread_params.form;

            {
                foreach (FileInfo file_info in thread_params.files)
                {                  
                    try
                    {
                        using (Bitmap bitmap = DevIL.DevIL.LoadBitmap(file_info.file_name))
                        {
                            Bitmap converted_image = bitmap.Clone(
                                new Rectangle(0, 0, bitmap.Width, bitmap.Height),
                                System.Drawing.Imaging.PixelFormat.Format32bppPArgb);

                            myMethodDelegate myD1 = new myMethodDelegate(form.UpdateImage);

                            object[] b = new object[] { file_info.x, file_info.y, converted_image };
                            form.Invoke(myD1, b);
                        }
                    }
                    catch {}
                }
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenMap();
        }
    }
}