namespace Map
{
	partial class Form1
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.list = new System.Windows.Forms.ListBox();
			this.radius = new System.Windows.Forms.ComboBox();
			this.del_obj = new System.Windows.Forms.Button();
			this.save_file = new System.Windows.Forms.Button();
			this.model_type = new System.Windows.Forms.ComboBox();
			this.load_file = new System.Windows.Forms.Button();
			this.f_save = new System.Windows.Forms.SaveFileDialog();
			this.f_load = new System.Windows.Forms.OpenFileDialog();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.m_w = new System.Windows.Forms.TextBox();
			this.m_h = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.r_map = new System.Windows.Forms.Button();
			this.label6 = new System.Windows.Forms.Label();
			this.cnt = new System.Windows.Forms.TextBox();
			this.model_name = new System.Windows.Forms.ComboBox();
			this.health = new System.Windows.Forms.ComboBox();
			this.fog = new System.Windows.Forms.ComboBox();
			this.label4 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.label8 = new System.Windows.Forms.Label();
			this.label9 = new System.Windows.Forms.Label();
			this.floor = new System.Windows.Forms.ComboBox();
			this.label10 = new System.Windows.Forms.Label();
			this.SuspendLayout();
// 
// list
// 
			this.list.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
			this.list.FormattingEnabled = true;
			this.list.ItemHeight = 14;
			this.list.Location = new System.Drawing.Point(847, 187);
			this.list.Margin = new System.Windows.Forms.Padding(3, 1, 3, 2);
			this.list.Name = "list";
			this.list.Size = new System.Drawing.Size(166, 466);
			this.list.TabIndex = 0;
			this.list.SelectedIndexChanged += new System.EventHandler(this.list_SelectedIndexChanged);
// 
// radius
// 
			this.radius.FormattingEnabled = true;
			this.radius.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8"});
			this.radius.Location = new System.Drawing.Point(848, 99);
			this.radius.Margin = new System.Windows.Forms.Padding(3, 3, 3, 1);
			this.radius.Name = "radius";
			this.radius.Size = new System.Drawing.Size(51, 21);
			this.radius.TabIndex = 1;
// 
// del_obj
// 
			this.del_obj.Location = new System.Drawing.Point(847, 657);
			this.del_obj.Margin = new System.Windows.Forms.Padding(3, 1, 1, 2);
			this.del_obj.Name = "del_obj";
			this.del_obj.Size = new System.Drawing.Size(80, 22);
			this.del_obj.TabIndex = 2;
			this.del_obj.Text = "Delete Object";
			this.del_obj.Click += new System.EventHandler(this.del_obj_Click);
// 
// save_file
// 
			this.save_file.Enabled = false;
			this.save_file.Location = new System.Drawing.Point(847, 683);
			this.save_file.Margin = new System.Windows.Forms.Padding(3, 2, 2, 3);
			this.save_file.Name = "save_file";
			this.save_file.Size = new System.Drawing.Size(80, 22);
			this.save_file.TabIndex = 3;
			this.save_file.Text = "Save MAP";
			this.save_file.Click += new System.EventHandler(this.save_file_Click);
// 
// model_type
// 
			this.model_type.FormattingEnabled = true;
			this.model_type.Items.AddRange(new object[] {
            "Animated Models",
            "Static Models",
            "Hero"});
			this.model_type.Location = new System.Drawing.Point(906, 99);
			this.model_type.Margin = new System.Windows.Forms.Padding(3, 3, 3, 1);
			this.model_type.Name = "model_type";
			this.model_type.Size = new System.Drawing.Size(108, 21);
			this.model_type.TabIndex = 4;
			this.model_type.SelectedIndexChanged += new System.EventHandler(this.shapes_SelectedIndexChanged);
// 
// load_file
// 
			this.load_file.Location = new System.Drawing.Point(933, 683);
			this.load_file.Margin = new System.Windows.Forms.Padding(1, 2, 3, 3);
			this.load_file.Name = "load_file";
			this.load_file.Size = new System.Drawing.Size(80, 22);
			this.load_file.TabIndex = 5;
			this.load_file.Text = "Load MAP";
			this.load_file.Click += new System.EventHandler(this.load_file_Click);
// 
// f_save
// 
			this.f_save.DefaultExt = "*.map";
			this.f_save.Filter = "RehoGame MAP files (*.map)|*.map|All files (*.*)|*.*";
			this.f_save.InitialDirectory = "c:\\\\";
// 
// f_load
// 
			this.f_load.Filter = "RehoGame MAP files (*.map)|*.map|All files (*.*)|*.*";
// 
// label1
// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(847, 170);
			this.label1.Margin = new System.Windows.Forms.Padding(3, 1, 3, 1);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(58, 14);
			this.label1.TabIndex = 6;
			this.label1.Text = "Object List";
// 
// label2
// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(846, 85);
			this.label2.Margin = new System.Windows.Forms.Padding(3, 1, 3, 3);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(62, 14);
			this.label2.TabIndex = 7;
			this.label2.Text = "Object Size";
// 
// label3
// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(907, 85);
			this.label3.Margin = new System.Windows.Forms.Padding(3, 1, 3, 3);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(65, 14);
			this.label3.TabIndex = 8;
			this.label3.Text = "Object Type";
// 
// m_w
// 
			this.m_w.AutoSize = false;
			this.m_w.Location = new System.Drawing.Point(848, 20);
			this.m_w.Margin = new System.Windows.Forms.Padding(3, 1, 1, 0);
			this.m_w.MaxLength = 3;
			this.m_w.Multiline = true;
			this.m_w.Name = "m_w";
			this.m_w.Size = new System.Drawing.Size(35, 18);
			this.m_w.TabIndex = 10;
			this.m_w.Text = "830";
			this.m_w.TextChanged += new System.EventHandler(this.m_w_TextChanged);
// 
// m_h
// 
			this.m_h.AutoSize = false;
			this.m_h.Location = new System.Drawing.Point(906, 22);
			this.m_h.Margin = new System.Windows.Forms.Padding(0, 1, 2, 1);
			this.m_h.MaxLength = 3;
			this.m_h.Multiline = true;
			this.m_h.Name = "m_h";
			this.m_h.Size = new System.Drawing.Size(31, 17);
			this.m_h.TabIndex = 11;
			this.m_h.Text = "720";
			this.m_h.TextChanged += new System.EventHandler(this.m_h_TextChanged);
// 
// label5
// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(846, 6);
			this.label5.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(33, 14);
			this.label5.TabIndex = 12;
			this.label5.Text = "Width";
// 
// r_map
// 
			this.r_map.Location = new System.Drawing.Point(933, 657);
			this.r_map.Margin = new System.Windows.Forms.Padding(1, 1, 3, 2);
			this.r_map.Name = "r_map";
			this.r_map.Size = new System.Drawing.Size(80, 22);
			this.r_map.TabIndex = 14;
			this.r_map.Text = "Reset MAP";
			this.r_map.Click += new System.EventHandler(this.r_map_Click);
// 
// label6
// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(959, 6);
			this.label6.Margin = new System.Windows.Forms.Padding(3, 3, 1, 1);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(44, 14);
			this.label6.TabIndex = 15;
			this.label6.Text = "Counter";
// 
// cnt
// 
			this.cnt.AutoSize = false;
			this.cnt.Location = new System.Drawing.Point(961, 22);
			this.cnt.Margin = new System.Windows.Forms.Padding(2, 1, 3, 0);
			this.cnt.MaxLength = 3;
			this.cnt.Multiline = true;
			this.cnt.Name = "cnt";
			this.cnt.ReadOnly = true;
			this.cnt.Size = new System.Drawing.Size(29, 17);
			this.cnt.TabIndex = 16;
// 
// model_name
// 
			this.model_name.FormattingEnabled = true;
			this.model_name.Location = new System.Drawing.Point(848, 137);
			this.model_name.Name = "model_name";
			this.model_name.Size = new System.Drawing.Size(90, 21);
			this.model_name.TabIndex = 17;
// 
// health
// 
			this.health.FormattingEnabled = true;
			this.health.Items.AddRange(new object[] {
            "50",
            "100",
            "150",
            "200",
            "250",
            "300",
            "500",
            "750",
            "999"});
			this.health.Location = new System.Drawing.Point(957, 137);
			this.health.Margin = new System.Windows.Forms.Padding(1, 3, 3, 3);
			this.health.Name = "health";
			this.health.Size = new System.Drawing.Size(51, 21);
			this.health.TabIndex = 18;
// 
// fog
// 
			this.fog.FormattingEnabled = true;
			this.fog.Items.AddRange(new object[] {
            "BLACK",
            "WHITE",
            "RED"});
			this.fog.Location = new System.Drawing.Point(846, 58);
			this.fog.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
			this.fog.Name = "fog";
			this.fog.Size = new System.Drawing.Size(61, 21);
			this.fog.TabIndex = 19;
// 
// label4
// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(845, 43);
			this.label4.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(54, 14);
			this.label4.TabIndex = 20;
			this.label4.Text = "Fog Color";
// 
// label7
// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(846, 123);
			this.label7.Margin = new System.Windows.Forms.Padding(3, 1, 3, 3);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(68, 14);
			this.label7.TabIndex = 21;
			this.label7.Text = "Model Name";
// 
// label8
// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(955, 123);
			this.label8.Margin = new System.Windows.Forms.Padding(3, 1, 3, 3);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(37, 14);
			this.label8.TabIndex = 22;
			this.label8.Text = "Health";
// 
// label9
// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(925, 43);
			this.label9.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(71, 14);
			this.label9.TabIndex = 23;
			this.label9.Text = "Floor Texture";
// 
// floor
// 
			this.floor.FormattingEnabled = true;
			this.floor.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9"});
			this.floor.Location = new System.Drawing.Point(926, 58);
			this.floor.Name = "floor";
			this.floor.Size = new System.Drawing.Size(80, 21);
			this.floor.TabIndex = 24;
// 
// label10
// 
			this.label10.AutoSize = true;
			this.label10.Location = new System.Drawing.Point(904, 6);
			this.label10.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(37, 14);
			this.label10.TabIndex = 25;
			this.label10.Text = "Height";
// 
// Form1
// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(1016, 741);
			this.Controls.Add(this.label10);
			this.Controls.Add(this.floor);
			this.Controls.Add(this.label9);
			this.Controls.Add(this.label8);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.fog);
			this.Controls.Add(this.health);
			this.Controls.Add(this.model_name);
			this.Controls.Add(this.cnt);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.r_map);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.m_h);
			this.Controls.Add(this.m_w);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.load_file);
			this.Controls.Add(this.model_type);
			this.Controls.Add(this.save_file);
			this.Controls.Add(this.del_obj);
			this.Controls.Add(this.radius);
			this.Controls.Add(this.list);
			this.MaximumSize = new System.Drawing.Size(1024, 768);
			this.MinimumSize = new System.Drawing.Size(1024, 768);
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "RehoGames MAP Editor  beta v6.08";
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.OnPaint);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_Move);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form1_Down);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ListBox list;
		private System.Windows.Forms.ComboBox radius;
		private System.Windows.Forms.Button del_obj;
		private System.Windows.Forms.Button save_file;
		private System.Windows.Forms.ComboBox model_type;
		private System.Windows.Forms.Button load_file;
		private System.Windows.Forms.SaveFileDialog f_save;
		private System.Windows.Forms.OpenFileDialog f_load;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox m_w;
		private System.Windows.Forms.TextBox m_h;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Button r_map;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.TextBox cnt;
		private System.Windows.Forms.ComboBox model_name;
		private System.Windows.Forms.ComboBox health;
		private System.Windows.Forms.ComboBox fog;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.ComboBox floor;
		private System.Windows.Forms.Label label10;


	}

}

	