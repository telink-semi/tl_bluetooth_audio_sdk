import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import configparser
import os

import datetime
from tlk_ota import *

class OTAConfigGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("OTA Config Tool")
        self.config_file = "ota_config_v2.ini"
        
        # 初始化配置
        self.config = configparser.ConfigParser()
        self.load_config()
        
        # 创建组件
        self.create_widgets()
        
        # 绑定参数修改事件
        self.bind_parameter_changes()

    def load_config(self):
        """加载配置文件"""
        if os.path.exists(self.config_file):
            self.config.read(self.config_file)
        else:
            # 默认配置
            self.config["D25F"] = {
                "enabled": "0",
                "file_path": "",
                "file": "",
                "address": "0x13040",
                "version": "1",
                "type": "1",
                "vid": "1",
                "pid": "1",
                "feature": "0"
            }
            self.config["N22"] = {
                "enabled": "0",
                "file_path": "",
                "file": "",
                "address": "0x50020000",
                "version": "1",
                "type": "2",
                "vid": "1",
                "pid": "1",
                "feature": "0"
            }
            self.config["DSP"] = {
                "enabled": "0",
                "file_path": "",
                "file": "",
                "address": "0x200040",
                "version": "1",
                "type": "3",
                "vid": "1",
                "pid": "1",
                "feature": "0"
            }
            self.config["Boot"] = {
                "enabled": "0",
                "file_path": "",
                "file": "",
                "address": "0x12000"
            }
            self.config["OTAFile"] = {
                "file": "tlk_ota_file",
                "timestamp_enabled": "0"
            }
            self.save_config()

    def save_config(self):
        """保存配置到文件"""
        with open(self.config_file, "w") as f:
            self.config.write(f)

    def create_widgets(self):
        # 标题行
        headers = ["", "Core", "File Path", "File", "Address", "Version", "Type", "VID", "PID", "Feature"]
        for col, header in enumerate(headers):
            label = ttk.Label(self.root, text=header, font=("Arial", 10, "bold"))
            label.grid(row=0, column=col, padx=5, pady=5, sticky="nsew")

        # D25F行
        self.d25f_enabled = tk.BooleanVar(value=self.config.getboolean("D25F", "enabled"))
        chk = ttk.Checkbutton(self.root, variable=self.d25f_enabled)
        chk.grid(row=1, column=0, padx=5, pady=5)
        
        ttk.Label(self.root, text="D25F").grid(row=1, column=1, padx=5, pady=5)
        
        self.d25f_filepath = ttk.Entry(self.root, width=20)
        self.d25f_filepath.insert(0, self.config.get("D25F", "file_path"))
        self.d25f_filepath.grid(row=1, column=2, padx=5, pady=5)
        
        self.d25f_file_btn = ttk.Button(self.root, text="....", width=4, command=lambda: self.select_file("D25F"))
        self.d25f_file_btn.grid(row=1, column=3, padx=5, pady=5)
        
        self.d25f_address = ttk.Entry(self.root, width=10)
        self.d25f_address.insert(0, self.config.get("D25F", "address"))
        self.d25f_address.grid(row=1, column=4, padx=5, pady=5)
        
        self.d25f_version = ttk.Entry(self.root, width=5)
        self.d25f_version.insert(0, self.config.get("D25F", "version"))
        self.d25f_version.grid(row=1, column=5, padx=5, pady=5)
        
        self.d25f_type = ttk.Entry(self.root, width=5)
        self.d25f_type.insert(0, self.config.get("D25F", "type"))
        self.d25f_type.grid(row=1, column=6, padx=5, pady=5)
        
        self.d25f_vid = ttk.Entry(self.root, width=5)
        self.d25f_vid.insert(0, self.config.get("D25F", "vid"))
        self.d25f_vid.grid(row=1, column=7, padx=5, pady=5)
        
        self.d25f_pid = ttk.Entry(self.root, width=5)
        self.d25f_pid.insert(0, self.config.get("D25F", "pid"))
        self.d25f_pid.grid(row=1, column=8, padx=5, pady=5)
        
        self.d25f_feature = ttk.Entry(self.root, width=5)
        self.d25f_feature.insert(0, self.config.get("D25F", "feature"))
        self.d25f_feature.grid(row=1, column=9, padx=5, pady=5)

        # N22行
        self.n22_enabled = tk.BooleanVar(value=self.config.getboolean("N22", "enabled"))
        chk = ttk.Checkbutton(self.root, variable=self.n22_enabled)
        chk.grid(row=2, column=0, padx=5, pady=5)
        
        ttk.Label(self.root, text="N22").grid(row=2, column=1, padx=5, pady=5)
        
        self.n22_filepath = ttk.Entry(self.root, width=20)
        self.n22_filepath.insert(0, self.config.get("N22", "file_path"))
        self.n22_filepath.grid(row=2, column=2, padx=5, pady=5)
        
        self.n22_file_btn = ttk.Button(self.root, text="....", width=4, command=lambda: self.select_file("N22"))
        self.n22_file_btn.grid(row=2, column=3, padx=5, pady=5)
        
        self.n22_address = ttk.Entry(self.root, width=10)
        self.n22_address.insert(0, self.config.get("N22", "address"))
        self.n22_address.grid(row=2, column=4, padx=5, pady=5)
        
        self.n22_version = ttk.Entry(self.root, width=5)
        self.n22_version.insert(0, self.config.get("N22", "version"))
        self.n22_version.grid(row=2, column=5, padx=5, pady=5)
        
        self.n22_type = ttk.Entry(self.root, width=5)
        self.n22_type.insert(0, self.config.get("N22", "type"))
        self.n22_type.grid(row=2, column=6, padx=5, pady=5)
        
        self.n22_vid = ttk.Entry(self.root, width=5)
        self.n22_vid.insert(0, self.config.get("N22", "vid"))
        self.n22_vid.grid(row=2, column=7, padx=5, pady=5)
        
        self.n22_pid = ttk.Entry(self.root, width=5)
        self.n22_pid.insert(0, self.config.get("N22", "pid"))
        self.n22_pid.grid(row=2, column=8, padx=5, pady=5)
        
        self.n22_feature = ttk.Entry(self.root, width=5)
        self.n22_feature.insert(0, self.config.get("N22", "feature"))
        self.n22_feature.grid(row=2, column=9, padx=5, pady=5)

        # DSP行
        self.dsp_enabled = tk.BooleanVar(value=self.config.getboolean("DSP", "enabled"))
        chk = ttk.Checkbutton(self.root, variable=self.dsp_enabled)
        chk.grid(row=3, column=0, padx=5, pady=5)
        
        ttk.Label(self.root, text="DSP").grid(row=3, column=1, padx=5, pady=5)
        
        self.dsp_filepath = ttk.Entry(self.root, width=20)
        self.dsp_filepath.insert(0, self.config.get("DSP", "file_path"))
        self.dsp_filepath.grid(row=3, column=2, padx=5, pady=5)
        
        self.dsp_file_btn = ttk.Button(self.root, text="....", width=4, command=lambda: self.select_file("DSP"))
        self.dsp_file_btn.grid(row=3, column=3, padx=5, pady=5)
        
        self.dsp_address = ttk.Entry(self.root, width=10)
        self.dsp_address.insert(0, self.config.get("DSP", "address"))
        self.dsp_address.grid(row=3, column=4, padx=5, pady=5)
        
        self.dsp_version = ttk.Entry(self.root, width=5)
        self.dsp_version.insert(0, self.config.get("DSP", "version"))
        self.dsp_version.grid(row=3, column=5, padx=5, pady=5)
        
        self.dsp_type = ttk.Entry(self.root, width=5)
        self.dsp_type.insert(0, self.config.get("DSP", "type"))
        self.dsp_type.grid(row=3, column=6, padx=5, pady=5)
        
        self.dsp_vid = ttk.Entry(self.root, width=5)
        self.dsp_vid.insert(0, self.config.get("DSP", "vid"))
        self.dsp_vid.grid(row=3, column=7, padx=5, pady=5)
        
        self.dsp_pid = ttk.Entry(self.root, width=5)
        self.dsp_pid.insert(0, self.config.get("DSP", "pid"))
        self.dsp_pid.grid(row=3, column=8, padx=5, pady=5)
        
        self.dsp_feature = ttk.Entry(self.root, width=5)
        self.dsp_feature.insert(0, self.config.get("DSP", "feature"))
        self.dsp_feature.grid(row=3, column=9, padx=5, pady=5)

        # Boot行
        self.boot_enabled = tk.BooleanVar(value=self.config.getboolean("Boot", "enabled"))
        chk = ttk.Checkbutton(self.root, variable=self.boot_enabled)
        chk.grid(row=4, column=0, padx=5, pady=5)
        
        ttk.Label(self.root, text="Boot").grid(row=4, column=1, padx=5, pady=5)
        
        self.boot_filepath = ttk.Entry(self.root, width=20)
        self.boot_filepath.insert(0, self.config.get("Boot", "file_path"))
        self.boot_filepath.grid(row=4, column=2, padx=5, pady=5)
        
        self.boot_file_btn = ttk.Button(self.root, text="....", width=4, command=lambda: self.select_file("Boot"))
        self.boot_file_btn.grid(row=4, column=3, padx=5, pady=5)
        
        self.boot_address = ttk.Entry(self.root, width=10)
        self.boot_address.insert(0, self.config.get("Boot", "address"))
        self.boot_address.grid(row=4, column=4, padx=5, pady=5)

        # OTA File行
        ttk.Label(self.root, text="OTA File").grid(row=5, column=1, padx=5, pady=5)
        
        self.ota_file = ttk.Entry(self.root, width=20)
        self.ota_file.insert(0, self.config.get("OTAFile", "file"))
        self.ota_file.grid(row=5, column=2, padx=5, pady=5)
        
        self.timestamp_enabled = tk.BooleanVar(value=self.config.getboolean("OTAFile", "timestamp_enabled"))
        chk = ttk.Checkbutton(self.root, variable=self.timestamp_enabled)
        chk.grid(row=5, column=3, padx=5, pady=5)
        
        ttk.Label(self.root, text="Timestamp").grid(row=5, column=4, padx=5, pady=5)

        # 生成按钮
        self.generate_btn = ttk.Button(self.root, text="Generate OTA Bin", command=self.generate_ota)
        self.generate_btn.grid(row=6, column=8, columnspan=2, padx=5, pady=10)

    def select_file(self, module):
        """选择.bin文件并更新路径"""
        file_path = filedialog.askopenfilename(
            title="Select BIN File",
            filetypes=(("BIN File", "*.bin"), ("All Files", "*.*"))
        )
        if file_path:
            # 更新对应模块的文件路径
            if module == "D25F":
                self.d25f_filepath.delete(0, tk.END)
                self.d25f_filepath.insert(0, file_path)
                self.config.set("D25F", "file_path", file_path)
                self.config.set("D25F", "file", os.path.basename(file_path))
            elif module == "N22":
                self.n22_filepath.delete(0, tk.END)
                self.n22_filepath.insert(0, file_path)
                self.config.set("N22", "file_path", file_path)
                self.config.set("N22", "file", os.path.basename(file_path))
            elif module == "DSP":
                self.dsp_filepath.delete(0, tk.END)
                self.dsp_filepath.insert(0, file_path)
                self.config.set("DSP", "file_path", file_path)
                self.config.set("DSP", "file", os.path.basename(file_path))
            elif module == "Boot":
                self.boot_filepath.delete(0, tk.END)
                self.boot_filepath.insert(0, file_path)
                self.config.set("Boot", "file_path", file_path)
                self.config.set("Boot", "file", os.path.basename(file_path))
            self.save_config()

    def generate_ota(self):
        try:
            d25f_bin = tlk_bin_file_info(None, 0x13040, type=0x01)
            n22_bin = tlk_bin_file_info(None, 0x50020000, type=2)
            dsp_bin = tlk_bin_file_info(None, 0x200040, type=3)
            if self.config.getboolean("D25F", "enabled"):
                if(self.config.get("D25F", "file_path") != ""):
                    d25f_bin.file_name = self.config.get("D25F", "file_path")
                    d25f_bin.bin_size = 0
                    d25f_bin.start_addr = int(self.config.get("D25F", "address"), 16)
                    d25f_bin.version = int(self.config.get("D25F", "version"))
                    d25f_bin.type = int(self.config.get("D25F", "type"))
                    d25f_bin.vid = int(self.config.get("D25F", "vid"))
                    d25f_bin.pid = int(self.config.get("D25F", "pid"))
                    d25f_bin.feature_map = int(self.config.get("D25F", "feature"), 16)

            if self.config.getboolean("N22", "enabled"):
                if(self.config.get("N22", "file_path") != ""):
                    n22_bin.file_name = self.config.get("N22", "file_path")
                    n22_bin.bin_size = 0
                    n22_bin.start_addr = int(self.config.get("N22", "address"), 16)
                    n22_bin.version = int(self.config.get("N22", "version"))
                    n22_bin.type = int(self.config.get("N22", "type"))
                    n22_bin.vid = int(self.config.get("N22", "vid"))
                    n22_bin.pid = int(self.config.get("N22", "pid"))
                    n22_bin.feature_map = int(self.config.get("N22", "feature"), 16)

            if self.config.getboolean("DSP", "enabled"):
                if(self.config.get("DSP", "file_path") != ""):
                    dsp_bin.file_name = self.config.get("DSP", "file_path")
                    dsp_bin.bin_size = 0
                    dsp_bin.start_addr = int(self.config.get("DSP", "address"), 16)
                    dsp_bin.version = int(self.config.get("DSP", "version"))
                    dsp_bin.type = int(self.config.get("DSP", "type"))
                    dsp_bin.vid = int(self.config.get("DSP", "vid"))
                    dsp_bin.pid = int(self.config.get("DSP", "pid"))
                    dsp_bin.feature_map = int(self.config.get("DSP", "feature"), 16)

            all_input_bin = [
                d25f_bin,
                n22_bin,
                dsp_bin,
            ]

            g_ota_header_value = tlk_ota_calc_header(all_input_bin)

            if self.config.getboolean("OTAFile", "timestamp_enabled"):
                output_file = self.config.get("OTAFile", "file") + "_" + datetime.datetime.now().strftime("%Y-%m-%d %H-%M-%S")
                output_with_boot_file = self.config.get("OTAFile", "file") + "_with_boot" + "_" + datetime.datetime.now().strftime("%Y-%m-%d %H-%M-%S")
            else:
                output_file = self.config.get("OTAFile", "file")
                output_with_boot_file = self.config.get("OTAFile", "file") + "_with_boot"
            output_file += ".bin"
            output_with_boot_file += ".bin"
            Output_file = open(output_file, "wb")
            Output_file.write(g_ota_header_value.tobytes())

            tlk_ota_write_bin(all_input_bin, Output_file)
            Output_file.close()

            if self.config.getboolean("Boot", "enabled") and self.config.get("Boot", "file_path") != "":
                with open(self.config.get("Boot", "file_path"), "rb") as boot_file , \
                    open(output_file, "rb") as ota_file, \
                    open(output_with_boot_file, "wb") as output_with_boot:
                    boot_data = boot_file.read()
                    output_with_boot.write(boot_data)
                    # print(len(boot_data))
                    # 填充0xFF
                    output_with_boot.write(b'\xFF' * (int(self.config.get("Boot", "address"), 16) - len(boot_data)))
                    ota_data = ota_file.read()
                    output_with_boot.write(ota_data)

            messagebox.showinfo("Result", "OTA file generated successfully")
        except Exception as e:
            messagebox.showerror("Error", str(e))

    def bind_parameter_changes(self):
        """绑定参数修改事件，同步保存到配置文件"""
        # 启用状态绑定
        self.d25f_enabled.trace_add("write", lambda *args: self.update_config("D25F", "enabled", str(self.d25f_enabled.get())))
        self.n22_enabled.trace_add("write", lambda *args: self.update_config("N22", "enabled", str(self.n22_enabled.get())))
        self.dsp_enabled.trace_add("write", lambda *args: self.update_config("DSP", "enabled", str(self.dsp_enabled.get())))
        self.boot_enabled.trace_add("write", lambda *args: self.update_config("Boot", "enabled", str(self.boot_enabled.get())))
        self.timestamp_enabled.trace_add("write", lambda *args: self.update_config("OTAFile", "timestamp_enabled", str(self.timestamp_enabled.get())))

        # 参数输入框绑定
        entries = [
            (self.d25f_address, "D25F", "address"),
            (self.d25f_version, "D25F", "version"),
            (self.d25f_type, "D25F", "type"),
            (self.d25f_vid, "D25F", "vid"),
            (self.d25f_pid, "D25F", "pid"),
            (self.d25f_feature, "D25F", "feature"),
            
            (self.n22_address, "N22", "address"),
            (self.n22_version, "N22", "version"),
            (self.n22_type, "N22", "type"),
            (self.n22_vid, "N22", "vid"),
            (self.n22_pid, "N22", "pid"),
            (self.n22_feature, "N22", "feature"),
            
            (self.dsp_address, "DSP", "address"),
            (self.dsp_version, "DSP", "version"),
            (self.dsp_type, "DSP", "type"),
            (self.dsp_vid, "DSP", "vid"),
            (self.dsp_pid, "DSP", "pid"),
            (self.dsp_feature, "DSP", "feature"),
            
            (self.boot_address, "Boot", "address"),
            (self.ota_file, "OTAFile", "file")
        ]

        for entry, section, key in entries:
            entry.bind("<FocusOut>", lambda e, s=section, k=key, ent=entry: self.update_config(s, k, ent.get()))
            entry.bind("<Return>", lambda e, s=section, k=key, ent=entry: self.update_config(s, k, ent.get()))

    def update_config(self, section, key, value):
        """更新配置并保存"""
        self.config.set(section, key, value)
        self.save_config()

if __name__ == "__main__":
    root = tk.Tk()
    app = OTAConfigGUI(root)
    root.mainloop()