#!/usr/bin/env python3
import subprocess
from tkinter.ttk import *
from tkinter.filedialog import *
try:
    from idlelib.WidgetRedirector import WidgetRedirector
except ImportError:
    from idlelib.redirector import WidgetRedirector
from subprocess import check_output


COMPILERS = [{'text': "No Compilers Found", 'path': ""}]
CPP_STANDARDS = ["c++14", "c++11", "c++98"]
OPTIMIZE_FLAGS = [
    {'text': "-O0 (Optimize for Compile Time)", 'value': "-O0"},
    {'text': "-O1 (Optimize for code size and speed)", 'value': "-O1"},
    {'text': "-O2 (Optimize for code size and speed, more than -O1)", 'value': "-O2"},
    {'text': "-O3 (Optimize for code size and speed, more than -O2)", 'value': "-O3"}
]


def get_gcc_version(path):
    return str(check_output([path, "--version"]).decode('UTF-8').split()[2])


def find_compilers():
    global COMPILERS

    results = check_output(["whereis", "-b", "g++"])[5:-1].decode('UTF-8').split()
    if len(results) == 0:
        raise Exception("Could not find any installs of g++")

    COMPILERS = []
    for c in results:
        COMPILERS.append({'text': "g++ " + get_gcc_version(c) + ": " + c, 'path': c})


class ReadOnlyText(Text):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.redirector = WidgetRedirector(self)
        self.insert = self.redirector.register("insert", lambda *a, **kw: "break")
        self.delete = self.redirector.register("delete", lambda *a, **kw: "break")


class SourcesBox(Frame):
    def __init__(self, master=None, callback=None):
        super().__init__(master)

        self.callback_func = callback

        self.rowconfigure(0, weight=9)
        self.rowconfigure(1, weight=1)

        self.columnconfigure(0, weight=1)
        self.columnconfigure(1, weight=1)
        self.columnconfigure(2, weight=1)

        self.sources_list = Listbox(self)
        self.sources_list.grid(row=0, column=0, columnspan=3, sticky=N+S+E+W)

        self.clear_button = Button(self, text="Clear", command=self.on_clear)
        self.clear_button.grid(row=1, column=0, sticky=N+S+E+W)

        self.remove_button = Button(self, text="Remove", command=self.on_remove)
        self.remove_button.grid(row=1, column=1, sticky=N+S+E+W)

        self.add_button = Button(self, text="Add", command=self.on_add)
        self.add_button.grid(row=1, column=2, sticky=N+S+E+W)

    def on_clear(self, *args, **kwargs):
        self.sources_list.delete(0, END)

        self.callback_func()

    def on_remove(self, *args, **kwargs):
        self.sources_list.delete(ANCHOR)

        self.callback_func()

    def on_add(self, *args, **kwargs):
        files = askopenfilenames(filetypes=[("C/C++ Files", ("*.cpp", "*.cxx", "*.cc", "*.c"))])

        if not files:
            return

        prefix = os.getcwd()

        for f in [os.path.relpath(p, prefix) for p in files]:
            self.sources_list.insert(END, f)

        self.callback_func()

    def get_sources(self):
        return self.sources_list.get(0, END)


class MainWindow(Frame):
    def __init__(self, master=None):
        super().__init__(master)

        self.__generated_cmd = ""

        self.grid(sticky=N+S+E+W)

        self.rowconfigure(0, weight=1)
        self.rowconfigure(1, weight=1)
        self.rowconfigure(2, weight=1)
        self.rowconfigure(3, weight=55)
        self.rowconfigure(4, weight=1)
        self.rowconfigure(5, weight=99)

        self.columnconfigure(0, weight=1)
        self.columnconfigure(1, weight=2)
        self.columnconfigure(2, weight=1)
        self.columnconfigure(3, weight=2)
        self.columnconfigure(4, weight=1)

        self.compiler_select_label = Label(self, text="Compiler")
        self.compiler_select_label.grid(row=0, column=0, sticky=W+E)
        self.compiler_select = Combobox(self, state=['readonly'], values=[x["text"] for x in COMPILERS])
        self.compiler_select.current(0)
        self.compiler_select.bind('<<ComboboxSelected>>', self.on_compiler_select)
        self.compiler_select.grid(row=0, column=1, sticky=W+E)

        self.standard_select_label = Label(self, text="Standard")
        self.standard_select_label.grid(row=0, column=2, sticky=W+E)
        self.standard_select = Combobox(self, state=['readonly'], values=CPP_STANDARDS)
        self.standard_select.current(0)
        self.standard_select.bind('<<ComboboxSelected>>', self.on_standard_select)
        self.standard_select.grid(row=0, column=3, columnspan=2, sticky=W+E)

        self.sources = SourcesBox(self, callback=self.rebuild_command)
        self.sources.grid(row=1, column=0, rowspan=3, columnspan=2, sticky=N+S+E+W)

        self.executable_label = Label(self, text="Executable")
        self.executable_label.grid(row=4, column=0, sticky=E+W)
        self.executable_data = StringVar()
        self.executable_data.set("a.out")
        self.executable_data.trace("w", lambda *a, **kw: self.rebuild_command())
        self.executable = Entry(self, textvariable=self.executable_data)
        self.executable.grid(row=4, column=1, sticky=E+W)

        self.debug_data = IntVar()
        self.debug = Checkbutton(
            self, text="Debug", variable=self.debug_data, command=lambda *a, **kw: self.rebuild_command()
        )
        self.debug.grid(row=1, column=4, sticky=E+W)

        self.optimize_label = Label(self, text="Optimize")
        self.optimize_label.grid(row=1, column=2, sticky=E+W)
        self.optimize_select = Combobox(self, state=['readonly'], values=[x["text"] for x in OPTIMIZE_FLAGS])
        self.optimize_select.current(0)
        self.optimize_select.bind('<<ComboboxSelected>>', self.on_optimize_select)
        self.optimize_select.grid(row=1, column=3, sticky=E+W)

        self.generated_command = ReadOnlyText(self)
        self.generated_command.grid(row=3, column=2, columnspan=3, sticky=N+S+E+W)

        self.compile_button = Button(self, text="Compile", command=self.run_compile)
        self.compile_button.grid(row=4, column=2, columnspan=3, sticky=E+W)

        self.compiler_output = ReadOnlyText(self)
        self.compiler_output.grid(row=5, column=0, columnspan=5, sticky=N + E + S + W)

        self.rebuild_command()

    def on_compiler_select(self, *args, **kwargs):
        self.compiler_select.selection_clear()
        self.rebuild_command()

    def on_standard_select(self, *args, **kwargs):
        self.standard_select.selection_clear()
        self.rebuild_command()

    def on_optimize_select(self, *args, **kwargs):
        self.optimize_select.selection_clear()
        self.rebuild_command()

    def rebuild_command(self):
        self.__generated_cmd = COMPILERS[self.compiler_select.current()]["path"] + " "
        self.__generated_cmd += "-std=" + self.standard_select.get() + " "
        self.__generated_cmd += OPTIMIZE_FLAGS[self.optimize_select.current()]["value"] + " "

        if self.debug_data.get() == 1:
            self.__generated_cmd += "-g "

        if self.executable.get() is not None and self.executable.get() is not "":
            self.__generated_cmd += "-o " + self.executable.get() + " "

        if len(self.sources.get_sources()) <= 0:
            self.compile_button.config(state='disabled')
        else:
            self.compile_button.config(state='normal')

        for file in self.sources.get_sources():
            self.__generated_cmd += "'" + file + "' "

        self.__generated_cmd.rstrip(' ')

        cmd = "Generated Command:\n\n" + self.__generated_cmd

        self.generated_command.delete("1.0", END)
        self.generated_command.insert(END, cmd)

    def append_compiler_output_line(self, msg):
        self.compiler_output.insert(END, msg + "\n")
        self.compiler_output.see(END)

    def run_compile(self):
        self.append_compiler_output_line(os.getcwd() + "> " + self.__generated_cmd)

        compiler = subprocess.Popen(
                self.__generated_cmd.rstrip() + " 2>&1",
                shell=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                cwd=os.getcwd()
            )

        (out, err) = compiler.communicate()
        rc = compiler.returncode

        self.append_compiler_output_line(out.decode('UTF-8'))

        self.append_compiler_output_line(
            COMPILERS[self.compiler_select.current()]["path"] + " exited with status " + str(rc) + "\n"
        )


if __name__ == "__main__":
    find_compilers()

    root = Tk()
    root.minsize(800, 480)
    root.rowconfigure(0, weight=1)
    root.columnconfigure(0, weight=1)

    app = MainWindow(master=root)
    app.master.title("Compiler Helper")

    root.geometry("800x480")

    app.mainloop()
