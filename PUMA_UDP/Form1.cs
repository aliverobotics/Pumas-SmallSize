using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;
using System.Net;
using System.Globalization;
using System.Threading;
using ZalSystem;
using Matrices;

namespace Gecko
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
    
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox serverIPTextBox;
        private System.Windows.Forms.TextBox consoleTextBox;
		private System.Windows.Forms.Button startButton;
		private System.Windows.Forms.Label label2;
		
		private ZalRobot mrRoboto;			//Robot instance
		
		private UDPClientASocket clientSoc;	//Socket para recibir datos de vision
        private UDPClientASocket ctrlSoc;   //Socket para recibir comandos de control

        RobotDatos[] RobotsDatos;           //para los datos de todos robots
        //int TotalRobots;                 //total de robots detectados

        Pelota Balon;                       //Estructura para los datos del balon
        Cancha Field;                       //Estructura para los datos propios de la cancha
        String inMessageTMPCtrl;                //cadena temporal para verificar si llego la misma cadena
        String inMessageTMPVision;                //cadena temporal para verificar si llego la misma cadena
        Int32 MotorPower;                        //Max Potencia para los motores
        Int32 TurnPower;                    //Porcentaje durante giros
        Int32 FactorCorr;                   //Factor de correccion para los motores
        int behavior;
        bool reset;
        bool autoshoot;
        bool autoctrl;
        int target;
        bool shooter;
        bool stopped;
        bool pumas_team;
        bool potencials;
        bool going_left; //false for Pumas
        double   [] vel;  //izq der
        double[] data;  //new velovcitires
        double[] repulsion;
        int frictionOffset;
        bool robonova;

        bool Prediction;
        string szDataV;
        bool go_and_predict;

        /// <summary>
        /// Time in Miliseconds
        /// </summary>
        int PredictionTime;

        float countPak;//counter to get the FPS
        float countTotPak;//counter for all the packets
        float TimeBetweenSocks;
        
        

        /// <summary>
        /// Enviar al Simulador Tmb?
        /// </summary>
        bool toSim;
        SocketPack tmpSockVISION;
		#region Constants

        const int base10 = 10;
        char[] cHexa = new char[]{'A','B','C','D','E','F'};
        int[] iHexaNumeric = new int[] {10,11,12,13,14,15};
        int[] iHexaIndices = new int[] {0,1,2,3,4,5};
        const int asciiDiff = 48;

		private char	_charSPACE = ' ';
		private string	_strENTER = "\r\n";
		private char    _charNULL  = '\0';
		private System.Windows.Forms.TextBox sendPortTextBox;
		private System.Windows.Forms.Label sendPortLabel;
		private System.Windows.Forms.TextBox rcvPortTextBox;
        private System.Windows.Forms.Label rcvPortLabel;
        //private int		_intERROR = -1;
        private MainMenu mainMenu1;
        private TextBox rcvCtrlPortText;
        private Label label14;
        private TextBox sndCtrlPorttextbox;
        private Label label1;
        private ComboBox cboRobNum;
        private Label label15;
        private ComboBox cboPow;
        private Label label16;
        private Label label17;
        private ComboBox cboTurn;
        private TrackBar trkPow;
        private Label label10;
        private CheckBox chkPuma;
        private CheckBox chkLeft;
        private MenuItem mnuMode;
        private MenuItem mnuRAW;
        private MenuItem mnuNN;
        private CheckBox chktoSim;
        private CheckBox checkBox1;
        private Label label3;
        private TrackBar trkPotenciales;
        private Label label4;
        private CheckBox checkBox2;
        private ComboBox cboPred;
        private System.Windows.Forms.Timer timer1;
        private Label label5;
        private TrackBar trkFriction;
        private CheckBox chkNova;
		
		private string[][] IPPortChart = {new string[]{"127.0.0.1"},
										  new string[]{"111", "8001", "8002", "ggp"},
										  new string[]{"112", "8003", "8004", "ggb"},
										  new string[]{"113", "8005", "8006", "gpb"},
										  new string[]{"114", "8007", "8008", "ppg"},
										  new string[]{"115", "8009", "8010", "ppb"}};
		#endregion

		public Form1()//constructor
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			this.mrRoboto				= new ZalRobot();
			this.clientSoc				= new UDPClientASocket(System.Text.Encoding.ASCII);
            ctrlSoc                     = new UDPClientASocket(System.Text.Encoding.ASCII);
            
            
            vel = new double[2] { 0, 0 };
            data = new double[2] { 0, 0 };
            repulsion = new double[2] { 0, 0 };
            pumas_team = true;
            autoshoot = true;

            nRepulsion = 50;//Campos potenciales
            reset = false;
            autoshoot = true;
            autoctrl = true;
            target = -1; //ball
            behavior = 1; //default attack
            shooter = false;
            //ADAL, before true
            stopped = false;
            pumas_team = true;
            going_left = false; //false for Pumas
            NNMode = true;//start with neural networks
            RAWMode = false;
            MotorPower = 255;//maximum motor power
            TurnPower = 80;//80% of power for turning
            FactorCorr = 50;
            potencials = true;
            frictionOffset = 0;
            Prediction = false;
            go_and_predict = false;
            robonova = true;

            RobotsDatos = new RobotDatos[10];
            TimeBetweenSocks = 0;
            
            PredictionTime = 100;
            cboPred.Text = (100).ToString();
            countTotPak = 0;
            countPak = 0;
            
            
			InitIPPortGUI();
            
			this.clientSoc.DataReceived	+= new Gecko.UDPClientASocket.DataReceivedEventHandler(clientSoc_DataReceived);
			this.clientSoc.SocketClosed	+= new Gecko.UDPClientASocket.SocketClosedEventHandler(clientSoc_SocketClosed);
            ctrlSoc.SocketClosed += new Gecko.UDPClientASocket.SocketClosedEventHandler(clientSoc_SocketClosed);
            ctrlSoc.DataReceived += new Gecko.UDPClientASocket.DataReceivedEventHandler(ctrlSoc_DataReceived);
			
			this.KeyDown += new KeyEventHandler(Form1_KeyDown);

		}
		
		void InitIPPortGUI()
		{
			try
			{
				string myIP = clientSoc.GetMyIP().ToString();
				string[] temp = myIP.Split('.');
				string localNum = temp[temp.Length-1];
                this.Text = myIP;
				for(int n=0; n<IPPortChart.Length; n++)
				{
					if(localNum == IPPortChart[n][0])
					{
						if(n==0)
						{
							MessageBox.Show("No esta conectado a una red\r\n Se cerrara el programa");
							this.Close();
						}
						this.Text =  myIP;
						this.rcvPortTextBox.Text = IPPortChart[n][1];
						this.sendPortTextBox.Text = IPPortChart[n][2];
						break;
					}
				}
			}
			catch(Exception){
			}
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		///
		protected override void Dispose( bool disposing )
		{
			//clientSoc.Close();
            //ctrlSoc.Close();
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.sendPortTextBox = new System.Windows.Forms.TextBox();
            this.sendPortLabel = new System.Windows.Forms.Label();
            this.startButton = new System.Windows.Forms.Button();
            this.consoleTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.serverIPTextBox = new System.Windows.Forms.TextBox();
            this.rcvPortTextBox = new System.Windows.Forms.TextBox();
            this.rcvPortLabel = new System.Windows.Forms.Label();
            this.mainMenu1 = new System.Windows.Forms.MainMenu();
            this.mnuMode = new System.Windows.Forms.MenuItem();
            this.mnuRAW = new System.Windows.Forms.MenuItem();
            this.mnuNN = new System.Windows.Forms.MenuItem();
            this.rcvCtrlPortText = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.sndCtrlPorttextbox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cboRobNum = new System.Windows.Forms.ComboBox();
            this.label15 = new System.Windows.Forms.Label();
            this.cboPow = new System.Windows.Forms.ComboBox();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.cboTurn = new System.Windows.Forms.ComboBox();
            this.trkPow = new System.Windows.Forms.TrackBar();
            this.label10 = new System.Windows.Forms.Label();
            this.chkPuma = new System.Windows.Forms.CheckBox();
            this.chkLeft = new System.Windows.Forms.CheckBox();
            this.chktoSim = new System.Windows.Forms.CheckBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.trkPotenciales = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.cboPred = new System.Windows.Forms.ComboBox();
            this.timer1 = new System.Windows.Forms.Timer();
            this.label5 = new System.Windows.Forms.Label();
            this.trkFriction = new System.Windows.Forms.TrackBar();
            this.chkNova = new System.Windows.Forms.CheckBox();
            // 
            // sendPortTextBox
            // 
            this.sendPortTextBox.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.sendPortTextBox.Location = new System.Drawing.Point(54, 28);
            this.sendPortTextBox.Size = new System.Drawing.Size(56, 19);
            this.sendPortTextBox.Text = "8001";
            // 
            // sendPortLabel
            // 
            this.sendPortLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Bold);
            this.sendPortLabel.ForeColor = System.Drawing.Color.White;
            this.sendPortLabel.Location = new System.Drawing.Point(0, 30);
            this.sendPortLabel.Size = new System.Drawing.Size(48, 21);
            this.sendPortLabel.Text = "VISION Snd Port:";
            // 
            // startButton
            // 
            this.startButton.Location = new System.Drawing.Point(197, 3);
            this.startButton.Size = new System.Drawing.Size(40, 21);
            this.startButton.Text = "Start";
            this.startButton.Click += new System.EventHandler(this.startButton_Click);
            // 
            // consoleTextBox
            // 
            this.consoleTextBox.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.consoleTextBox.Location = new System.Drawing.Point(0, 76);
            this.consoleTextBox.Multiline = true;
            this.consoleTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.consoleTextBox.Size = new System.Drawing.Size(240, 33);
            // 
            // label2
            // 
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.label2.ForeColor = System.Drawing.Color.White;
            this.label2.Location = new System.Drawing.Point(5, 3);
            this.label2.Size = new System.Drawing.Size(64, 16);
            this.label2.Text = "Remote IP:";
            // 
            // serverIPTextBox
            // 
            this.serverIPTextBox.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.serverIPTextBox.Location = new System.Drawing.Point(77, 3);
            this.serverIPTextBox.Size = new System.Drawing.Size(114, 19);
            this.serverIPTextBox.Text = "192.168.213.150";
            // 
            // rcvPortTextBox
            // 
            this.rcvPortTextBox.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.rcvPortTextBox.Location = new System.Drawing.Point(54, 52);
            this.rcvPortTextBox.Size = new System.Drawing.Size(56, 19);
            this.rcvPortTextBox.Text = "8011";
            // 
            // rcvPortLabel
            // 
            this.rcvPortLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Bold);
            this.rcvPortLabel.ForeColor = System.Drawing.Color.White;
            this.rcvPortLabel.Location = new System.Drawing.Point(0, 52);
            this.rcvPortLabel.Size = new System.Drawing.Size(40, 21);
            this.rcvPortLabel.Text = "VISION Rcv Port:";
            // 
            // mainMenu1
            // 
            this.mainMenu1.MenuItems.Add(this.mnuMode);
            // 
            // mnuMode
            // 
            this.mnuMode.MenuItems.Add(this.mnuRAW);
            this.mnuMode.MenuItems.Add(this.mnuNN);
            this.mnuMode.Text = "Mode";
            // 
            // mnuRAW
            // 
            this.mnuRAW.Text = "RAW/MV Mode";
            this.mnuRAW.Click += new System.EventHandler(this.mnuRAW_Click);
            // 
            // mnuNN
            // 
            this.mnuNN.Checked = true;
            this.mnuNN.Text = "Neural Networks";
            this.mnuNN.Click += new System.EventHandler(this.mnuNN_Click);
            // 
            // rcvCtrlPortText
            // 
            this.rcvCtrlPortText.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.rcvCtrlPortText.Location = new System.Drawing.Point(189, 52);
            this.rcvCtrlPortText.Size = new System.Drawing.Size(48, 19);
            this.rcvCtrlPortText.Text = "6001";
            // 
            // label14
            // 
            this.label14.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Bold);
            this.label14.ForeColor = System.Drawing.Color.White;
            this.label14.Location = new System.Drawing.Point(138, 28);
            this.label14.Size = new System.Drawing.Size(50, 21);
            this.label14.Text = "CTRL Snd Port:";
            // 
            // sndCtrlPorttextbox
            // 
            this.sndCtrlPorttextbox.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.sndCtrlPorttextbox.Location = new System.Drawing.Point(189, 28);
            this.sndCtrlPorttextbox.Size = new System.Drawing.Size(48, 19);
            this.sndCtrlPorttextbox.Text = "6000";
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Bold);
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(138, 50);
            this.label1.Size = new System.Drawing.Size(40, 21);
            this.label1.Text = "CTRL Rcv Port:";
            // 
            // cboRobNum
            // 
            this.cboRobNum.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.cboRobNum.Items.Add("0");
            this.cboRobNum.Items.Add("1");
            this.cboRobNum.Items.Add("2");
            this.cboRobNum.Items.Add("3");
            this.cboRobNum.Items.Add("4");
            this.cboRobNum.Location = new System.Drawing.Point(48, 112);
            this.cboRobNum.Size = new System.Drawing.Size(42, 20);
            this.cboRobNum.SelectedIndexChanged += new System.EventHandler(this.cboRobNum_SelectedIndexChanged);
            // 
            // label15
            // 
            this.label15.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.label15.ForeColor = System.Drawing.Color.White;
            this.label15.Location = new System.Drawing.Point(3, 112);
            this.label15.Size = new System.Drawing.Size(56, 17);
            this.label15.Text = "RobNum";
            // 
            // cboPow
            // 
            this.cboPow.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.cboPow.Items.Add("0");
            this.cboPow.Items.Add("50");
            this.cboPow.Items.Add("100");
            this.cboPow.Items.Add("150");
            this.cboPow.Items.Add("200");
            this.cboPow.Items.Add("255");
            this.cboPow.Location = new System.Drawing.Point(196, 112);
            this.cboPow.Size = new System.Drawing.Size(41, 20);
            this.cboPow.SelectedIndexChanged += new System.EventHandler(this.cboPow_SelectedIndexChanged);
            // 
            // label16
            // 
            this.label16.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.label16.ForeColor = System.Drawing.Color.White;
            this.label16.Location = new System.Drawing.Point(157, 114);
            this.label16.Size = new System.Drawing.Size(43, 18);
            this.label16.Text = "Power";
            // 
            // label17
            // 
            this.label17.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.label17.ForeColor = System.Drawing.Color.White;
            this.label17.Location = new System.Drawing.Point(5, 135);
            this.label17.Size = new System.Drawing.Size(74, 20);
            this.label17.Text = "TurnPow(%)";
            // 
            // cboTurn
            // 
            this.cboTurn.Items.Add("0");
            this.cboTurn.Items.Add("20");
            this.cboTurn.Items.Add("40");
            this.cboTurn.Items.Add("60");
            this.cboTurn.Items.Add("80");
            this.cboTurn.Items.Add("100");
            this.cboTurn.Location = new System.Drawing.Point(73, 135);
            this.cboTurn.Size = new System.Drawing.Size(37, 22);
            this.cboTurn.SelectedIndexChanged += new System.EventHandler(this.cboTurn_SelectedIndexChanged);
            // 
            // trkPow
            // 
            this.trkPow.LargeChange = 10;
            this.trkPow.Location = new System.Drawing.Point(146, 154);
            this.trkPow.Maximum = 100;
            this.trkPow.Size = new System.Drawing.Size(88, 15);
            this.trkPow.Value = 50;
            this.trkPow.ValueChanged += new System.EventHandler(this.trkPow_ValueChanged);
            // 
            // label10
            // 
            this.label10.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.label10.ForeColor = System.Drawing.Color.White;
            this.label10.Location = new System.Drawing.Point(138, 140);
            this.label10.Size = new System.Drawing.Size(98, 14);
            this.label10.Text = "Balance Motores";
            // 
            // chkPuma
            // 
            this.chkPuma.Checked = true;
            this.chkPuma.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkPuma.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.chkPuma.ForeColor = System.Drawing.Color.White;
            this.chkPuma.Location = new System.Drawing.Point(96, 113);
            this.chkPuma.Size = new System.Drawing.Size(61, 18);
            this.chkPuma.Text = "Puma?";
            this.chkPuma.CheckStateChanged += new System.EventHandler(this.chkPuma_CheckStateChanged);
            // 
            // chkLeft
            // 
            this.chkLeft.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.chkLeft.ForeColor = System.Drawing.Color.White;
            this.chkLeft.Location = new System.Drawing.Point(3, 156);
            this.chkLeft.Size = new System.Drawing.Size(87, 21);
            this.chkLeft.Text = "Going Left?";
            this.chkLeft.CheckStateChanged += new System.EventHandler(this.checkBox1_CheckStateChanged);
            // 
            // chktoSim
            // 
            this.chktoSim.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.chktoSim.ForeColor = System.Drawing.Color.White;
            this.chktoSim.Location = new System.Drawing.Point(3, 174);
            this.chktoSim.Size = new System.Drawing.Size(97, 21);
            this.chktoSim.Text = "Send to Sim?";
            this.chktoSim.CheckStateChanged += new System.EventHandler(this.chktoSim_CheckStateChanged);
            // 
            // checkBox1
            // 
            this.checkBox1.Checked = true;
            this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox1.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.checkBox1.ForeColor = System.Drawing.Color.White;
            this.checkBox1.Location = new System.Drawing.Point(3, 192);
            this.checkBox1.Size = new System.Drawing.Size(97, 21);
            this.checkBox1.Text = "Potencials?";
            this.checkBox1.CheckStateChanged += new System.EventHandler(this.checkBox1_CheckStateChanged_1);
            // 
            // label3
            // 
            this.label3.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.label3.ForeColor = System.Drawing.Color.White;
            this.label3.Location = new System.Drawing.Point(138, 172);
            this.label3.Size = new System.Drawing.Size(98, 14);
            this.label3.Text = "Cal. Potenciales";
            // 
            // trkPotenciales
            // 
            this.trkPotenciales.LargeChange = 10;
            this.trkPotenciales.Location = new System.Drawing.Point(146, 186);
            this.trkPotenciales.Maximum = 100;
            this.trkPotenciales.Size = new System.Drawing.Size(88, 15);
            this.trkPotenciales.Value = 50;
            this.trkPotenciales.ValueChanged += new System.EventHandler(this.trkPotenciales_ValueChanged);
            // 
            // label4
            // 
            this.label4.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.label4.ForeColor = System.Drawing.Color.White;
            this.label4.Location = new System.Drawing.Point(5, 229);
            this.label4.Size = new System.Drawing.Size(98, 11);
            this.label4.Text = "T Prediction";
            // 
            // checkBox2
            // 
            this.checkBox2.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.checkBox2.ForeColor = System.Drawing.Color.White;
            this.checkBox2.Location = new System.Drawing.Point(3, 210);
            this.checkBox2.Size = new System.Drawing.Size(107, 21);
            this.checkBox2.Text = "Use Prediction?";
            this.checkBox2.CheckStateChanged += new System.EventHandler(this.checkBox2_CheckStateChanged);
            // 
            // cboPred
            // 
            this.cboPred.Items.Add("100");
            this.cboPred.Items.Add("200");
            this.cboPred.Items.Add("300");
            this.cboPred.Items.Add("400");
            this.cboPred.Items.Add("500");
            this.cboPred.Items.Add("600");
            this.cboPred.Items.Add("700");
            this.cboPred.Items.Add("800");
            this.cboPred.Items.Add("900");
            this.cboPred.Items.Add("1000");
            this.cboPred.Items.Add("1500");
            this.cboPred.Items.Add("2000");
            this.cboPred.Items.Add("2500");
            this.cboPred.Location = new System.Drawing.Point(5, 243);
            this.cboPred.Size = new System.Drawing.Size(96, 22);
            this.cboPred.SelectedIndexChanged += new System.EventHandler(this.cboPred_SelectedIndexChanged);
            // 
            // timer1
            // 
            this.timer1.Interval = 1000;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.label5.ForeColor = System.Drawing.Color.White;
            this.label5.Location = new System.Drawing.Point(139, 204);
            this.label5.Size = new System.Drawing.Size(98, 14);
            this.label5.Text = "Friction";
            // 
            // trkFriction
            // 
            this.trkFriction.LargeChange = 10;
            this.trkFriction.Location = new System.Drawing.Point(146, 216);
            this.trkFriction.Maximum = 100;
            this.trkFriction.Size = new System.Drawing.Size(88, 15);
            this.trkFriction.ValueChanged += new System.EventHandler(this.trkFriction_ValueChanged);
            // 
            // chkNova
            // 
            this.chkNova.Checked = true;
            this.chkNova.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkNova.Font = new System.Drawing.Font("Tahoma", 8F, System.Drawing.FontStyle.Regular);
            this.chkNova.ForeColor = System.Drawing.Color.White;
            this.chkNova.Location = new System.Drawing.Point(138, 243);
            this.chkNova.Size = new System.Drawing.Size(92, 18);
            this.chkNova.Text = "Robonova?";
            this.chkNova.CheckStateChanged += new System.EventHandler(this.chkNova_CheckStateChanged);
            // 
            // Form1
            // 
            this.BackColor = System.Drawing.Color.MediumSeaGreen;
            this.ClientSize = new System.Drawing.Size(240, 268);
            this.Controls.Add(this.chkNova);
            this.Controls.Add(this.cboTurn);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.trkFriction);
            this.Controls.Add(this.cboPred);
            this.Controls.Add(this.checkBox2);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.trkPotenciales);
            this.Controls.Add(this.chktoSim);
            this.Controls.Add(this.chkLeft);
            this.Controls.Add(this.chkPuma);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.trkPow);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.cboPow);
            this.Controls.Add(this.cboRobNum);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.sndCtrlPorttextbox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.rcvCtrlPortText);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.consoleTextBox);
            this.Controls.Add(this.startButton);
            this.Controls.Add(this.sendPortLabel);
            this.Controls.Add(this.sendPortTextBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.serverIPTextBox);
            this.Controls.Add(this.rcvPortTextBox);
            this.Controls.Add(this.rcvPortLabel);
            this.Controls.Add(this.checkBox1);
            this.ForeColor = System.Drawing.SystemColors.ActiveCaption;
            this.MaximizeBox = false;
            this.Menu = this.mainMenu1;
            this.MinimizeBox = false;
            this.Text = "Tracking UDP";
            this.Load += new System.EventHandler(this.Form1_Load);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void startButton_Click(object sender, System.EventArgs e)
		{
			IgniteTerminal();

            timer1.Enabled = true;

            Field.anchoX=20;
            Field.largoY=20;
            
            Cte.Matrix = new double[,] { { -0.836, -0.718 }, { -0.99, 0.757 }, { -1.078, -0.883 } };
            Cte2.Matrix = new double[,] { { 0.17, -1.557 }, { -1.623, 0.017 }, { -0.211, -0.353 } };
            Cte3.Matrix = new double[,] { { -0.511, -0.518 }, { -1.811, 1.790 }, { -0.484, -0.492 } };
            Cte4.Matrix = new double[,] { { -0.029, -1.212 }, { -1.210, -0.034 }, { -0.115, -0.114 } };

            /*///////////////////////////////////////////////////////////////
            if (mrRoboto.sPort.Opened)
            {
                consoleTextBox.Text = "Pto Abierto";
                mrRoboto.sPort.WriteString("5da00100ff");
                Thread.Sleep(15);
                mrRoboto.sPort.WriteString("5d0a0100ff");
                consoleTextBox.Text += mrRoboto.sPort.Read();
            }
            else
            {
                consoleTextBox.Text = "pto cerrado";
            }*/
		}

		private void IgniteTerminal()
		{
			mrRoboto.sPort.Open();
			startButton.Enabled = false;
			consoleTextBox.Text += "Waiting for connection..." + _strENTER;

			try
			{
				clientSoc.ConnectTo(serverIPTextBox.Text, Int32.Parse(sendPortTextBox.Text), Int32.Parse(rcvPortTextBox.Text));
                ctrlSoc.ConnectTo(serverIPTextBox.Text, Int32.Parse(sndCtrlPorttextbox.Text), Int32.Parse(rcvCtrlPortText.Text));
				consoleTextBox.Text += "Connected to VISION server @"
					+ sendPortTextBox.Text + _strENTER;
                consoleTextBox.Text += "Connected to CONTROL server @"
                    + sndCtrlPorttextbox.Text + _strENTER;
                consoleTextBox.Text += "Listening to VISION server @"
                    + rcvPortTextBox.Text + _strENTER;
                consoleTextBox.Text += "Listening to CONTROL server @"
                    + rcvCtrlPortText.Text + _strENTER;
                if (clientSoc.Connected && ctrlSoc.Connected)
                {
                    consoleTextBox.Text += "Vision AND Control connected\r\n";
                    clientSoc.WaitForData();
                    ctrlSoc.WaitForData();
                }
                else
                    consoleTextBox.Text += "Error al conectar a los servidores";
			}
			catch(Exception e234)
			{
				MessageBox.Show("Error while trying to connect:"+ _strENTER + e234.ToString());
				consoleTextBox.Text = e234.ToString();
				clientSoc.Close();
				startButton.Enabled = true;
			}
		}

		private void clientSoc_SocketClosed(object sender, SocketPack socPak)
		{
			consoleTextBox.Text += _strENTER + "Close Event occurred";
			//clientSoc.Close();
			startButton.Enabled = true;
			consoleTextBox.Text = "";
		}
        RobotDatos previousRobot;
        RobotDatos predictionRobot;
        private void DataReceivedAtnVISION()//SocketPack socPak1)
        {
            SocketPack socPak = tmpSockVISION;
            try
            {
                szDataV = clientSoc.DecodeBytesToString(socPak.dataBuffer);
                //PartialVisionParsing(szDataV);
                if (!szDataV.Equals(inMessageTMPVision))
                {//no hacer nada si se recibe la misma cadena
                    if (NNMode)//vision
                        ParseVisionData(szDataV);
                    inMessageTMPVision = szDataV;
                    if (go_and_predict && Prediction)
                    {
                        previousRobot.Angulo = mrRoboto.Angle;
                        previousRobot.posX = mrRoboto.X;
                        previousRobot.posY = mrRoboto.Y;
                        
                    }
                    go_and_predict = true;
                    if (!stopped)
                        Test_net(behavior);//GenRaw()
                }
                clientSoc.WaitForData();                
            }
            catch (System.NullReferenceException sne)
            {
                consoleTextBox.Text += "Error @ OnDataReceived NULL ref " + sne.ToString();
            }
            catch (System.Net.Sockets.SocketException)
            {
                consoleTextBox.Text = "Disconnected";
                clientSoc.Close();
            }
            catch (Exception ex)
            {
                consoleTextBox.Text += "Error @ OnDataReceived" + ex.ToString();
            }
        }
        
        /// <summary>
        /// Vision
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="socPak"></param>
		private void clientSoc_DataReceived(object sender, SocketPack socPak)
		{
            Thread newThread =
            new Thread(new ThreadStart(DataReceivedAtnVISION));
            tmpSockVISION = socPak;
            newThread.Start();
            countPak++;
            countTotPak++;
        }
        
        SocketPack tmpSockCTRL;
        /// <summary>
        /// Control
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="socPak"></param>
        private void ctrlSoc_DataReceived(object sender, SocketPack socPak)
        {
            Thread newThread =
            new Thread(new ThreadStart(DataReceivedAtnCTRL));
            tmpSockCTRL = socPak;
            newThread.Start();
        }
        #region partialvisionparsing
        /*
        private bool PartialVisionParsing(string inMessage)
        {
            String[] Token;
            String[] tmp;
            int j = 0;//contador auxiliar
            int i = 0;//contador aux
            int r = 0;//contador de robots
            bool parseerror = false;

            if (inMessage == null)
                return false;

            //First, we clean the message from those characters that we found useless
            inMessage = clientSoc.ReadUntil(inMessage, '\0'.ToString());
            inMessage = inMessage.TrimEnd(new char[] { _charSPACE, _charNULL });
            inMessage.ToUpper();
            consoleTextBox.Text += _strENTER + "VISION:" + inMessage;

            Token = inMessage.Split(new char[] { '\n', '\r', _charSPACE, _charNULL });

            #region TokensOK
            for (i = 0; i < Token.Length; i++)
            {//Contar cuantos NO nulos hay
                if (Token[i] != "")
                    j++;
            }
            tmp = new String[j];
            j = 0;
            for (i = 0; i < Token.Length; i++)
            {//Guardar sólo los NO nulos
                if (Token[i] != "")
                    tmp[j++] = Token[i];
            }

            Token = tmp;
            #endregion
            j = 0;//contador para token
            i = 0;//contador para robots

            
            if(Token[0].Equals("INFO")){
                //primero checar si ya existen los datos de ese robot para solo actualizarlos
                switch(Token[1]){
                    case "P"://pumas        INFO P ID X Y Vel Ang END
                        try
                        {
                            TotalRobots++;

                        }
                        catch (Exception)
                        {
                            consoleTextBox.Text = "Error while parsing";
                        }
                        break;
                    case "A"://contrario    INFO A X Y Vel END
                        try
                        {
                            TotalRobots++;
                        }
                        catch (Exception)
                        {
                            consoleTextBox.Text = "Error while parsing";
                        }
                        break;
                    case "B"://pelota       INFO B X Y Vel
                        try
                        {
                        }
                        catch (Exception)
                        {
                            consoleTextBox.Text = "Error while parsing";
                        }
                        break;
                }
            }
            else if(Token[0].Equals("FRAMEEND"))
                return true;
            return true;
        }
        */
        #endregion

        private double Normalize(double value, double m, double b)
        {
            double v = (float)0.0;
            //normalizing
            v = value*m+b;
            if (v > 1.0) v = (float)1.0;
            if (v < -1.0)v = (float)-1.0;
            return v;
        }

        Matriz Cte = new Matriz(3, 2);
        Matriz Cte2 = new Matriz(3, 2);
        Matriz Cte3 = new Matriz(3, 2);
        Matriz Cte4 = new Matriz(3, 2);

        private void Test_net(int behavior)
        {
            if (!robonova)
            {

                //we check for shooter
                if (autoshoot)
                    if (mrRoboto.Dist2tgt < 1.0)
                        /*if ((going_left == true && mrRoboto.Angle < (float)Math.PI * 3 / 4 && mrRoboto.Angle > (float)Math.PI / 4) ||
                         (going_left == false && mrRoboto.Angle < (float)-Math.PI / 4 && mrRoboto.Angle > (float)(-Math.PI * 3 / 4)) ||
                         (!going_left && mrRoboto.Angle > (float)(5*Math.PI) / 4 && mrRoboto.Angle < (Math.PI * 7 / 4))
                         )*/
                        if (robonova)
                            kick_l();
                        else
                            shoot_on();
                    else
                        if (!robonova)
                            shoot_off();

                if (reset)
                {
                    data[0] = 0.0;
                    data[1] = 0.0;
                    reset = false;
                    shoot_off();
                }
                else
                    if (behavior > 0)
                    {
                        //we test the net
                        if (behavior == 1 && (mrRoboto.Dist2tgt > 10.0 || Math.Abs(mrRoboto.Ang2tgt) > 1.0))
                            Try_Net(2); //approach
                        else
                            Try_Net(behavior);

                        /*string o0 = data[0].ToString();
                        string o1 = data[1].ToString();

                        consoleTextBox.Text +=
                            _strENTER + "OUTPUT NET => [" + o0 + "," + o1 + "]";*/

                        data[0] *= MotorPower;//generar velocidades
                        data[1] *= MotorPower;

                        if (potencials)
                        {
                            getRepulsion();
                            data[0] += repulsion[0] * (double)MotorPower / (double)512;
                            data[1] += repulsion[1] * (double)MotorPower / (double)512;
                        }
                    }
                    else
                    {
                        getRepulsion();
                        data[0] = repulsion[0];
                        data[1] = repulsion[1];
                    }

                {
                    /*
                    string o0 = data[0].ToString();
                    string o1 = data[1].ToString();
                    string v0 = vel[0].ToString();
                    string v1 = vel[1].ToString();

                    consoleTextBox.Text +=
                        _strENTER + "DATA => [" + o0 + "," + o1 + "] - " + v0 + "," + v1 + "]";
                    */
                }

                if (FactorCorr < 50)
                    data[1] *= FactorCorr / (100.0 - FactorCorr);
                else
                    data[0] *= (100.0 - FactorCorr) / (FactorCorr);

                if (data[0] * data[1] < 0.0)
                { //turning
                    data[0] *= TurnPower / 100.0;
                    data[1] *= TurnPower / 100.0;
                }

                //if (data[0] > 511)
                //    data[0] = 511;
                //if (data[0] < -511)
                //    data[0] = -511;
                //if (data[1] > 511)
                //    data[1] = 511;
                //if (data[1] < -511)
                //    data[1] = -511;

                //To Break the friction of the field
                data[0] += frictionOffset;
                data[1] += frictionOffset;
                if (data[0] > 255)
                    data[0] = 255;
                if (data[0] < -255)
                    data[0] = -255;
                if (data[1] > 255)
                    data[1] = 255;
                if (data[1] < -255)
                    data[1] = -255;

                if (Math.Abs(data[0] - vel[0]) > 5 || Math.Abs(data[1] - vel[1]) > 5)
                {
                    //we adjust new velocities
                    vel[0] = data[0];
                    vel[1] = data[1];

                    //string o0 = data[0].ToString();
                    //string o1 = data[1].ToString();

                    data[0] = Math.Abs(data[0]);
                    data[1] = Math.Abs(data[1]);

                    int data0 = Convert.ToInt16(data[0]);
                    int data1 = Convert.ToInt16(data[1]);
                    /*consoleTextBox.Text +=
                        _strENTER + "OUTPUT MOTORS => [" + o0 + "," + o1 + "]";*/

                    string va0 = data0.ToString("x4");
                    string v0a = data1.ToString("x4");

                    //va0 = CompleteString(va0);
                    //v0a = CompleteString(v0a);


                    string msg = "5da0" + getSigno(data[0]) + va0;

                    WriteString(msg);//

                    Thread.Sleep(30);//necesario para que el micro responda

                    msg = "5d0a" + getSigno(data[1]) + v0a;

                    WriteString(msg);
                    Thread.Sleep(20);
                }
            }
            else
            {
                //robonova control

                //kicking algorithm
                //if kick distance (15cms) 
                    //if left foot is closer
                        //approach to the ball until reaching left's foot angle
                    //else
                        //approach to the ball until reaching right's foot angle
                //if left foot is closer
                    //kick left
                //else
                    //kick right

                //we check for shooter
                if (autoshoot)
                    if (mrRoboto.Dist2tgt < 1.5)
                        /*if ((going_left == true && mrRoboto.Angle < (float)Math.PI * 3 / 4 && mrRoboto.Angle > (float)Math.PI / 4) ||
                         (going_left == false && mrRoboto.Angle < (float)-Math.PI / 4 && mrRoboto.Angle > (float)(-Math.PI * 3 / 4)) ||
                         (!going_left && mrRoboto.Angle > (float)(5*Math.PI) / 4 && mrRoboto.Angle < (Math.PI * 7 / 4))
                         )*/
                         if (mrRoboto.Ang2tgt >= 0)
                             if (mrRoboto.Ang2tgt < 0.3)
                                kick_l();
                             else
                                turn_l();
                         else
                            if (mrRoboto.Ang2tgt > -0.3)
                                kick_r();
                            else
                                turn_r();

                //approaching algorithm
                //repeat
                    //if too much distance to the ball (more than 40 cms)
                        //turn fast towards the ball
                        //run to the ball
                    //else
                        //repeat
                            //if too much angle difference
                                //turn fast towards the ball
                            //else
                                //turn slightly towards the ball 
                            //walk a step forward
                        //until beeing in front of the ball (15 cms)
                //until beeing in front of the ball (15 cms)
                            
                if (Math.Abs(mrRoboto.Ang2tgt) > 0.3) //15 degrees
                    if (Math.Abs(mrRoboto.Ang2tgt) > 0.5) //30 degrees
                        if (mrRoboto.Ang2tgt > 0) //must turn left
                            //left();
                            turn_l();
                        else
                            //right();
                            turn_r();
                    else
                        if (mrRoboto.Ang2tgt > 0) //must turn left
                            turn_l();
                        else
                            turn_r();
                else
                    if (mrRoboto.Dist2tgt > 1.0)
                        if (mrRoboto.Dist2tgt > 4.0)
                            forward_f();
                        else
                            step_f();
            }
        }
        /// <summary>
        /// Funcion q envia al puerto serie y via sockets cuando esta definido
        /// </summary>
        /// <param name="MSG">Mensaje a Enviar</param>
        private void WriteString(string MSG)
        {
            mrRoboto.sPort.WriteString(MSG);
            if (toSim)
                ctrlSoc.SendMsg("raw " + MSG + "\n\0");
        }

        private void SendString(string MSG)
        {
            mrRoboto.sPort.SendString(MSG);
            if (toSim)
                ctrlSoc.SendMsg(MSG + "\n\0");
        }

        float nRepulsion;
        private void getRepulsion()
        {
            int i;
            int index = mrRoboto.MyIdNum;
            if (!pumas_team) index += 5;

            float n; //factor para Frepul 
            float d0; //distancia minima al obstaculo
            float e1; //Constante Fatrac
            float e2; //Constante FatracLejos
            double DistMin; //Distancia minima para tomar Fatraclejos

            n = nRepulsion; //factor para Frepul 
            d0 = 1; //distancia minima al obstaculo
            if (behavior>0) { e1 = 10; e2 = 0; DistMin = 2; }
            else { e1 = 5; e2 = 2; DistMin = 20; } //only to get final approach to the ball
            //Distancia minima para tomar Fatraclejos

            //imprime_pos();
            float Fx = 0;
            float Fy = 0;
            float Xobst, Yobst;

            //we get the sum of forces
            for (i = -1; i < 10; i++)
                if (i != index)
                {
                    if (i < 0)
                    {
                        Xobst = Balon.posX;
                        Yobst = Balon.posY;
                    }
                    else
                    {
                        Xobst = RobotsDatos[i].posX;
                        Yobst = RobotsDatos[i].posY;
                    }

                    float VX = RobotsDatos[index].posX - Xobst;
                    float VY = RobotsDatos[index].posY - Yobst;
                    float dist = (float)Math.Sqrt(VX * VX + VY * VY);

                    if (i == target)
                    { //attractive force must go out
                        if (dist > DistMin)
                        { //fatraclejos
                            Fx += -1 * e2 * VX / dist;
                            Fy += -1 * e2 * VY / dist;
                        }
                        else
                        { //fatrac cerca
                            Fx += -1 * e1 * VX;
                            Fy += -1 * e1 * VY;
                        }
                    }
                    else
                    { //repulsive forces
                        Fx+= -1 * n * (1/dist - 1/d0) * 1/(dist*dist) * VX/dist;
                        Fy+= -1 * n * (1/dist - 1/d0) * 1/(dist*dist) * VY/dist;
                    }
                }

            float magnitude = (float)Math.Sqrt(Fx * Fx + Fy * Fy);
            float angulo_to_force = Angulo(Fx, Fy);
            float angulo_robot = deg2rad(mrRoboto.Angle);
            float delta_ang = normalizeAngle(angulo_to_force - angulo_robot);

            //only for debugging
            /*
            printf ("Angulo to pel = %f ", angulo_to_pel*180/M_PI);
            printf ("Angulo robot = %f ", angulo_robot*180/M_PI);
            printf ("deltaang  = %f\n ", delta_ang*180/M_PI);
            */

            repulsion[0] = magnitude * 10 * (Math.Cos(delta_ang) - Math.Sin(delta_ang)); //left velocity
            repulsion[1] = magnitude * 10 * (Math.Cos(delta_ang) + Math.Sin(delta_ang)); //right velocity

            //printf ("%5.3f\t%5.3f\t%5.3f ", dist, angulo_to_pel, angulo_robot/*delta_ang*/);
        }

        private void Try_Net(int behavior)
        {
            Matriz Intel = new Matriz(1, 3);
            Matriz Intel2 = new Matriz(1, 3);

            Matriz V1 = null, V2 = null;

            double dist = Normalize(mrRoboto.Dist2tgt, 0.1, -1.0);
            double angle = Normalize(mrRoboto.Ang2tgt, 0.318471, 0.0);

            /*consoleTextBox.Text +=
               _strENTER + "[" + mrRoboto.Dist2tgt.ToString() + "," +
                            mrRoboto.Ang2tgt.ToString() + "] ---> " +
                            " Normalized [" + dist.ToString() + "," + angle.ToString() + "]";*/

            Intel.Matrix = new double[,] { { dist, angle, (double)1 } };

            string i0 = Intel.Matrix[0, 0].ToString(); 
            string i1 = Intel.Matrix[0, 1].ToString();

            switch (behavior)
            {
                case 1://attack
                    V1 = Intel.Multiplicar(Cte);
                    break;
                case 2://approach
                    V1 = Intel.Multiplicar(Cte3);
                    break;
            }

            /*if (behavior == 2)
                consoleTextBox.Text += _strENTER + "evade";
            else
                consoleTextBox.Text += _strENTER + "attack";

            consoleTextBox.Text +=
               _strENTER + " layer 1 result [" + V1.Matrix[0,0].ToString() + "," + V1.Matrix[0,1].ToString() + "]";*/

            if (V1 != null)//si se multiplico correctamente:
            {
                V1.TanSigMat();

            /*consoleTextBox.Text +=
               _strENTER + " layer 1 tansig [" + V1.Matrix[0,0].ToString() + "," + V1.Matrix[0,1].ToString() + "]";*/

                Intel2.Matrix[0, 0] = V1.Matrix[0, 0];
                Intel2.Matrix[0, 1] = V1.Matrix[0, 1];
                Intel2.Matrix[0, 2] = 1;
                switch (behavior)
                {
                    case 1://attack
                        V2 = Intel2.Multiplicar(Cte2);
                        break;
                    case 2://approach
                        V2 = Intel2.Multiplicar(Cte4);
                        break;
                }

               /*consoleTextBox.Text +=
               _strENTER + " layer 2 result [" + V2.Matrix[0,0].ToString() + "," + V2.Matrix[0,1].ToString() + "]";

                V2.TanSigMat();

                   consoleTextBox.Text +=
               _strENTER + " layer 2 tansig [" + V2.Matrix[0,0].ToString() + "," + V2.Matrix[0,1].ToString() + "]";*/
            }

            data[0] = V2.Matrix[0, 0];//vels
            data[1] = V2.Matrix[0, 1];

            /*string o0 = data[0].ToString();
            string o1 = data[1].ToString();

            consoleTextBox.Text += 
                _strENTER + "INPUT NET ["+i0+","+i1+"] => ["+o0+","+o1+"]";*/
        }

        private string CompleteString(string str)
        {
            string tmp = str;
            switch (str.Length)
            {
                case 0:
                    str = "0000";
                    break;
                case 1:
                    str = "000"+tmp;
                    break;
                case 2:
                    str = "00"+tmp;
                    break;
                case 3:
                    str = "0"+tmp;
                    break;
            }
            return str;
        }

        private string DecimalToBase(int iDec, int numbase)
        {
            string strBin = "";
            int[] result = new int[32];
            int MaxBit = 32;
            for(; iDec > 0; iDec/=numbase)
            {
                int rem = iDec % numbase;
                result[--MaxBit] = rem;
            } 
            for (int i=0;i<result.Length;i++)
                  if ((int) result.GetValue(i) >= base10)
                    strBin += cHexa[(int)result.GetValue(i)%base10];
                  else
                    strBin += result.GetValue(i);
            strBin = strBin.TrimStart(new char[] {'0'});
            return strBin;
        }
        /// <summary>
        /// Devuelve el valor necesario para generar la cadena RAW en el robot
        /// </summary>
        /// <param name="p">Valor (double) a verificar</param>
        /// <returns>"01": si el signo es +; "02",si es negativo;"00" si es 0</returns>
        private string getSigno(double p)
        {
            if (p > 0)
                return "01";
            else if (p < 0)
                return "02";
            else
                return "00";
        }

        /// <summary>
        /// Contiene los datos recibidos por el socket de control
        /// </summary>
        string szDataC;
        private void DataReceivedAtnCTRL()
        {
            SocketPack socPak = tmpSockCTRL;
            try
            {
                szDataC = ctrlSoc.DecodeBytesToString(socPak.dataBuffer);
                if (inMessageTMPCtrl != szDataC)
                {//para disminuir procesamiento innecesario, si se recibe la misma cadena
                    ParseCtrlData(szDataC);//Parseo de datos de ctrl
                    inMessageTMPCtrl = szDataC;
                }
                ctrlSoc.WaitForData();
            }
            catch (System.NullReferenceException sne)
            {
                consoleTextBox.Text += "Error @ OnDataReceivedCtrl NULL ref " + sne.ToString();
            }
            catch (System.Net.Sockets.SocketException)
            {
                consoleTextBox.Text = "Disconnected";
                ctrlSoc.Close();
            }
            catch (Exception ex)
            {
                consoleTextBox.Text += "Error @ OnDataReceivedCtrl" + ex.ToString();
            }
        }


        #region visionparsing
        
		/// <summary>
		/// Interpreta el mensaje enviado por visión y obtiene los datos de todos los robots
        /// que están en la cancha y los de la pelota
		/// </summary>
		/// <param name="inMessage">string to be parsed</param>
		/// <returns>nothing</returns>
		public void ParseVisionData(string inMessage)
		{
            String[] Token;
            String[] tmp;
            int j = 0;//contador auxiliar
            int i = 0;//contador aux
            int r=0;//contador de robots
            bool parseerror=false;
            
			if(inMessage == null)
				return;

			//First, we clean the message from those characters that we found useless
			inMessage = clientSoc.ReadUntil(inMessage, '\0'.ToString());
			inMessage = inMessage.TrimEnd(new char[]{_charSPACE, _charNULL});
            inMessage.ToUpper();
            //consoleTextBox.Text += _strENTER + "VISION:" + inMessage;

            Token = inMessage.Split(new char[]{'\n','\r',_charSPACE,_charNULL,','});

            #region TokensOK
            for (i = 0; i < Token.Length; i++)
            {//Contar cuantos NO nulos hay
                if (Token[i] != "")
                    j++;
            }
            tmp = new String[j];
            j = 0;
            for (i = 0; i < Token.Length; i++)
            {//Guardar sólo los NO nulos
                if (Token[i] != "")
                    tmp[j++] = Token[i];
                if(Token[i]=="INFO")
                    r++;//contar cuantos INFOS hay
            }

            Token = tmp;
            #endregion
            j = 0;//contador para token
            i = 0;//contador para robots

            //RobotsDatos = new RobotDatos[10];//ya es fijo

            r = 0;
            //lstListaRobots.Items.Clear();
            NumberFormatInfo numInfo=new NumberFormatInfo();
            numInfo.CurrencyDecimalSeparator = ".";
            numInfo.CurrencyGroupSeparator = ",";
            

            if (Token.Length > 1)
            {
                //if (Token[j++] == "FRAMESTART")
                //{
                try
                {
                    while (j < Token.Length)
                    {
                        while (Token[j++].Equals("INFO") && !parseerror && !Token[j].Equals("FRAMEEND"))
                        {
                            if (Token[j].Equals("P"))//pumas        INFO P ID X Y Vel Ang END
                            {
                                try
                                {
                                    i = Int32.Parse(Token[++j], numInfo);
                                    RobotsDatos[i].Puma = true;
                                    RobotsDatos[i].RobNum = i;
                                    RobotsDatos[i].posX = float.Parse(Token[++j], numInfo);
                                    RobotsDatos[i].posY = float.Parse(Token[++j], numInfo);
                                    RobotsDatos[i].Velocidad = float.Parse(Token[++j], numInfo);
                                    RobotsDatos[i].Angulo = float.Parse(Token[++j], numInfo);
                                    if (RobotsDatos[i].RobNum == mrRoboto.MyIdNum)
                                    {
                                        mrRoboto.X = RobotsDatos[i].posX;
                                        mrRoboto.Y = RobotsDatos[i].posY;
                                        mrRoboto.Angle = RobotsDatos[i].Angulo;
                                        if (countPak > 0)//si hay valor para fps
                                        {
                                            TimeBetweenSocks = (countTotPak / countPak)*1000;
                                            countTotPak = 0;
                                        }

                                        //if (LastTime.Year!=1)
                                        //{
                                        //    TimeSpan diff = DateTime.Now - LastTime;
                                        //    TimeBetweenSocks = diff.TotalMilliseconds;
                                        //    consoleTextBox.Text = DateTime.Now.Millisecond.ToString();
                                        //}
                                        //LastTime = DateTime.Now;
                                    }
                                    r++;
                                    /*if (RobotsDatos.Length - 1 > i)
                                        i++;*/
                                    if (!Token[++j].Equals("END"))
                                        parseerror = true;
                                    //lstListaRobots.Items.Add("Rob#" + i);
                                }
                                catch (System.FormatException)
                                {
                                    //se recibio incorrectamente la cadena;
                                    //desechar el paquete recien recibido
                                    consoleTextBox.Text = _strENTER + "Error while parsing:" + Token[j - 1] + Token[j] + Token[j + 1] + _strENTER;
                                    parseerror = true;
                                }
                            }
                            else if (Token[j].Equals("A"))//contrario    INFO A X Y Vel END
                            {
                                try
                                {
                                    i = Int32.Parse(Token[++j], numInfo)+5;
                                    RobotsDatos[i].Puma = false;
                                    RobotsDatos[i].RobNum = i;
                                    RobotsDatos[i].posX = float.Parse(Token[++j], numInfo);
                                    RobotsDatos[i].posY = float.Parse(Token[++j], numInfo);
                                    RobotsDatos[i].Velocidad = float.Parse(Token[++j], numInfo);
                                    RobotsDatos[i].Angulo = float.Parse(Token[++j], numInfo);
                                    r++;
                                    /*if (RobotsDatos.Length - 1 > i)
                                        i++;*/
                                    if (!Token[++j].Equals("END"))
                                        parseerror = true;
                                    //lstListaRobots.Items.Add("Rob#" + i);
                                }
                                catch (System.FormatException)
                                {
                                    //se recibio incorrectamente la cadena;
                                    //desechar el paquete recien recibido
                                    consoleTextBox.Text = _strENTER + "Error while parsing:" + Token[j - 1] + Token[j] + Token[j + 1] + _strENTER;
                                    parseerror = true;
                                }
                            }
                            else if (Token[j].Equals("B"))
                            {
                                try
                                {
                                    Balon.posX = float.Parse(Token[++j], numInfo);
                                    Balon.posY = float.Parse(Token[++j], numInfo);
                                    Balon.Velocidad = float.Parse(Token[++j], numInfo);
                                    //Balon.Angulo = float.Parse(Token[++j]);
                                    if (!Token[++j].Equals("END"))
                                        parseerror = true;
                                }
                                catch (System.FormatException)
                                {
                                    //se recibio incorrectamente la cadena;
                                    //desechar el paquete recien recibido
                                    consoleTextBox.Text = _strENTER + "Error while parsing:" + Token[j - 1] + Token[j] + Token[j + 1] + _strENTER;
                                    parseerror = true;
                                }
                            }
                            else
                            {//ni jugador puma,enemigo o pelota: error
                                parseerror = true;
                                consoleTextBox.Text = _strENTER + "Error while parsing:" + Token[j - 1] + Token[j] + Token[j + 1] + _strENTER;
                            }
                            if (Token.Length - 1 > j && !parseerror)
                                j++;
                        }
                    }
                }
                catch (Exception)
                {
                    consoleTextBox.Text = "Incorrect DAta Received";
                }
                    //if (Token[j - 1] == "FRAMEEND" && i > 0 && !parseerror)
                    //{//se recibió la cadena correctamente

                GetRelDist(-1);//obtener los datos a la pelota

                
                //lblBallAng.Text = Balon.Angulo.ToString();
                //lblBallVel.Text = Balon.Velocidad.ToString();
                //lblBallX.Text = Balon.posX.ToString();
                //lblBallY.Text = Balon.posY.ToString();
                /*for (i = 0; i < 10; i++)
                    lstListaRobots.Items.Add("Rob#" + RobotsDatos[i].RobNum.ToString());*/

                    //}
                //}
            }
            //else : error no se recibio una cadena correcta no hacer nada.
            return;
		}
#endregion

        /// <summary>
        /// Realiza las acciones enviada por el socket de control
        /// </summary>
        /// <param name="inMessage">Mensage que fue recibido en el socket</param>
        public void ParseCtrlData(string inMessage)
        {
            String [] Token;
            int i,j=0;

            if(inMessage==null)
                return;
            //First, we clean the message from those characters that we found useless
            inMessage = this.ctrlSoc.ReadUntil(inMessage, '\0'.ToString());
            inMessage = inMessage.TrimEnd(new char[] { _charSPACE, _charNULL });

            consoleTextBox.Text += _strENTER + "CONTROL:" + inMessage;
            inMessage=inMessage.ToLower();
            Token = inMessage.Split(new char[] { '\n', '\r', _charSPACE, _charNULL });

            #region TokensOK
            for (i = 0; i < Token.Length; i++)
            {//Contar cuantos NO nulos hay
                if (Token[i] != "")
                    j++;
            }
            String[] tmp = new String[j];
            j = 0;
            for (i = 0; i < Token.Length; i++)
            {//Guardar sólo los NO nulos
                if (Token[i] != "")
                    tmp[j++] = Token[i];
            }
            Token = tmp;
            #endregion

            if (RAWMode)//activa comandos raw
                ParseActionData(inMessage);
            if(Token.Length>0)//si se recibio algo, continuar
                switch (Token[0])
                {
                    case "robnum":
                        try
                        {
                            mrRoboto.MyIdNum = Int32.Parse(Token[2]);
                            //consoleTextBox.Text = "ROBNUM:OK";
                        }
                        catch (System.FormatException)
                        {
                            consoleTextBox.Text += "ROBNUM:Failed";
                        }
                        break;
                    case "fielddim":
                        try
                        {
                            Field.anchoX = float.Parse(Token[2]);
                            Field.largoY = float.Parse(Token[3]);
                            //consoleTextBox.Text = "FIELDDIM:OK";
                        }
                        catch (Exception)
                        {
                            consoleTextBox.Text += "FIELDDIM:Failed";
                        }
                        break;
                    case "attack":
                        behavior = 1;
                        break;
                    case "approach":
                        behavior = 2;
                        break;
                    case "evade":
                        behavior = 0;
                        break;
                    case "go":
                        stopped = false;
                        break;
                    case "reverse":
                        reverse();
                        break;
                    case "rotl":
                        rotatel();
                        //consoleTextBox.Text = "rotl,recibido " + Token[0];
                        break;
                    case "rotr":
                        rotater();
                        break;
                    case "reset":
                        reset = true;
                        break;
                    case "goal":
                        stop();
                        rotatel();
                        Thread.Sleep(2000);
                        stop();
                        break;
                    case "shooton":
                        shoot_on();
                        break;
                    case "shootoff":
                        shoot_off();
                        break;
                    case "stop":
                        stop();
                        break;
                    case "youshoot":
                        autoshoot = true;
                        break;
                    case "dontshootn":
                        autoshoot = false;
                        break;
                    case "takectrl":
                        autoctrl = true;
                        //printf ("Control taken !.\n");
                        break;
                    case "givectrl":
                        autoctrl = false;
                        //printf ("Control given to Clips !\n");
                        break;
                    case "target":
                        if (Token[1] == "b")
                            target = -1;
                        else
                        {
                            if (Token[1].Substring(0, 1) == "p")
                                target = int.Parse(Token[1].Substring(1, 2));
                            else if (Token[1].Substring(0, 1) == "a")
                                target = int.Parse(Token[1].Substring(1, 2)) + 5;
                        }
                        break;
                    case "run":
                        this.consoleTextBox.Text += "run received\r\n";
                        forward_f();
                        break;
                    case "walk":
                        forward();
                        break;
                    case "escape":
                        back_f();
                        break;
                    case "back":
                        back();
                        break;
                    case "turnl":
                        turn_l();
                        break;
                    case "turnr":
                        turn_r();
                        break;
                    case "kickl":
                        kick_l();
                        break;
                    case "kickr":
                        kick_r();
                        break;
                    case "faceup":
                        face_up();
                        break;
                    case "facedown":
                        face_down();
                        break;
                    case "stepl":
                        step_l();
                        break;
                    case "stepr":
                        step_r();
                        break;
                    case "stepf":
                        step_f();
                        break;
                    case "stepb":
                        step_b();
                        break;
                    case "keepl":
                        keep_l();
                        break;
                    case "keepr":
                        keep_r();
                        break;
                    case "left":
                        left();
                        break;
                    case "right":
                        right();
                        break;
                    case "penalty":
                        forward_f();
                        Thread.Sleep(2000);
                        kick_l();
                        break;
                }
        }

        /// <summary>
        /// normaliza un angulo entre -pi y +pi
        /// </summary>
        public float normalizeAngle(float angle)
        {

            if (angle < -(float)Math.PI)
                angle += (float)(2.0 * Math.PI);

            if (angle >  (float)Math.PI)
                angle -= (float)(2.0 * Math.PI);

            return (angle);
        }

        /// <summary>
        /// funcion que devuelve el angulo de un vector en radianes
        /// </summary>
        public float Angulo(float VX, float VY)
        {
            if ((float)Math.Abs(VX) < (float)0.1 && (float)Math.Abs(VY) < (float)0.1) return 0;
            if (VX == (float)0)
                if (VY > (float)0)
                    return (float)Math.PI / (float)2.0; // pi/2 radianes = 90o
                else
                    return (float)1.5 * (float)Math.PI; //-pi / 2
            else
                if (VY == 0)
                    if (VX > 0)
                        return 0; // 0 radianes = 0o
                    else
                        return (float)Math.PI; //pi
                else
                {
                    double Theta = (float)Math.Atan(VY / VX);

                    if (VY < 0)
                        if (VX < 0)
                            return (float)Math.PI + (float)Theta; //3er cuadrante
                        else
                            return (float)2.0 * (float)Math.PI + (float)Theta; //4o cuadrante
                    else
                        if (VX < 0)
                            return (float)Math.PI + (float)Theta; //2o cuadrante
                        else
                            return (float)Theta; //1er cuadrante
                }
        }

        /// <summary>
        /// Convert from degrees to radians
        /// </summary>
        public float deg2rad(float deg)
        {
            return (deg * (float)Math.PI / (float)180.0);
        }

        

        /// <summary>
        /// Obtiene la distancia y el angulo del robot a la pelota
        /// y las pone en el objeto ZalRobot en las 
        /// respectivas variables
        /// </summary>
        public void GetRelDist(int destiny)
        {//valores RAW
            //raw\
            int index = mrRoboto.MyIdNum;
            if (!pumas_team) index += 5;

            float Xdest,Ydest;

            if (destiny < 0)
            {
                Xdest = Balon.posX;
                Ydest = Balon.posY;
            }
            else
            {
                Xdest = RobotsDatos[index].posX;
                Ydest = RobotsDatos[index].posY;
            }
            //prediction goes here 
            if (go_and_predict && Prediction)//if saved position
            {
                //calculate predicted x,y with X1,Y1,Theta1,X2,Y2,Theta2,t0,estimation time
                //mrRoboto.X =  prediction.X;
                //mrRoboto.Y =  prediction.Y;
                //mrRoboto.Angle=prediction.Angle;
                double Yr, Xr, R, wVel,b1,b2,m1,m2;

                m1=Math.Tan(previousRobot.Angulo);
                m2 = Math.Tan(mrRoboto.Angle);

                b1 = previousRobot.posY + (previousRobot.posX / m1);
                b2 = mrRoboto.Y + (mrRoboto.X / m2);

                Xr = ((m1 * m2) / (m2 - m1 + 0.000001F)) * (b1 - b2);
                Yr = -(((m2) / (m2 - m1 + 0.000001F)) * (b1 - b2)) + b1;

                R = Math.Sqrt(Math.Pow(previousRobot.posX - Xr, 2) + Math.Pow(previousRobot.posY - Yr,2));

                wVel = (mrRoboto.Angle - previousRobot.Angulo) / TimeBetweenSocks + 0.000001;

                predictionRobot.Angulo = (float)(mrRoboto.Ang2tgt + (wVel * PredictionTime));
                predictionRobot.posX = (float)(Xr + R * Math.Cos(predictionRobot.Angulo));
                predictionRobot.posY = (float)(Yr + R * Math.Sin(predictionRobot.Angulo));

                mrRoboto.X = predictionRobot.posY;
                mrRoboto.Y = predictionRobot.posY;
                mrRoboto.Angle = predictionRobot.Angulo;
            }
            //degrees
            //float angulo_robot = deg2rad(mrRoboto.Angle);
            //radians
            float angulo_robot = mrRoboto.Angle;
            
            float Vx = (Xdest - mrRoboto.X);
            float Vy = (Ydest - mrRoboto.Y);
            mrRoboto.Dist2tgt = (float)(Math.Sqrt(Math.Pow(Vx, 2) + Math.Pow(Vy, 2)));
            float angulo_to_pel = Angulo(Vx, Vy);
            //raw
            mrRoboto.Ang2tgt = normalizeAngle(angulo_to_pel - angulo_robot);
            
            
        }

        //------------------------------------------------------------------------------------
        //Function: ParseActionData
        //Purpose:  Parses the incoming string from sockets and executes an action
        //Accepts:	Message incoming from sockets
        //Returns:	Whether the string is acceptable in the established grammar
        //Throws:	
        //------------------------------------------------------------------------------------
        /// <summary>
        /// Parses a command string, and executes de command
        /// </summary>
        /// <param name="inMessage">string to be parsed</param>
        /// <returns>Parse success</returns>
        private string[] msgTokens;
        public void /*ExecList*/ ParseActionData(string inMessage)
        {
            //ExecList cmdRequest = new ExecList();

            //if (inMessage == null)
            //    return cmdRequest;

            //First, we clean the message from those characters that we found useless
            inMessage = clientSoc.ReadUntil(inMessage, '\n'.ToString());
            inMessage = inMessage.TrimEnd(new char[] { _charSPACE, _charNULL });

            //Second, we look for a command ID or the sender's IP
            if (inMessage.IndexOf("@") != -1)
            {
                msgTokens = inMessage.Split('@');

                try
                {
                    //cmdRequest.cmdID = int.Parse(msgTokens[1]);
                    inMessage = msgTokens[0];
                }
                catch (System.FormatException)
                {
                    try
                    {
                        if (msgTokens[1].Equals(clientSoc.ip.ToString()))
                        {
                            inMessage = msgTokens[0];
                        }
                    }
                    catch (System.FormatException)
                    {
                        //cmdRequest = new ExecList();
                        //return cmdRequest;
                    }
                }
            }

            //Clean the command submessage and divide in commandTokens
            inMessage = inMessage.ToLower();
            string[] commandTokens;
            inMessage = inMessage.TrimEnd(_charSPACE);
            commandTokens = inMessage.Split(_charSPACE);	//Our grammar divides commandTokens with space character

            int spaceCont = 0;
            for (int tokCont = 0; tokCont < commandTokens.Length; tokCont++)
            {
                if (commandTokens[tokCont] == "")
                    spaceCont++;
            }

            string[] finalTokens = new string[commandTokens.Length - spaceCont];
            int finalCont = 0;
            for (int tokCont = 0; tokCont < commandTokens.Length; tokCont++)
            {
                if (commandTokens[tokCont] != "")
                {
                    finalTokens[finalCont] = commandTokens[tokCont];
                    finalCont++;
                }
            }

            try
            {
                switch (finalTokens[0])
                {
                    //To Be Confirmed  commands (depends whether they are programmed on robot or not)
                    #region TBC Commands
                    //					case "hp":
                    //						break;
                    //					case "left":
                    //						mrRoboto.TurnLeft();
                    //						consoleTextBox.Text += "\r\nI'm here";
                    //						break;
                    //					case "right":
                    //						mrRoboto.TurnRight();
                    //						break;
                    //					case "forward":
                    //						mrRoboto.MoveForward();
                    //						break;
                    //					case "backward":
                    //						mrRoboto.MoveBackward();
                    //						break;
                    //					case "stop":
                    //						mrRoboto.Stop();
                    //						break;
                    //					case "raw":
                    //						if(commandTokens.Length > 1)
                    //						{
                    //							mrRoboto.sPort.WriteString(commandTokens[1]);
                    //							lastMessage = commandTokens[1];
                    //							messageSPort = commandTokens[1];
                    //						}
                    //						break;
                    //					case "setid":
                    //						if(commandTokens.Length>1)
                    //						{
                    //							mrRoboto.idNum = Int32.Parse(commandTokens[1]); 
                    //						}
                    //						break;
                    //					case "setsp":
                    //						if(commandTokens.Length > 1)
                    //						{
                    //							mrRoboto.SetSpeed(Int32.Parse(commandTokens[1]));
                    //						}
                    //						break;
                    //					case "shid":
                    //						if(clientSoc.clientSocket.Connected)
                    //						{
                    //							clientSoc.SendMsg(mrRoboto.idNum.ToString());
                    //						}
                    //						break;
                    //					case "shs":
                    //						if(commandTokens.Length>1)
                    //						{
                    //							clientSoc.SendMsg(mrRoboto.ReqSensorVal(commandTokens[1], Int32.Parse(commandTokens[2])));
                    //						}
                    //						break;
                    //					case "bye":
                    //						throw new Exception("Call to end program");
                    #endregion
                    case "mv":
                        if (finalTokens.Length > 2)
                        {
                            try
                            {
                                
                                //cmdRequest.mvReq = true;
                                //cmdRequest.mvDist = float.Parse(finalTokens[1], System.Globalization.NumberStyles.Float);
                                //cmdRequest.mvAngle = float.Parse(finalTokens[2], System.Globalization.NumberStyles.Float);
                                consoleTextBox.Text += _strENTER + "Moving Robot";
                                WriteString(mrRoboto.TurnAndMove(float.Parse(finalTokens[1], System.Globalization.NumberStyles.Float), float.Parse(finalTokens[2], System.Globalization.NumberStyles.Float)));
                            }
                            catch (System.Exception se)
                            {
                                consoleTextBox.Text += "Error:" + se +_strENTER;
                            }
                        }
                        break;
                    case "raw"://new
                        if (finalTokens[1].Length == 10)
                            WriteString(finalTokens[1]);
                        //this.consoleTextBox.Text += "rawok:"+ finalTokens[1] + "\r\n";
                        break;
                    case "shooton":
                        {
                            //consoleTextBox.Text += _strENTER + "Activating Shooter...";
                            WriteString("FF00000000");
                            //mainCmdRequest.shOnReq = false;
                            Thread.Sleep(100);
                        }
                        break;
                    case "shootoff":
                        {
                            //consoleTextBox.Text += _strENTER + "Deactivating Shooter...";
                            WriteString("FF00000001");
                            //mainCmdRequest.shOffReq = false;
                            Thread.Sleep(100);
                        }
                        break;
                    default: break;
                }

            }
            catch (Exception excp)
            {
                consoleTextBox.Text += _strENTER + "Error while Parsing."
                    + _strENTER + excp.ToString();
                //cmdRequest = new ExecList();
            }
            //return cmdRequest;
        }


		/// <summary>
		/// When the ENTER button is pressed on the ppc, 
		/// Application tries to connect to server
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void Form1_KeyDown(object sender, KeyEventArgs e)
		{
			if(e.KeyCode == Keys.Return && startButton.Enabled == true)
				IgniteTerminal();
		}

		private void clearButton_Click(object sender, System.EventArgs e)
		{
			//countLabel.Text = commandCount.ToString();
            consoleTextBox.Text = "";
		}

        private void menuItem1_Click(object sender, EventArgs e)
        {
            //this.Close();
            //clientSoc.Close();
        }

        private void lstListaRobots_SelectedIndexChanged(object sender, EventArgs e)
        {
            /*int i = lstListaRobots.SelectedIndex;

            lblRobAng.Text = RobotsDatos[int.Parse(lstListaRobots.Text.Substring(4))].Angulo.ToString();
            lblRobNum.Text = RobotsDatos[int.Parse(lstListaRobots.Text.Substring(4))].RobNum.ToString();
            lblRobX.Text = RobotsDatos[int.Parse(lstListaRobots.Text.Substring(4))].posX.ToString();
            lblRobY.Text = RobotsDatos[int.Parse(lstListaRobots.Text.Substring(4))].posY.ToString();

            if (RobotsDatos[i].Puma)
                lblTeam.Text = "Pumas";
            else
                lblTeam.Text = "Contrario";*/
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            cboPow.Text = (100).ToString();
            MotorPower = 100;

            cboRobNum.Text = (0).ToString();
            mrRoboto.MyIdNum = 0;

            cboTurn.Text = (40).ToString();
            TurnPower = 40;

            toSim = false;
        }

        private void cboRobNum_SelectedIndexChanged(object sender, EventArgs e)
        {
            /*
             * Pockets		30/mar/06                   manda recibe
                ipaq hp 192.168.213.200 (2 ms)          6001 6000
                ipaq hp 192.168.213.201 (2 ms)          6101 6100
                ipaq hp 192.168.213.202  (2 ms)         6201 6200
                ipaq hp viejita 192.168.213.203 (4 ms)  6301 6300
                ipaq hp viejita 192.168.213.204 (4 ms)  6401 6400
             * */
            mrRoboto.MyIdNum = Int32.Parse(cboRobNum.Text);
            if(pumas_team)
                switch (Int32.Parse(cboRobNum.Text))
                {
                    case 0:
                        sndCtrlPorttextbox.Text = (6001).ToString();
                        rcvCtrlPortText.Text = (6000).ToString();
                        break;
                    case 1:
                        sndCtrlPorttextbox.Text = (6101).ToString();
                        rcvCtrlPortText.Text = (6100).ToString();
                        break;
                    case 2:
                        sndCtrlPorttextbox.Text = (6201).ToString();
                        rcvCtrlPortText.Text = (6200).ToString();
                        break;
                    case 3:
                        sndCtrlPorttextbox.Text = (6301).ToString();
                        rcvCtrlPortText.Text = (6300).ToString();
                        break;
                    case 4:
                        sndCtrlPorttextbox.Text = (6401).ToString();
                        rcvCtrlPortText.Text = (6400).ToString();
                        break;
                }
        }

        private void cboPow_SelectedIndexChanged(object sender, EventArgs e)
        {
            MotorPower = Int32.Parse(cboPow.Text);
        }

        private void cboTurn_SelectedIndexChanged(object sender, EventArgs e)
        {
            TurnPower = Int32.Parse(cboTurn.Text); 
        }

        private void trkPow_ValueChanged(object sender, EventArgs e)
        {
            FactorCorr = trkPow.Value;
        }


        private void shoot_on ()
        {
	        if (shooter) return;
            WriteString("FF00000001");
	        shooter = true;
        }

        private void shoot_off ()
        {
	        if (!shooter) return;
            WriteString("FF00000000");
	        shooter = false;
        }

        void reverse()
	    {
            stop();
            
            WriteString("5da00100ff");
            Thread.Sleep(20);
            WriteString("5d0a0100ff");
            Thread.Sleep(20);
            vel[0] = -255;
            vel[1] = -255;
	    }

        void stop()
	    {
		    stopped = true;
            WriteString("5da0010000");
            Thread.Sleep(20);
            WriteString("5d0a010000");
            Thread.Sleep(20);
		    vel[0] = 0.0;
		    vel[1] = 0.0;
	    }

        void rotatel()
	    {
		    stop();
            
            WriteString("5da00200ff");
            Thread.Sleep(20);
            WriteString("5d0a0100ff");
            Thread.Sleep(20);
		    vel[0] = -255;
		    vel[1] = +255;
	    }

	    void rotater()
	    {
            stop();
            WriteString("5d0a0200ff");
            Thread.Sleep(20);
            WriteString("5da00100ff");
            Thread.Sleep(20);
		    vel[0] = +255;
		    vel[1] = -255;
	    }

        //always advances 1 meter, fast
        void forward_f()
        {
            SendString("01");
            //Thread.Sleep(2000);
        }

        //advances one step forward
        void forward()
        {
            SendString("02");
            //Thread.Sleep(2000);
        }
        //goes back four steps fast
        //without turning
        void back_f()
        {
            SendString("03");
            //Thread.Sleep(2000);
        }
        //goes back four steps
        void back()
        {
            SendString("04");
            //Thread.Sleep(2000);
        }
        //turns left 15 degrees
        void turn_l()
        {
            SendString("05");
            //Thread.Sleep(2000);
        }
        //turns right 15 degrees
        void turn_r()
        {
            SendString("06");
            //Thread.Sleep(2000);
        }
        //left kick
        void kick_l()
        {
            SendString("07");
            //Thread.Sleep(2000);
        }
        //right kick
        void kick_r()
        {
            SendString("08");
            //Thread.Sleep(2000);
        }
        //goes up from chest
        void face_up()
        {
            SendString("09");
            //Thread.Sleep(2000);
        }
        //goes up from back
        void face_down()
        {
            SendString("0A");
            //Thread.Sleep(2000);
        }
        //one step to the left
        void step_l()
        {
            SendString("0B");
            //Thread.Sleep(2000);
        }
        //one step to the right
        void step_r()
        {
            SendString("0C");
            //Thread.Sleep(2000);
        }
        //one step to the front
        void step_f()
        {
            SendString("0D");
            //Thread.Sleep(2000);
        }
        //one step to the back
        void step_b()
        {
            SendString("0E");
            //Thread.Sleep(2000);
        }
        //goal keeper left
        void keep_l()
        {
            SendString("0F");
            //Thread.Sleep(2000);
        }
        //goal keeper right
        void keep_r()
        {
            SendString("10");
            //Thread.Sleep(2000);
        }
        //left turn moving left foot back
        void left()
        {
            SendString("11");
            //Thread.Sleep(2000);
        }
        //right turn moving right foot back
        void right()
        {
            SendString("12");
            //Thread.Sleep(2000);
        }

        private void chkPuma_CheckStateChanged(object sender, EventArgs e)
        {
            if (chkPuma.Checked)
            {
                pumas_team = true;
                sndCtrlPorttextbox.Text = Convert.ToString(Convert.ToInt16(sndCtrlPorttextbox.Text) - 1000);
                rcvCtrlPortText.Text = Convert.ToString(Convert.ToInt16(rcvCtrlPortText.Text) - 1000);
            }
            else
            {
                pumas_team = false;
                sndCtrlPorttextbox.Text = Convert.ToString(Convert.ToInt16(sndCtrlPorttextbox.Text) + 1000);
                rcvCtrlPortText.Text = Convert.ToString(Convert.ToInt16(rcvCtrlPortText.Text) + 1000);
            }
        }

        private void checkBox1_CheckStateChanged(object sender, EventArgs e)
        {
            if (chkLeft.Checked)
                going_left = true;
            else
                going_left = false;
        }
        bool RAWMode;
        private void mnuRAW_Click(object sender, EventArgs e)
        {
            if (!RAWMode)
            {
                mnuNN.Checked = false;
                NNMode = false;

                RAWMode = true;
                mnuRAW.Checked = true;
            }
            else
            {
                RAWMode = false;
                mnuRAW.Checked = false;
            }
        }
        bool NNMode;
        private void mnuNN_Click(object sender, EventArgs e)
        {
            if (!NNMode)
            {
                RAWMode = false;
                mnuRAW.Checked = false;

                mnuNN.Checked = true;
                NNMode = true;
            }
            else
            {
                mnuNN.Checked = false;
                NNMode = false;
            }
        }

        private void chktoSim_CheckStateChanged(object sender, EventArgs e)
        {
            String tmp;

            toSim = chktoSim.Checked;

            tmp=sndCtrlPorttextbox.Text;
            sndCtrlPorttextbox.Text=rcvCtrlPortText.Text;
            rcvCtrlPortText.Text = tmp;
        }

        private void checkBox1_CheckStateChanged_1(object sender, EventArgs e)
        {
            potencials = checkBox1.Checked;
        }

        private void trkPotenciales_ValueChanged(object sender, EventArgs e)
        {
            nRepulsion = (float)trkPotenciales.Value;
        }

        private void checkBox2_CheckStateChanged(object sender, EventArgs e)
        {
            checkBox2.Checked = !checkBox2.Checked;
            Prediction = !Prediction;
        }
        
        private void cboPred_SelectedIndexChanged(object sender, EventArgs e)
        {
            PredictionTime = Convert.ToInt16(cboPred.Text);
        }

        private void trkFriction_ValueChanged(object sender, EventArgs e)
        {
            frictionOffset = trkFriction.Value;
        }

        
        private void timer1_Tick(object sender, EventArgs e)
        {
            consoleTextBox.Text = "fps:"+countPak.ToString() + " total:" + countTotPak.ToString() + " time:" + TimeBetweenSocks.ToString() + "us";
            countPak = 0;
        }

        private void chkNova_CheckStateChanged(object sender, EventArgs e)
        {
            if (chkNova.Checked)
            {
                robonova = true;
            }
            else
            {
                robonova = false;
            }
        }
	}
}