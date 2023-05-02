using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace ConsoleApp2
{
    class CMD
    {
        public static void CMDOperator(string cmd,string argv)
        {
            Process process = new Process();
            process.StartInfo.FileName = cmd;
            process.StartInfo.Arguments = "/c" + argv;
            process.StartInfo.UseShellExecute = false; //是否使用操作系统shell启动
            process.StartInfo.CreateNoWindow = false; //是否在新窗口中启动该进程的值 (不显示程序窗口)
            process.Start();
            process.WaitForExit(); //等待程序执行完退出进程
            process.Close();
        }
    }
}
