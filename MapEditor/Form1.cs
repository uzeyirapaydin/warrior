#region Using directives

using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

#endregion


namespace Map
{
	partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}
		int map_w=830, map_h=720;
		private void OnPaint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
			Graphics g = e.Graphics;
			Color[] a=new Color[20];
			SolidBrush brush = new SolidBrush(Color.White);
			Pen pen = new Pen(Color.Black);
			Font fnt = new Font("Arial", 14);
			bool can_save = false;
			int x, y,  tmp = 2, R, mdl_type, mdl_no, hlth;
			a[0] = Color.Bisque;
			a[1] = Color.Chartreuse;
			a[2] = Color.Cyan;
			a[3] = Color.Purple;
			a[4] = Color.SeaGreen;
			a[5] = Color.Gold;
			a[6] = Color.LightSlateGray;
			a[7] = Color.Blue;
			a[8] = Color.BlueViolet;
			a[9] = Color.RoyalBlue;
			a[10] = Color.ForestGreen;
			a[11] = Color.Tomato;
			a[12] = Color.YellowGreen;
			a[13] = Color.White;
			a[14] = Color.SaddleBrown;
			a[15] = Color.SkyBlue;
			a[16] = Color.Goldenrod;
			a[17] = Color.MediumSeaGreen;
			a[18] = Color.SlateGray;
			a[19] = Color.OldLace;
					
			brush.Color = Color.White; ;
			g.FillRectangle(brush, 10, 10, map_w, map_h);
			g.DrawRectangle(pen, 10, 10, map_w, map_h);
			for (int i = 0; i<list.Items.Count; i++)
			{
				string s;
				s = list.Items[i].ToString();
				x= Int32.Parse(s.Substring(0, 4))+10;
				y = Int32.Parse(s.Substring(5, 4))+10;
				R = Int32.Parse(s.Substring(10, 1));
				mdl_type = Int32.Parse(s.Substring(12, 1));
				mdl_no = Int32.Parse(s.Substring(14, 2));
				hlth = Int32.Parse(s.Substring(17, 3));

				if (mdl_type == 2)
					can_save = true;
				if (!(x >= 0 && y >= 0 && x <= map_w && y <= map_h))
				{
					list.Items.RemoveAt(i);
					i = -1;
					continue;
				}
				brush.Color = a[R-1];
				

				switch (mdl_type)
				{
					case 0:
						g.FillEllipse(brush, x - R, y - R, R * 2, R * 2);
						g.DrawEllipse(pen, x - R, y - R, R * 2, R * 2);
						break;
					case 1:
						g.FillRectangle(brush, x - R, y - R, R * 2, R * 2);
						g.DrawRectangle(pen, x - R, y - R, R * 2, R * 2);
						break;
					case 2:
						R = 3;
						g.FillRectangle(brush, x - R, y - R, R * 2, R * 2);
						g.DrawRectangle(pen, x - R, y - R, R * 2, R * 2);
						break;
					default: break;
				}

				if (i == list.SelectedIndex)
				{
					g.DrawRectangle(pen, x - R - tmp, y - R - tmp, (R + tmp) * 2, (R + tmp) * 2);
					g.DrawLine(pen, x - R - tmp, y - R - tmp, x - R - tmp + (R + tmp) * 2, y - R - tmp + (R + tmp) * 2);
					g.DrawLine(pen, x - R - tmp + (R + tmp) * 2, y - R - tmp , x - R - tmp, y - R - tmp + (R + tmp) * 2);
				}

			}

			del_obj.Enabled = false;
			r_map.Enabled = false;
			save_file.Enabled = can_save;

			if (list.Items.Count > 0)
			{
				del_obj.Enabled = true;
				r_map.Enabled = true;
			}
			cnt.Text = list.Items.Count.ToString();
	}

		private void Form1_Move(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			int x=e.X-10 , y=e.Y-10;
			if (x >= 0 && y >= 0 && x <= map_w  && y <= map_h)
				this.Text = "RehoGames MAP Editor  beta 1.1";
			else
				this.Text = "RehoGames MAP Editor  beta 1.1 -- Out of area!!!";
		}

		private void Form1_Down(object sender, System.Windows.Forms.MouseEventArgs e)                                                                                                                                                                                            // 990326 UZEYIR APAYDIN
		{
			string s=radius.Items[radius.SelectedIndex].ToString(),X,Y,R,mdl_type,mdl_no,hlth;
			int x=e.X - 10, y=e.Y - 10,t = Int32.Parse(s);
			X = x.ToString();
			Y = y.ToString();
			R = radius.Items[radius.SelectedIndex].ToString() ;
			mdl_type = model_type.SelectedIndex.ToString();
			mdl_no = model_name.SelectedIndex.ToString();
			hlth = health.Items[health.SelectedIndex].ToString();
    
			if (map_h < 100)
			{
				map_h = 100;
				m_h.Text = "100";
			}
			else if (m_h.Text == "")
				m_h.Text = map_h.ToString();
			if (map_w < 100)
			{
				map_w = 100;
				m_w.Text = "100";
			}
			else if (m_w.Text == "")
				m_w.Text = map_w.ToString();

			if (checkObject(x, y,t)){
				if (findHero() < 1)
					list.Items.Add(X.PadLeft(4)+","+
						Y.PadLeft(4)+","+ R.PadLeft(1)+","+
						mdl_type.PadLeft(1)+","+mdl_no.PadLeft(2)+","+
						hlth.PadLeft(3));

				else if(model_type.SelectedIndex!=2)
					list.Items.Add(X.PadLeft(4) + "," +
						Y.PadLeft(4) + "," + R.PadLeft(1) + "," +
						mdl_type.PadLeft(1) + "," + mdl_no.PadLeft(2) + "," +
						hlth.PadLeft(3));
				Invalidate();
			}
			list.SelectedIndex = findObject(x, y);
            for (int i = 0; i < list.Items.Count; i++)
            {
                s = list.Items[i].ToString();
                if (Int32.Parse(s.Substring(12, 1)) == 2)
                {
                    list.Items.Add(s);
                    list.Items.RemoveAt(i);
                }

            }
        }
		public int findHero()
		{
			int cnt = 0,typ;
			string s;
			for (int i = 0; i < list.Items.Count; i++)
			{
				s = list.Items[i].ToString();
				typ = Int32.Parse(s.Substring(12, 1));
				if (typ == 2)
					cnt++;
			}
			return cnt;
		}

		public bool checkObject(int x, int y,int r)
		{
			int cX, cY, R, mdl_type, mdl_no, hlth,tmp;
			string s;

			if (!(x - r > 0 && y - r > 0 && x + r < map_w && y + r < map_h))
				return false;
			for (int i = 0; i < list.Items.Count; i++)
			{
				s = list.Items[i].ToString();
				cX = Int32.Parse(s.Substring(0, 4));
				cY = Int32.Parse(s.Substring(5, 4));
				R = Int32.Parse(s.Substring(10, 1));
				mdl_type = Int32.Parse(s.Substring(12, 1));
				mdl_no = Int32.Parse(s.Substring(14, 2));
				hlth = Int32.Parse(s.Substring(17, 3));
				if (r > R)
					tmp = r;
				else
					tmp = R;

				if ( Math.Sqrt(Math.Pow(cX - x, 2) + Math.Pow(cY - y,2)) <= (R+r+tmp) )
					return false;
			}
		return true;
		}
		public int findObject(int x, int y)
		{
			int cX, cY, R,mdl_type,mdl_no,hlth;
			string s;
			if (!(x  > 0 && y  > 0 && x  < map_w && y  < map_h))
				return -1;
			for (int i = 0; i < list.Items.Count; i++)
			{
				s = list.Items[i].ToString();
		
				cX = Int32.Parse(s.Substring(0, 4));
				cY = Int32.Parse(s.Substring(5, 4));
				R=Int32.Parse(s.Substring(10,1));
				mdl_type = Int32.Parse(s.Substring(12, 1));
				mdl_no = Int32.Parse(s.Substring(14, 2));
				hlth = Int32.Parse(s.Substring(17, 3));


				if (Math.Sqrt(Math.Pow(cX - x, 2) + Math.Pow(cY - y, 2)) <= (R))
					return i;
			}
			return -1;
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			model_name.Items.Add("tris01");
			model_name.Items.Add("tris02");
			model_name.Items.Add("tris03");
			model_name.Items.Add("tris04");
			model_name.Items.Add("tris05");
			model_name.Items.Add("tris06");
			model_name.Items.Add("tris07");
			model_name.Items.Add("tris08");
			model_name.Items.Add("tris09");
			model_name.SelectedIndex = 0;
			radius.SelectedIndex =0;
			model_type.SelectedIndex = 0;
			health.SelectedIndex = 1;
			fog.SelectedIndex = 0;
			floor.SelectedIndex = 0;
			cnt.Text = list.Items.Count.ToString();
		}

		private void del_obj_Click(object sender, EventArgs e)
		{
			if(list.SelectedIndex!=-1)
			list.Items.RemoveAt(list.SelectedIndex);
			Invalidate();
		}
		private static void AddText(FileStream fs, string value)
		{
			byte[] info = new UTF8Encoding(true).GetBytes(value);
			fs.Write(info, 0, info.Length);
		}
		
		private void save_file_Click(object sender, EventArgs e)
		{
            int type1 = 0, type2 = 0;
            string s;
            
            if (list.Items.Count>0 && f_save.ShowDialog().ToString() == "OK")
			{
                string fileName = f_save.FileName;
				using (FileStream fileStream = new FileStream(fileName, FileMode.Create))
				{
                    for (int i = 0; i < list.Items.Count; i++)
                    {
                        s = list.Items[i].ToString();
                        if (Int32.Parse(s.Substring(12, 1)) == 0)
                            type1++;
                        else if (Int32.Parse(s.Substring(12, 1)) == 1)
                            type2++;
                    }
                    AddText(fileStream, list.Items.Count.ToString().PadLeft(3));
                    AddText(fileStream,type1.ToString().PadLeft(3));
                    AddText(fileStream,type2.ToString().PadLeft(3));
                    AddText(fileStream, m_w.Text.PadLeft(4) );
					AddText(fileStream, m_h.Text.PadLeft(4));
					AddText(fileStream,fog.SelectedIndex.ToString());
					AddText(fileStream,floor.SelectedIndex.ToString());
                    
               
                    for (int i = 0; i < list.Items.Count; i++)
							AddText(fileStream, list.Items[i].ToString().Replace(",", ""));
				}
			}
		}

		private void load_file_Click(object sender, EventArgs e)
		{
			byte[] dataArray = new byte[20];
			string s;
			int cX, cY, R, mdl_type, mdl_no, hlth;
			if (f_load.ShowDialog().ToString()=="OK")
			{
				list.Items.Clear();
				string fileName = f_load.FileName;
				using (FileStream fileStream = new FileStream(fileName, FileMode.Open))
				{
					fileStream.Seek(0, SeekOrigin.Begin);
					fileStream.Read(dataArray, (int)0, 19);
					s = new UTF8Encoding(true).GetString(dataArray, 0, 19);
					m_w.Text = s.Substring(9, 4);
					m_h.Text = s.Substring(13, 4);
                    fog.SelectedIndex = Int32.Parse(s.Substring(17, 1)) % fog.Items.Count;
					floor.SelectedIndex = Int32.Parse(s.Substring(18, 1));
					for (long i = 19; i < fileStream.Length; i += 15)
					{
						fileStream.Seek(i, SeekOrigin.Begin);
						fileStream.Read(dataArray, (int)0, 15);
						s = new UTF8Encoding(true).GetString(dataArray, 0, 15);

						cX = Int32.Parse(s.Substring(0, 4));
						cY = Int32.Parse(s.Substring(4, 4));
						R = Int32.Parse(s.Substring(8, 1));
						mdl_type= Int32.Parse(s.Substring(9, 1));
						mdl_no = Int32.Parse(s.Substring(10, 2));
						hlth = Int32.Parse(s.Substring(12, 3));

						list.Items.Add(cX.ToString().PadLeft(4) + "," +
						cY.ToString().PadLeft(4) + "," + R.ToString().PadLeft(1) + "," +
						mdl_type.ToString().PadLeft(1) + "," + mdl_no.ToString().PadLeft(2) + "," +
						hlth.ToString().PadLeft(3));
					}

				}
				Invalidate();
			}
		}

		private void m_w_TextChanged(object sender, EventArgs e)
		{
			if (m_w.Text.Length != 3)
				return;
			else
			{
				map_w = Int32.Parse(m_w.Text);
				if (map_w > 830)
				{
					map_w = 830;
					m_w.Text = "830";
					m_w.SelectionStart = 3;
				}
				else if (map_w < 100)
				{
					map_w = 100;
					m_w.Text = "100";
					m_w.SelectionStart = 3;
				}
			}
			Invalidate();
		}

		private void m_h_TextChanged(object sender, EventArgs e)
		{
			 if (m_h.Text.Length != 3)
				return;
			else
			{
				map_h = Int32.Parse(m_h.Text);
				if (map_h > 720)
				{
					map_h = 720;
					m_h.Text = "720";
					m_h.SelectionStart = 3;
				}
				else if (map_h < 100)
				{
					map_h = 100;
					m_h.Text = "100";
					m_h.SelectionStart = 3;
				}
			}
			Invalidate();
		}

		private void r_map_Click(object sender, EventArgs e)
		{
			list.Items.Clear();
			Invalidate();
		}

		private void list_SelectedIndexChanged(object sender, EventArgs e)
		{
			Invalidate();
		}

		private void shapes_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (model_type.SelectedIndex == 0) {
				model_name.Items.Clear();
				model_name.Items.Add("tris01");
				model_name.Items.Add("tris02");
				model_name.Items.Add("tris03");
				model_name.Items.Add("tris04");
				model_name.Items.Add("tris05");
				model_name.Items.Add("tris06");
				model_name.Items.Add("tris07");
				model_name.Items.Add("tris08");
				model_name.Items.Add("tris09");
				radius.Items.Clear();
				radius.Items.Add(1);
				radius.Items.Add(2);
			}
			else if (model_type.SelectedIndex == 1)
			{
				model_name.Items.Clear();
				model_name.Items.Add("model00");
				model_name.Items.Add("model01");
				model_name.Items.Add("model02");
				model_name.Items.Add("model03");
				model_name.Items.Add("model04");
				model_name.Items.Add("model05");
				model_name.Items.Add("model06");
				model_name.Items.Add("model07");
				model_name.Items.Add("model08");
				model_name.Items.Add("model09");
				model_name.Items.Add("model10");
				model_name.Items.Add("model11");
				radius.Items.Clear();
				for(int i=1; i<=8; i++)
					radius.Items.Add(i.ToString());
			}
			else
			{
				model_name.Items.Clear();
				model_name.Items.Add("Hero");
				radius.Items.Clear();
				radius.Items.Add(1);
			}
			model_name.SelectedIndex = 0;
			radius.SelectedIndex = 0;
		}
	}
}