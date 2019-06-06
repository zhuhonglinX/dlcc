import tkinter as tk
import tkinter.filedialog as filedialog
import os
import subprocess


def die():
    root.destroy()


class Create:
    # init函数定义类的一些属性
    def __init__(self, root):
        self.root = (root)
        self.textpad = tk.Text(root)
        # expand 允许拉伸，fill允许（xy两个方向）填充
        self.textpad.pack(expand=tk.YES, fill=tk.BOTH)
        self.textpad.bind("<Tab>", self.tab)
        # 菜单
        self.menubar = tk.Menu(root)

        self.filemenu = tk.Menu(self.menubar, tearoff=0)
        self.menubar.add_cascade(label="File", menu=self.filemenu)
        self.filemenu.add_command(label="New", command=self.newfile)
        self.filemenu.add_command(label="Open", command=self.openfile)
        self.filemenu.add_separator()
        self.filemenu.add_command(label="Save", command=self.save)
        self.filemenu.add_command(label="Save to", command=self.saveto)
        self.filemenu.add_separator()
        self.filemenu.add_command(label="Exit", command=die)

        self.editmenu = tk.Menu(self.menubar, tearoff=0)
        self.menubar.add_cascade(label="Edit", menu=self.editmenu)
        self.editmenu.add_command(label="Undo", command=self.donothing)
        self.editmenu.add_command(label="Cut", command=self.donothing)
        self.editmenu.add_command(label="Copy", command=self.donothing)

        self.runmenu = tk.Menu(self.menubar, tearoff=0)
        self.menubar.add_cascade(label="Run", menu=self.runmenu)
        self.runmenu.add_command(label="Run Now", command=self.runnow)

        self.helpmenu = tk.Menu(self.menubar, tearoff=0)
        self.menubar.add_cascade(label="Help", menu=self.helpmenu)
        self.helpmenu.add_command(label="Help Index", command=self.donothing)
        self.helpmenu.add_command(label="About...", command=self.donothing)

        self.root.config(menu=self.menubar)

    def donothing(self):
        filewin = tk.Toplevel(self.root)
        button = tk.Button(filewin, text="hi，添加你的代码")
        button.pack()

    def newfile(self):
        filename = filedialog.asksaveasfilename(title='save new file')
        f = open(filename, 'w')
        f.close()
        self.curr_file = filename
        root.title("FileName: {}".format(os.path.basename(filename)))
        self.textpad.delete('1.0', tk.END)


    def openfile(self):
        filename = filedialog.askopenfilename(title="打开")
        if filename == '':
            filename = None
        else:
            self.curr_file = filename
            root.title('FileName: ' + os.path.basename(filename))
            self.textpad.delete(1.0, tk.END)
            f = open(filename, 'r+')
            self.textpad.insert(1.0, f.read())
            f.close()

    def save(self):
        f = open(self.curr_file, 'w+')
        msg = self.textpad.get(1.0, tk.END)
        f.write(msg)
        f.close()

    def saveto(self):
        sname = filedialog.asksaveasfilename(title="保存")
        f = open(sname, 'w+')
        msg = self.textpad.get(1.0, tk.END)
        f.write(msg)
        # self.curr_file = sname
        # root.title('FileName: ' + os.path.basename(filename))
        f.close()

    def runnow(self):
        cmd = "dlcc {} -i".format(self.curr_file)
        return_code, output = subprocess.getstatusoutput(cmd)
        if return_code != 0:
            print("run error")
        print(output)
        print(self.curr_file)
        tl = tk.Toplevel(self.root)
        tl.title("Terminal")
        tl.geometry('800x500')
        text = tk.Text(tl)
        text.pack(expand=tk.YES, fill=tk.BOTH)
        text.insert(tk.END, output)

    def tab(self, arg):
        self.textpad.insert(tk.INSERT, " " * 4)
        return 'break'


if __name__ == "__main__":
    root = tk.Tk()
    root.title('dlcc Editor')
    root.geometry("800x500")
    window = Create(root)
    root.mainloop()
